import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import Sitemap from "vite-plugin-sitemap";
import viteCompression from "vite-plugin-compression";

export default defineConfig({
  plugins: [
    svelte(),
    Sitemap({ hostname: "https://qualpal.cc" }),
    viteCompression({ algorithm: "brotliCompress" }),
  ],
  build: {
    target: "esnext",
    minify: "esbuild",
    sourcemap: false,
    cssCodeSplit: true,
    assetsInlineLimit: 4096,
  },
  server: {
    fs: {
      allow: [".."],
    },
  },
});
