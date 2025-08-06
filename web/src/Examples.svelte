<script lang="ts">
  import { onMount, afterUpdate } from "svelte";

  // Add type declarations
  declare const d3: any;
  declare const topojson: any;

  export let palette = [];

  let barChartContainer;
  let scatterPlotContainer;
  let lineChartContainer;
  let mapContainer;

  $: barData = palette.map((_, i) => ({
    category: String.fromCharCode(65 + i),
    value: 20 + ((i * 7 + i * i * 3) % 41),
  }));

  $: scatterData = palette.flatMap((_, groupIndex) =>
    Array.from({ length: 3 }, (__, pointIndex) => ({
      x:
        10 +
        (((groupIndex * 3 + pointIndex) * 13 + groupIndex * pointIndex * 11) %
          81),
      y:
        20 +
        (((groupIndex * 3 + pointIndex) * 17 +
          groupIndex * groupIndex * 7 +
          pointIndex * 5) %
          51),
      group: String.fromCharCode(65 + groupIndex),
    })),
  );

  $: lineData = Array.from({ length: 6 }, (_, monthIndex) => {
    const month = ["Jan", "Feb", "Mar", "Apr", "May", "Jun"][monthIndex];
    const dataPoint = { month };

    palette.forEach((_, seriesIndex) => {
      // More "random-looking" deterministic value between 25-54
      dataPoint[String.fromCharCode(65 + seriesIndex)] =
        25 +
        (((monthIndex + 1) * (seriesIndex + 2) + monthIndex * seriesIndex * 3) %
          30);
    });

    return dataPoint;
  });

  let usStatesData = [];
  let mapLoaded = false;

  $: mapWidth = mapContainer?.clientWidth || 500;
  $: mapHeight = mapContainer?.clientHeight || 280;

  $: stateGroups = (() => {
    if (palette.length === 0 || usStatesData.length === 0) return {};

    const groupMap = {};

    // Randomly assign each state to a color group
    usStatesData.forEach((state, index) => {
      groupMap[state.name] = index % palette.length;
    });

    return groupMap;
  })();

  function createSimpleMap() {
    // Fallback simple grid representation
    if (!mapContainer || palette.length === 0) return;

    const svg = d3.select(mapContainer);
    svg.selectAll("*").remove();

    // Create simple rectangles as fallback
    const numStates = Math.min(20, Math.max(palette.length * 3, 12));
    const cols = 5;

    Array.from({ length: numStates }, (_, i) => `State ${i + 1}`);

    svg.forEach((stateName, i) => {
      svg
        .append("rect")
        .attr("class", "state-rect")
        .attr("x", (i % cols) * 100 + 20)
        .attr("y", Math.floor(i / cols) * 60 + 20)
        .attr("width", 80)
        .attr("height", 50)
        .attr("fill", palette[i % palette.length]?.hex || "#3b82f6")
        .attr("stroke", "#fff")
        .attr("stroke-width", 2);
    });
  }

  async function loadUSStatesData() {
    try {
      // Use the D3 gallery TopoJSON source
      const response = await fetch(
        "https://cdn.jsdelivr.net/npm/us-atlas@3/states-10m.json",
      );
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const us = await response.json();

      // Convert TopoJSON to GeoJSON and create path generator
      const states = topojson.feature(us, us.objects.states);
      const width = 200;
      const height = 200;
      const projection = d3
        .geoAlbersUsa()
        .scale(width * 1.75)
        .translate([width / 1.3, height / 2]);
      const path = d3.geoPath().projection(projection);

      usStatesData = states.features.map((feature, index) => ({
        name: feature.properties.name,
        path: path(feature),
        id: feature.id,
      }));

      mapLoaded = true;
      createMap();
    } catch (error) {
      console.error("Failed to load US states data:", error);
      mapLoaded = true;
      createSimpleMap();
    }
  }

  function getColorForIndex(index) {
    if (palette.length === 0) return "#3b82f6";
    return palette[index % palette.length].hex;
  }

  function createBarChart() {
    if (!barChartContainer || palette.length === 0) return;

    const svg = d3.select(barChartContainer);
    svg.selectAll("*").remove();

    const margin = { top: 20, right: 20, bottom: 40, left: 40 };
    const width = 300 - margin.left - margin.right;
    const height = 200 - margin.bottom - margin.top;

    const g = svg
      .append("g")
      .attr("transform", `translate(${margin.left},${margin.top})`);

    const x = d3
      .scaleBand()
      .domain(barData.map((d) => d.category))
      .range([0, width])
      .padding(0.2);

    const y = d3
      .scaleLinear()
      .domain([0, d3.max(barData, (d) => d.value)])
      .range([height, 0]);

    g.selectAll(".bar")
      .data(barData)
      .enter()
      .append("rect")
      .attr("class", "bar")
      .attr("x", (d) => x(d.category))
      .attr("width", x.bandwidth())
      .attr("y", (d) => y(d.value))
      .attr("height", (d) => height - y(d.value))
      .attr("fill", (d, i) => getColorForIndex(i));

    g.append("g")
      .attr("transform", `translate(0,${height})`)
      .call(d3.axisBottom(x))
      .selectAll("text")
      .style("font-size", "10px");

    g.append("g")
      .call(d3.axisLeft(y).ticks(5, "s"))
      .selectAll("text")
      .style("font-size", "10px");
  }

  function createScatterPlot() {
    if (!scatterPlotContainer || palette.length === 0) return;

    const svg = d3.select(scatterPlotContainer);
    svg.selectAll("*").remove();

    const margin = { top: 20, right: 20, bottom: 40, left: 40 };
    const width = 300 - margin.left - margin.right;
    const height = 200 - margin.bottom - margin.top;

    const g = svg
      .append("g")
      .attr("transform", `translate(${margin.left},${margin.top})`);

    const x = d3
      .scaleLinear()
      .domain(d3.extent(scatterData, (d) => d.x))
      .range([10, width]);

    const y = d3
      .scaleLinear()
      .domain(d3.extent(scatterData, (d) => d.y))
      .range([height - 10, 0]);

    const groups = [...new Set(scatterData.map((d) => d.group))];

    g.selectAll(".dot")
      .data(scatterData)
      .enter()
      .append("circle")
      .attr("class", "dot")
      .attr("cx", (d) => x(d.x))
      .attr("cy", (d) => y(d.y))
      .attr("r", 4)
      .attr("fill", (d) => getColorForIndex(groups.indexOf(d.group)));

    g.append("g")
      .attr("transform", `translate(0,${height})`)
      .call(d3.axisBottom(x).ticks(5))
      .selectAll("text")
      .style("font-size", "10px");

    g.append("g")
      .call(d3.axisLeft(y).ticks(5))
      .selectAll("text")
      .style("font-size", "10px");
  }

  function createLineChart() {
    if (!lineChartContainer || palette.length === 0) return;

    const svg = d3.select(lineChartContainer);
    svg.selectAll("*").remove();

    const margin = { top: 20, right: 20, bottom: 40, left: 40 };
    const width = 300 - margin.left - margin.right;
    const height = 200 - margin.bottom - margin.top;

    const g = svg
      .append("g")
      .attr("transform", `translate(${margin.left},${margin.top})`);

    const x = d3
      .scalePoint()
      .domain(lineData.map((d) => d.month))
      .range([0, width]);

    // Gather all values for y domain
    const allValues = lineData
      .flatMap((d) => palette.map((_, i) => d[String.fromCharCode(65 + i)]))
      .filter((v) => v !== undefined);

    const y = d3
      .scaleLinear()
      .domain([d3.min(allValues) - 5, d3.max(allValues) + 5])
      .range([height - 10, 0]);

    const line = d3
      .line()
      .x((d) => x(d.month))
      .y((d) => y(d.value));

    palette.forEach((_, i) => {
      const seriesName = String.fromCharCode(65 + i);
      const seriesData = lineData
        .map((d) => ({
          month: d.month,
          value: d[seriesName],
        }))
        .filter((d) => d.value !== undefined);

      if (seriesData.length > 0) {
        g.append("path")
          .datum(seriesData)
          .attr("fill", "none")
          .attr("stroke", getColorForIndex(i))
          .attr("stroke-width", 2)
          .attr("d", line);
      }
    });

    g.append("g")
      .attr("transform", `translate(0,${height})`)
      .call(d3.axisBottom(x))
      .selectAll("text")
      .style("font-size", "10px");

    g.append("g")
      .call(d3.axisLeft(y))
      .selectAll("text")
      .style("font-size", "10px");
  }

  function createMap() {
    if (!mapContainer || palette.length === 0 || !mapLoaded) return;

    const svg = d3.select(mapContainer);
    svg.selectAll("*").remove();

    svg
      .selectAll(".state")
      .data(usStatesData)
      .enter()
      .append("path")
      .attr("class", "state")
      .attr("d", (d) => d.path)
      .attr("fill", (d, i) => getColorForIndex(i))
      .attr("stroke", "#fff")
      .attr("stroke-width", 1);
  }

  function updateCharts() {
    createBarChart();
    createScatterPlot();
    createLineChart();
    createMap();
  }

  onMount(async () => {
    await loadUSStatesData();
    updateCharts();
  });

  afterUpdate(() => {
    updateCharts();
  });
</script>

<div class="bg-white rounded-lg shadow-sm border p-6">
  <h2 class="text-xl text-left font-semibold text-gray-900 mb-4">Examples</h2>

  <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
    <svg
      bind:this={barChartContainer}
      class="border rounded w-full h-auto"
      viewBox="0 0 300 200"
      preserveAspectRatio="xMidYMid meet"
    ></svg>

    <div class="text-center">
      <svg
        bind:this={scatterPlotContainer}
        class="border rounded w-full h-auto"
        viewBox="0 0 300 200"
        preserveAspectRatio="xMidYMid meet"
      ></svg>
    </div>
    <svg
      bind:this={lineChartContainer}
      class="border rounded w-full h-auto"
      viewBox="0 0 300 200"
      preserveAspectRatio="xMidYMid meet"
    ></svg>

    <svg
      bind:this={mapContainer}
      class="border rounded w-full h-auto"
      viewBox="0 0 300 200"
      preserveAspectRatio="xMidYMid meet"
    ></svg>
  </div>
</div>
