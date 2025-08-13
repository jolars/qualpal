<script lang="ts">
  import CopyButton from "./CopyButton.svelte";
  let { OUTPUT_TABS, activeTab, setActiveTab, outputText, oncopied } = $props();
</script>

<section>
  <h3 class="text-xl text-left font-semibold text-gray-900 mb-4">Output</h3>
  <div class="mb-4 border-b flex gap-2 border-gray-200">
    {#each OUTPUT_TABS as tab}
      <button
        class="
          px-4 py-2 -mb-px border-b-2 font-mono text-sm transition-colors cursor-pointer
          {activeTab === tab
          ? 'border-blue-600 text-blue-700 font-semibold'
          : 'border-transparent text-gray-500 hover:text-blue-600'}
        "
        onclick={() => setActiveTab(tab)}
        role="tab"
        aria-selected={activeTab === tab}
      >
        {tab}
      </button>
    {/each}
  </div>
  <div class="relative bg-gray-50 rounded-lg p-4 mt-2 border border-gray-200">
    <CopyButton
      text={outputText()}
      ariaLabel="Copy output"
      title="Copy output"
      extraClass="absolute top-2 right-2 p-1"
      {oncopied}
    />
    <pre
      class="text-sm font-mono text-gray-700 overflow-x-auto">{outputText()}</pre>
  </div>
</section>
