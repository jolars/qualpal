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

npm run build:citations --prefix web

pids=()
trap 'if (("${#pids[@]}")); then kill "${pids[@]}" 2>/dev/null || true; fi' EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

PATH="$workspace/python-env/bin:$PATH" MPLBACKEND=Agg "${DIPLODOCUS:-diplodocus}"   serve --no-cache --host 127.0.0.1 --port 8000 --output build/docs-preview &
pids+=("$!")
(
  cd web
  QUALPAL_DOCS_PREVIEW=1 exec ./node_modules/.bin/vite --host 127.0.0.1 --strictPort
) &
pids+=("$!")
wait -n "${pids[@]}"
