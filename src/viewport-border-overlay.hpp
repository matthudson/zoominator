#pragma once

#include <QColor>
#include <QRect>
#include <QWidget>

class ViewportBorderOverlay final : public QWidget {
public:
	explicit ViewportBorderOverlay();

	bool showViewport(const QRect &viewport, int thickness, const QColor &color,
			  bool allowWithoutCaptureExclusion = false);
	void hideViewport();

private:
	bool ensureCaptureExcluded(bool force = false);
	bool captureExclusionReady = false;
	quintptr captureExclusionWindowId = 0;
};
