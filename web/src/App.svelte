<script lang="ts">
  import { onMount } from "svelte";
  import { toast, showToast } from "./stores/toast.js";
  import {
    paletteParams,
    palette,
    analysis,
    moduleLoaded,
    initializeModule,
    generatePalette,
  } from "./stores/paletteStore.js";

  import Examples from "./components/Examples.svelte";
  import PaletteAnalysis from "./components/PaletteAnalysis.svelte";
  import PaletteOutput from "./components/PaletteOutput.svelte";
  import ParameterSidebar from "./components/ParameterSidebar.svelte";
  import Toast from "./components/Toast.svelte";
  import Footer from "./components/Footer.svelte";
  import AboutModal from "./components/AboutModal.svelte";

  let selectedDomain = $state<string | null>(null);
  let selectedPalette = $state<string | null>(null);

  let showAbout = $state<boolean>(false);

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
      setSelectedDomain={(d: any) => (selectedDomain = d)}
      {selectedPalette}
      setSelectedPalette={(p: any) => (selectedPalette = p)}
    />

    <!-- Main Content -->
    <main class="flex-1 p-4">
      <div class="space-y-6">
        {#if $moduleLoaded}
          <!-- Palette Display -->
          <div class="flex items-center justify-between mb-4">
            <h2 class="text-xl font-semibold text-gray-900">
              Generated Palette
            </h2>
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

          <Examples
            palette={$palette}
            useBackground={$paletteParams.useBackground}
            backgroundColor={$paletteParams.backgroundColor}
          />

          <PaletteOutput
            {OUTPUT_TABS}
            {activeTab}
            setActiveTab={(tab: any) => (activeTab = tab)}
            {outputText}
            oncopied={() => showToast("Copied output")}
          />

          <PaletteAnalysis
            matrix={$analysis?.normal?.differenceMatrix ?? []}
            labels={$palette?.length > 0 ? $palette.map((c) => c.hex) : []}
            minDistances={$analysis?.normal?.minDistances ?? []}
          />
        {:else}
          <div class="flex items-center justify-center min-h-screen">
            <div class="text-center">
              <div
                class="animate-spin h-8 w-8 border-4 border-blue-500 border-t-transparent rounded-full mx-auto mb-4"
              ></div>
              <p class="text-gray-600">Loading Qualpal...</p>
            </div>
          </div>
        {/if}
      </div>
    </main>
  </div>

  <Footer on:about={() => (showAbout = true)} />

  <Toast show={$toast.show} message={$toast.message} />

  <AboutModal open={showAbout} onclose={() => (showAbout = false)} />
</div>
