# Windows presenter controls

This guide covers the Windows-specific independent wheel zoom, reset binding,
and presenter viewport guide.

## Before configuring

- Use a supported Windows 10 or Windows 11 release.
- Add the displays or windows you intend to present to the OBS scene.
- Open **Tools → Zoominator**, select the target screen, and include the scene
  sources that should be transformed.
- Keep an OBS preview visible during initial verification.

## Independent wheel zoom

Enable **Independent Wheel Zoom** in the Trigger tab and select one activation
mode:

- **Hold modifiers:** wheel adjustment is armed while the exact configured
  modifier combination is held. This is the simplest option.
- **Hold keyboard shortcut:** hold a chord ending in an ordinary key.
- **Toggle keyboard shortcut:** press a chord once to arm and again to disarm.

Wheel input is consumed only while the binding is armed and the pointer is over
the selected target display. Disarming retains the current magnification. The
Advanced tab controls zoom-in and zoom-out step sizes and minimum/maximum zoom.

If the settings status reports an invalid binding, unavailable backend, or
unavailable target, Zoominator deliberately passes the wheel event through.

## Reset to default zoom

The reset action returns the live magnification to the configured **Zoom
Factor** and keeps zoom active. It can use either a keyboard shortcut or a mouse
button with exact modifiers.

Example configuration:

```text
Zoom Factor: 2.0x
Reset trigger: Mouse Button
Button: Middle Mouse
Modifiers: Ctrl + Alt
```

With that configuration, `Ctrl+Alt+Middle Mouse` returns the active view to 2x.
The matching mouse click is consumed so it is not also delivered to the
foreground application.

## Presenter viewport guide

Enable **Show the visible viewport boundary above the activation zoom** in the
Advanced tab and choose the threshold. The guide appears only when the rendered
zoom is greater than that value.

The guide is a click-through topmost Windows overlay. Its 4 px border sits
outside the recorded viewport with a 2 px transparent safety gap. Zoominator
also requests `WDA_EXCLUDEFROMCAPTURE` and verifies that Windows accepted the
flag after the native overlay window is shown.

### Capture method behaviour

- **Windows Graphics Capture:** preferred. Zoominator requests capture
  exclusion, but the result still depends on Windows, OBS, and the graphics
  driver. Verify it in the preview.
- **Automatic/DXGI:** Zoominator hides the guide by default because composed
  overlay windows may be captured. Enable **Show anyway when capture exclusion
  cannot be guaranteed** only after preview verification.

The safety gap is useful with either capture method because small rounding or
display-scaling differences cannot pull the border into the intended viewport.

## Verification checklist

1. Start OBS and confirm Zoominator opens from the Tools menu.
2. Arm wheel zoom and verify that the foreground application does not scroll.
3. Disarm it and verify the chosen magnification is retained.
4. Invoke the reset binding and verify it returns to the configured Zoom Factor.
5. Pan at a zoom above the guide threshold and check that the border follows the
   viewport without entering it.
6. Inspect the OBS preview or make a short disposable recording to confirm the
   guide is absent from program output.
7. Change scenes and move OBS between monitors to exercise display-scale and
   scene-transition handling.

## Safe upgrade and rollback

Close OBS before replacing plugin files. Back up these items together:

```text
C:\Program Files\obs-studio\obs-plugins\64bit\zoominator.dll
C:\Program Files\obs-studio\data\obs-plugins\zoominator\
%APPDATA%\obs-studio\plugin_config\zoominator\zoominator.json
```

Install a new build by copying both the archive's `obs-plugins` and `data`
directories into the OBS installation directory. Do not delete the JSON file
when upgrading.

To roll back, close OBS and restore the previous DLL and matching plugin data.
Restore the saved JSON only if the configuration itself must also be reverted.
