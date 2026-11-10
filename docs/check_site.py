"""Check generated documentation links and the published URL contract."""

import argparse
import json
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import unquote, urljoin, urlsplit


class Page(HTMLParser):
    def __init__(self, source):
        super().__init__()
        self.anchors = set()
        self.references = []
        self.feed(source)

    def handle_starttag(self, tag, attributes):
        attributes = dict(attributes)
        for name in ("id", "name"):
            if name in attributes:
                self.anchors.add(attributes[name])
        attribute = {
            "a": "href",
            "link": "href",
            "img": "src",
            "script": "src",
            "iframe": "src",
            "object": "data",
        }.get(tag)
        if attribute in attributes:
            self.references.append(attributes[attribute])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("site", type=Path)
    args = parser.parse_args()
    contract = json.loads(Path(__file__).with_name("site-contract.json").read_text())
    pages = {
        path.relative_to(args.site).as_posix(): Page(path.read_text())
        for path in args.site.rglob("*.html")
    }
    errors = []
    for path in contract["paths"]:
        if path not in pages:
            errors.append(f"Missing published page: {path}")
    for path, anchors in contract["anchors"].items():
        if path in pages:
            for anchor in anchors:
                if anchor not in pages[path].anchors:
                    errors.append(f"Missing published anchor: {path}#{anchor}")
    for path, page in pages.items():
        for reference in page.references:
            target = urlsplit(urljoin(f"https://qualpal.cc/docs/{path}", reference))
            if target.netloc != "qualpal.cc" or not target.path.startswith("/docs/"):
                continue
            name = unquote(target.path.removeprefix("/docs/"))
            if not name or name.endswith("/"):
                name += "index.html"
            if not (args.site / name).is_file():
                errors.append(f"Broken reference in {path}: {reference}")
    if errors:
        raise SystemExit("\n".join(errors))
    print(
        f"Checked {len(pages)} HTML pages and {len(contract['paths'])} published paths."
    )


if __name__ == "__main__":
    main()
