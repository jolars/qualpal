<script lang="ts">
  export let message = "Loading...";
</script>

<div class="text-center py-8">
  <div
    class="animate-spin h-8 w-8 border-4 border-blue-500 border-t-transparent rounded-full mx-auto mb-4"
  ></div>
  <p class="text-gray-600">{message}</p>
</div>
