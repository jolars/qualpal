<script lang="ts">
  export let matrix: number[][] = [];
  export let labels: string[] = [];
  export let minDistances: number[] = [];

  // Compute color for each cell (greyscale for low/high difference)
  function cellColor(value: number, min: number, max: number) {
    if (max === min) return "rgb(200,200,200)";
    const t = (value - min) / (max - min);
    const grey = Math.round(255 - t * 180); // 255 (white) to 75 (dark)
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

  // Flatten matrix for min/max
  const flat = matrix.flat();
  const min = flat.length ? Math.min(...flat) : 0;
  const max = flat.length ? Math.max(...flat) : 1;
</script>

{#if !matrix || matrix.length === 0}
  <div class="text-red-500 text-sm">No matrix data received.</div>
{:else}
  <h3 class="text-xl font-semibold text-gray-900 mb-4">Palette Analysis</h3>
  <div class="grid grid-cols-1 lg:grid-cols-4 gap-6">
    <!-- Difference Matrix -->
    <div class="lg:col-span-2">
      <h4 class="text-lg font-medium text-gray-800 mb-3">
        Color Difference Matrix
      </h4>
      <div class="overflow-x-auto">
        <table class="border-collapse">
          <thead>
            <tr>
              <th></th>
              {#each labels as label}
                <th class="text-xs font-mono text-gray-700 px-1 py-1 w-12">
                  <div class="flex flex-col items-center">
                    <span
                      class="inline-block w-4 h-4 sm:w-5 sm:h-5 rounded mb-1 border border-gray-300"
                      style="background:{label};"
                      title={label}
                    ></span>
                    <span class="hidden sm:inline">{label}</span>
                    <span class="sm:hidden text-[10px]"
                      >{label.slice(0, 3)}</span
                    >
                  </div>
                </th>
              {/each}
            </tr>
          </thead>
          <tbody>
            {#each matrix as row, i}
              <tr>
                <th class="text-xs font-mono text-gray-700 px-1 py-1 w-12">
                  <div class="flex flex-col items-center">
                    <span
                      class="inline-block w-4 h-4 sm:w-5 sm:h-5 rounded mb-1 border border-gray-300"
                      style="background:{labels[i]};"
                      title={labels[i]}
                    ></span>
                    <span class="hidden sm:inline">{labels[i]}</span>
                    <span class="sm:hidden text-[10px]"
                      >{labels[i].slice(0, 3)}</span
                    >
                  </div>
                </th>
                {#each row as value, j}
                  <td
                    style="background:{cellColor(
                      value,
                      min,
                      max,
                    )};width:32px;height:32px;position:relative;"
                    class="sm:w-12 sm:h-12 border border-gray-200 text-xs font-mono text-center align-middle"
                    title={labels && labels[j]
                      ? `${labels[i]} vs ${labels[j]}: ${value.toFixed(2)}`
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
    </div>

    <!-- Minimum Distances -->
    <div class="lg:col-span-2">
      <h4 class="text-lg font-medium text-gray-800 mb-3">Minimum Distances</h4>
      <div class="space-y-2">
        {#each minDistances as distance, i}
          <div
            class="flex items-center justify-between p-2 bg-gray-50 rounded border"
          >
            <div class="flex items-center space-x-2">
              <span
                class="inline-block w-4 h-4 rounded border border-gray-300"
                style="background:{labels[i]};"
                title={labels[i]}
              ></span>
              <span class="text-sm font-mono text-gray-700">{labels[i]}</span>
            </div>
            <span class="text-sm font-semibold text-gray-900"
              >{distance.toFixed(2)}</span
            >
          </div>
        {/each}
      </div>
    </div>
  </div>
{/if}
