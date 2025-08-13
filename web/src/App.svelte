<script lang="ts">
  import Examples from "./components/Examples.svelte";
  import HueWheel from "./components/HueWheel.svelte";
  import LightnessSlider from "./components/LightnessSlider.svelte";
  import PaletteAnalysis from "./components/PaletteAnalysis.svelte";
  import ParameterSidebar from "./components/ParameterSidebar.svelte";
  import SaturationSlider from "./components/SaturationSlider.svelte";
  import Toast from "./components/Toast.svelte";
  import { onMount } from "svelte";
  import { toast, showToast } from "./stores/toast.js";
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
  import AboutModal from "./components/AboutModal.svelte";
  import CopyButton from "./components/CopyButton.svelte";

  let selectedDomain = $state<string | null>(null);
  let selectedPalette = $state<string | null>(null);

  let showAbout = $state<boolean>(false);

  const domainList = $derived(Object.keys($availablePalettes));
  const paletteList = $derived(
    selectedDomain ? $availablePalettes[selectedDomain] || [] : [],
  );

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

  function setInputMode(mode: "colorspace" | "fixed") {
    $paletteParams.inputMode = mode;
    debouncedGenerate($paletteParams);
  }
  function clearFixedInput() {
    $paletteParams.fixedInput = "";
    debouncedGenerate($paletteParams);
  }

  // Output tab state
  const OUTPUT_TABS = ["JSON", "R", "Python", "CSS"] as const;
  type OutputTab = (typeof OUTPUT_TABS)[number];
  let activeTab = $state<OutputTab>("JSON");

  // Output text computed property
  const outputText = $derived(() => {
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
  });

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
  const fixedCandidates = $derived(
    $paletteParams.inputMode === "fixed"
      ? parseFixedCandidates($paletteParams.fixedInput)
      : [],
  );
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
    <ParameterSidebar
      {selectedDomain}
      setSelectedDomain={(d) => (selectedDomain = d)}
      {selectedPalette}
      setSelectedPalette={(p) => (selectedPalette = p)}
    />

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
                  onclick={() => copyColor(color.hex)}
                  onkeydown={(e) => e.key === "Enter" && copyColor(color.hex)}
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
          {#each OUTPUT_TABS as tab}
            <button
              class="
                    px-4 py-2 -mb-px border-b-2 font-mono text-sm transition-colors cursor-pointer
                    {activeTab === tab
                ? 'border-blue-600 text-blue-700 font-semibold'
                : 'border-transparent text-gray-500 hover:text-blue-600'}
                  "
              onclick={() => (activeTab = tab)}
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
          <CopyButton
            text={outputText()}
            ariaLabel="Copy output"
            title="Copy output"
            extraClass="absolute top-2 right-2 p-1"
            oncopied={() => showToast("Copied output")}
          />
          <pre
            class="text-sm font-mono text-gray-700 overflow-x-auto">{outputText()}</pre>
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

  <Toast show={$toast.show} message={$toast.message} />

  <AboutModal open={showAbout} onclose={() => (showAbout = false)} />
</div>
