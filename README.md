# Zoominator

[![Build](https://github.com/matthudson/zoominator/actions/workflows/push.yaml/badge.svg?branch=main)](https://github.com/matthudson/zoominator/actions/workflows/push.yaml)
[![License: GPL-2.0](https://img.shields.io/badge/License-GPL--2.0-blue.svg)](LICENSE)

Zoominator is an OBS Studio plugin that zooms and pans an entire scene around
the mouse pointer. Because the transform is applied at scene level, it works
with display capture, window capture, cameras, overlays, and other source types
without adding a filter to every source.

[Watch the original demonstration](https://www.youtube.com/watch?v=R3KQnsnzIAM).

## Features

- Scene-wide animated zoom and pan with clamping to prevent black edges.
- Center, static-cursor, or continuously-following zoom anchors.
- Ultrawide edge tracking and configurable idle freeze.
- Keyboard and mouse-button activation with toggle or hold behaviour.
- Optional click halo.
- Windows independent wheel zoom with configurable activation, sensitivity,
  minimum, and maximum magnification.
- Windows reset binding, including exact modifier and mouse-button chords such
  as `Ctrl+Alt+Middle Mouse`.
- Windows presenter viewport guide with a configurable activation zoom,
  capture-safety status, explicit show-anyway override, and a rounding-safe gap
  that keeps the guide outside the recorded viewport.
- Legacy Mouse X2 plus wheel adjustment on Windows and X11.

## Platform support

| Capability | Windows | Linux/X11 | macOS |
| --- | --- | --- | --- |
| Scene zoom and mouse following | Yes | Yes, via XInput2 | Yes, with Accessibility permission |
| Legacy Mouse X2 wheel zoom | Yes | Yes | No |
| Independent wheel activation modes | Yes | Deferred | Deferred |
| Reset keyboard/mouse binding | Yes | Deferred | Deferred |
| Presenter viewport guide | Yes | Deferred | Deferred |

Wayland sessions are detected and X11 hooks are disabled. Wayland does not yet
provide the passive global pointer facilities Zoominator needs for full mouse
tracking.

## Installation

### Windows

1. Close OBS completely.
2. Back up any existing `zoominator.dll`, plugin data directory, and settings.
3. Extract the Windows release archive into the OBS installation directory,
   preserving both top-level directories:

   ```text
   C:\Program Files\obs-studio\
   ├── obs-plugins\64bit\zoominator.dll
   └── data\obs-plugins\zoominator\locale\...
   ```

4. Start OBS and open **Tools → Zoominator**.

User settings are stored separately at:

```text
%APPDATA%\obs-studio\plugin_config\zoominator\zoominator.json
```

Replacing the binary and data files does not require deleting that settings
file. See the [Windows presenter controls guide](docs/windows-presenter-controls.md)
for setup, verification, and rollback instructions.

### macOS

Install the release `.pkg` or `.dmg`, restart OBS, and grant Accessibility
permission when prompted for global input tracking.

### Linux/X11

Install the packaged plugin when available, or build from source and copy the
result into the OBS plugin directory. A typical per-user location is:

```text
~/.config/obs-studio/plugins/
```

## Quick setup

1. Open **Tools → Zoominator**.
2. Select the target monitor and the scene sources that should participate.
3. Configure the primary zoom trigger and zoom factor.
4. On Windows, optionally configure independent wheel zoom and a reset binding
   in the **Trigger** tab.
5. Configure wheel sensitivity, limits, mouse following, and the presenter
   viewport guide in the **Advanced** tab.
6. Test the complete workflow in the OBS preview before recording or streaming.

The independent wheel hook consumes vertical wheel input only while its binding
is armed and the pointer is over the selected target screen. Empty or invalid
bindings, unsupported platforms, and unavailable targets pass input through.
Activation keys themselves are not swallowed, so choose combinations that do
not conflict with Windows or the foreground application.

## Presenter viewport guide

The Windows guide outlines the portion of the selected display currently
visible in OBS once zoom exceeds its configured threshold. It is click-through,
keeps a small transparent gap outside the viewport, and requests
`WDA_EXCLUDEFROMCAPTURE` from Windows.

Windows Graphics Capture is the preferred OBS Display Capture method. Capture
exclusion remains dependent on Windows, OBS, and the graphics driver, so the
settings page reports the active compatibility state and asks users to verify
the result in their preview. Automatic/DXGI sources hide the guide by default;
**Show anyway when capture exclusion cannot be guaranteed** enables it for
workflows where the external positioning and safety gap are sufficient.

## Building and testing

Requirements:

- CMake 3.28 or newer.
- A C++17 compiler.
- Qt 6 and OBS Studio development dependencies.
- Windows SDK for Windows builds; X11, XRandR, and XInput2 development packages
  for Linux builds.

Use the checked-in CMake presets for the host platform:

```bash
cmake --preset windows-x64
cmake --build --preset windows-x64
ctest --test-dir build_x64 -C RelWithDebInfo --output-on-failure
```

Equivalent presets are `ubuntu-x86_64` and `macos`. CI builds all three
platforms with warnings treated as errors and packages the resulting artifacts.

## Troubleshooting

- Confirm the plugin loaded in **Help → Log Files → View Current Log** and look
  for `[zoominator] [Zoominator] loaded`.
- If wheel zoom passes through, check its status in the Trigger tab, confirm a
  non-empty binding, and ensure the pointer is over the selected target screen.
- If the viewport guide is hidden, read the compatibility message in Advanced
  settings. Switch the source capture method or deliberately enable the
  show-anyway override.
- If an upgrade behaves unexpectedly, close OBS and restore the DLL and plugin
  data from the same backup set. Restore the JSON settings only when rolling
  configuration back intentionally.

## Contributing

Keep platform-specific behaviour guarded, preserve input pass-through when a
binding cannot be honoured, and add deterministic coverage for state-machine or
geometry changes. Run the relevant local build and tests before opening a pull
request; the GitHub Actions matrix is the final Windows, Ubuntu, and macOS build
gate.

See [CHANGELOG.md](CHANGELOG.md) for the current development changes.

## License

Zoominator is distributed under the [GNU General Public License v2.0](LICENSE).
