# Zoominator – Smart Scene Zoom & Follow for OBS Studio

[![Watch the demo](https://img.youtube.com/vi/R3KQnsnzIAM/0.jpg)](https://www.youtube.com/watch?v=R3KQnsnzIAM)

Zoominator is an OBS Studio plugin that dynamically **zooms and pans the entire scene** to follow your mouse, keeping the focus exactly where the action happens.

It operates at the **scene level**, meaning it works with any source automatically no per-source setup, no dock, no complexity.

---

## What It Does

- **Scene-wide Zoom & Pan**  
  Smoothly transforms the entire scene based on mouse position.

- **Smart Clamping**  
  Ensures the canvas is always fully covered no black edges, even with cropped sources.

- **Mouse-Driven Focus**  
  Choose where the zoom is anchored: the canvas center, the cursor position at
  trigger time (static), or the cursor position with continuous following.

- **Ultrawide Edge Tracking**
  Optionally keeps the cursor centered until the captured scene reaches an edge.

- **Idle Freeze**
  Pauses mouse following after a configurable idle timeout and resumes on movement.

- **Click Highlight (Halo)**  
  Optional click-only visual feedback using a configurable halo.

- **Flexible Activation**  
  Toggle or hold behavior with customizable key combinations.

- **Independent Wheel Zoom (Windows)**
  Enable this in the Trigger tab and choose one explicit activation mode:
  hold modifiers, hold a keyboard shortcut, or toggle a keyboard shortcut.
  While armed over the selected target screen, vertical wheel input changes
  zoom and is consumed so the foreground application does not also scroll.
  Releasing or disarming keeps the chosen magnification and mouse-follow state.
  Empty bindings and unavailable targets always pass wheel input through.
  Activation keys are not swallowed, so choose shortcuts that do not conflict
  with Windows or the foreground application.

- **Reset to Default Zoom (Windows)**
  Assign either a keyboard chord or a mouse button with exact modifiers to
  return the live magnification to the configured Zoom Factor while keeping
  zoom active. Independent wheel adjustments no longer overwrite that saved
  default. For example, Mouse Middle with Ctrl and Alt resets to a 2x Zoom
  Factor. Matching mouse chords are consumed to avoid a foreground-app click.

- **Legacy Mouse X2 Wheel Zoom (Windows and X11)**
  With Mouse X2 in toggle mode and no modifiers, hold the button and scroll to
  change zoom. Release the button to keep the selected level, or click it
  without scrolling to reset. The plugin consumes wheel events during the
  gesture so the application below the pointer does not scroll.

- **Presenter Viewport Guide (Windows)**
  Optionally outlines the part of the selected screen currently visible in
  OBS once zoom exceeds a configurable threshold. The click-through outline
  sits outside the viewport and uses Windows capture exclusion so it does not
  appear in the recording. The included Display Capture source must explicitly
  use Windows Graphics Capture; Automatic and DXGI Desktop Duplication capture
  composed overlay windows, so Zoominator fails closed and hides the guide.

---

## Installation

### Windows
1. Download the latest release
2. Extract the archive and move the zoominator.dll file into your OBS Studio directory:
   ```
   C:\Program Files\obs-studio\obs-plugins\64bit
   ```
3. Restart OBS

### macOS
1. Download the `.pkg` or `.dmg` from releases
2. Install and restart OBS

### Linux (X11)
1. Build from source or install via package (if available)
2. Copy plugin files into:
   ```
   ~/.config/obs-studio/plugins/
   ```
3. Restart OBS

---

## Build from Source

### Requirements
- OBS Studio development libraries
- CMake (3.20+ recommended)
- C++17 compatible compiler
- Qt6

### Steps
```bash
git clone https://github.com/mmlTools/zoominator.git
cd zoominator
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

---

## Compatibility Notes

- **Windows:** Full support (global input, smooth tracking, independent wheel zoom, reset-to-default keyboard/mouse chords, presenter viewport guide, and legacy Mouse X2 + wheel zoom)
- **macOS:** Requires Accessibility permissions for input tracking
- **Linux (X11):** Supported via XInput2
- **Independent wheel zoom:** Supported on Windows. The current X11 passive
  Mouse5 grab cannot safely suppress arbitrary modifier/keyboard gestures, and
  the current macOS event tap is listen-only, so the new activation modes are
  disabled there rather than allowing both OBS and the foreground application
  to react. Legacy Mouse X2 wheel zoom remains supported on Windows and X11.
  Zoom-in/out sensitivity, limits, and animation durations are configured in
  the Advanced tab.
- **Presenter viewport guide:** Supported on Windows 10 version 2004 and later,
  where `WDA_EXCLUDEFROMCAPTURE` can keep the guide out of display capture.
  Set each included Display Capture source's Capture Method to `Windows 10
  (1903 and up)` (Windows Graphics Capture). The guide stays hidden with
  Automatic/DXGI and on other platforms.
- **Reset to default zoom:** Keyboard and mouse-button bindings are implemented
  on Windows. Linux/X11 and macOS keep this new binding disabled for now; their
  existing activation and legacy wheel behavior are unchanged.
- **Wayland:** Native sessions are detected and X11 hooks are disabled. The
  Global Shortcuts portal can support hotkeys, but Wayland currently has no
  standard passive global cursor-position portal, so full mouse tracking still
  requires compositor-specific input capture support.

---

## Use Cases

- Tutorials & live coding  
- Product demos  
- Gameplay & analysis  
- Vertical / short-form content  

---

## Notes

Zoominator behaves like a **virtual camera system inside OBS**, applying transformations at the scene level for maximum flexibility and reliability.

## Clang format fix

```bash
git add --chmod=+x build-aux/.run-format.zsh build-aux/run-clang-format build-aux/run-gersemi build-aux/run-swift-format build-aux/.functions/*
git add --renormalize .
git commit -m "Make build scripts executable and normalize line endings"
```

```gitattributes
.gitattributes
*.sh text eol=lf
*.zsh text eol=lf
.github/scripts/** text eol=lf
.functions/* text eol=lf
run-clang-format text eol=lf
run-gersemi text eol=lf
run-swift-format text eol=lf
.run-format.zsh text eol=lf
```

```bash
git add --renormalize .
git commit -m "Normalize line endings for Unix scripts"
git push
```
