<script lang="ts">
  import { onMount } from "svelte";
  import citationFormats from "../lib/citations.generated.json";
  import CopyButton from "./CopyButton.svelte";
  import showToast from "./../App.svelte";

  let { open = false, onclose, oncopied } = $props();

  let activeFormat = $state("bibtex");
  interface CitationStyle {
    key: string;
    label: string;
    value: string;
  }
  const currentCitation = $derived(
    () =>
      (citationFormats as Array<any>).find((f) => f.key === activeFormat) ?? {},
  );
  let activeStyleKey = $state<string | undefined>(undefined);
  $effect(() => {
    const citation = currentCitation();
    if (citation && citation.styles) {
      if (
        !activeStyleKey ||
        !citation.styles.some((s: CitationStyle) => s.key === activeStyleKey)
      ) {
        activeStyleKey = citation.styles[0].key;
      }
    } else {
      activeStyleKey = undefined;
    }
  });
  const selectedStyle = $derived(() =>
    currentCitation()?.styles
      ? (currentCitation().styles.find(
          (s: CitationStyle) => s.key === activeStyleKey,
        ) ?? null)
      : null,
  );
  const displayedCitationValue = $derived(() =>
    selectedStyle() ? selectedStyle().value : currentCitation().value,
  );

  function close() {
    onclose?.();
  }

  async function copyCitation() {
    try {
      await navigator.clipboard.writeText(displayedCitationValue());
      oncopied?.();
    } catch {
      /* ignore */
    }
  }

  function handleKey(e: KeyboardEvent) {
    if (e.key === "Escape") close();
  }

  let dialogEl = $state<HTMLDialogElement | null>(null);
  onMount(() => {
    if (open && dialogEl) dialogEl.focus();
  });
  $effect(() => {
    if (open && dialogEl) setTimeout(() => dialogEl && dialogEl.focus(), 0);
  });
</script>

{#if open}
  <div
    class="fixed inset-0 z-50 flex items-start md:items-center justify-center bg-black/50 p-4"
    role="presentation"
    onclick={close}
    onkeydown={handleKey}
  >
    <div
      class="bg-white rounded-lg shadow-lg w-full max-w-lg overflow-hidden"
      role="dialog"
      aria-modal="true"
      aria-labelledby="about-title"
      tabindex="-1"
      bind:this={dialogEl}
      onclick={(e) => e.stopPropagation()}
      onkeydown={(e) => e.stopPropagation()}
    >
      <div class="px-5 py-4 border-b flex justify-between items-center">
        <h2 id="about-title" class="text-lg font-semibold">About Qualpal</h2>
        <button
          type="button"
          class="text-gray-500 hover:text-gray-700 cursor-pointer"
          aria-label="Close"
          onclick={close}
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
          <h3 class="font-medium mb-2">Citation</h3>
          <div class="flex flex-wrap gap-2 mb-2">
            {#each citationFormats as f}
              <button
                type="button"
                class="px-2 py-1 text-xs rounded border cursor-pointer transition
                  {activeFormat === f.key
                  ? 'bg-blue-600 text-white border-blue-600'
                  : 'bg-white text-gray-700 border-gray-300 hover:bg-gray-100'}"
                onclick={() => (activeFormat = f.key)}
                aria-pressed={activeFormat === f.key}
              >
                {f.label}
              </button>
            {/each}
          </div>
          {#if currentCitation().styles}
            <div class="flex items-center gap-2 mb-2">
              <label class="text-xs font-medium" for="citation-style-select"
                >Style:</label
              >
              <select
                id="citation-style-select"
                class="text-xs border rounded px-1 py-0.5 bg-white"
                bind:value={activeStyleKey}
              >
                {#each currentCitation().styles as s}
                  <option value={s.key}>{s.label}</option>
                {/each}
              </select>
            </div>
          {/if}
          <div class="relative">
            {#if currentCitation().code === true && !currentCitation().styles}
              <pre
                class="bg-gray-100 p-2 rounded text-xs overflow-x-auto"
                id="citation-block">{displayedCitationValue()}</pre>
            {:else}
              <div
                class="p-2 rounded text-xs border bg-white whitespace-pre-wrap"
                id="citation-block"
              >
                {displayedCitationValue()}
              </div>
            {/if}

            <CopyButton
              text={displayedCitationValue()}
              ariaLabel="Copy citation"
              title="Copy citation"
              extraClass="absolute top-1 right-1 p-1"
              oncopied={() => showToast("Copied output")}
            />
          </div>
        </div>
        <p class="text-xs text-gray-500">
          © {new Date().getFullYear()} Qualpal. MIT License.
        </p>
      </div>
    </div>
  </div>
{/if}
