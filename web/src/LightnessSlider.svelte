<script lang="ts">
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
    const target = event.target as SVGElement;
    const svg = target.ownerSVGElement || target.closest("svg");
    if (!svg) return;

    const rect = svg.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const value = xToValue(x);

    if (type === "min") {
      onChange({ lightMin: Math.round(value * 10) / 10, lightMax });
    } else {
      onChange({ lightMin, lightMax: Math.round(value * 10) / 10 });
    }
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
      style="cursor: pointer;"
      on:pointerdown={(event) => {
        const moveHandler = (ev: PointerEvent) => handleDrag("min", ev);
        const upHandler = () => {
          window.removeEventListener("pointermove", moveHandler);
          window.removeEventListener("pointerup", upHandler);
        };
        window.addEventListener("pointermove", moveHandler);
        window.addEventListener("pointerup", upHandler);
      }}
    />

    <!-- Max handle -->
    <path
      d={`M${valueToX(lightMax) - 8},2 L${valueToX(lightMax) + 8},2 L${valueToX(lightMax)},18 Z`}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="cursor: pointer;"
      on:pointerdown={(event) => {
        const moveHandler = (ev: PointerEvent) => handleDrag("max", ev);
        const upHandler = () => {
          window.removeEventListener("pointermove", moveHandler);
          window.removeEventListener("pointerup", upHandler);
        };
        window.addEventListener("pointermove", moveHandler);
        window.addEventListener("pointerup", upHandler);
      }}
    />
  </svg>
</div>
<div class="flex justify-between mt-2 text-sm">
  <span>Min: {lightMin}</span>
  <span>Max: {lightMax}</span>
</div>
