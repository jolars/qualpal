<script lang="ts">
  import { sliderKeyValue } from "./sliderKeyValue";

  export let lightMin = 0;
  export let lightMax = 1;
  export let onChange: (values: {
    lightMin: number;
    lightMax: number;
  }) => void = () => {};

  const width = 200;
  const height = 20;
  const handleRadius = 8;
  const padding = handleRadius; // Add padding equal to handle radius

  function valueToX(value: number) {
    return padding + value * (width - 2 * padding);
  }

  function xToValue(x: number) {
    return Math.max(0, Math.min(1, (x - padding) / (width - 2 * padding)));
  }

  function handleDrag(type: "min" | "max", event: PointerEvent) {
    const target = event.currentTarget as SVGCircleElement;
    if (!target.hasPointerCapture(event.pointerId)) return;

    const svg = target.ownerSVGElement;
    if (!svg) return;

    const rect = svg.getBoundingClientRect();
    const x = event.clientX - rect.left;
    updateValue(type, xToValue(x));
  }

  function updateValue(type: "min" | "max", value: number) {
    const rounded = Math.round(value * 10) / 10;
    onChange(
      type === "min" ? { lightMin: rounded, lightMax } : { lightMin, lightMax: rounded },
    );
  }

  function handleKey(type: "min" | "max", event: KeyboardEvent) {
    const current = type === "min" ? lightMin : lightMax;
    const value = sliderKeyValue(event.key, current, 1, 0.1);
    if (value === undefined) return;

    event.preventDefault();
    updateValue(type, value);
  }
</script>

<div class="flex justify-center">
  <svg {width} height={height + 20} style="touch-action:none;">
    <defs>
      <linearGradient id="lightness-gradient" x1="0%" y1="0%" x2="100%" y2="0%">
        <stop offset="0%" stop-color="#000000" />
        <stop offset="100%" stop-color="#ffffff" />
      </linearGradient>
    </defs>

    <!-- Background gradient bar -->
    <rect
      x={padding}
      y="10"
      width={width - 2 * padding}
      {height}
      fill="url(#lightness-gradient)"
      rx="4"
    />

    <!-- Selection range overlay -->
    <rect
      x={valueToX(lightMin)}
      y="10"
      width={valueToX(lightMax) - valueToX(lightMin)}
      {height}
      fill="rgba(255,255,255,0.3)"
      stroke="rgba(0,0,0,0.5)"
      stroke-width="1"
      rx="4"
    />

    <!-- Min handle -->
    <path
      d={`M${valueToX(lightMin) - 8},2 L${valueToX(lightMin) + 8},2 L${valueToX(lightMin)},18 Z`}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="pointer-events: none;"
    />
    <!-- Larger invisible touch target -->
    <circle
      cx={valueToX(lightMin)}
      cy="10"
      r="16"
      fill="transparent"
      style="cursor: pointer; touch-action: none;"
      role="slider"
      tabindex="0"
      aria-label="Lightness minimum"
      aria-valuemin="0"
      aria-valuemax="1"
      aria-valuenow={lightMin}
      on:keydown={(event) => handleKey("min", event)}
      on:pointerdown={(event) => event.currentTarget.setPointerCapture(event.pointerId)}
      on:pointermove={(event) => handleDrag("min", event)}
    />

    <!-- Max handle -->
    <path
      d={`M${valueToX(lightMax) - 8},2 L${valueToX(lightMax) + 8},2 L${valueToX(lightMax)},18 Z`}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="pointer-events: none;"
    />
    <!-- Larger invisible touch target -->
    <circle
      cx={valueToX(lightMax)}
      cy="10"
      r="16"
      fill="transparent"
      style="cursor: pointer; touch-action: none;"
      role="slider"
      tabindex="0"
      aria-label="Lightness maximum"
      aria-valuemin="0"
      aria-valuemax="1"
      aria-valuenow={lightMax}
      on:keydown={(event) => handleKey("max", event)}
      on:pointerdown={(event) => event.currentTarget.setPointerCapture(event.pointerId)}
      on:pointermove={(event) => handleDrag("max", event)}
    />
  </svg>
</div>
<div class="flex justify-between mt-2 text-sm">
  <span>Min: {lightMin}</span>
  <span>Max: {lightMax}</span>
</div>

<style>
  circle:focus-visible {
    fill: currentColor;
    fill-opacity: 0.15;
  }
</style>
