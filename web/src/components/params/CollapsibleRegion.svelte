<script lang="ts">
  import HueWheel from "./HueWheel.svelte";
  import SaturationSlider from "./SaturationSlider.svelte";
  import LightnessSlider from "./LightnessSlider.svelte";

  export let region: {
    hueMin: number;
    hueMax: number;
    satMin: number;
    satMax: number;
    lightMin: number;
    lightMax: number;
  };
  export let index: number;
  export let isOpen: boolean = false;
  export let canRemove: boolean = false;
  export let onToggle: () => void;
  export let onRemove: () => void;
  export let onChange: (updates: Partial<typeof region>) => void;

  function formatRange(min: number, max: number, decimals: number = 0): string {
    return `${min.toFixed(decimals)}-${max.toFixed(decimals)}`;
  }

  function getHueMidpoint(hueMin: number, hueMax: number): number {
    // If hueMin > hueMax, the range wraps around 0°
    if (hueMin > hueMax) {
      // Wrapping case: go from hueMin through 360/0 to hueMax
      const arcLength = 360 - hueMin + hueMax;
      return (hueMin + arcLength / 2) % 360;
    } else {
      // Normal case: go directly from hueMin to hueMax
      return (hueMin + hueMax) / 2;
    }
  }
</script>

<div class="border rounded bg-white shadow-sm">
  <!-- Header with collapse/expand -->
  <div class="flex items-center overflow-hidden">
    <button
      onclick={onToggle}
      class="flex-1 px-3 py-2 flex items-center gap-2 hover:bg-gray-50 transition text-left min-w-0"
    >
      <svg
        class="w-3 h-3 transition-transform flex-shrink-0 {isOpen
          ? 'rotate-90'
          : ''}"
        fill="currentColor"
        viewBox="0 0 20 20"
      >
        <path
          fill-rule="evenodd"
          d="M7.293 14.707a1 1 0 010-1.414L10.586 10 7.293 6.707a1 1 0 011.414-1.414l4 4a1 1 0 010 1.414l-4 4a1 1 0 01-1.414 0z"
          clip-rule="evenodd"
        />
      </svg>
      <span class="font-medium text-sm flex-shrink-0">Region {index + 1}</span>
      {#if !isOpen}
        <span class="text-xs text-gray-500 truncate min-w-0">
          H {formatRange(region.hueMin, region.hueMax)}°, S {formatRange(
            region.satMin,
            region.satMax,
            1,
          )}, L {formatRange(region.lightMin, region.lightMax, 1)}
        </span>
        <!-- Mini color preview gradient (clockwise from hueMin to hueMax) -->
        <div
          class="h-3 w-12 rounded border border-gray-200 flex-shrink-0"
          style="background: linear-gradient(90deg, 
                 hsl({region.hueMin}, {region.satMax * 100}%, 50%), 
                 hsl({getHueMidpoint(
            region.hueMin,
            region.hueMax,
          )}, {region.satMax * 100}%, 50%),
                 hsl({region.hueMax}, {region.satMax * 100}%, 50%))"
        ></div>
      {/if}
    </button>

    {#if canRemove}
      <button
        onclick={onRemove}
        class="text-red-600 text-lg px-3 py-2 hover:bg-red-50 transition border-l leading-none"
        title="Remove this region"
      >
        ×
      </button>
    {/if}
  </div>

  <!-- Expanded content -->
  {#if isOpen}
    <div class="px-3 pb-3 space-y-4 border-t pt-3">
      <div>
        <label
          for="region-{index}-hue"
          class="text-xs font-medium text-gray-700 block mb-2">Hue</label
        >
        <HueWheel
          hueMin={region.hueMin}
          hueMax={region.hueMax}
          onChange={({ hueMin, hueMax }) => onChange({ hueMin, hueMax })}
        />
      </div>

      <div>
        <label
          for="region-{index}-saturation"
          class="text-xs font-medium text-gray-700 block mb-2">Saturation</label
        >
        <SaturationSlider
          satMin={region.satMin}
          satMax={region.satMax}
          onChange={({ satMin, satMax }) => onChange({ satMin, satMax })}
        />
      </div>

      <div>
        <label
          for="region-{index}-lightness"
          class="text-xs font-medium text-gray-700 block mb-2">Lightness</label
        >
        <LightnessSlider
          lightMin={region.lightMin}
          lightMax={region.lightMax}
          onChange={({ lightMin, lightMax }) =>
            onChange({ lightMin, lightMax })}
        />
      </div>
    </div>
  {/if}
</div>
