<script lang="ts">
  export let open = false;
  export let onToggle: () => void;
  export let label: string;
</script>

<div class="bg-gray-50 p-3 rounded-lg">
  <h3>
    <button
      class="w-full flex justify-start items-center py-1 font-semibold bg-transparent transition rounded cursor-pointer"
      aria-expanded={open}
      on:click={onToggle}
    >
      <span class="flex-1 text-left text-gray-700">{label}</span>
      <svg
        class="w-4 h-4 ml-1 transition-transform {open ? 'rotate-180' : ''}"
        fill="none"
        stroke="currentColor"
        viewBox="0 0 24 24"
      >
        <path
          stroke-linecap="round"
          stroke-linejoin="round"
          stroke-width="3"
          d="M19 9l-7 7-7-7"
        />
      </svg>
    </button>
  </h3>
  <div class="py-2 text-sm" style="display: {open ? 'block' : 'none'};">
    <slot />
  </div>
</div>
