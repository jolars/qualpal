import { writable, derived, get } from "svelte/store";

export const cvdSimulation = writable({
  enabled: false,
  type: "protan", // "protan" | "deutan" | "tritan"
  severity: 0.5, // 0-1
});

let qualpalModule = null;

export function setQualpalModule(module) {
  qualpalModule = module;
}

export function simulateColor(hex) {
  const simulation = get(cvdSimulation);
  if (!simulation.enabled || !qualpalModule) {
    return hex;
  }

  // Convert hex to RGB (0-1 range)
  const r = parseInt(hex.slice(1, 3), 16) / 255;
  const g = parseInt(hex.slice(3, 5), 16) / 255;
  const b = parseInt(hex.slice(5, 7), 16) / 255;

  try {
    const result = qualpalModule.simulateCvd(
      r,
      g,
      b,
      simulation.type,
      simulation.severity,
    );
    return result.hex;
  } catch (error) {
    console.error("CVD simulation failed:", error);
    return hex;
  }
}
