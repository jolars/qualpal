<script lang="ts">
  import { createEventDispatcher } from "svelte";

  export let text: string;
  export let ariaLabel: string = "Copy";
  export let title: string = "Copy";
  export let extraClass: string = "";
  export let showFeedback = true;

  const dispatch = createEventDispatcher<{ copied: void; error: void }>();
  let copied = false;
  let timeout: ReturnType<typeof setTimeout> | null = null;

  async function handleCopy() {
    try {
      await navigator.clipboard.writeText(text);
      dispatch("copied");
      if (showFeedback) {
        copied = true;
        if (timeout) clearTimeout(timeout);
        timeout = setTimeout(() => (copied = false), 1200);
      }
    } catch {
      dispatch("error");
    }
  }
</script>

<button
  type="button"
  class="inline-flex items-center justify-center {extraClass}  rounded text-gray-600 hover:text-gray-800 hover:bg-white/80 bg-white/60 border border-gray-200 cursor-pointer"
  aria-label={ariaLabel}
  {title}
  on:click={handleCopy}
>
  {#if copied}
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width="16"
      height="16"
      fill="currentColor"
      class="bi bi-clipboard-check"
      viewBox="0 0 16 16"
    >
      <path
        fill-rule="evenodd"
        d="M10.854 7.146a.5.5 0 0 1 0 .708l-3 3a.5.5 0 0 1-.708 0l-1.5-1.5a.5.5 0 1 1 .708-.708L7.5 9.793l2.646-2.647a.5.5 0 0 1 .708 0"
      />
      <path
        d="M4 1.5H3a2 2 0 0 0-2 2V14a2 2 0 0 0 2 2h10a2 2 0 0 0 2-2V3.5a2 2 0 0 0-2-2h-1v1h1a1 1 0 0 1 1 1V14a1 1 0 0 1-1 1H3a1 1 0 0 1-1-1V3.5a1 1 0 0 1 1-1h1z"
      />
      <path
        d="M9.5 1a.5.5 0 0 1 .5.5v1a.5.5 0 0 1-.5.5h-3a.5.5 0 0 1-.5-.5v-1a.5.5 0 0 1 .5-.5zm-3-1A1.5 1.5 0 0 0 5 1.5v1A1.5 1.5 0 0 0 6.5 4h3A1.5 1.5 0 0 0 11 2.5v-1A1.5 1.5 0 0 0 9.5 0z"
      />
    </svg>
  {:else}
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width="16"
      height="16"
      fill="currentColor"
      class="bi bi-clipboard"
      viewBox="0 0 16 16"
    >
      <path
        d="M4 1.5H3a2 2 0 0 0-2 2V14a2 2 0 0 0 2 2h10a2 2 0 0 0 2-2V3.5a2 2 0 0 0-2-2h-1v1h1a1 1 0 0 1 1 1V14a1 1 0 0 1-1 1H3a1 1 0 0 1-1-1V3.5a1 1 0 0 1 1-1h1z"
      />
      <path
        d="M9.5 1a.5.5 0 0 1 .5.5v1a.5.5 0 0 1-.5.5h-3a.5.5 0 0 1-.5-.5v-1a.5.5 0 0 1 .5-.5zm-3-1A1.5 1.5 0 0 0 5 1.5v1A1.5 1.5 0 0 0 6.5 4h3A1.5 1.5 0 0 0 11 2.5v-1A1.5 1.5 0 0 0 9.5 0z"
      />
    </svg>
  {/if}
  <span class="sr-only">{title}</span>
</button>
