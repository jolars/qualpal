"""Generate the GitHub Pages compatibility site after publishing to Workers."""

import argparse
import json
import shutil
from html import escape
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("site", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.site.resolve()):
        parser.error("The compatibility site must be outside the documentation site.")
    # Retain public images and other asset URLs used outside the documentation.
    shutil.copytree(
        args.site,
        args.output,
        dirs_exist_ok=True,
        ignore=shutil.ignore_patterns("_redirects"),
    )
    for page in args.site.rglob("*.html"):
        path = page.relative_to(args.site)
        suffix = "" if path.as_posix() == "index.html" else path.as_posix()
        target = "https://qualpal.cc/docs/" + suffix
        (args.output / path).write_text(f"""<!doctype html>
<html lang="en">
<meta charset="utf-8">
<title>Qualpal documentation has moved</title>
<link rel="canonical" href="{escape(target, quote=True)}">
<script>
const target = new URL({json.dumps(target)});
target.search = location.search;
target.hash = location.hash;
location.replace(target.href);
</script>
<meta http-equiv="refresh" content="0; url={escape(target, quote=True)}">
<p>The documentation has moved to <a href="{escape(target, quote=True)}">qualpal.cc/docs/</a>.</p>
</html>
""")
    (args.output / ".nojekyll").touch()
    (args.output / "404.html").write_text("""<!doctype html>
<html lang="en">
<meta charset="utf-8">
<title>Page not found</title>
<h1>Page not found</h1>
<p>Browse the <a href="https://qualpal.cc/docs/">Qualpal documentation</a>.</p>
</html>
""")


if __name__ == "__main__":
    main()
