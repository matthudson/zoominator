#pragma once

#include <cstdint>

enum class WheelZoomActivationMode {
	HoldModifiers,
	HoldShortcut,
	ToggleShortcut,
};

struct WheelZoomModifierState {
	bool leftCtrl = false;
	bool rightCtrl = false;
	bool leftAlt = false;
	bool rightAlt = false;
	bool leftShift = false;
	bool rightShift = false;
	bool leftMeta = false;
	bool rightMeta = false;
};

struct WheelZoomModifierRequirement {
	bool any = false;
	bool left = false;
	bool right = false;
};

struct WheelZoomBinding {
	bool enabled = false;
	WheelZoomActivationMode mode = WheelZoomActivationMode::HoldModifiers;
	uint32_t key = 0;
	WheelZoomModifierRequirement ctrl;
	WheelZoomModifierRequirement alt;
	WheelZoomModifierRequirement shift;
	WheelZoomModifierRequirement meta;

	bool valid() const;
};

double computeWheelZoomTarget(double configuredTarget, bool zoomActive, int steps, double zoomInStep,
			      double zoomOutStep, double minimum, double maximum);

class WheelZoomState final {
public:
	void configure(const WheelZoomBinding &binding);
	void reset();

	bool handleKey(uint32_t key, bool down, bool repeat, const WheelZoomModifierState &modifiers);
	bool activeForWheel(const WheelZoomModifierState &modifiers) const;
	bool armed() const { return armed_; }

	int addWheelDelta(int delta, int deltaPerStep);
	void resetWheelDelta() { wheelDeltaRemainder_ = 0; }

	static bool modifiersExactlyMatch(const WheelZoomBinding &binding, const WheelZoomModifierState &modifiers);

private:
	WheelZoomBinding binding_;
	bool shortcutKeyDown_ = false;
	bool armed_ = false;
	int wheelDeltaRemainder_ = 0;
};
