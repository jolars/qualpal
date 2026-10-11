import assert from "node:assert/strict";
import { test } from "node:test";
import worker from "./index.js";

function assets() {
  return {
    ASSETS: {
      async fetch(request) {
        const markdown = new URL(request.url).pathname === "/index.md";
        return new Response(
          request.method === "HEAD"
            ? null
            : markdown
              ? "# Qualpal\n"
              : "<!doctype html>",
          {
            headers: {
              "Content-Type": markdown ? "text/plain" : "text/html",
              Vary: "Accept-Encoding",
              "Cache-Control": "public, max-age=0, must-revalidate",
            },
          },
        );
      },
    },
  };
}

test("negotiates Markdown only for an explicit, acceptable media type", async () => {
  for (const [accept, markdown] of [
    ["text/markdown", true],
    ["text/html, text/markdown;q=0.5", true],
    ["TEXT/MARKDOWN; charset=utf-8", true],
    ["text/markdown;q=0", false],
    ["text/markdown;q=invalid", false],
    ["text/markdown-extra", false],
    ["text/html, */*;q=0.8", false],
    ["", false],
  ]) {
    const response = await worker.fetch(
      new Request("https://qualpal.cc/", { headers: { Accept: accept } }),
      assets(),
    );
    assert.equal(
      response.headers.get("Content-Type"),
      markdown ? "text/markdown; charset=utf-8" : "text/html",
      accept,
    );
    assert.equal(response.headers.get("Vary"), "Accept-Encoding, Accept");
    assert.equal(
      await response.text(),
      markdown ? "# Qualpal\n" : "<!doctype html>",
    );
  }
});

test("supports HEAD and leaves asset requests alone", async () => {
  const head = await worker.fetch(
    new Request("https://qualpal.cc/?source=agent", {
      method: "HEAD",
      headers: { Accept: "text/markdown" },
    }),
    assets(),
  );
  assert.equal(
    head.headers.get("Content-Type"),
    "text/markdown; charset=utf-8",
  );
  assert.equal(await head.text(), "");
  const response = await worker.fetch(
    new Request("https://qualpal.cc/favicon.svg", {
      headers: { Accept: "text/markdown" },
    }),
    assets(),
  );
  assert.equal(response.headers.get("Vary"), "Accept-Encoding");
});
