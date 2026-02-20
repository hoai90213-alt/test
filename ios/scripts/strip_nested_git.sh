#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

targets=(
  "$ROOT_DIR/app/src/main/cpp/box64/.git"
  "$ROOT_DIR/app/src/main/cpp/glfw/.git"
)

for path in "${targets[@]}"; do
  if [[ -e "$path" ]]; then
    echo "[strip_nested_git] removing $path"
    rm -rf "$path"
  else
    echo "[strip_nested_git] skip (not found): $path"
  fi
done

echo "[strip_nested_git] done"
