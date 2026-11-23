import legacy from "../../site/legacy.json" with { type: "json" };

export function siteWorker(pages) {
  return {
    async fetch(request, env) {
      if (request.method === "GET" || request.method === "HEAD") {
        const url = new URL(request.url);
        const path = url.pathname;
        const oldPage = path.startsWith("/docs/") && path.slice("/docs/".length);
        const target =
          path === "/app" ? "/app/" :
          path === "/docs" ? "/docs/" :
          oldPage && Object.hasOwn(legacy.paths, oldPage) ? "/" + legacy.paths[oldPage] :
          null;
        if (target) {
          url.pathname = target;
          return Response.redirect(url, 301);
        }
      }
      return pages.fetch(request, env);
    },
  };
}
