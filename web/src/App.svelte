<script lang="ts">
  import { onMount } from "svelte";
  import HueWheel from "./HueWheel.svelte";
  import LightnessSlider from "./LightnessSlider.svelte";
  import createQualpalModule from "./qualpal.mjs";
  import SaturationSlider from "./SaturationSlider.svelte";
  import Examples from "./Examples.svelte";
  import PaletteAnalysis from "./PaletteAnalysis.svelte";

  // State
  let moduleLoaded = false;
  let qualpalModule = null;
  let loading = false;
  let palette = [];
  let analysis = null;

  // Parameters
  let params = {
    numColors: 5,
    hueMin: 20,
    hueMax: 280,
    satMin: 0.4,
    satMax: 0.8,
    lightMin: 0.3,
    lightMax: 0.7,
    useBackground: false,
    backgroundColor: "#ffffff",
    cvd: {
      protan: 0,
      deutan: 0,
      tritan: 0,
    },
  };

  // Toast notification
  let toast = {
    show: false,
    message: "",
  };

  // Debounce timer
  let debounceTimer: ReturnType<typeof setTimeout>;

  // Initialize the module
  onMount(async () => {
    console.log("Loading Qualpal module...");
    try {
      qualpalModule = await createQualpalModule();
      moduleLoaded = true;
      console.log("Qualpal loaded successfully!");
      generatePalette();
    } catch (error) {
      console.error("Failed to load Qualpal:", error);
      moduleLoaded = false;
    }
  });

  // Debounced generate function
  function debouncedGenerate() {
    clearTimeout(debounceTimer);
    debounceTimer = setTimeout(() => {
      generatePalette();
    }, 300);
  }

  function isDarkColor(hex: string): boolean {
    const c = hex.replace("#", "");
    const r = parseInt(c.substring(0, 2), 16);
    const g = parseInt(c.substring(2, 4), 16);
    const b = parseInt(c.substring(4, 6), 16);
    return 0.299 * r + 0.587 * g + 0.114 * b < 128;
  }

  // Generate palette
  async function generatePalette() {
    if (!qualpalModule) return;

    loading = true;

    try {
      const qp = new qualpalModule.Qualpal();

      // Handle wraparound hue ranges using negative values
      let normalizedHueMin = params.hueMin;
      let normalizedHueMax = params.hueMax;

      // If we have a wraparound case (min > max), convert max to negative
      if (params.hueMin > params.hueMax) {
        // For wraparound ranges like 270° to 30°, convert to 270° to -330°
        normalizedHueMin = params.hueMin - 360;
      }

      qp.setInputColorspace(
        normalizedHueMin,
        normalizedHueMax,
        params.satMin,
        params.satMax,
        params.lightMin,
        params.lightMax,
      );

      if (params.useBackground) {
        const bgColor = hexToRgb(params.backgroundColor);
        if (bgColor) {
          qp.setBackground(bgColor.r / 255, bgColor.g / 255, bgColor.b / 255);
        }
      }

      // Set CVD if any slider > 0
      const { protan, deutan, tritan } = params.cvd;
      if (
        (protan > 0 || deutan > 0 || tritan > 0) &&
        qualpalModule?.Qualpal.prototype.setCvd
      ) {
        qp.setCvd({ protan, deutan, tritan });
      }

      const newPalette = qp.generate(params.numColors);
      palette = newPalette;
      console.log("Generated palette:", newPalette);

      // Analyze palette
      try {
        const result = await qualpalModule.analyzePalette(
          newPalette.map((c) => hexToRgb(c.hex)),
          params.cvd,
          params.useBackground ? hexToRgb(params.backgroundColor) : null,
          4,
        );
        console.log("Palette analysis result:", result);
        analysis = result;
      } catch (e) {
        console.error("Error analyzing palette:", e);
        analysis = null;
      }
    } catch (error) {
      console.error("Error generating palette:", error);
      showToast("Error generating palette");
    } finally {
      loading = false;
    }
  }

  // Copy single color to clipboard
  async function copyColor(hex: string) {
    try {
      await navigator.clipboard.writeText(hex);
      showToast(`Copied ${hex}`);
    } catch (error) {
      console.error("Failed to copy to clipboard:", error);
      showToast("Failed to copy");
    }
  }

  // Output tab state
  let activeTab = "JSON";

  // Output text computed property
  $: outputText = (() => {
    const hexColors = palette.map((color) => color.hex);
    switch (activeTab) {
      case "JSON":
        return JSON.stringify(hexColors, null, 2);
      case "R":
        return `c(${hexColors.map((h) => `"${h}"`).join(", ")})`;
      case "Python":
        return `[${hexColors.map((h) => `'${h}'`).join(", ")}]`;
      case "CSS":
        return hexColors.map((h, i) => `--color${i + 1}: ${h};`).join("\n");
      default:
        return "";
    }
  })();

  // Copy output function
  async function copyOutput() {
    try {
      await navigator.clipboard.writeText(outputText);
      showToast("Copied output");
    } catch (error) {
      showToast("Failed to copy");
    }
  }

  // Show toast notification
  function showToast(message: string) {
    toast.message = message;
    toast.show = true;
    setTimeout(() => {
      toast.show = false;
    }, 3000);
  }

  // Convert hex to RGB
  function hexToRgb(hex: string) {
    const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result
      ? {
          r: parseInt(result[1], 16),
          g: parseInt(result[2], 16),
          b: parseInt(result[3], 16),
        }
      : null;
  }
</script>

<div class="bg-gray-100">
  <!-- Header -->
  <header class="bg-white shadow-sm border-b">
    <div class="max-w-7xl mx-auto px-4 py-4">
      <h1 class="text-2xl font-bold text-gray-900">
        <span class="text-blue-600">Qualpal</span> Color Palette Generator
      </h1>
      <p class="text-gray-600 mt-1">
        Generate perceptually uniform color palettes for data visualization
      </p>
    </div>
  </header>

  <div class="bg-gray-100 flex flex-col md:flex-row min-h-screen">
    <!-- Sidebar -->
    <aside
      class="w-full md:w-64 bg-gray-100 p-4 border-b md:border-b-0 md:border-r"
    >
      <div class="sticky top-6">
        <h2 class="text-lg font-semibold text-gray-900 mb-4">Parameters</h2>

        {#if !moduleLoaded}
          <div class="text-center py-8">
            <div
              class="animate-spin h-8 w-8 border-4 border-blue-500 border-t-transparent rounded-full mx-auto mb-4"
            ></div>
            <p class="text-gray-600">Loading Qualpal module...</p>
          </div>
        {:else}
          <div class="space-y-6">
            <!-- Number of Colors -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <div class="form-group">
                <label
                  for="numColors"
                  class="block text-sm font-medium text-gray-700 mb-2"
                >
                  Number of Colors
                  <span class="text-blue-600 font-semibold"
                    >{params.numColors}</span
                  >
                </label>
                <input
                  id="numColors"
                  type="range"
                  min="2"
                  max="12"
                  bind:value={params.numColors}
                  on:input={debouncedGenerate}
                  class="w-full h-2 bg-gray-200 rounded-lg appearance-none cursor-pointer slider"
                />
                <div class="flex justify-between text-xs text-gray-500 mt-1">
                  <span>2</span>
                  <span>12</span>
                </div>
              </div>
            </div>

            <!-- Hue Range -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <h3 class="font-medium text-gray-900 mb-3">Hue Range</h3>

              <HueWheel
                hueMin={params.hueMin}
                hueMax={params.hueMax}
                onChange={({ hueMin, hueMax }) => {
                  params.hueMin = hueMin;
                  params.hueMax = hueMax;
                  debouncedGenerate();
                }}
              />
            </div>

            <!-- Saturation Range -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <h3 class="font-medium text-gray-900 mb-3">Saturation Range</h3>

              <SaturationSlider
                satMin={params.satMin}
                satMax={params.satMax}
                onChange={({ satMin, satMax }) => {
                  params.satMin = satMin;
                  params.satMax = satMax;
                  debouncedGenerate();
                }}
              />
            </div>

            <!-- Lightness Range -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <h3 class="font-medium text-gray-900 mb-3">Lightness Range</h3>

              <LightnessSlider
                lightMin={params.lightMin}
                lightMax={params.lightMax}
                onChange={({ lightMin, lightMax }) => {
                  params.lightMin = lightMin;
                  params.lightMax = lightMax;
                  debouncedGenerate();
                }}
              />
            </div>

            <!-- Background Color -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <h3 class="font-medium text-gray-900 mb-3">Background Color</h3>

              <div class="mb-3">
                <label class="flex items-center gap-2 cursor-pointer">
                  <input
                    type="checkbox"
                    bind:checked={params.useBackground}
                    on:change={debouncedGenerate}
                    class="rounded border-gray-300 text-blue-600 focus:ring-blue-500"
                  />
                  <span class="text-sm text-gray-700"
                    >Optimize for background color</span
                  >
                </label>
              </div>

              {#if params.useBackground}
                <div class="flex items-center gap-2">
                  <input
                    type="color"
                    bind:value={params.backgroundColor}
                    on:change={debouncedGenerate}
                    class="w-1/3 h-8 rounded border border-gray-300 cursor-pointer"
                    title="Select background color"
                  />
                  <input
                    type="text"
                    bind:value={params.backgroundColor}
                    on:input={debouncedGenerate}
                    class="w-2/3 px-2 py-1 text-sm border border-gray-300 rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
                    placeholder="#ffffff"
                    pattern="^#[0-9A-Fa-f]{6}$"
                  />
                </div>
                <p class="text-xs text-gray-500 mt-2">
                  Colors are optimized to be distinct against this background
                </p>
              {:else}
                <p class="text-xs text-gray-500">
                  Colors will be optimized for general use without a specific
                  background
                </p>
              {/if}
            </div>
          </div>
        {/if}

        <!-- CVD Settings -->
        <div class="bg-gray-50 p-4 rounded-lg">
          <h3 class="font-medium text-gray-900 mb-3">
            Color Vision Deficiency
          </h3>
          <div class="space-y-3">
            <div class="max-w-xs w-full">
              <label class="text-sm block mb-1">Protanopia</label>
              <div class="flex items-center gap-2">
                <input
                  type="range"
                  min="0"
                  max="1"
                  step="0.01"
                  bind:value={params.cvd.protan}
                  on:input={debouncedGenerate}
                  class="flex-1 w-full"
                />
                <span class="text-xs w-8 text-right">{params.cvd.protan}</span>
              </div>
            </div>
            <div class="max-w-xs w-full">
              <label class="text-sm block mb-1">Deuteranopia</label>
              <div class="flex items-center gap-2">
                <input
                  type="range"
                  min="0"
                  max="1"
                  step="0.01"
                  bind:value={params.cvd.deutan}
                  on:input={debouncedGenerate}
                  class="flex-1 w-full"
                />
                <span class="text-xs w-8 text-right">{params.cvd.deutan}</span>
              </div>
            </div>
            <div class="max-w-xs w-full">
              <label class="text-sm block mb-1">Tritanopia</label>
              <div class="flex items-center gap-2">
                <input
                  type="range"
                  min="0"
                  max="1"
                  step="0.01"
                  bind:value={params.cvd.tritan}
                  on:input={debouncedGenerate}
                  class="flex-1 w-full"
                />
                <span class="text-xs w-8 text-right">{params.cvd.tritan}</span>
              </div>
            </div>
            <p class="text-xs text-gray-500 mt-2">
              Adapt palette to users with color vision deficiency.
            </p>
          </div>
        </div>
      </div>
    </aside>

    <!-- Main Content -->
    <main class="flex-1 p-4">
      {#if moduleLoaded && palette.length > 0}
        <div class="space-y-6">
          <!-- Palette Display -->
          <div class="bg-white rounded-lg shadow-sm border p-6">
            <div class="flex items-center justify-between mb-4">
              <h2 class="text-xl font-semibold text-gray-900">
                Generated Palette
              </h2>
              <span class="text-sm text-gray-500">{palette.length} colors</span>
            </div>

            <div
              class="p-4 rounded-lg border-2 border-dashed border-gray-200"
              style="background-color: {params.useBackground
                ? params.backgroundColor
                : '#ffffff'}"
            >
              <div class="grid grid-cols-3 lg:grid-cols-5 xl:grid-cols-6 gap-4">
                {#each palette as color}
                  <div class="group">
                    <button
                      class="aspect-square rounded-lg cursor-pointer border-2 border-gray-200 hover:border-gray-400 transition-all duration-200 hover:shadow-md relative overflow-hidden w-full"
                      style="background-color: {color.hex}"
                      on:click={() => copyColor(color.hex)}
                      on:keydown={(e) =>
                        e.key === "Enter" && copyColor(color.hex)}
                      title="Click to copy {color.hex}"
                      aria-label="Copy color {color.hex}"
                    >
                      <div
                        class="absolute inset-0 bg-black bg-opacity-0 group-hover:bg-opacity-10 transition-all duration-200 flex items-center justify-center"
                      >
                        <span
                          class="text-white opacity-0 group-hover:opacity-100 font-medium text-sm bg-black bg-opacity-50 px-2 py-1 rounded"
                        >
                          Copy
                        </span>
                      </div>
                    </button>
                    <div class="mt-2 text-center">
                      <div
                        class="text-sm font-mono"
                        style="color: {isDarkColor(
                          params.useBackground
                            ? params.backgroundColor
                            : '#ffffff',
                        )
                          ? '#fff'
                          : '#222'}"
                      >
                        {color.hex}
                      </div>
                    </div>
                  </div>
                {/each}
              </div>
            </div>
          </div>

          <!-- Examples -->
          <Examples
            {palette}
            useBackground={params.useBackground}
            backgroundColor={params.backgroundColor}
          />

          <!-- JSON Output -->
          <div class="bg-white rounded-lg shadow-sm border p-6">
            <h3 class="text-xl text-left font-semibold text-gray-900 mb-4">
              Output
            </h3>
            <div class="mb-4 border-b flex gap-2">
              {#each ["JSON", "R", "Python", "CSS"] as tab}
                <button
                  class="
                    px-4 py-2 -mb-px border-b-2 font-mono text-sm transition-colors
                    {activeTab === tab
                    ? 'border-blue-600 text-blue-700 font-semibold'
                    : 'border-transparent text-gray-500 hover:text-blue-600'}
                  "
                  on:click={() => (activeTab = tab)}
                  role="tab"
                  aria-selected={activeTab === tab}
                >
                  {tab}
                </button>
              {/each}
            </div>
            <div class="relative bg-gray-50 rounded-lg p-4 border mt-2">
              <button
                class="absolute top-2 right-2 px-2 py-1 bg-green-600 text-white rounded hover:bg-green-700 text-xs"
                on:click={copyOutput}
                title="Copy output"
              >
                📋 Copy
              </button>
              <pre
                class="text-sm font-mono text-gray-700 overflow-x-auto">{outputText}</pre>
            </div>
          </div>

          <PaletteAnalysis
            matrix={analysis?.normal?.differenceMatrix ?? []}
            labels={palette.map((c) => c.hex)}
          />
        </div>
      {:else if moduleLoaded}
        <div class="bg-white rounded-lg shadow-sm border p-12 text-center">
          <div class="text-gray-400 text-6xl mb-4">🎨</div>
          <h2 class="text-xl font-semibold text-gray-900 mb-2">
            Ready to Generate
          </h2>
          <p class="text-gray-600">
            Adjust the parameters in the sidebar and click "Generate New
            Palette" to get started.
          </p>
        </div>
      {/if}
    </main>
  </div>

  <!-- Toast Notification -->
  {#if toast.show}
    <div
      class="fixed bottom-4 right-4 bg-gray-800 text-white px-4 py-3 rounded-lg shadow-lg z-50 transition-all duration-300"
    >
      <div class="flex items-center">
        <span class="mr-2">✓</span>
        {toast.message}
      </div>
    </div>
  {/if}
</div>

<style>
  .slider::-webkit-slider-thumb {
    appearance: none;
    height: 20px;
    width: 20px;
    border-radius: 50%;
    background: #3b82f6;
    cursor: pointer;
    border: 2px solid white;
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
  }

  .slider::-moz-range-thumb {
    height: 20px;
    width: 20px;
    border-radius: 50%;
    background: #3b82f6;
    cursor: pointer;
    border: 2px solid white;
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
  }

  .form-group {
    margin-bottom: 1rem;
  }
</style>
