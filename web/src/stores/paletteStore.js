import { writable, derived } from "svelte/store";

// Palette parameters
export const paletteParams = writable({
  numColors: 5,
  hueMin: 20,
  hueMax: 280,
  satMin: 0.4,
  satMax: 0.8,
  lightMin: 0.3,
  lightMax: 0.7,
  useBackground: false,
  backgroundColor: "#ffffff",
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
let qualpalModule = null;

// Debounce timer
let debounceTimer;

// Helper function to convert hex to RGB
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

// Helper function to parse existing palette string
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

// Initialize the qualpal module
export async function initializeModule() {
  try {
    console.log("Loading Qualpal module...");
    const createQualpalModule = (await import("../qualpal.mjs")).default;
    qualpalModule = await createQualpalModule();
    moduleLoaded.set(true);
    console.log("Qualpal loaded successfully!");
    return true;
  } catch (error) {
    console.error("Failed to load Qualpal:", error);
    moduleLoaded.set(false);
    return false;
  }
}

// Generate palette function
export async function generatePalette(params) {
  if (!qualpalModule) return;

  loading.set(true);

  try {
    const qp = new qualpalModule.Qualpal();

    // Handle wraparound hue ranges using negative values
    let normalizedHueMin = params.hueMin;
    let normalizedHueMax = params.hueMax;

    // If we have a wraparound case (min > max), convert max to negative
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

    if (params.useBackground) {
      const bgColor = hexToRgb(params.backgroundColor);
      if (bgColor) {
        qp.setBackground(bgColor.r / 255, bgColor.g / 255, bgColor.b / 255);
      }
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
      // Parse existing palette and extend it
      const existingColors = parseExistingPalette(params.existingPalette);
      if (existingColors.length > 0) {
        console.log("Extending palette with existing colors:", existingColors);
        newPalette = qp.extend(existingColors, params.numColors);
      } else {
        console.warn("No valid existing colors found, generating new palette");
        newPalette = qp.generate(params.numColors);
      }
    } else {
      // Generate new palette
      newPalette = qp.generate(params.numColors);
    }

    palette.set(newPalette);
    console.log("Generated palette:", newPalette);

    // Analyze palette
    try {
      const result = await qualpalModule.analyzePalette(
        newPalette.map((c) => hexToRgb(c.hex)),
        params.cvd,
        params.useBackground ? hexToRgb(params.backgroundColor) : null,
        4,
      );
      console.log("Palette analysis result:", result);
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

// Debounced generate function
export function debouncedGenerate(params) {
  clearTimeout(debounceTimer);
  debounceTimer = setTimeout(() => {
    generatePalette(params);
  }, 300);
}

// Auto-generate when parameters change
export const autoGenerate = derived(paletteParams, ($params, set) => {
  debouncedGenerate($params);
});
