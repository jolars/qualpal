<script lang="ts">
  import { createEventDispatcher, onMount } from "svelte";

  export let open: boolean = false;

  const citationBibtex = `@software{
    qualpal,
  	location = {Lund, Sweden},
  	title = {qualpal},
  	rights = {{MIT}},
  	url = {https://github.com/jolars/qualpal},
  	abstract = {Qualitative color palettes},
  	version = {2.4.0},
  	author = {Larsson, Johan},
  	urldate = {2025-08-12},
  	date = {2025-08-12},
  	note = {original-date: 2023-11-01T20:52:14Z},
  }`;

  const dispatch = createEventDispatcher();

  function close() {
    open = false;
    dispatch("close");
  }

  async function copyCitation() {
    try {
      await navigator.clipboard.writeText(citationBibtex);
      dispatch("copied");
    } catch {
      /* ignore */
    }
  }

  function handleKey(e: KeyboardEvent) {
    if (e.key === "Escape") close();
  }

  let dialogEl: HTMLDivElement | null = null;
  onMount(() => {
    if (open && dialogEl) {
      dialogEl.focus();
    }
  });
  $: if (open && dialogEl) {
    // focus when it opens
    setTimeout(() => dialogEl && dialogEl.focus(), 0);
  }
</script>

{#if open}
  <div
    class="fixed inset-0 z-50 flex items-start md:items-center justify-center bg-black/50 p-4"
    role="presentation"
    on:click={close}
    on:keydown={handleKey}
  >
    <div
      class="bg-white rounded-lg shadow-lg w-full max-w-lg overflow-hidden"
      role="dialog"
      aria-modal="true"
      aria-labelledby="about-title"
      tabindex="-1"
      bind:this={dialogEl}
      on:click|stopPropagation
      on:keydown|stopPropagation
    >
      <div class="px-5 py-4 border-b flex justify-between items-center">
        <h2 id="about-title" class="text-lg font-semibold">About Qualpal</h2>
        <button
          type="button"
          class="text-gray-500 hover:text-gray-700 cursor-pointer"
          aria-label="Close"
          on:click={close}
        >
          ✕
        </button>
      </div>
      <div class="p-5 space-y-4 text-sm text-gray-700">
        <p>
          Qualpal generates perceptually uniform qualitative color palettes
          optimized for distinctness using DIN99d color difference. Configure a
          colorspace or provide fixed candidate colors, optionally extend an
          existing palette, and analyze accessibility impacts.
        </p>
        <div>
          <h3 class="font-medium mb-1">Citation</h3>
          <pre
            class="bg-gray-100 p-2 rounded text-xs overflow-x-auto"
            id="citation-block">{citationBibtex}</pre>
          <button
            type="button"
            class="mt-2 px-2 py-1 text-xs bg-blue-600 text-white rounded hover:bg-blue-700 cursor-pointer"
            on:click={copyCitation}
          >
            Copy BibTeX
          </button>
        </div>
        <p class="text-xs text-gray-500">
          © {new Date().getFullYear()} Qualpal. MIT License.
        </p>
      </div>
    </div>
  </div>
{/if}
