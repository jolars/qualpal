#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
workspace="$PWD/build/site-workspace"

if ! cmp -s site/sources.env "$workspace/prepared-sources.env" ||
   [[ ! -x "$workspace/python-env/bin/python" ||
      ! -f "$workspace/Doxyfile.api" ||
      ! -f "$workspace/qualpalr/src/qualpalr.so" ]]; then
  bash site/build.sh --prepare-only
fi

npm run build:component --prefix web

PATH="$workspace/python-env/bin:$PATH" MPLBACKEND=Agg \
  exec "${DIPLODOCUS:-diplodocus}" serve --host 127.0.0.1 --port "${DIPLODOCUS_PORT:-8000}" --output build/docs-preview
