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
const citation = cff["preferred-citation"] || cff;

// Build minimal CSL JSON object
const authors =
  (citation.authors || []).map((a) => ({
    given: a["given-names"] || a.given || "",
    family: a["family-names"] || a.family || "",
  })) || [];

const issuedYear =
  citation["date-published"]?.split("-")[0] ||
  citation["date-released"]?.split("-")[0];

const csl = {
  id: "larsson_2025_qualpal",
  type: citation.type || "article-journal",
  title: citation.title,
  author: authors.map((a) => ({
    family: a.family,
    given: a.given,
  })),
  issued: parseDateParts(
    citation["date-published"] || citation["date-released"],
  ),
  date: citation["date-published"] || citation["date-released"],
  "container-title": citation.journal,
  DOI: citation.doi,
  ISSN: citation.issn,
  issue: citation.issue,
  page: citation.start ? String(citation.start) : undefined,
  URL: citation.url,
  volume: citation.volume,
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
    { key: "harvard", template: "harvard1", label: "Harvard" },
  ].map((s) => ({
    ...s,
    value: trimTrailing(
      cite.format("bibliography", { format: "text", template: s.template }),
    ),
  }));

  const biblatex = cite.format("biblatex");

  const cslYaml = yaml.stringify({
    references: [
      {
        id: csl.id,
        type: csl.type,
        title: csl.title,
        author: csl.author,
        issued: [
          {
            year: issuedYear ? parseInt(issuedYear, 10) : undefined,
            month:
              citation["date-published"]?.split("-")[1] ||
              citation["date-released"]?.split("-")[1],
            day:
              citation["date-published"]?.split("-")[2] ||
              citation["date-released"]?.split("-")[2],
          },
        ],
        "container-title": csl["container-title"],
        DOI: csl.DOI,
        ISSN: csl.ISSN,
        issue: csl.issue,
        page: csl.page,
        URL: csl.URL,
        volume: csl.volume,
      },
    ],
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
