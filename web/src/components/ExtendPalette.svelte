<script lang="ts">
  import {
    paletteParams,
    debouncedGenerate,
  } from "./../stores/paletteStore.js";
  import { get } from "svelte/store";

  // Local parser (previously in App.svelte)
  function parseExistingPalette(input: string): string[] {
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
    const result: string[] = [];
    for (const c of candidates) {
      const up = c.toUpperCase();
      if (/^#[0-9A-F]{6}$/.test(up) && !seen.has(up)) {
        seen.add(up);
        result.push(up);
      }
    }
    return result;
  }

  $: parsedColors =
    $paletteParams.useExtend && $paletteParams.existingPalette.trim()
      ? parseExistingPalette($paletteParams.existingPalette)
      : [];

  function onInput() {
    // Trigger palette regeneration (debounced)
    debouncedGenerate(get(paletteParams));
  }
</script>

<div class="bg-gray-50 p-4 rounded-lg">
  <h3 class="font-medium text-gray-900 mb-3">Extend Palette</h3>

  <div class="mb-3">
    <label class="flex items-center gap-2 cursor-pointer">
      <input
        type="checkbox"
        bind:checked={$paletteParams.useExtend}
        on:change={onInput}
        class="rounded border-gray-300 text-blue-600 focus:ring-blue-500"
      />
      <span class="text-sm text-gray-700">Extend existing palette</span>
    </label>
  </div>

  {#if $paletteParams.useExtend}
    <div class="space-y-3">
      <div>
        <label for="existing-palette" class="block text-xs text-gray-600 mb-1"
          >Existing colors (hex values, any format / pasted list)</label
        >
        <textarea
          id="existing-palette"
          bind:value={$paletteParams.existingPalette}
          on:input={onInput}
          placeholder="#ff0000, #00ff00, #0000ff&#10;or&#10;#ff0000 #00ff00 #0000ff"
          class="w-full px-2 py-2 text-sm border border-gray-300 rounded focus:outline-none focus:ring-2 focus:ring-blue-500 resize-none"
          rows="3"
        ></textarea>
      </div>

      {#if $paletteParams.existingPalette.trim()}
        {#if parsedColors.length > 0}
          <div>
            <p class="text-xs text-gray-600 mb-2">
              Existing colors ({parsedColors.length})
            </p>
            <div class="flex flex-wrap gap-1">
              {#each parsedColors as color}
                <div
                  class="w-6 h-6 rounded border border-gray-300"
                  style="background-color: {color}"
                  title={color}
                ></div>
              {/each}
            </div>
          </div>
        {:else}
          <p class="text-xs text-red-600">
            No valid hex colors found. Use format: #ff0000
          </p>
        {/if}
      {/if}
    </div>

    <p class="text-xs text-gray-500 mt-2">
      New colors will be added while preserving the existing ones.
    </p>
  {:else}
    <p class="text-xs text-gray-500">Generate a completely new palette</p>
  {/if}
</div>
