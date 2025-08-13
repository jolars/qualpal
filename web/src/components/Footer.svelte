<script lang="ts">
  import { createEventDispatcher } from "svelte";
  const dispatch = createEventDispatcher();
  const year = new Date().getFullYear();
  function openAbout() {
    dispatch("about");
  }
</script>

<footer class="border-t bg-white border-gray-200">
  <div
    class="max-w-7xl mx-auto px-4 py-5 flex flex-col md:flex-row gap-4 md:items-center justify-between"
  >
    <div class="text-gray-600 text-sm flex items-center gap-2">
      <span class="font-semibold text-gray-900">Johan Larsson</span>
      <span class="hidden sm:inline">© {year}</span>
    </div>
    <nav
      aria-label="Footer"
      class="flex flex-wrap items-center gap-x-5 gap-y-2 text-sm"
    >
      <button
        type="button"
        class="inline-flex items-center gap-1 text-gray-600 hover:text-gray-900 cursor-pointer"
        on:click={openAbout}
      >
        <svg
          xmlns="http://www.w3.org/2000/svg"
          width="16"
          height="16"
          fill="currentColor"
          class="bi bi-info-circle"
          viewBox="0 0 16 16"
        >
          <path
            d="M8 15A7 7 0 1 1 8 1a7 7 0 0 1 0 14m0 1A8 8 0 1 0 8 0a8 8 0 0 0 0 16"
          />
          <path
            d="m8.93 6.588-2.29.287-.082.38.45.083c.294.07.352.176.288.469l-.738 3.468c-.194.897.105 1.319.808 1.319.545 0 1.178-.252 1.465-.598l.088-.416c-.2.176-.492.246-.686.246-.275 0 -.375-.193-.304-.533zM9 4.5a1 1 0 1 1-2 0 1 1 0 0 1 2 0"
          />
        </svg>
        About
      </button>
      <a
        href="https://github.com/jolars/qualpal"
        class="inline-flex items-center gap-1 text-gray-600 hover:text-gray-900"
        target="_blank"
        rel="noopener"
      >
        <svg
          xmlns="http://www.w3.org/2000/svg"
          width="16"
          height="16"
          fill="currentColor"
          class="bi bi-github"
          viewBox="0 0 16 16"
        >
          <path
            d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18 1.32-.27 2-.27s1.36.09 2 .27c1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.01 8.01 0 0 0 16 8c0-4.42-3.58-8-8-8"
          />
        </svg>
        Source Code
      </a>
    </nav>
  </div>
</footer>
