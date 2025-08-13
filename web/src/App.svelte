<script lang="ts">
  import { onMount } from "svelte";
  import HueWheel from "./components/HueWheel.svelte";
  import LightnessSlider from "./components/LightnessSlider.svelte";
  import SaturationSlider from "./components/SaturationSlider.svelte";
  import Examples from "./components/Examples.svelte";
  import PaletteAnalysis from "./components/PaletteAnalysis.svelte";
  import Toast from "./components/Toast.svelte";
  import {
    paletteParams,
    palette,
    analysis,
    moduleLoaded,
    initializeModule,
    generatePalette,
    debouncedGenerate,
    availablePalettes,
    getPaletteHex,
  } from "./stores/paletteStore.js";
  import ExtendPalette from "./components/ExtendPalette.svelte";
  import Footer from "./components/Footer.svelte";

  let selectedDomain: string | null = null;
  let selectedPalette: string | null = null;

  // About modal state & citation
  let showAbout = false;
  const citationBibtex = `@software{
    qualpal,
	location = {Lund, Sweden},
	title = {qualpal},
	rights = {{MIT}},
	url = {https://github.com/jolars/qualpal},
	abstract = {Qualitative color palettes},
	version = {2.4.0},
	author = {Larsson, Johan},
	urldate = {2025-08-12},
	date = {2025-08-12},
	note = {original-date: 2023-11-01T20:52:14Z},
}`;

  async function copyCitation() {
    try {
      await navigator.clipboard.writeText(citationBibtex);
      toast.message = "Citation copied";
      toast.show = true;
      setTimeout(() => (toast.show = false), 2000);
    } catch {
      /* noop */
    }
  }

  $: domainList = Object.keys($availablePalettes);
  $: paletteList = selectedDomain
    ? $availablePalettes[selectedDomain] || []
    : [];

  function addBuiltInPalette(replace = false) {
    if (!selectedDomain || !selectedPalette) return;
    const colors = getPaletteHex(selectedDomain, selectedPalette);
    if (!colors || colors.length === 0) return;
    const existing = replace
      ? []
      : $paletteParams.fixedInput.match(/#[0-9A-Fa-f]{6}/g) || [];
    const merged = Array.from(
      new Set([...existing, ...colors.map((c) => c.toUpperCase())]),
    );
    $paletteParams.fixedInput = merged.join("\n");
    debouncedGenerate($paletteParams);
  }

  // Toast notification
  let toast = {
    show: false,
    message: "",
  };

  // Initialize the module
  onMount(async () => {
    const success = await initializeModule();
    if (success) {
      generatePalette($paletteParams);
    }
  });

  function isDarkColor(hex: string): boolean {
    const c = hex.replace("#", "");
    const r = parseInt(c.substring(0, 2), 16);
    const g = parseInt(c.substring(2, 4), 16);
    const b = parseInt(c.substring(4, 6), 16);
    return 0.299 * r + 0.587 * g + 0.114 * b < 128;
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
    const hexColors = $palette.map((color) => color.hex);
    switch (activeTab) {
      case "JSON":
        return JSON.stringify(hexColors, null, 2);
      case "R":
        return `c(\n${hexColors.map((h) => `  "${h}"`).join(",\n")}\n)`;
      case "Python":
        return `[\n${hexColors.map((h) => `  "${h}"`).join(",\n")}\n]`;
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

  function parseFixedCandidates(input: string): string[] {
    if (!input || !input.trim()) return [];
    const raw = input.trim();
    const hexMatches = raw.match(/#[0-9A-Fa-f]{6}/g) || [];
    let candidates: string[];
    if (hexMatches.length > 0) {
      candidates = hexMatches;
    } else {
      candidates = raw
        .split(/[,;\s\n]+/)
        .map((c) => c.trim())
        .filter(Boolean);
    }
    const seen = new Set<string>();
    const out: string[] = [];
    for (const c of candidates) {
      const up = c.toUpperCase();
      if (/^#[0-9A-F]{6}$/.test(up) && !seen.has(up)) {
        seen.add(up);
        out.push(up);
      }
    }
    return out;
  }
  $: fixedCandidates =
    $paletteParams.inputMode === "fixed"
      ? parseFixedCandidates($paletteParams.fixedInput)
      : [];
</script>

<div>
  <header class="border-g">
    <div class="max-w-7xl mx-auto px-4 py-4 border-gray-200 border-b">
      <h1 class="text-2xl font-bold text-gray-900">
        <span class="text-blue-600">Qualpal</span> Color Palette Generator
      </h1>
      <p class="text-gray-600 mt-1">
        Generate perceptually uniform color palettes for data visualization
      </p>
    </div>
  </header>

  <div class="flex flex-col md:flex-row min-h-screen">
    <!-- Sidebar -->
    <aside
      class="w-full md:w-64 bg-gray-100 p-4 md:border-r md:border-gray-200"
    >
      <div class="sticky top-6 border-g">
        <h2 class="text-lg font-semibold text-gray-900 mb-4">Parameters</h2>

        {#if !$moduleLoaded}
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
                    >{$paletteParams.numColors}</span
                  >
                </label>
                <input
                  id="numColors"
                  type="range"
                  min="2"
                  max="12"
                  bind:value={$paletteParams.numColors}
                  on:input={() => debouncedGenerate($paletteParams)}
                  class="w-full h-2 bg-gray-200 rounded-lg appearance-none cursor-pointer slider"
                />
                <div class="flex justify-between text-xs text-gray-500 mt-1">
                  <span>2</span>
                  <span>12</span>
                </div>
              </div>
            </div>

            <!-- Input Mode -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <h3 class="text-sm font-medium text-gray-900 mb-3">Input Mode</h3>
              <div class="flex gap-2 mb-4">
                <button
                  class="px-3 py-1 text-sm rounded border transition
                  {$paletteParams.inputMode === 'colorspace'
                    ? 'bg-blue-600 text-white border-blue-600'
                    : 'bg-white text-gray-700 border-gray-300 hover:bg-gray-100'}"
                  on:click={() => {
                    $paletteParams.inputMode = "colorspace";
                    debouncedGenerate($paletteParams);
                  }}
                >
                  Colorspace
                </button>
                <button
                  class="px-3 py-1 text-sm rounded border transition
                  {$paletteParams.inputMode === 'fixed'
                    ? 'bg-blue-600 text-white border-blue-600'
                    : 'bg-white text-gray-700 border-gray-300 hover:bg-gray-100'}"
                  on:click={() => {
                    $paletteParams.inputMode = "fixed";
                    debouncedGenerate($paletteParams);
                  }}
                >
                  Fixed Set
                </button>
              </div>

              {#if $paletteParams.inputMode === "fixed"}
                <div class="space-y-4">
                  <!-- Built-in palette picker -->
                  <div class="flex flex-col gap-2">
                    <div class="flex flex-col gap-2">
                      <select
                        class="border rounded px-2 py-1 text-sm w-full min-w-0"
                        bind:value={selectedDomain}
                        on:change={() => {
                          selectedPalette = null;
                        }}
                      >
                        <option value={null}>Select domain</option>
                        {#each domainList as d}
                          <option value={d}>{d}</option>
                        {/each}
                      </select>
                      <select
                        class="border rounded px-2 py-1 text-sm w-full min-w-0"
                        bind:value={selectedPalette}
                        disabled={!selectedDomain}
                      >
                        <option value={null}>Select palette</option>
                        {#each paletteList as p}
                          <option value={p}>{p}</option>
                        {/each}
                      </select>
                    </div>
                    <div class="flex gap-2">
                      <button
                        class="px-2 py-1 text-xs rounded border border-gray-300 bg-white hover:bg-gray-100 disabled:opacity-50"
                        disabled={!selectedDomain || !selectedPalette}
                        on:click={() => addBuiltInPalette(false)}
                      >
                        Add
                      </button>
                      <button
                        class="px-2 py-1 text-xs rounded border border-gray-300 bg-white hover:bg-gray-100 disabled:opacity-50"
                        disabled={!selectedDomain || !selectedPalette}
                        on:click={() => addBuiltInPalette(true)}
                      >
                        Replace
                      </button>
                    </div>
                  </div>

                  <div>
                    <label
                      for="fixed-candidates"
                      class="block text-xs text-gray-600 mb-1"
                    >
                      Candidate colors (paste or add built-in)
                    </label>
                    <textarea
                      id="fixed-candidates"
                      bind:value={$paletteParams.fixedInput}
                      on:input={() => debouncedGenerate($paletteParams)}
                      placeholder="#FF0000, #00FF00 ... (hex codes auto-detected)"
                      class="w-full px-2 py-2 text-sm border border-gray-300 rounded focus:outline-none focus:ring-2 focus:ring-blue-500 resize-none"
                      rows="4"
                    ></textarea>
                  </div>
                  {#if fixedCandidates.length > 0}
                    <div>
                      <p class="text-xs text-gray-600 mb-1">
                        {fixedCandidates.length} candidate {fixedCandidates.length ===
                        1
                          ? "color"
                          : "colors"}
                      </p>
                      <div class="flex flex-wrap gap-1">
                        {#each fixedCandidates as c}
                          <div
                            class="w-6 h-6 rounded border border-gray-300"
                            style="background-color:{c}"
                            title={c}
                          ></div>
                        {/each}
                      </div>
                    </div>
                  {:else if $paletteParams.fixedInput.trim()}
                    <p class="text-xs text-red-600">
                      No valid hex colors found.
                    </p>
                  {/if}
                  <p class="text-xs text-gray-500">
                    Palette will be chosen from these candidates (built-in +
                    pasted). Empty falls back to colorspace.
                  </p>
                </div>
              {/if}
            </div>

            {#if $paletteParams.inputMode === "colorspace"}
              <!-- Hue Range -->
              <div class="bg-gray-50 p-4 rounded-lg">
                <h3 class="font-medium text-gray-900 mb-3">Hue Range</h3>
                <HueWheel
                  hueMin={$paletteParams.hueMin}
                  hueMax={$paletteParams.hueMax}
                  onChange={({ hueMin, hueMax }) => {
                    $paletteParams.hueMin = hueMin;
                    $paletteParams.hueMax = hueMax;
                    debouncedGenerate();
                  }}
                />
              </div>

              <!-- Saturation Range -->
              <div class="bg-gray-50 p-4 rounded-lg">
                <h3 class="font-medium text-gray-900 mb-3">Saturation Range</h3>
                <SaturationSlider
                  satMin={$paletteParams.satMin}
                  satMax={$paletteParams.satMax}
                  onChange={({ satMin, satMax }) => {
                    $paletteParams.satMin = satMin;
                    $paletteParams.satMax = satMax;
                    debouncedGenerate();
                  }}
                />
              </div>

              <!-- Lightness Range -->
              <div class="bg-gray-50 p-4 rounded-lg">
                <h3 class="font-medium text-gray-900 mb-3">Lightness Range</h3>
                <LightnessSlider
                  lightMin={$paletteParams.lightMin}
                  lightMax={$paletteParams.lightMax}
                  onChange={({ lightMin, lightMax }) => {
                    $paletteParams.lightMin = lightMin;
                    $paletteParams.lightMax = lightMax;
                    debouncedGenerate();
                  }}
                />
              </div>
            {/if}

            <!-- Background Color -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <h3 class="font-medium text-gray-900 mb-3">Background Color</h3>

              <div class="mb-3">
                <label class="flex items-center gap-2 cursor-pointer">
                  <input
                    type="checkbox"
                    bind:checked={$paletteParams.useBackground}
                    on:change={() => debouncedGenerate($paletteParams)}
                    class="rounded border-gray-300 text-blue-600 focus:ring-blue-500"
                  />
                  <span class="text-sm text-gray-700"
                    >Optimize for background color</span
                  >
                </label>
              </div>

              {#if $paletteParams.useBackground}
                <div class="flex items-center gap-2">
                  <input
                    type="color"
                    bind:value={$paletteParams.backgroundColor}
                    on:change={() => debouncedGenerate($paletteParams)}
                    class="w-1/3 h-8 rounded border border-gray-300 cursor-pointer"
                    title="Select background color"
                  />
                  <input
                    type="text"
                    bind:value={$paletteParams.backgroundColor}
                    on:input={() => debouncedGenerate($paletteParams)}
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

            <!-- Extend Palette -->
            <ExtendPalette />

            <!-- CVD Settings -->
            <div class="bg-gray-50 p-4 rounded-lg">
              <h3 class="font-medium text-gray-900 mb-3">
                Color Vision Deficiency
              </h3>
              <div class="space-y-3">
                <div class="max-w-xs w-full">
                  <label for="protan-slider" class="text-sm block mb-1"
                    >Protanopia</label
                  >
                  <div class="flex items-center gap-2">
                    <input
                      id="protan-slider"
                      type="range"
                      min="0"
                      max="1"
                      step="0.01"
                      bind:value={$paletteParams.cvd.protan}
                      on:input={() => debouncedGenerate($paletteParams)}
                      class="flex-1 w-full"
                    />
                    <span class="text-xs w-8 text-right"
                      >{$paletteParams.cvd.protan}</span
                    >
                  </div>
                </div>
                <div class="max-w-xs w-full">
                  <label for="deutan-slider" class="text-sm block mb-1"
                    >Deuteranopia</label
                  >
                  <div class="flex items-center gap-2">
                    <input
                      id="deutan-slider"
                      type="range"
                      min="0"
                      max="1"
                      step="0.01"
                      bind:value={$paletteParams.cvd.deutan}
                      on:input={() => debouncedGenerate($paletteParams)}
                      class="flex-1 w-full"
                    />
                    <span class="text-xs w-8 text-right"
                      >{$paletteParams.cvd.deutan}</span
                    >
                  </div>
                </div>
                <div class="max-w-xs w-full">
                  <label for="tritan-slider" class="text-sm block mb-1"
                    >Tritanopia</label
                  >
                  <div class="flex items-center gap-2">
                    <input
                      id="tritan-slider"
                      type="range"
                      min="0"
                      max="1"
                      step="0.01"
                      bind:value={$paletteParams.cvd.tritan}
                      on:input={() => debouncedGenerate($paletteParams)}
                      class="flex-1 w-full"
                    />
                    <span class="text-xs w-8 text-right"
                      >{$paletteParams.cvd.tritan}</span
                    >
                  </div>
                </div>
                <p class="text-xs text-gray-500 mt-2">
                  Adapt palette to users with color vision deficiency.
                </p>
              </div>
            </div>
          </div>
        {/if}
      </div>
    </aside>

    <!-- Main Content -->
    <main class="flex-1 p-4">
      <div class="space-y-6">
        <!-- Palette Display -->
        <div class="flex items-center justify-between mb-4">
          <h2 class="text-xl font-semibold text-gray-900">Generated Palette</h2>
          <span class="text-sm text-gray-500">{$palette.length} colors</span>
        </div>

        <div
          class="p-4 rounded-lg border-2 border-dashed border-gray-200"
          style="background-color: {$paletteParams.useBackground
            ? $paletteParams.backgroundColor
            : '#ffffff'}"
        >
          <div class="grid grid-cols-3 lg:grid-cols-5 xl:grid-cols-6 gap-4">
            {#each $palette as color}
              <div class="group">
                <button
                  class="aspect-square rounded-lg cursor-pointer border-2 border-gray-200 hover:border-gray-400 transition-all duration-200 hover:shadow-md relative overflow-hidden w-full"
                  style="background-color: {color.hex}"
                  on:click={() => copyColor(color.hex)}
                  on:keydown={(e) => e.key === "Enter" && copyColor(color.hex)}
                  title="Click to copy {color.hex}"
                  aria-label="Copy color {color.hex}"
                >
                  <div
                    class="absolute inset-0 bg-black/0 group-hover:bg-black/10 transition-all duration-200 flex items-center justify-center"
                  >
                    <span
                      class="text-white opacity-0 group-hover:opacity-100 font-medium text-sm bg-black bg-black/50 px-2 py-1 rounded"
                    >
                      Copy
                    </span>
                  </div>
                </button>
                <div class="mt-2 text-center">
                  <div
                    class="text-sm font-mono"
                    style="color: {isDarkColor(
                      $paletteParams.useBackground
                        ? $paletteParams.backgroundColor
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

        <!-- Examples -->
        <Examples
          palette={$palette}
          useBackground={$paletteParams.useBackground}
          backgroundColor={$paletteParams.backgroundColor}
        />

        <!-- JSON Output -->
        <h3 class="text-xl text-left font-semibold text-gray-900 mb-4">
          Output
        </h3>
        <div class="mb-4 border-b flex gap-2 border-gray-200">
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
        <div
          class="relative bg-gray-50 rounded-lg p-4 mt-2 border border-gray-200"
        >
          <button
            class="absolute top-2 right-2 px-2 py-1 bg-gray-400 text-white rounded hover:bg-gray-700 text-xs"
            on:click={copyOutput}
            title="Copy output"
          >
            📋 Copy
          </button>
          <pre
            class="text-sm font-mono text-gray-700 overflow-x-auto">{outputText}</pre>
        </div>

        <PaletteAnalysis
          matrix={$analysis?.normal?.differenceMatrix ?? []}
          labels={$palette?.length > 0 ? $palette.map((c) => c.hex) : []}
          minDistances={$analysis?.normal?.minDistances ?? []}
        />
      </div>
    </main>
  </div>

  <Footer on:about={() => (showAbout = true)} />

  <!-- Toast Notification -->
  <Toast show={toast.show} message={toast.message} />
  {#if showAbout}
    <div
      class="fixed inset-0 z-50 flex items-start md:items-center justify-center bg-black/50 p-4"
      role="presentation"
      on:click={() => (showAbout = false)}
      on:keydown={(e) => e.key === "Escape" && (showAbout = false)}
    >
      <div
        class="bg-white rounded-lg shadow-lg w-full max-w-lg overflow-hidden"
        role="dialog"
        aria-modal="true"
        aria-labelledby="about-title"
        tabindex="-1"
        on:click|stopPropagation
        on:keydown|stopPropagation
      >
        <div class="px-5 py-4 border-b flex justify-between items-center">
          <h2 id="about-title" class="text-lg font-semibold">About Qualpal</h2>
          <button
            class="text-gray-500 hover:text-gray-700"
            aria-label="Close"
            on:click={() => (showAbout = false)}
          >
            ✕
          </button>
        </div>
        <div class="p-5 space-y-4 text-sm text-gray-700">
          <p>
            Qualpal generates perceptually uniform qualitative color palettes
            optimized for distinctness using DIN99d color difference. Configure
            a colorspace or provide fixed candidate colors, optionally extend an
            existing palette, and analyze accessibility impacts.
          </p>
          <div>
            <h3 class="font-medium mb-1">Citation</h3>
            <pre
              class="bg-gray-100 p-2 rounded text-xs overflow-x-auto"
              id="citation-block">{citationBibtex}</pre>
            <button
              class="mt-2 px-2 py-1 text-xs bg-blue-600 text-white rounded hover:bg-blue-700"
              on:click={copyCitation}
            >
              Copy BibTeX
            </button>
          </div>
          <p class="text-xs text-gray-500">
            © {new Date().getFullYear()} Qualpal. MIT License.
          </p>
        </div>
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
