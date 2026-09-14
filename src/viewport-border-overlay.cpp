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

bool ViewportBorderOverlay::ensureCaptureExcluded(bool force)
{
#ifdef _WIN32
	const QOperatingSystemVersion version = QOperatingSystemVersion::current();
	if (version.majorVersion() < 10 || (version.majorVersion() == 10 && version.microVersion() < 19041))
		return false;

	HWND hwnd = reinterpret_cast<HWND>(winId());
	if (!hwnd)
		return false;
	const quintptr windowId = reinterpret_cast<quintptr>(hwnd);
	if (!force && captureExclusionReady && captureExclusionWindowId == windowId)
		return true;

	LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
	SetWindowLongPtrW(hwnd, GWL_EXSTYLE, style | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW);
	DWORD affinity = WDA_NONE;
	captureExclusionReady = SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE) != FALSE &&
				GetWindowDisplayAffinity(hwnd, &affinity) != FALSE &&
				affinity == WDA_EXCLUDEFROMCAPTURE;
	captureExclusionWindowId = windowId;
	return captureExclusionReady;
#else
	(void)force;
	return false;
#endif
}

bool ViewportBorderOverlay::showViewport(const QRect &viewport, int thickness, const QColor &color,
					 bool allowWithoutCaptureExclusion)
{
	if (!viewport.isValid() || thickness <= 0 || (!ensureCaptureExcluded() && !allowWithoutCaptureExclusion)) {
		hideViewport();
		return false;
	}

	/* Keep the guide just outside the computed viewport. The small transparent
	 * gap absorbs integer rounding and Windows display-scaling differences. */
	constexpr int safetyPadding = 2;
	const int outside = thickness + safetyPadding;
	const QRect outer = viewport.adjusted(-outside, -outside, outside, outside);
	const QRect inner(QPoint(thickness, thickness), viewport.size() + QSize(safetyPadding * 2, safetyPadding * 2));
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
	/* Showing a Qt top-level widget may recreate its native window. Reapply and
	 * read back the affinity on the final HWND before leaving it visible. */
	if (!ensureCaptureExcluded(true) && !allowWithoutCaptureExclusion) {
		hideViewport();
		return false;
	}
#endif
	return true;
}

void ViewportBorderOverlay::hideViewport()
{
	if (isVisible())
		hide();
}
