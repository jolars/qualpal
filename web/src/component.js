import { mount as mountSvelte, unmount } from "svelte";
import "./app.css";
import App from "./App.svelte";

export function mount(element, { assetBaseUrl }) {
  const shadow = element.attachShadow({ mode: "open" });
  const stylesheet = document.createElement("link");
  stylesheet.rel = "stylesheet";
  stylesheet.href = new URL("component.css", assetBaseUrl).href;
  const target = document.createElement("div");
  target.id = "app";
  shadow.append(stylesheet, target);
  const app = mountSvelte(App, { target, props: { embedded: true } });
  return () => unmount(app);
}
