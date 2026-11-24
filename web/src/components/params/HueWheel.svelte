<script lang="ts">
  import { sliderKeyValue } from "./sliderKeyValue";

  export let hueMin = 0;
  export let hueMax = 360;
  export let onChange: (values: {
    hueMin: number;
    hueMax: number;
  }) => void = () => {};

  const radius = 80;
  const center = 100;
  const segmentCount = 120; // Increase for smoother wheel
  const handleRadius = 10;

  $: hueSpan = hueMax - hueMin + (hueMin > hueMax ? 360 : 0);
  $: singleHue = hueSpan === 0;

  function angleToPos(angle: number) {
    const rad = ((angle - 90) * Math.PI) / 180;
    return {
      x: center + radius * Math.cos(rad),
      y: center + radius * Math.sin(rad),
    };
  }

  function handleDrag(type: "min" | "max", event: PointerEvent) {
    const target = event.currentTarget as SVGCircleElement;
    if (!target.hasPointerCapture(event.pointerId)) return;

    const svg = target.ownerSVGElement;
    if (!svg) return;

    const rect = svg.getBoundingClientRect();
    const x = event.clientX - rect.left - center;
    const y = event.clientY - rect.top - center;
    let angle = (Math.atan2(y, x) * 180) / Math.PI + 90;
    if (angle < 0) angle += 360;

    updateValue(type, angle);
  }

  function updateValue(type: "min" | "max", value: number) {
    const rounded = Math.round(value);
    onChange(
      type === "min" ? { hueMin: rounded, hueMax } : { hueMin, hueMax: rounded },
    );
  }

  function handleKey(type: "min" | "max", event: KeyboardEvent) {
    const current = type === "min" ? hueMin : hueMax;
    const value = sliderKeyValue(event.key, current, 360, 1);
    if (value === undefined) return;

    event.preventDefault();
    updateValue(type, value);
  }

  function describeArc(
    cx: number,
    cy: number,
    r: number,
    startAngle: number,
    endAngle: number,
  ) {
    const start = polarToCartesian(cx, cy, r, endAngle);
    const end = polarToCartesian(cx, cy, r, startAngle);
    const largeArcFlag = endAngle - startAngle <= 180 ? "0" : "1";
    return [
      "M",
      start.x,
      start.y,
      "A",
      r,
      r,
      0,
      largeArcFlag,
      0,
      end.x,
      end.y,
    ].join(" ");
  }

  function polarToCartesian(cx: number, cy: number, r: number, angle: number) {
    const rad = ((angle - 90) * Math.PI) / 180.0;
    return {
      x: cx + r * Math.cos(rad),
      y: cy + r * Math.sin(rad),
    };
  }
</script>

<div class="flex justify-center">
  <svg width="200" height="200" style="touch-action:none;">
    <!-- Base hue circle using CSS background -->

    {#each Array(segmentCount) as _, i}
      <path
        d={describeArc(
          center,
          center,
          radius,
          i * (360 / segmentCount),
          (i + 1) * (360 / segmentCount) + 1,
        )}
        stroke={`hsl(${i * (360 / segmentCount)}, 100%, 50%)`}
        stroke-width="16"
        stroke-linecap="butt"
        fill="none"
        style="pointer-events: none;"
      />
    {/each}
    <circle
      cx={center}
      cy={center}
      r={radius - 8}
      fill="none"
      stroke="#f3f4f6"
      stroke-width="2"
      style="z-index: 1;"
    />
    <circle
      cx={center}
      cy={center}
      r={radius + 8}
      fill="none"
      stroke="#f3f4f6"
      stroke-width="2"
      style="z-index: 1;"
    />
    {#if singleHue}
      <circle
        cx={center}
        cy={center}
        r={radius}
        stroke="rgba(0,0,0,0.7)"
        stroke-width="16"
        fill="none"
      />
    {:else}
      <path
        d={`M${angleToPos(hueMax).x},${angleToPos(hueMax).y}
          A${radius},${radius} 0 ${(hueMin + 360 - hueMax) % 360 > 180 ? 1 : 0},1 ${angleToPos(hueMin).x},${angleToPos(hueMin).y}`}
        stroke="rgba(0,0,0,0.7)"
        stroke-width="16"
        fill="none"
        style="z-index: 2;"
      />
    {/if}
    <!-- Handles -->
    <circle
      cx={angleToPos(hueMin).x}
      cy={angleToPos(hueMin).y}
      r={handleRadius}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="cursor: pointer; touch-action: none; z-index: 3;"
      role="slider"
      tabindex="0"
      aria-label="Hue start"
      aria-valuemin="0"
      aria-valuemax="360"
      aria-valuenow={hueMin}
      on:keydown={(event) => handleKey("min", event)}
      on:pointerdown={(event) => event.currentTarget.setPointerCapture(event.pointerId)}
      on:pointermove={(event) => handleDrag("min", event)}
    />
    <circle
      cx={angleToPos(hueMax).x}
      cy={angleToPos(hueMax).y}
      r={handleRadius}
      fill={singleHue ? `hsl(${hueMax}, 100%, 50%)` : "#fff"}
      stroke="#333"
      stroke-width="2"
      style="cursor: pointer; touch-action: none; z-index: 3;"
      role="slider"
      tabindex="0"
      aria-label="Hue end"
      aria-valuemin="0"
      aria-valuemax="360"
      aria-valuenow={hueMax}
      on:keydown={(event) => handleKey("max", event)}
      on:pointerdown={(event) => event.currentTarget.setPointerCapture(event.pointerId)}
      on:pointermove={(event) => handleDrag("max", event)}
    />
  </svg>
</div>
<div class="flex justify-between mt-2 text-sm">
  {#if singleHue}
    <span>Single hue: {hueMax}°</span>
  {:else}
    <span>Start: {hueMin}°</span>
    <span>End: {hueMax}°</span>
  {/if}
</div>
