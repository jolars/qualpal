import assert from "node:assert/strict";
import test from "node:test";
import legacy from "../../site/legacy.json" with { type: "json" };
import { siteWorker } from "./index.js";

const worker = siteWorker({
  fetch: (request) => new Response(new URL(request.url).pathname, { status: 404 }),
});

test("redirects every historical page and preserves queries", async () => {
  for (const [old, target] of Object.entries(legacy.paths)) {
    const response = await worker.fetch(
      new Request(`https://qualpal.cc/docs/${old}?source=legacy`, { method: "HEAD" }),
    );
    assert.equal(response.status, 301);
    assert.equal(response.headers.get("Location"), `https://qualpal.cc/${target}?source=legacy`);
  }
});

test("canonicalizes directory URLs and delegates other requests", async () => {
  for (const path of ["/app", "/docs"]) {
    const response = await worker.fetch(new Request(`https://qualpal.cc${path}`));
    assert.equal(response.headers.get("Location"), `https://qualpal.cc${path}/`);
  }
  const response = await worker.fetch(new Request("https://qualpal.cc/docs/missing.html"));
  assert.equal(response.status, 404);
  assert.equal(await response.text(), "/docs/missing.html");
});
