function acceptsMarkdown(accept) {
  return (accept || "").split(",").some((range) => {
    const [type, ...parameters] = range.trim().toLowerCase().split(";");
    if (type.trim() !== "text/markdown") return false;
    const quality = parameters.find((parameter) =>
      parameter.trim().startsWith("q="),
    );
    const value = quality ? Number(quality.trim().slice(2)) : 1;
    return Number.isFinite(value) && value > 0 && value <= 1;
  });
}

export default {
  async fetch(request, env) {
    const url = new URL(request.url);
    const isPage = url.pathname === "/" || url.pathname === "/index.html";
    if (!isPage || !["GET", "HEAD"].includes(request.method)) {
      return env.ASSETS.fetch(request);
    }

    const markdown = acceptsMarkdown(request.headers.get("Accept"));
    if (markdown) url.pathname = "/index.md";
    const response = await env.ASSETS.fetch(new Request(url, request));
    const headers = new Headers(response.headers);
    const vary = headers.get("Vary");
    if (
      vary !== "*" &&
      !vary?.split(",").some((value) => value.trim().toLowerCase() === "accept")
    ) {
      headers.set("Vary", vary ? `${vary}, Accept` : "Accept");
    }
    if (markdown && response.ok) {
      headers.set("Content-Type", "text/markdown; charset=utf-8");
    }
    return new Response(response.body, {
      status: response.status,
      statusText: response.statusText,
      headers,
    });
  },
};
