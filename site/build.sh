#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
source site/sources.env
workspace="$PWD/build/site-workspace"
mkdir -p "$workspace"
rm -f "$workspace/prepared-sources.env"
stage() {
  local name=$1 revision=$2 source="../$1" target="$workspace/$1"
  if [[ ! -d "$target/.git" ]]; then
    if [[ ! -d "$source" ]]; then source="https://github.com/jolars/$name.git"; fi
    git clone --quiet --no-checkout "$source" "$target"
  fi
  if ! git -C "$target" cat-file -e "$revision^{commit}" 2>/dev/null; then
    git -C "$target" fetch --quiet origin "$revision"
  fi
  git -C "$target" checkout --quiet --force --detach "$revision"
}
stage qualpalr "$QUALPALR_REV"
stage qualpal-py "$QUALPAL_PY_REV"
rm -rf "$workspace/qualpalr/vignettes"
printf 'PROJECT_NAME = Qualpal\nPROJECT_NUMBER = %s\nINPUT = include\nRECURSIVE = YES\nFILE_PATTERNS = *.h\nEXCLUDE_SYMBOLS = qualpal::detail qualpal::metrics::detail\nWARN_AS_ERROR = YES\nQUIET = YES\n' "$(cat version.txt)" > "$workspace/Doxyfile.api"
python3 site/prepare.py "$workspace"
(cd "$workspace/qualpalr" && Rscript --vanilla -e "pkgbuild::compile_dll('.', force=TRUE, compile_attributes=FALSE, debug=FALSE)")
python3 -m venv "$workspace/python-env"
"$workspace/python-env/bin/python" -m pip install 'sphinx==9.2.0' -e "$workspace/qualpal-py[viz]"
cp site/sources.env "$workspace/prepared-sources.env"
if [[ ${1:-} == --prepare-only ]]; then exit 0; fi
PATH="$workspace/python-env/bin:$PATH" MPLBACKEND=Agg "${DIPLODOCUS:-diplodocus}" build --no-cache --output build/site
npm run build --prefix web
python3 site/package.py
