#include "viewport-border-overlay.hpp"

#include <QPalette>
#include <QOperatingSystemVersion>
#include <QRegion>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifndef WDA_EXCLUDEFROMCAPTURE
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif
#endif

ViewportBorderOverlay::ViewportBorderOverlay()
	: QWidget(nullptr, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus)
{
	setAttribute(Qt::WA_ShowWithoutActivating);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setFocusPolicy(Qt::NoFocus);
	setAutoFillBackground(true);
}

bool ViewportBorderOverlay::ensureCaptureExcluded()
{
#ifdef _WIN32
	if (captureExclusionReady)
		return true;
	if (captureExclusionFailed)
		return false;
	const QOperatingSystemVersion version = QOperatingSystemVersion::current();
	if (version.majorVersion() < 10 || (version.majorVersion() == 10 && version.microVersion() < 19041)) {
		captureExclusionFailed = true;
		return false;
	}

	HWND hwnd = reinterpret_cast<HWND>(winId());
	if (!hwnd) {
		captureExclusionFailed = true;
		return false;
	}

	LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
	SetWindowLongPtrW(hwnd, GWL_EXSTYLE, style | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW);
	captureExclusionReady = SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE) != FALSE;
	captureExclusionFailed = !captureExclusionReady;
	return captureExclusionReady;
#else
	captureExclusionFailed = true;
	return false;
#endif
}

bool ViewportBorderOverlay::showViewport(const QRect &viewport, int thickness, const QColor &color)
{
	if (!viewport.isValid() || thickness <= 0 || !ensureCaptureExcluded()) {
		hideViewport();
		return false;
	}

	const QRect outer = viewport.adjusted(-thickness, -thickness, thickness, thickness);
	const QRect inner(QPoint(thickness, thickness), viewport.size());
	QRegion ring(QRect(QPoint(0, 0), outer.size()));
	ring = ring.subtracted(QRegion(inner));

	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, color);
	setPalette(palette);
	setGeometry(outer);
	setMask(ring);
	show();
	raise();

#ifdef _WIN32
	SetWindowPos(reinterpret_cast<HWND>(winId()), HWND_TOPMOST, 0, 0, 0, 0,
		     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
#endif
	return true;
}

void ViewportBorderOverlay::hideViewport()
{
	if (isVisible())
		hide();
}
