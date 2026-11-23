<script lang="ts">
  import { onMount } from "svelte";
  import { toast } from "./stores/toast.js";
  import {
    paletteParams,
    palette,
    analysis,
    autoAnalyze,
    moduleLoaded,
    initializeModule,
    generatePalette,
  } from "./stores/paletteStore.js";

  import AboutModal from "./components/AboutModal.svelte";
  import Examples from "./components/Examples.svelte";
  import Footer from "./components/Footer.svelte";
  import PaletteAnalysis from "./components/PaletteAnalysis.svelte";
  import PaletteDisplay from "./components/PaletteDisplay.svelte";
  import PaletteOutput from "./components/PaletteOutput.svelte";
  import ParameterSidebar from "./components/ParameterSidebar.svelte";
  import LoadingSpinner from "./components/LoadingSpinner.svelte";
  import Toast from "./components/Toast.svelte";

  let { embedded = false } = $props();
  let showAbout = $state<boolean>(false);

  // Initialize the module
  onMount(async () => {
    const success = await initializeModule();
    if (success) {
      generatePalette($paletteParams);
    }
  });

  onMount(() => autoAnalyze.subscribe(() => {}));
</script>

{#if !embedded}
  <header class="bg-gray-800">
    <div class="max-w-7xl mx-auto px-4 py-4 w-full">
      <h1 class="text-2xl font-bold text-gray-100">Qualpal</h1>
      <p class="text-gray-300 mt-1">
        Generate maximally distinct color palettes for categorical data
      </p>
    </div>
  </header>
{/if}

<div class="main-content">
  <div class="grid gap-6 md:grid-cols-[18rem_minmax(0,1fr)] md:items-start">
    <ParameterSidebar />

    <!-- Main Content -->
    <div class="min-w-0">
      <div class="space-y-6">
        {#if $moduleLoaded}
          <PaletteDisplay {palette} {paletteParams} />

          <Examples {palette} {paletteParams} />

          <PaletteOutput {palette} />

          <PaletteAnalysis {analysis} {palette} />
        {:else}
          <LoadingSpinner message="Loading..." />
        {/if}
      </div>
    </div>
  </div>

  <Toast {toast} />
</div>

{#if !embedded}
  <AboutModal open={showAbout} onclose={() => (showAbout = false)} />
  <Footer onclick={() => (showAbout = true)} />
{/if}
