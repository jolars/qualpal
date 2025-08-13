import { readFileSync, writeFileSync, mkdirSync } from "node:fs";
import { dirname } from "node:path";
import yaml from "yaml";
import { Cite } from "@citation-js/core";
import "@citation-js/plugin-cff";
import "@citation-js/plugin-bibtex";
import "@citation-js/plugin-csl";

function ensureDir(path) {
  try {
    mkdirSync(path, { recursive: true });
  } catch {}
}

const CFF_PATH = "../CITATION.cff";
const OUT_PATH = "src/lib/citations.generated.json";

function parseDateParts(dateStr) {
  if (!dateStr) return undefined;
  const [y, m, d] = dateStr.split("-").map((n) => parseInt(n, 10));
  if (!y) return undefined;
  return { "date-parts": [[y, m || 1, d || 1]] };
}

const raw = readFileSync(CFF_PATH, "utf8");
const cff = yaml.parse(raw);

// Build minimal CSL JSON object
const authors =
  (cff.authors || []).map((a) => ({
    given: a["given-names"] || a.given || "",
    family: a["family-names"] || a.family || "",
  })) || [];

const csl = {
  id:
    cff.identifiers?.[0]?.value ||
    cff.title?.toLowerCase().replace(/\s+/g, "-") ||
    "software",
  type: "software",
  title: cff.title,
  version: cff.version,
  URL: cff["repository-code"] || cff.url,
  abstract: cff.abstract,
  issued: parseDateParts(cff["date-released"]),
  author: authors,
  license: cff.license,
};

async function generateCitations() {
  const cite = new Cite(csl);

  function trimTrailing(str) {
    return str.replace(/\s+$/g, "");
  }

  const bibtex = trimTrailing(cite.format("bibtex"));
  const bibliographyStyles = [
    { key: "apa", template: "apa", label: "APA" },
    { key: "vancouver", template: "vancouver", label: "Vancouver" },
    { key: "chicago", template: "chicago-author-date", label: "Chicago" },
  ].map((s) => ({
    ...s,
    value: trimTrailing(
      cite.format("bibliography", { format: "text", template: s.template }),
    ),
  }));

  const biblatex = cite.format("biblatex");

  const cslYaml = yaml.stringify({
    id: csl.id,
    type: csl.type,
    title: csl.title,
    version: csl.version,
    abstract: csl.abstract,
    URL: csl.URL,
    issued: csl.issued,
    author: csl.author,
    license: csl.license,
  });

  const formats = [
    { key: "bibtex", label: "BibTeX", value: bibtex, code: true },
    { key: "biblatex", label: "BibLaTeX", value: biblatex, code: true },
    { key: "csl", label: "CSL YAML", value: `---\n${cslYaml}---`, code: true },
    {
      key: "bibliography",
      label: "Bibliography",
      value: bibliographyStyles[0].value,
      code: false,
      styles: bibliographyStyles.map(({ key, label, value }) => ({
        key,
        label,
        value,
      })),
    },
  ];

  ensureDir(dirname(OUT_PATH));
  writeFileSync(OUT_PATH, JSON.stringify(formats, null, 2) + "\n", "utf8");

  console.log(`Generated ${formats.length} citation formats -> ${OUT_PATH}`);
}

generateCitations().catch(console.error);
