<script lang="ts">
  import * as d3 from "d3";

  let { analysis, palette } = $props();

  const matrix = $derived(() => $analysis?.normal?.differenceMatrix ?? []);
  const labels = $derived(() =>
    $palette?.length > 0 ? $palette.map((c: any) => c.hex) : [],
  );
  const minDistances = $derived(() => $analysis?.normal?.minDistances ?? []);

  let minDistSvg = $state<SVGSVGElement | undefined>(undefined);

  function renderMinDistBarChart() {
    if (!minDistSvg || !labels().length || !minDistances().length) return;

    const svg = d3.select(minDistSvg);
    svg.selectAll("*").remove();

    const width = Math.max(320, labels().length * 48);
    const height = 220;
    const margin = { top: 16, right: 8, bottom: 32, left: 32 };

    const x = d3
      .scaleBand()
      .domain(labels())
      .range([margin.left, width - margin.right])
      .padding(0.2);

    const y = d3
      .scaleLinear()
      .domain([0, 70])
      .nice()
      .range([height - margin.bottom, margin.top]);

    // Bars
    svg
      .selectAll("rect")
      .data(minDistances())
      .join("rect")
      .attr("x", (_d: number, i: number) => x(labels()[i]))
      .attr("y", (d: number) => y(d))
      .attr("width", x.bandwidth())
      .attr("height", (d: number) => y(0) - y(d))
      .attr("fill", (_: number, i: number) => labels()[i])
      .attr("stroke", "#eaeaea");

    // X axis
    svg
      .append("g")
      .attr("transform", `translate(0,${height - margin.bottom})`)
      .call(
        d3
          .axisBottom(x)
          .tickFormat((d: string) => d)
          .tickSizeOuter(0),
      )
      .selectAll("text")
      .attr("font-size", 10)
      .attr("transform", "rotate(-30)")
      .style("text-anchor", "end");

    // Y axis
    svg
      .append("g")
      .attr("transform", `translate(${margin.left},0)`)
      .call(d3.axisLeft(y).ticks(5))
      .selectAll("text")
      .attr("font-size", 10);
  }

  $effect(() => {
    if (minDistSvg && labels().length && minDistances().length) {
      renderMinDistBarChart();
    }
  });

  // Compute color for each cell (greyscale for low/high difference)
  function cellColor(value: number, min: number, max: number) {
    if (max === min) return "rgb(200,200,200)";
    const t = (value - min) / (max - min);
    const grey = Math.round(255 - t * 200); // 255 (white) to 75 (dark)
    return `rgb(${grey},${grey},${grey})`;
  }

  // Determine text color based on background brightness
  function textColor(value: number, min: number, max: number) {
    if (max === min) return "#333";
    const t = (value - min) / (max - min);
    const grey = Math.round(255 - t * 180);
    // Use white text on dark backgrounds, dark text on light backgrounds
    return grey < 150 ? "#ffffff" : "#333333";
  }

  const min = 0;
  const max = 70;
</script>

{#if !matrix() || matrix().length === 0}
  <div class="text-red-500 text-sm">No matrix data received.</div>
{:else}
  <h3 class="text-xl font-semibold text-gray-900 mb-4">Palette Analysis</h3>
  <!-- Difference Matrix -->
  <div class="w-full max-w-full overflow-x-auto">
    <h4 class="text-lg font-medium text-gray-800 mb-3">
      Color Difference Matrix
    </h4>
    <p class="text-xs text-gray-500 mt-2 mb-4">
      Values represent the color difference (in the CIEDE2000 metric) between
      palette colors.
    </p>
    <table class="border-collapse w-max">
      <thead>
        <tr>
          <th></th>
          {#each labels() as label}
            <th class="text-xs font-mono text-gray-700 px-1 py-1 w-12">
              <div class="flex flex-col items-center">
                <span
                  class="inline-block w-4 h-4 sm:w-5 sm:h-5 rounded mb-1 border border-gray-300"
                  style="background:{label};"
                  title={label}
                ></span>
                <span class="hidden sm:inline">{label}</span>
                <span class="sm:hidden text-[10px]">{label.slice(0, 3)}</span>
              </div>
            </th>
          {/each}
        </tr>
      </thead>
      <tbody>
        {#each matrix() as row, i}
          <tr>
            <th class="text-xs font-mono text-gray-700 px-1 py-1 w-12">
              {#if labels()[i]}
                <div class="flex flex-col items-center">
                  <span
                    class="inline-block w-4 h-4 sm:w-5 sm:h-5 rounded mb-1 border border-gray-300"
                    style="background:{labels()[i]};"
                    title={labels()[i]}
                  ></span>
                  <span class="hidden sm:inline">{labels()[i]}</span>
                  <span class="sm:hidden text-[10px]"
                    >{labels()[i].slice(0, 3)}</span
                  >
                </div>
              {:else}
                <div class="flex flex-col items-center">
                  <span class="text-xs">#{i}</span>
                </div>
              {/if}
            </th>
            {#each row as value, j}
              <td
                style="background:{cellColor(
                  value,
                  min,
                  max,
                )};width:32px;height:32px;position:relative;"
                class="sm:w-12 sm:h-12 border border-gray-200 text-xs font-mono text-center align-middle"
                title={labels()[i] && labels()[j]
                  ? `${labels()[i]} vs ${labels()[j]}: ${value.toFixed(2)}`
                  : value.toFixed(2)}
              >
                <span
                  style="position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);color:{textColor(
                    value,
                    min,
                    max,
                  )};font-weight:600;"
                  class="text-[10px] sm:text-xs"
                >
                  {value.toFixed(2)}
                </span>
              </td>
            {/each}
          </tr>
        {/each}
      </tbody>
    </table>
  </div>

  <!-- Minimum Distances -->
  <h4 class="text-lg font-medium text-gray-800 mb-3 flex items-center gap-2">
    Minimum Distances
  </h4>
  <p class="text-xs text-gray-500 mb-2">
    Shows the smallest color difference (in the CIEDE2000 metric) for each
    palette color.
  </p>
  <div class="w-full overflow-x-auto">
    <svg
      bind:this={minDistSvg}
      width={Math.max(320, labels().length * 48)}
      height="230"
      class="block"
    ></svg>
  </div>
{/if}
