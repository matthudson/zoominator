#include "wheel-zoom-state.hpp"
#include "viewport-border-state.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

namespace {

void require(bool condition, const char *message)
{
	if (!condition) {
		std::cerr << "FAILED: " << message << '\n';
		std::exit(1);
	}
}

WheelZoomBinding holdCtrlAlt()
{
	WheelZoomBinding binding;
	binding.enabled = true;
	binding.mode = WheelZoomActivationMode::HoldModifiers;
	binding.ctrl.any = true;
	binding.alt.any = true;
	return binding;
}

void testEmptyBindingsFailClosed()
{
	WheelZoomBinding empty;
	empty.enabled = true;
	require(!empty.valid(), "empty modifier binding must be invalid");
	empty.mode = WheelZoomActivationMode::HoldShortcut;
	require(!empty.valid(), "empty shortcut binding must be invalid");
}

void testExactModifierMatching()
{
	const auto binding = holdCtrlAlt();
	WheelZoomModifierState modifiers;
	modifiers.leftCtrl = true;
	modifiers.rightAlt = true;
	require(WheelZoomState::modifiersExactlyMatch(binding, modifiers), "configured families should match");
	modifiers.leftShift = true;
	require(!WheelZoomState::modifiersExactlyMatch(binding, modifiers), "extra modifiers must not match");

	WheelZoomBinding sided = holdCtrlAlt();
	sided.ctrl.any = false;
	sided.ctrl.left = true;
	sided.ctrl.right = false;
	modifiers = {};
	modifiers.leftCtrl = true;
	modifiers.leftAlt = true;
	require(WheelZoomState::modifiersExactlyMatch(sided, modifiers), "left-specific modifier should match");
	modifiers.rightCtrl = true;
	require(!WheelZoomState::modifiersExactlyMatch(sided, modifiers), "unrequested right modifier must not match");
}

void testHoldModifierReleaseOrdering()
{
	WheelZoomState state;
	state.configure(holdCtrlAlt());
	WheelZoomModifierState modifiers;
	modifiers.leftCtrl = true;
	state.handleKey(1, true, false, modifiers);
	require(!state.armed(), "partial modifier chord must not arm");
	modifiers.leftAlt = true;
	state.handleKey(2, true, false, modifiers);
	require(state.armed(), "complete modifier chord should arm");
	modifiers.leftCtrl = false;
	state.handleKey(1, false, false, modifiers);
	require(!state.armed(), "releasing any required modifier must disarm");
}

void testHeldShortcutAndAutoRepeat()
{
	WheelZoomBinding binding;
	binding.enabled = true;
	binding.mode = WheelZoomActivationMode::HoldShortcut;
	binding.key = 42;
	binding.ctrl.any = true;
	WheelZoomState state;
	state.configure(binding);
	WheelZoomModifierState modifiers;
	modifiers.leftCtrl = true;
	state.handleKey(42, true, false, modifiers);
	require(state.armed(), "held shortcut should arm");
	state.handleKey(42, true, true, modifiers);
	require(state.armed(), "auto-repeat must not alter held state");
	modifiers.leftCtrl = false;
	state.handleKey(1, false, false, modifiers);
	require(!state.armed(), "modifier-first release must disarm held shortcut");
}

void testToggleActsOncePerPhysicalPress()
{
	WheelZoomBinding binding;
	binding.enabled = true;
	binding.mode = WheelZoomActivationMode::ToggleShortcut;
	binding.key = 42;
	WheelZoomState state;
	state.configure(binding);
	WheelZoomModifierState modifiers;
	state.handleKey(42, true, false, modifiers);
	require(state.armed(), "first toggle press should arm");
	state.handleKey(42, true, true, modifiers);
	require(state.armed(), "auto-repeat must not toggle");
	state.handleKey(42, false, false, modifiers);
	require(state.armed(), "toggle release should retain armed state");
	state.handleKey(42, true, false, modifiers);
	require(!state.armed(), "second physical press should disarm");
}

void testDeltaAccumulationAndReset()
{
	WheelZoomState state;
	state.configure(holdCtrlAlt());
	require(state.addWheelDelta(60, 120) == 0, "fractional positive delta should accumulate");
	require(state.addWheelDelta(60, 120) == 1, "two half deltas should produce one step");
	require(state.addWheelDelta(-240, 120) == -2, "multiple negative detents should be preserved");
	require(state.addWheelDelta(60, 120) == 0, "new fractional delta should accumulate");
	state.reset();
	require(state.addWheelDelta(60, 120) == 0, "reset must discard stale fractional input");
}

void testReconfigureClearsTransientState()
{
	WheelZoomBinding binding;
	binding.enabled = true;
	binding.mode = WheelZoomActivationMode::ToggleShortcut;
	binding.key = 42;
	WheelZoomState state;
	state.configure(binding);
	state.handleKey(42, true, false, {});
	require(state.armed(), "toggle should be armed before reconfigure");
	state.configure(binding);
	require(!state.armed(), "settings replacement must clear transient activation");
}

void testTargetCurveAndBounds()
{
	require(std::fabs(computeWheelZoomTarget(4.0, false, 1, 0.2, 0.2, 2.0, 5.0) - 1.2) < 0.000001,
		"first wheel step must start from 1x instead of the stored preset");
	require(computeWheelZoomTarget(4.9, true, 2, 0.2, 0.2, 1.0, 5.0) == 5.0,
		"multiple positive steps must clamp at the upper bound");
	require(computeWheelZoomTarget(1.1, true, -2, 0.2, 0.2, 1.0, 5.0) == 1.0,
		"negative steps must clamp at the base bound");
	double target = computeWheelZoomTarget(2.0, true, 3, 0.2, 0.1, 1.0, 5.0);
	target = computeWheelZoomTarget(target, true, -2, 0.2, 0.1, 1.0, 5.0);
	require(std::fabs(target - 2.4) < 0.000001, "rapid direction changes must preserve signed additive steps");
}

void testViewportBorderTracksInverseZoomTransform()
{
	auto viewport = computeViewportBorderState(1920.0, 1080.0, 0.0, 0.0, 1920.0, 1080.0, 2.0, 960.0, 540.0, 960.0,
						   540.0, 0.0, 0.0);
	require(viewport.valid, "centered viewport should be valid");
	require(std::fabs(viewport.left - 0.25) < 0.000001, "2x viewport should begin one quarter in");
	require(std::fabs(viewport.top - 0.25) < 0.000001, "2x viewport should begin one quarter down");
	require(std::fabs(viewport.right - 0.75) < 0.000001, "2x viewport should end three quarters in");
	require(std::fabs(viewport.bottom - 0.75) < 0.000001, "2x viewport should end three quarters down");

	viewport =
		computeViewportBorderState(1920.0, 1080.0, 0.0, 0.0, 1920.0, 1080.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	require(viewport.valid, "edge-anchored viewport should be valid");
	require(std::fabs(viewport.left) < 0.000001, "edge-anchored viewport should retain its left edge");
	require(std::fabs(viewport.right - (1.0 / 3.0)) < 0.000001,
		"3x edge-anchored viewport should cover one third of the screen");
}

} // namespace

int main()
{
	testEmptyBindingsFailClosed();
	testExactModifierMatching();
	testHoldModifierReleaseOrdering();
	testHeldShortcutAndAutoRepeat();
	testToggleActsOncePerPhysicalPress();
	testDeltaAccumulationAndReset();
	testReconfigureClearsTransientState();
	testTargetCurveAndBounds();
	testViewportBorderTracksInverseZoomTransform();
	std::cout << "wheel zoom state tests passed\n";
	return 0;
}
