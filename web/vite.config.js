import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import viteCompression from "vite-plugin-compression";

export default defineConfig(({ mode }) => {
  const component = mode === "component";

  return {
    base: component ? "./" : "/app/",
    publicDir: component ? false : "public",
    plugins: [svelte(), !component && viteCompression({ algorithm: "brotliCompress" })],
    build: {
      target: "esnext",
      minify: "oxc",
      cssCodeSplit: !component,
      assetsInlineLimit: 4096,
      ...(component && {
        outDir: "../build/palette-generator",
        emptyOutDir: true,
        lib: {
          entry: "src/component.js",
          formats: ["es"],
          fileName: () => "index.js",
          cssFileName: "component",
        },
      }),
    },
    server: {
      fs: {
        allow: [".."],
      },
    },
  };
});
