# iOS Port Status (Main Menu Milestone)

This file tracks practical progress toward the first playable target: reaching the game main menu.

## Completed in this workspace

- Added a cross-platform compatibility header for native window and `android_dlext` usage:
  - `app/src/main/cpp/platform_compat.h`
- Made shared headers stop directly depending on Android-only native window headers:
  - `app/src/main/cpp/zomdroid.h`
  - `app/src/main/cpp/zomdroid_globals.h`
- Added non-Android logging fallback (stderr) so native code can run outside Android:
  - `app/src/main/cpp/logger.h`
- Added non-Android loader shims in core startup path:
  - `app/src/main/cpp/zomdroid.c`
  - fallback to `dlopen/dlsym` wrappers for loader hooks
  - env override support:
    - `ZOMDROID_LIBRARY_DIR`
    - `ZOMDROID_LINKER_LIB`
    - `ZOMDROID_JVM_LIB`
    - `ZOMDROID_VULKAN_LOADER_NAME`
- Added GitHub iOS packaging workflows:
  - `.github/workflows/build-ios-tipa.yml` (bootstrap `.tipa`)
  - `.github/workflows/build-ios-runtime-tipa.yml` (runtime probe + `.tipa`)
- Added iOS build scripts:
  - `ios/scripts/build_tipa.sh`
  - `ios/scripts/build_runtime_probe.sh`
- Added iOS bootstrap runtime probe UI:
  - `ios/bootstrap/main.m` now checks bundled runtime dylibs and reports load status.
- Added non-Android-safe branch for `android_load_sphal_library`:
  - `app/src/main/cpp/linker.c`
- Kept Android JNI surface path intact while making non-Android stub explicit:
  - `app/src/main/cpp/zomdroid_jni.c`

## Still required for main menu on iOS

- Build `box64` and `zomdroidlinker` as Mach-O targets for iOS.
- Replace remaining Android linker-namespace assumptions with iOS/dyld equivalents.
- Integrate native startup with an iOS launcher shell (Amethyst-style process and JRE handling).
- Validate JNI bridge flow for first native game libs:
  - `PZClipper64`, `PZBullet64`, `PZBulletNoOpenGL64`, `Lighting64`, `PZPathFind64`, `PZPopMan64`, `fmodintegration64`
- Wire rendering and input surface path from iOS view into GLFW integration.

## Immediate next technical checkpoint

Load `libzomdroidlinker` + `libjvm` on iOS runtime and reach Java entrypoint (`main`) without crashing.
