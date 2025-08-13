<script lang="ts">
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

  function angleToPos(angle: number) {
    const rad = ((angle - 90) * Math.PI) / 180;
    return {
      x: center + radius * Math.cos(rad),
      y: center + radius * Math.sin(rad),
    };
  }

  function handleDrag(type: "min" | "max", event: PointerEvent) {
    const target = event.target as SVGElement;
    const svg = target.ownerSVGElement || target.closest("svg");
    if (!svg) return;

    const rect = svg.getBoundingClientRect();
    const x = event.clientX - rect.left - center;
    const y = event.clientY - rect.top - center;
    let angle = (Math.atan2(y, x) * 180) / Math.PI + 90;
    if (angle < 0) angle += 360;

    const roundedAngle = Math.round(angle);

    if (type === "min") {
      onChange({ hueMin: roundedAngle, hueMax });
    } else {
      onChange({ hueMin, hueMax: roundedAngle });
    }
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
    <path
      d={`M${angleToPos(hueMax).x},${angleToPos(hueMax).y}
        A${radius},${radius} 0 ${(hueMin + 360 - hueMax) % 360 > 180 ? 1 : 0},1 ${angleToPos(hueMin).x},${angleToPos(hueMin).y}`}
      stroke="rgba(0,0,0,0.7)"
      stroke-width="16"
      fill="none"
      style="z-index: 2;"
    />
    <!-- Handles -->
    <circle
      cx={angleToPos(hueMin).x}
      cy={angleToPos(hueMin).y}
      r={handleRadius}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="cursor: pointer; touch-action: none; z-index: 3;"
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
    <circle
      cx={angleToPos(hueMax).x}
      cy={angleToPos(hueMax).y}
      r={handleRadius}
      fill="#fff"
      stroke="#333"
      stroke-width="2"
      style="cursor: pointer; touch-action: none; z-index: 3;"
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
  <span>Start: {hueMin}°</span>
  <span>End: {hueMax}°</span>
</div>
