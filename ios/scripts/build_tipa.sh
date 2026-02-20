#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
APP_NAME="${APP_NAME:-ZomdroidIOSPoC}"
BUNDLE_ID="${BUNDLE_ID:-org.zomdroid.iospoc}"
MIN_IOS="${MIN_IOS:-14.0}"
OUT_DIR="${OUT_DIR:-$ROOT_DIR/artifacts}"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build/ios}"
APP_DIR="$BUILD_DIR/$APP_NAME.app"
FRAMEWORKS_DIR="$APP_DIR/Frameworks"
PAYLOAD_DIR="$OUT_DIR/Payload"
TIPA_PATH="$OUT_DIR/${APP_NAME}.tipa"
RUNTIME_LIB_DIR="${RUNTIME_LIB_DIR:-$OUT_DIR/runtime-libs}"

SDK_PATH="$(xcrun --sdk iphoneos --show-sdk-path)"
CLANG_BIN="$(xcrun --sdk iphoneos --find clang)"

echo "[build_tipa] Cleaning previous build output"
rm -rf "$BUILD_DIR" "$PAYLOAD_DIR" "$TIPA_PATH"
mkdir -p "$APP_DIR" "$FRAMEWORKS_DIR" "$PAYLOAD_DIR"

echo "[build_tipa] Compiling iOS executable"
"$CLANG_BIN" \
  -arch arm64 \
  -isysroot "$SDK_PATH" \
  -miphoneos-version-min="$MIN_IOS" \
  -fobjc-arc \
  -Wl,-rpath,@executable_path/Frameworks \
  -framework UIKit \
  -framework Foundation \
  "$ROOT_DIR/ios/bootstrap/main.m" \
  -o "$APP_DIR/$APP_NAME"
chmod 755 "$APP_DIR/$APP_NAME"

echo "[build_tipa] Preparing Info.plist"
cp "$ROOT_DIR/ios/bootstrap/Info.plist" "$APP_DIR/Info.plist"
/usr/libexec/PlistBuddy -c "Set :CFBundleExecutable $APP_NAME" "$APP_DIR/Info.plist"
/usr/libexec/PlistBuddy -c "Set :CFBundleName $APP_NAME" "$APP_DIR/Info.plist"
/usr/libexec/PlistBuddy -c "Set :CFBundleIdentifier $BUNDLE_ID" "$APP_DIR/Info.plist"
/usr/libexec/PlistBuddy -c "Set :MinimumOSVersion $MIN_IOS" "$APP_DIR/Info.plist"

echo "[build_tipa] Preparing entitlements"
ENTITLEMENTS_PATH="$BUILD_DIR/entitlements.plist"
cp "$ROOT_DIR/ios/bootstrap/entitlements.trollstore.plist" "$ENTITLEMENTS_PATH"
/usr/libexec/PlistBuddy -c "Set :application-identifier $BUNDLE_ID" "$ENTITLEMENTS_PATH"

if command -v ldid >/dev/null 2>&1; then
  echo "[build_tipa] Signing with ldid"
  ldid -S"$ENTITLEMENTS_PATH" "$APP_DIR/$APP_NAME"
else
  echo "[build_tipa] ldid not found, using ad-hoc codesign fallback"
  codesign -s - --force "$APP_DIR/$APP_NAME"
fi

if [[ -d "$RUNTIME_LIB_DIR" ]]; then
  shopt -s nullglob
  runtime_candidates=("$RUNTIME_LIB_DIR"/*.dylib)
  shopt -u nullglob
  if [[ "${#runtime_candidates[@]}" -gt 0 ]]; then
    echo "[build_tipa] Copying runtime dylibs from $RUNTIME_LIB_DIR"
    cp -f "${runtime_candidates[@]}" "$FRAMEWORKS_DIR/"
    chmod 755 "$FRAMEWORKS_DIR"/*.dylib
    if command -v ldid >/dev/null 2>&1; then
      for dylib in "$FRAMEWORKS_DIR"/*.dylib; do
        ldid -S "$dylib"
      done
    else
      for dylib in "$FRAMEWORKS_DIR"/*.dylib; do
        codesign -s - --force "$dylib"
      done
    fi
  else
    echo "[build_tipa] Runtime lib dir exists but no dylib files found"
  fi
else
  echo "[build_tipa] Runtime lib dir not found, skipping runtime embedding"
fi

echo "[build_tipa] Packaging Payload"
cp -R "$APP_DIR" "$PAYLOAD_DIR/"

(
  cd "$OUT_DIR"
  zip -qry "$(basename "$TIPA_PATH")" Payload
)

echo "[build_tipa] Done: $TIPA_PATH"
