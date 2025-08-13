<script lang="ts">
  import HueWheel from "./params/HueWheel.svelte";
  import LightnessSlider from "./params/LightnessSlider.svelte";
  import SaturationSlider from "./params/SaturationSlider.svelte";
  import ExtendPalette from "./params/ExtendPalette.svelte";
  import { showToast } from "../stores/toast.js";
  import {
    paletteParams,
    moduleLoaded,
    debouncedGenerate,
    availablePalettes,
    getPaletteHex,
  } from "../stores/paletteStore.js";

  let {
    selectedDomain,
    setSelectedDomain,
    selectedPalette,
    setSelectedPalette,
  } = $props();

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

  function setInputMode(mode: "colorspace" | "fixed") {
    $paletteParams.inputMode = mode;
    debouncedGenerate($paletteParams);
  }
  function clearFixedInput() {
    $paletteParams.fixedInput = "";
    debouncedGenerate($paletteParams);
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
  const fixedCandidates = $derived(
    $paletteParams.inputMode === "fixed"
      ? parseFixedCandidates($paletteParams.fixedInput)
      : [],
  );
</script>

<aside class="w-full sm:w-72 bg-gray-100 p-4 md:border-r md:border-gray-200">
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
              oninput={() => debouncedGenerate($paletteParams)}
              class="w-full h-2 bg-gray-200 rounded-lg appearance-none cursor-pointer slider"
            />
            <div class="flex justify-between text-xs text-gray-500 mt-1">
              <span>2</span>
              <span>12</span>
            </div>
          </div>
        </div>

        <!-- Input -->
        <div class="bg-gray-50 p-4 rounded-lg">
          <h3 class="font-medium text-gray-900 mb-3">Input Mode</h3>
          <div class="flex gap-2 mb-4">
            <button
              class="px-3 py-1 text-sm rounded border transition cursor-pointer
              {$paletteParams.inputMode === 'colorspace'
                ? 'bg-blue-600 text-white border-blue-600'
                : 'bg-white text-gray-700 border-gray-300 hover:bg-gray-100'}"
              onclick={() => setInputMode("colorspace")}
            >
              Colorspace
            </button>
            <button
              class="px-3 py-1 text-sm rounded border transition cursor-pointer
              {$paletteParams.inputMode === 'fixed'
                ? 'bg-blue-600 text-white border-blue-600'
                : 'bg-white text-gray-700 border-gray-300 hover:bg-gray-100'}"
              onclick={() => setInputMode("fixed")}
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
                    onchange={(e) =>
                      setSelectedDomain((e.target as HTMLSelectElement).value)}
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
                    onchange={(e) =>
                      setSelectedPalette((e.target as HTMLSelectElement).value)}
                  >
                    <option value={null}>Select palette</option>
                    {#each paletteList as p}
                      <option value={p}>{p}</option>
                    {/each}
                  </select>
                </div>
                <div class="flex gap-2">
                  <button
                    class="px-2 py-1 text-xs rounded border border-gray-300 bg-white hover:bg-gray-100 disabled:opacity-50 cursor-pointer"
                    disabled={!selectedDomain || !selectedPalette}
                    onclick={() => addBuiltInPalette(false)}
                  >
                    Add
                  </button>
                  <button
                    class="px-2 py-1 text-xs rounded border border-gray-300 bg-white hover:bg-gray-100 disabled:opacity-50 cursor-pointer"
                    disabled={!selectedDomain || !selectedPalette}
                    onclick={() => addBuiltInPalette(true)}
                  >
                    Replace
                  </button>
                  <button
                    class="px-2 py-1 text-xs rounded border border-gray-300 bg-white hover:bg-gray-100 disabled:opacity-50 cursor-pointer"
                    onclick={() => {
                      clearFixedInput();
                      showToast("Cleared fixed candidates");
                    }}
                  >
                    Clear
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
                  oninput={() => debouncedGenerate($paletteParams)}
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
                <p class="text-xs text-red-600">No valid hex colors found.</p>
              {/if}
              <p class="text-xs text-gray-500">
                Palette will be chosen from these candidates (built-in +
                pasted). Empty falls back to colorspace.
              </p>
              {#if $paletteParams.inputMode === "fixed" && fixedCandidates.length > 0 && $paletteParams.numColors > fixedCandidates.length}
                <p class="text-xs text-red-600 font-semibold mb-1">
                  Number of colors requested ({$paletteParams.numColors})
                  exceeds number of fixed candidates ({fixedCandidates.length}).
                </p>
              {/if}
            </div>
          {/if}

          {#if $paletteParams.inputMode === "colorspace"}
            <!-- Hue -->
            <h3 class="font-medium text-gray-900 mb-3">Hue</h3>
            <HueWheel
              hueMin={$paletteParams.hueMin}
              hueMax={$paletteParams.hueMax}
              onChange={({ hueMin, hueMax }) => {
                $paletteParams.hueMin = hueMin;
                $paletteParams.hueMax = hueMax;
                debouncedGenerate();
              }}
            />

            <!-- Saturation -->
            <h3 class="font-medium text-sm text-gray-900 mb-3 mt-5">
              Saturation
            </h3>
            <SaturationSlider
              satMin={$paletteParams.satMin}
              satMax={$paletteParams.satMax}
              onChange={({ satMin, satMax }) => {
                $paletteParams.satMin = satMin;
                $paletteParams.satMax = satMax;
                debouncedGenerate();
              }}
            />

            <!-- Lightness -->
            <h3 class="font-medium text-gray-900 mb-3 mt-5">Lightness</h3>
            <LightnessSlider
              lightMin={$paletteParams.lightMin}
              lightMax={$paletteParams.lightMax}
              onChange={({ lightMin, lightMax }) => {
                $paletteParams.lightMin = lightMin;
                $paletteParams.lightMax = lightMax;
                debouncedGenerate();
              }}
            />
          {/if}
        </div>

        <!-- Background Color -->
        <div class="bg-gray-50 p-4 rounded-lg">
          <h3 class="font-medium text-gray-900 mb-3">Background Color</h3>

          <div class="flex items-center gap-2">
            <input
              type="color"
              bind:value={$paletteParams.backgroundColor}
              onchange={() => debouncedGenerate($paletteParams)}
              class="w-1/2 h-8 rounded border border-gray-300 cursor-pointer"
              title="Select background color"
            />
            <input
              type="text"
              bind:value={$paletteParams.backgroundColor}
              oninput={() => debouncedGenerate($paletteParams)}
              class="w-1/2 px-2 py-1 text-sm border border-gray-300 rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
              placeholder="#ffffff"
              pattern="^#[0-9A-Fa-f]{6}$"
            />
          </div>

          <div class="mt-3">
            <label class="flex items-center gap-2 cursor-pointer">
              <input
                type="checkbox"
                bind:checked={$paletteParams.useBackground}
                onchange={() => debouncedGenerate($paletteParams)}
                class="rounded border-gray-300 text-blue-600 focus:ring-blue-500"
              />
              <span class="text-sm text-gray-700"
                >Optimize for background color</span
              >
            </label>
          </div>
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
                  oninput={() => debouncedGenerate($paletteParams)}
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
                  oninput={() => debouncedGenerate($paletteParams)}
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
                  oninput={() => debouncedGenerate($paletteParams)}
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
</style>
