import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import viteCompression from "vite-plugin-compression";

const docsTarget = "http://127.0.0.1:8000";

const docsProxy = {
  "^/(?:index\\.html)?(?:\\?.*)?$": {
    target: docsTarget,
    selfHandleResponse: true,
    configure(proxy) {
      proxy.on("proxyRes", (upstream, request, response) => {
        const headers = { ...upstream.headers };
        delete headers["content-length"];
        response.writeHead(upstream.statusCode, headers);
        if (request.method === "HEAD") {
          upstream.resume();
          response.end();
          return;
        }
        upstream.setEncoding("utf8");
        let body = "";
        upstream.on("data", (chunk) => { body += chunk; });
        upstream.on("end", () => {
          response.end(body.replaceAll("https://qualpal.cc/app/", "/app/"));
        });
        upstream.on("error", (error) => response.destroy(error));
      });
    },
  },
  "^/(?!app(?:/|$|\\?))": { target: docsTarget },
};

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
    proxy: process.env.QUALPAL_DOCS_PREVIEW ? docsProxy : undefined,
    fs: {
      allow: [".."],
    },
  },
});
