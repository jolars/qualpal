<script lang="ts">
  export let satMin = 0;
  export let satMax = 1;
  export let onChange: (values: {
    satMin: number;
    satMax: number;
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
      onChange({ satMin: Math.round(value * 10) / 10, satMax });
    } else {
      onChange({ satMin, satMax: Math.round(value * 10) / 10 });
    }
  }
</script>

<div class="flex justify-center">
  <svg {width} height={height + 20} style="touch-action:none;">
    <defs>
      <linearGradient
        id="saturation-gradient"
        x1="0%"
        y1="0%"
        x2="100%"
        y2="0%"
      >
        <stop offset="0%" stop-color="#808080" />
        <stop offset="100%" stop-color="#ff0000" />
      </linearGradient>
    </defs>

    <!-- Background gradient bar -->
    <rect
      x={padding}
      y="10"
      width={width - 2 * padding}
      {height}
      fill="url(#saturation-gradient)"
      rx="4"
    />

    <!-- Selection range overlay -->
    <rect
      x={valueToX(satMin)}
      y="10"
      width={valueToX(satMax) - valueToX(satMin)}
      {height}
      fill="rgba(255,255,255,0.3)"
      stroke="rgba(0,0,0)"
      stroke-width="1"
      rx="4"
    />

    <!-- Min handle -->
    <path
      d={`M${valueToX(satMin) - 8},2 L${valueToX(satMin) + 8},2 L${valueToX(satMin)},18 Z`}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="cursor: pointer;"
      on:pointerdown={() => {
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
      d={`M${valueToX(satMax) - 8},2 L${valueToX(satMax) + 8},2 L${valueToX(satMax)},18 Z`}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="cursor: pointer;"
      on:pointerdown={() => {
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
  <span>Min: {satMin}</span>
  <span>Max: {satMax}</span>
</div>
