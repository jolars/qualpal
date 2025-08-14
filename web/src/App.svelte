<script lang="ts">
  import { onMount } from "svelte";
  import { toast } from "./stores/toast.js";
  import {
    paletteParams,
    palette,
    analysis,
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
  import Toast from "./components/Toast.svelte";

  let showAbout = $state<boolean>(false);

  // Initialize the module
  onMount(async () => {
    const success = await initializeModule();
    if (success) {
      generatePalette($paletteParams);
    }
  });
</script>

<header class="bg-gray-800">
  <div class="max-w-7xl mx-auto px-4 py-4 w-full">
    <h1 class="text-2xl font-bold text-gray-100">Qualpal</h1>
    <p class="text-gray-300 mt-1">
      Generate maximally distinct color palettes for categorical data
    </p>
  </div>
</header>

<div class="main-content">
  <div class="flex flex-col sm:flex-row min-h-screen">
    <ParameterSidebar />

    <!-- Main Content -->
    <main class="flex-1 p-4 overflow-x-auto">
      <div class="space-y-6">
        {#if $moduleLoaded}
          <PaletteDisplay palette={$palette} paletteParams={$paletteParams} />

          <Examples
            palette={$palette}
            backgroundColor={$paletteParams.backgroundColor}
          />

          <PaletteOutput {palette} />

          <PaletteAnalysis
            matrix={$analysis?.normal?.differenceMatrix ?? []}
            labels={$palette?.length > 0 ? $palette.map((c) => c.hex) : []}
            minDistances={$analysis?.normal?.minDistances ?? []}
          />
        {:else}
          <div class="flex items-center justify-center min-h-screen">
            <div class="text-center">
              <div
                class="animate-spin h-8 w-8 border-4 border-blue-500 border-t-transparent rounded-full mx-auto mb-4"
              ></div>
              <p class="text-gray-600">Loading Qualpal...</p>
            </div>
          </div>
        {/if}
      </div>
    </main>
  </div>

  <Toast show={$toast.show} message={$toast.message} />

  <AboutModal open={showAbout} onclose={() => (showAbout = false)} />
</div>

<Footer on:about={() => (showAbout = true)} />
