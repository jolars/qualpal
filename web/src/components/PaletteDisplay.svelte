<script lang="ts">
  import { showToast } from "./../stores/toast.js";
  import { simulateColor, cvdSimulation } from "../stores/cvdStore.js";

  let { palette, paletteParams } = $props();

  function isDarkColor(hex: string): boolean {
    const c = hex.replace("#", "");
    const r = parseInt(c.substring(0, 2), 16);
    const g = parseInt(c.substring(2, 4), 16);
    const b = parseInt(c.substring(4, 6), 16);
    return 0.299 * r + 0.587 * g + 0.114 * b < 128;
  }

  async function copyColor(hex: string) {
    try {
      await navigator.clipboard.writeText(hex);
      showToast(`Copied ${hex}`);
    } catch (error) {
      console.error("Failed to copy to clipboard:", error);
      showToast("Failed to copy");
    }
  }

  const displayedPalette = $derived(() => {
    const simEnabled = $cvdSimulation.enabled;
    return $palette.map((color) => {
      const simulated = simEnabled ? simulateColor(color.hex) : color.hex;
      return {
        ...color,
        originalHex: color.hex,
        displayHex: simulated,
      };
    });
  });
</script>

<div class="flex items-center justify-between mb-4">
  <h2 class="text-xl font-semibold text-gray-900">Generated Palette</h2>
  <span class="text-sm text-gray-500">{$palette.length} colors</span>
</div>

<div
  class="p-4 rounded-lg border-1 border-gray-200"
  style="background-color: {$paletteParams.backgroundColor}"
>
  <div
    class="grid grid-cols-3 sm:grid-cols-4 lg:grid-cols-5 xl:grid-cols-6 gap-4"
  >
    {#each displayedPalette() as color}
      <div class="group">
        <button
          class="aspect-square rounded-lg cursor-pointer border-2 border-gray-200 hover:border-gray-400 transition-all duration-200 hover:shadow-md relative overflow-hidden w-full"
          style="background-color: {color.displayHex}"
          onclick={() => copyColor(color.originalHex)}
          onkeydown={(e) => e.key === "Enter" && copyColor(color.originalHex)}
          title="Click to copy {color.originalHex}"
          aria-label="Copy color {color.originalHex}"
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
            style="color: {isDarkColor($paletteParams.backgroundColor)
              ? '#fff'
              : '#222'}"
          >
            {color.originalHex}
          </div>
        </div>
      </div>
    {/each}
  </div>
</div>
