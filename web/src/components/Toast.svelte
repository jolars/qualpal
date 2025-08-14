<script lang="ts">
  let { toast } = $props();
</script>

{#if $toast.show}
  <div
    class="fixed bottom-4 right-4 bg-gray-800 text-white px-4 py-3 rounded-lg shadow-lg z-50 transition-all duration-300"
  >
    <div class="flex items-center">
      <span class="mr-2">✓</span>
      {$toast.message}
    </div>
  </div>
{/if}
