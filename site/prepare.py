"""Correct examples in staged release sources before extraction."""

import sys
from pathlib import Path

workspace = Path(sys.argv[1])


def replace(path, old, new):
    source = path.read_text()
    if source.count(old) != 1:
        raise SystemExit(f"Expected one match in {path}: {old!r}")
    path.write_text(source.replace(old, new))


replace(
    workspace / "qualpalr/R/qualpal-deprecated.R",
    "#' The functions listed below",
    "#' @description\n#' The functions listed below",
)
python = workspace / "qualpal-py/qualpal"
replace(python / "color.py", ">>> red.distance(green)", ">>> round(red.distance(green), 2)")
replace(
    python / "color.py",
    ">>> red.distance('#00ff00', metric='din99d')",
    ">>> round(red.distance('#00ff00', metric='din99d'), 2)",
)
replace(
    python / "utils.py",
    ">>> print(palettes.keys())\n    dict_keys(['ColorBrewer', 'Pokemon', 'Ochre', ...])",
    ">>> 'ColorBrewer' in palettes\n    True",
)
