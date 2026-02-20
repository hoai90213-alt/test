#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
MIN_IOS="${MIN_IOS:-14.0}"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build/ios-native}"
OUT_DIR="${OUT_DIR:-$ROOT_DIR/artifacts}"
RUNTIME_OUT_DIR="$OUT_DIR/runtime-libs"
LOG_PATH="$OUT_DIR/runtime-probe-build.log"

mkdir -p "$OUT_DIR" "$RUNTIME_OUT_DIR"
rm -rf "$BUILD_DIR"

{
  echo "[runtime_probe] ROOT_DIR=$ROOT_DIR"
  echo "[runtime_probe] MIN_IOS=$MIN_IOS"
  echo "[runtime_probe] BUILD_DIR=$BUILD_DIR"
  echo "[runtime_probe] RUNTIME_OUT_DIR=$RUNTIME_OUT_DIR"

  SDK_PATH="$(xcrun --sdk iphoneos --show-sdk-path)"
  echo "[runtime_probe] SDK_PATH=$SDK_PATH"

  cmake -S "$ROOT_DIR/app/src/main/cpp" -B "$BUILD_DIR" \
    -G Xcode \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_SYSROOT="$SDK_PATH" \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="$MIN_IOS" \
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
    -DCMAKE_BUILD_TYPE=Release \
    -DZOMDROID_BUILD_GLFW=OFF \
    -DZOMDROID_BUILD_ANDROID_JNI=OFF \
    -DZOMDROID_BUILD_LINKER=ON \
    -DARM_DYNAREC=ON \
    -DARM64=ON

  cmake --build "$BUILD_DIR" --config Release -j"$(sysctl -n hw.logicalcpu)"

  mapfile -t dylibs < <(find "$BUILD_DIR" -type f -name "*.dylib" | sort)
  if [[ "${#dylibs[@]}" -eq 0 ]]; then
    echo "[runtime_probe] No dylibs were produced"
    exit 1
  fi

  echo "[runtime_probe] Produced dylibs:"
  printf '  %s\n' "${dylibs[@]}"

  cp -f "${dylibs[@]}" "$RUNTIME_OUT_DIR/"
  ls -la "$RUNTIME_OUT_DIR"
} 2>&1 | tee "$LOG_PATH"

echo "[runtime_probe] Done"
