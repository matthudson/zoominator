#include "wheel-zoom-state.hpp"

#include <algorithm>

namespace {

bool familyMatches(const WheelZoomModifierRequirement &required, bool left, bool right)
{
	if (!required.any && !required.left && !required.right)
		return !left && !right;
	if (required.left && !left)
		return false;
	if (required.right && !right)
		return false;
	if (!required.any) {
		if (!required.left && left)
			return false;
		if (!required.right && right)
			return false;
	}
	return required.any ? (left || right) : true;
}

bool hasAnyModifier(const WheelZoomBinding &binding)
{
	auto present = [](const WheelZoomModifierRequirement &requirement) {
		return requirement.any || requirement.left || requirement.right;
	};
	return present(binding.ctrl) || present(binding.alt) || present(binding.shift) || present(binding.meta);
}

} // namespace

double computeWheelZoomTarget(double configuredTarget, bool zoomActive, int steps, double zoomInStep,
			      double zoomOutStep, double minimum, double maximum)
{
	const double current = zoomActive ? std::max(1.0, configuredTarget) : 1.0;
	double next = current + (steps > 0 ? zoomInStep * (double)steps : zoomOutStep * (double)steps);
	const double lowerBound = current < minimum ? 1.0 : minimum;
	return std::clamp(next, lowerBound, maximum);
}

bool WheelZoomBinding::valid() const
{
	if (!enabled)
		return false;
	if (mode == WheelZoomActivationMode::HoldModifiers)
		return hasAnyModifier(*this);
	return key != 0;
}

void WheelZoomState::configure(const WheelZoomBinding &binding)
{
	binding_ = binding;
	reset();
}

void WheelZoomState::reset()
{
	shortcutKeyDown_ = false;
	armed_ = false;
	wheelDeltaRemainder_ = 0;
}

bool WheelZoomState::handleKey(uint32_t key, bool down, bool repeat, const WheelZoomModifierState &modifiers)
{
	const bool wasArmed = armed_;
	if (!binding_.valid()) {
		reset();
		return wasArmed;
	}

	if (binding_.mode == WheelZoomActivationMode::HoldModifiers) {
		armed_ = modifiersExactlyMatch(binding_, modifiers);
	} else if (binding_.mode == WheelZoomActivationMode::HoldShortcut) {
		if (key == binding_.key) {
			if (down && !repeat)
				shortcutKeyDown_ = true;
			else if (!down)
				shortcutKeyDown_ = false;
		}
		armed_ = shortcutKeyDown_ && modifiersExactlyMatch(binding_, modifiers);
	} else if (key == binding_.key) {
		if (down && !repeat && !shortcutKeyDown_ && modifiersExactlyMatch(binding_, modifiers))
			armed_ = !armed_;
		shortcutKeyDown_ = down;
	}

	if (wasArmed && !armed_)
		wheelDeltaRemainder_ = 0;
	return wasArmed != armed_;
}

bool WheelZoomState::activeForWheel(const WheelZoomModifierState &modifiers) const
{
	if (!binding_.valid())
		return false;
	if (binding_.mode == WheelZoomActivationMode::ToggleShortcut)
		return armed_;
	return armed_ && modifiersExactlyMatch(binding_, modifiers);
}

int WheelZoomState::addWheelDelta(int delta, int deltaPerStep)
{
	if (deltaPerStep <= 0)
		return 0;
	wheelDeltaRemainder_ += delta;
	const int steps = wheelDeltaRemainder_ / deltaPerStep;
	wheelDeltaRemainder_ %= deltaPerStep;
	return steps;
}

bool WheelZoomState::modifiersExactlyMatch(const WheelZoomBinding &binding, const WheelZoomModifierState &modifiers)
{
	return familyMatches(binding.ctrl, modifiers.leftCtrl, modifiers.rightCtrl) &&
	       familyMatches(binding.alt, modifiers.leftAlt, modifiers.rightAlt) &&
	       familyMatches(binding.shift, modifiers.leftShift, modifiers.rightShift) &&
	       familyMatches(binding.meta, modifiers.leftMeta, modifiers.rightMeta);
}
