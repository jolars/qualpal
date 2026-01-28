import { writable, derived, get } from "svelte/store";
import { setQualpalModule, simulateColor, cvdSimulation } from "./cvdStore.js";

export const paletteParams = writable({
  numColors: 5,
  // Legacy single region params (kept for backward compatibility)
  hueMin: 170,
  hueMax: 60,
  satMin: 0.0,
  satMax: 0.7,
  lightMin: 0.2,
  lightMax: 0.8,
  // New multi-region support
  colorspaceRegions: [
    {
      hueMin: 170,
      hueMax: 60,
      satMin: 0.0,
      satMax: 0.7,
      lightMin: 0.2,
      lightMax: 0.8,
    },
  ],
  useBackground: false,
  backgroundColor: "#ffffff",
  whitePoint: "D65", // D65, D50, D55, A, E
  inputMode: "colorspace", // 'colorspace' | 'fixed'
  fixedInput: "",
  useExtend: false,
  existingPalette: "",
  cvd: {
    protan: 0,
    deutan: 0,
    tritan: 0,
  },
});

// Palette state
export const palette = writable([]);
export const analysis = writable(null);
export const loading = writable(false);

// Module state
export const moduleLoaded = writable(false);
export const availablePalettes = writable({}); // { domain: [palette,...] }
let qualpalModule = null;

// Debounce timer
let debounceTimer;

/**
 * @param {string} hex
 * @returns {{r: number, g: number, b: number} | null}
 */
function hexToRgb(hex) {
  const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result
    ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16),
      }
    : null;
}

// Helper to enumerate object keys from emscripten val-like objects
/**
 * @param {object} o
 * @returns {string[]}
 */
function jsObjectKeys(o) {
  return Object.keys(o);
}

// Helper function to parse existing palette string
/**
 * @param {string} input
 * @returns {{r: number, g: number, b: number}[]}
 */
function parseExistingPalette(input) {
  if (!input || !input.trim()) return [];

  const raw = input.trim();

  // Extract all hex codes first (handles pasted JSON, R c(""), Python lists, CSS, etc.)
  const hexMatches = raw.match(/#[0-9A-Fa-f]{6}/g) || [];

  let candidates;
  if (hexMatches.length > 0) {
    candidates = hexMatches;
  } else {
    // Fallback split
    candidates = raw
      .split(/[,;\s\n]+/)
      .map((c) => c.trim())
      .filter(Boolean);
  }

  const seen = new Set();
  const result = [];
  for (const c of candidates) {
    const up = c.toUpperCase();
    if (/^#[0-9A-F]{6}$/.test(up) && !seen.has(up)) {
      seen.add(up);
      const rgb = hexToRgb(up);
      if (rgb) {
        result.push({ r: rgb.r / 255, g: rgb.g / 255, b: rgb.b / 255 });
      }
    }
  }
  return result;
}

// Extract unique uppercase hex list (for fixed input mode)
/**
 * @param {string} input
 * @returns {string[]}
 */
function extractHexList(input) {
  if (!input || !input.trim()) return [];
  const raw = input.trim();
  const hexMatches = raw.match(/#[0-9A-Fa-f]{6}/g) || [];
  let candidates;
  if (hexMatches.length > 0) {
    candidates = hexMatches;
  } else {
    candidates = raw
      .split(/[,;\s\n]+/)
      .map((c) => c.trim())
      .filter(Boolean);
  }
  const seen = new Set();
  const out = [];
  for (const c of candidates) {
    const up = c.toUpperCase();
    if (/^#[0-9A-F]{6}$/.test(up) && !seen.has(up)) {
      seen.add(up);
      out.push(up);
    }
  }
  return out;
}

// Initialize the qualpal module
export async function initializeModule() {
  try {
    console.log("Loading Qualpal module...");
    const createQualpalModule = (await import("../qualpal.mjs")).default;
    const module = await createQualpalModule();
    qualpalModule = module;
    setQualpalModule(module);

    moduleLoaded.set(true);
    console.log("Qualpal loaded successfully!");

    // Load built‑in palettes
    try {
      const data = qualpalModule.listAvailablePalettes();
      const obj = {};
      const keys = jsObjectKeys(data);
      for (const k of keys) {
        const arr = data[k];
        const len = arr.length;
        obj[k] = [];
        for (let i = 0; i < len; i++) obj[k].push(arr[i]);
      }
      availablePalettes.set(obj);
    } catch (e) {
      console.warn("Could not load available palettes:", e);
    }
    return true;
  } catch (error) {
    console.error("Failed to load Qualpal:", error);
    moduleLoaded.set(false);
    return false;
  }
}

// Helper: get built-in palette hex list (domain + palette)
/**
 * @param {string} domain
 * @param {string} name
 * @returns {string[]}
 */
export function getPaletteHex(domain, name) {
  if (!qualpalModule) return [];
  try {
    const arr = qualpalModule.getPaletteHex(domain, name);
    const out = [];
    const len = arr.length;
    for (let i = 0; i < len; i++) {
      out.push(String(arr[i]).toUpperCase());
    }
    return out;
  } catch (e) {
    console.warn("getPaletteHex failed:", e);
    return [];
  }
}

// Generate palette function
/**
 * @param {object} params
 * @returns {Promise<void>}
 */
export async function generatePalette(params) {
  if (!qualpalModule) return;

  loading.set(true);

  try {
    const qp = new qualpalModule.Qualpal();

    // Choose input source
    if (params.inputMode === "fixed") {
      const hexList = extractHexList(params.fixedInput);
      if (hexList.length > 0) {
        qp.setInputHex(hexList);
      } else {
        let normalizedHueMin = params.hueMin;
        let normalizedHueMax = params.hueMax;

        if (params.hueMin > params.hueMax) {
          normalizedHueMin = params.hueMin - 360;
        }

        qp.setInputColorspace(
          normalizedHueMin,
          normalizedHueMax,
          params.satMin,
          params.satMax,
          params.lightMin,
          params.lightMax,
        );
      }
    } else {
      // Use multi-region API
      const regions = (params.colorspaceRegions || []).map((region) => {
        let normalizedHueMin = region.hueMin;
        if (region.hueMin > region.hueMax) {
          normalizedHueMin = region.hueMin - 360;
        }
        return {
          h_min: normalizedHueMin,
          h_max: region.hueMax,
          s_or_c_min: region.satMin,
          s_or_c_max: region.satMax,
          l_min: region.lightMin,
          l_max: region.lightMax,
        };
      });

      if (regions.length > 0) {
        qp.setInputColorspaceRegions(regions, "HSL");
      } else {
        // Fallback to legacy single region
        let normalizedHueMin = params.hueMin;
        let normalizedHueMax = params.hueMax;
        if (params.hueMin > params.hueMax) {
          normalizedHueMin = params.hueMin - 360;
        }
        qp.setInputColorspace(
          normalizedHueMin,
          normalizedHueMax,
          params.satMin,
          params.satMax,
          params.lightMin,
          params.lightMax,
        );
      }
    }

    if (params.useBackground) {
      const bgColor = hexToRgb(params.backgroundColor);
      if (bgColor) {
        qp.setBackground(bgColor.r / 255, bgColor.g / 255, bgColor.b / 255);
      }
    }

    // Set white point
    if (params.whitePoint && qualpalModule?.Qualpal.prototype.setWhitePoint) {
      qp.setWhitePoint(params.whitePoint);
    }

    // Set CVD if any slider > 0
    const { protan, deutan, tritan } = params.cvd;
    if (
      (protan > 0 || deutan > 0 || tritan > 0) &&
      qualpalModule?.Qualpal.prototype.setCvd
    ) {
      qp.setCvd({ protan, deutan, tritan });
    }

    let newPalette;
    if (params.useExtend && params.existingPalette.trim()) {
      const existingColors = parseExistingPalette(params.existingPalette);
      if (existingColors.length > 0) {
        console.log("Extending palette with existing colors:", existingColors);
        newPalette = qp.extend(existingColors, params.numColors);
      } else {
        console.warn("No valid existing colors found, generating new palette");
        newPalette = qp.generate(params.numColors);
      }
    } else {
      newPalette = qp.generate(params.numColors);
    }

    palette.set(newPalette);

    // Analyze palette
    try {
      const sim = get(cvdSimulation);
      const result = await qualpalModule.analyzePalette(
        newPalette.map((/** @type {{hex: string}} */ c) => hexToRgb(c.hex)),
        sim.enabled
          ? {
              protan: sim.severity,
              deutan: sim.severity,
              tritan: sim.severity,
            }
          : params.cvd,
        params.useBackground ? hexToRgb(params.backgroundColor) : null,
        4,
      );
      analysis.set(result);
    } catch (e) {
      console.error("Error analyzing palette:", e);
      analysis.set(null);
    }
  } catch (error) {
    console.error("Error generating palette:", error);
    throw error;
  } finally {
    loading.set(false);
  }
}

export const autoAnalyze = derived(
  [palette, cvdSimulation, paletteParams],
  ([$palette, $cvdSimulation, $paletteParams], set) => {
    if (!$palette || $palette.length === 0 || !qualpalModule) {
      analysis.set(null);
      return;
    }
    const cvd = $cvdSimulation.enabled
      ? {
          protan: $cvdSimulation.severity,
          deutan: $cvdSimulation.severity,
          tritan: $cvdSimulation.severity,
        }
      : $paletteParams.cvd;
    const bg = $paletteParams.useBackground
      ? hexToRgb($paletteParams.backgroundColor)
      : null;
    try {
      const result = qualpalModule.analyzePalette(
        $palette.map((c) => hexToRgb(c.hex)),
        cvd,
        bg,
        4,
      );
      analysis.set(result);
    } catch {
      analysis.set(null);
    }
  },
);

// Debounced generate function
/**
 * @param {object} [params]
 */
export function debouncedGenerate(params) {
  clearTimeout(debounceTimer);
  const p = params ?? get(paletteParams); // fallback to current store if not passed
  debounceTimer = setTimeout(() => {
    generatePalette(p);
  }, 300);
}

// Auto-generate when parameters change
export const autoGenerate = derived(paletteParams, ($params) => {
  debouncedGenerate($params);
});
