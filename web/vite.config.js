import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import viteCompression from "vite-plugin-compression";

export default defineConfig({
  base: "/app/",
  plugins: [
    svelte(),
    viteCompression({ algorithm: "brotliCompress" }),
  ],
  build: {
    target: "esnext",
    minify: "oxc",
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
