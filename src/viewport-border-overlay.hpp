#pragma once

#include <QColor>
#include <QRect>
#include <QWidget>

class ViewportBorderOverlay final : public QWidget {
public:
	explicit ViewportBorderOverlay();

	bool showViewport(const QRect &viewport, int thickness, const QColor &color);
	void hideViewport();

private:
	bool ensureCaptureExcluded();
	bool captureExclusionReady = false;
	bool captureExclusionFailed = false;
};
