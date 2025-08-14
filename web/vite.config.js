import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import Sitemap from "vite-plugin-sitemap";

export default defineConfig({
  plugins: [svelte(), Sitemap({ hostname: "https://qualpal.cc" })],
  server: {
    fs: {
      allow: [".."],
    },
  },
});
