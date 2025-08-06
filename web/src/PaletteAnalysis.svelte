<script lang="ts">
  export let matrix: number[][] = [];
  export let labels: string[] = [];

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
  <div class="bg-white rounded-lg shadow-sm border p-6 mb-4">
    <h3 class="text-xl font-semibold text-gray-900 mb-4">Palette Analysis</h3>
    <div class="flex justify-center">
      <table class="border-collapse">
        <thead>
          <tr>
            <th></th>
            {#each labels as label}
              <th class="text-xs font-mono text-gray-700 px-1 py-1">
                <div class="flex flex-col items-center">
                  <span
                    class="inline-block w-5 h-5 rounded mb-1 border border-gray-300"
                    style="background:{label};"
                    title={label}
                  ></span>
                  <span>{label}</span>
                </div>
              </th>
            {/each}
          </tr>
        </thead>
        <tbody>
          {#each matrix as row, i}
            <tr>
              <th class="text-xs font-mono text-gray-700 px-1 py-1">
                <div class="flex flex-col items-center">
                  <span
                    class="inline-block w-5 h-5 rounded mb-1 border border-gray-300"
                    style="background:{labels[i]};"
                    title={labels[i]}
                  ></span>
                  <span>{labels[i]}</span>
                </div>
              </th>
              {#each row as value, j}
                <td
                  style="background:{cellColor(
                    value,
                    min,
                    max,
                  )};width:48px;height:48px;position:relative;"
                  title={labels && labels[j]
                    ? `${labels[i]} vs ${labels[j]}: ${value.toFixed(2)}`
                    : value.toFixed(2)}
                  class="border border-gray-200 text-xs font-mono text-center align-middle"
                >
                  <span
                    style="position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);color:{textColor(
                      value,
                      min,
                      max,
                    )};font-weight:600;"
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
{/if}
