#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
MIN_IOS="${MIN_IOS:-14.0}"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build/ios-native}"
OUT_DIR="${OUT_DIR:-$ROOT_DIR/artifacts}"
RUNTIME_OUT_DIR="$OUT_DIR/runtime-libs"
LOG_PATH="$OUT_DIR/runtime-probe-build.log"
BUILD_CONFIG="${BUILD_CONFIG:-Release}"
CMAKE_GENERATOR="${CMAKE_GENERATOR:-Xcode}"
ZOMDROID_BUILD_GLFW="${ZOMDROID_BUILD_GLFW:-OFF}"
ZOMDROID_BUILD_LINKER="${ZOMDROID_BUILD_LINKER:-ON}"
ZOMDROID_BUILD_ANDROID_JNI="${ZOMDROID_BUILD_ANDROID_JNI:-OFF}"
ARM_DYNAREC="${ARM_DYNAREC:-ON}"
ARM64="${ARM64:-ON}"
REQUIRED_RUNTIME_LIBS="${REQUIRED_RUNTIME_LIBS:-libbox64.dylib libzomdroid.dylib libzomdroidlinker.dylib}"

mkdir -p "$OUT_DIR" "$RUNTIME_OUT_DIR"
rm -rf "$BUILD_DIR"

{
  echo "[runtime_probe] ROOT_DIR=$ROOT_DIR"
  echo "[runtime_probe] MIN_IOS=$MIN_IOS"
  echo "[runtime_probe] BUILD_DIR=$BUILD_DIR"
  echo "[runtime_probe] RUNTIME_OUT_DIR=$RUNTIME_OUT_DIR"
  echo "[runtime_probe] BUILD_CONFIG=$BUILD_CONFIG"
  echo "[runtime_probe] CMAKE_GENERATOR=$CMAKE_GENERATOR"
  echo "[runtime_probe] ZOMDROID_BUILD_GLFW=$ZOMDROID_BUILD_GLFW"
  echo "[runtime_probe] ZOMDROID_BUILD_LINKER=$ZOMDROID_BUILD_LINKER"
  echo "[runtime_probe] ZOMDROID_BUILD_ANDROID_JNI=$ZOMDROID_BUILD_ANDROID_JNI"

  SDK_PATH="$(xcrun --sdk iphoneos --show-sdk-path)"
  echo "[runtime_probe] SDK_PATH=$SDK_PATH"

  cmake -S "$ROOT_DIR/app/src/main/cpp" -B "$BUILD_DIR" \
    -G "$CMAKE_GENERATOR" \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_SYSROOT="$SDK_PATH" \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="$MIN_IOS" \
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
    -DCMAKE_BUILD_TYPE="$BUILD_CONFIG" \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED=NO \
    -DZOMDROID_BUILD_GLFW="$ZOMDROID_BUILD_GLFW" \
    -DZOMDROID_BUILD_ANDROID_JNI="$ZOMDROID_BUILD_ANDROID_JNI" \
    -DZOMDROID_BUILD_LINKER="$ZOMDROID_BUILD_LINKER" \
    -DARM_DYNAREC="$ARM_DYNAREC" \
    -DARM64="$ARM64"

  build_targets=(zomdroid)
  if [[ "$ZOMDROID_BUILD_LINKER" == "ON" ]]; then
    build_targets+=(zomdroidlinker box64)
  fi

  echo "[runtime_probe] Building targets: ${build_targets[*]}"
  cmake --build "$BUILD_DIR" --config "$BUILD_CONFIG" --target "${build_targets[@]}" -j"$(sysctl -n hw.logicalcpu)"

  dylibs=()
  while IFS= read -r dylib; do
    dylibs+=("$dylib")
  done < <(find "$BUILD_DIR" -type f -name "*.dylib" | sort)
  if [[ "${#dylibs[@]}" -eq 0 ]]; then
    echo "[runtime_probe] No dylibs were produced"
    exit 1
  fi

  echo "[runtime_probe] Produced dylibs:"
  for dylib in "${dylibs[@]}"; do
    dylib_size="$(stat -f%z "$dylib" 2>/dev/null || stat -c%s "$dylib")"
    echo "  $dylib ($dylib_size bytes)"
  done

  cp -f "${dylibs[@]}" "$RUNTIME_OUT_DIR/"
  missing_required=()
  for required_lib in $REQUIRED_RUNTIME_LIBS; do
    if [[ ! -f "$RUNTIME_OUT_DIR/$required_lib" ]]; then
      missing_required+=("$required_lib")
    fi
  done

  if [[ "${#missing_required[@]}" -gt 0 ]]; then
    echo "[runtime_probe] Missing required runtime libs:"
    printf '  %s\n' "${missing_required[@]}"
    echo "[runtime_probe] Available runtime libs:"
    ls -la "$RUNTIME_OUT_DIR"
    exit 1
  fi

  echo "[runtime_probe] Runtime package ready:"
  ls -la "$RUNTIME_OUT_DIR"
} 2>&1 | tee "$LOG_PATH"

echo "[runtime_probe] Done"
