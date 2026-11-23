"""Assemble the static assets and validate the historical documentation URLs."""

import html
import json
import re
import shutil
from pathlib import Path

root = Path("build/site")
legacy = json.loads(Path("site/legacy.json").read_text())
contract = json.loads(Path("docs/site-contract.json").read_text())
assert set(contract["paths"]) == set(legacy["paths"])
assert {key: set(ids) for key, ids in contract["anchors"].items()} == {
    key: set(ids) for key, ids in legacy["anchors"].items()
}


def page(route):
    return root / (route + ("index.html" if route.endswith("/") else ".html"))


def add_aliases(source, aliases):
    headings = list(re.finditer(r"<h[1-6]\b[^>]*>(.*?)</h[1-6]>", source))
    for anchor, label in aliases.items():
        if f'id="{anchor}"' in source:
            continue
        name, _, signature = label.partition("|")
        positions = []
        for index, heading in enumerate(headings):
            title = html.unescape(re.sub(r"<[^>]+>", "", heading[1])).strip()
            if title == name:
                end = headings[index + 1].start() if index + 1 < len(headings) else len(source)
                context = html.unescape(re.sub(r"<[^>]+>", "", source[heading.end():end]))
                if not signature or signature in context:
                    positions.append(heading.start())
        if not positions and name:
            raise SystemExit(f"Missing legacy anchor destination: {anchor} ({label})")
        pos = positions[0] if positions else next(
            heading.start() for heading in headings if heading.group().startswith("<h1")
        )
        source = source[:pos] + f'<span id="{html.escape(anchor, quote=True)}"></span>' + source[pos:]
        headings = list(re.finditer(r"<h[1-6]\b[^>]*>(.*?)</h[1-6]>", source))
    return source


for old, route in legacy["paths"].items():
    target = page(route)
    if not target.is_file():
        raise SystemExit(f"Missing legacy redirect target for {old}: {target}")
    if old in legacy["anchors"]:
        target.write_text(add_aliases(target.read_text(), legacy["anchors"][old]))

assets = root / "assets"
shutil.copyfile("docs/images/logo-small-dark.png", assets / "logo-small-dark.png")
with (assets / "site.css").open("a") as stylesheet:
    stylesheet.write(
        '\n:root[data-theme="dark"] .brand-logo {'
        ' content: url("./logo-small-dark.png"); }\n'
    )
