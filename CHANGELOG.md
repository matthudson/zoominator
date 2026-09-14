# Changelog

Notable changes to this fork are recorded here. The project currently reports
plugin version 2.0.5; entries under **Unreleased** are development changes and
do not imply a tagged upstream release.

## Unreleased

### Added

- Independent Windows wheel zoom with modifier-hold, shortcut-hold, and
  shortcut-toggle activation modes.
- Configurable wheel sensitivity and magnification limits.
- A Windows reset-to-default zoom binding supporting keyboard shortcuts or an
  exact modifier plus mouse-button chord.
- A Windows presenter viewport guide with configurable activation zoom,
  capture compatibility messaging, and an explicit show-anyway override.
- Deterministic tests for wheel binding state, modifier matching, accumulated
  wheel deltas, zoom bounds, viewport geometry, and capture-method gating.

### Changed

- Wheel adjustment retains the selected magnification and mouse-follow state
  when its activation is released.
- Presenter guide updates are coalesced onto the UI thread from render-time
  viewport changes to reduce visible tracking lag.
- The guide now leaves a 2 px transparent safety gap outside the calculated
  viewport to absorb rounding and display-scaling differences.
- Windows capture exclusion is reapplied and read back after the final native
  overlay window is shown.

### Fixed

- Added native Windows wheel handling while preserving the existing X11
  behaviour.
- Deferred target refresh after OBS scene-change events to avoid enumerating a
  scene while the frontend is still transitioning it.
- Invalid, empty, unsupported, or unavailable wheel bindings now fail open so
  the foreground application continues receiving wheel input.
- The viewport guide no longer disappears without an explanation when its
  capture method is incompatible; the settings page reports the reason and the
  available override.

### Platform status

- Windows: implemented, built in CI, and runtime-verified on a multi-monitor OBS
  workflow with both WGC and Automatic/DXGI capture modes.
- Linux/X11: existing zoom and legacy Mouse X2 wheel behaviour retained; the new
  independent binding and reset modes are deferred. CI build verified.
- macOS: existing zoom behaviour retained; the new independent binding, reset,
  and presenter guide are deferred. CI build verified.
