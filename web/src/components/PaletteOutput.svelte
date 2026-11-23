<script lang="ts">
  import CopyButton from "./CopyButton.svelte";
  import { showToast } from "./../stores/toast.js";

  let { palette } = $props();

  const OUTPUT_TABS = ["JSON", "R", "Python", "CSS"] as const;
  type OutputTab = (typeof OUTPUT_TABS)[number];
  let activeTab = $state<OutputTab>("JSON");

  // Output text computed property
  const outputText = $derived(() => {
    const hexColors = $palette.map((color: any) => color.hex);
    switch (activeTab) {
      case "JSON":
        return JSON.stringify(hexColors, null, 2);
      case "R":
        return `c(\n${hexColors.map((h: any) => `  "${h}"`).join(",\n")}\n)`;
      case "Python":
        return `[\n${hexColors.map((h: any) => `  "${h}"`).join(",\n")}\n]`;
      case "CSS":
        return hexColors
          .map((h: any, i: any) => `--color${i + 1}: ${h};`)
          .join("\n");
      default:
        return "";
    }
  });

  function setActiveTab(tab: OutputTab) {
    activeTab = tab;
  }
</script>

<section>
  <h3 class="text-xl text-left font-semibold text-ink mb-4">Output</h3>
  <div class="mb-4 border-b flex flex-wrap gap-2 border-line">
    {#each OUTPUT_TABS as tab}
      <button
        class="
          px-2 sm:px-4 py-2 -mb-px border-b-2 font-mono text-sm transition-colors cursor-pointer
          {activeTab === tab
          ? 'border-accent text-link font-semibold'
          : 'border-transparent text-muted hover:text-link'}
        "
        onclick={() => setActiveTab(tab)}
        role="tab"
        aria-selected={activeTab === tab}
      >
        {tab}
      </button>
    {/each}
  </div>
  <div class="relative bg-surface rounded-lg p-4 mt-2 border border-line">
    <CopyButton
      text={outputText()}
      ariaLabel="Copy Output"
      title="Copy output"
      extraClass="absolute top-2 right-2 p-1"
      oncopied={() => showToast("Copied Output")}
    />
    <pre
      class="text-sm font-mono text-ink overflow-x-auto">{outputText()}</pre>
  </div>
</section>
