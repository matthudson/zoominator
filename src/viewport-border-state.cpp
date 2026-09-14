#include "viewport-border-state.hpp"

#include <cmath>

bool viewportCaptureMethodExcludesWindows(int configuredMethod)
{
	/* OBS monitor capture values: 0 = Automatic, 1 = DXGI Desktop
	 * Duplication, 2 = Windows Graphics Capture. Only an explicit WGC choice
	 * gives us a reliable capture-exclusion contract. Automatic normally
	 * resolves to DXGI and must therefore fail closed. */
	return configuredMethod == 2;
}

ViewportBorderState computeViewportBorderState(double canvasWidth, double canvasHeight, double contentMinX,
					       double contentMinY, double contentMaxX, double contentMaxY, double zoom,
					       double focusX, double focusY, double anchorX, double anchorY,
					       double offsetX, double offsetY)
{
	ViewportBorderState result;
	const double contentWidth = contentMaxX - contentMinX;
	const double contentHeight = contentMaxY - contentMinY;
	if (canvasWidth <= 0.0 || canvasHeight <= 0.0 || contentWidth <= 0.0 || contentHeight <= 0.0 || zoom <= 0.0)
		return result;

	/* Zoominator maps a scene point as:
	 *   output = anchor + (scene - focus) * zoom + offset
	 * Invert that transform at the four canvas edges, then normalize it to
	 * the captured screen's original content bounds. */
	const double sceneLeft = (0.0 - anchorX - offsetX) / zoom + focusX;
	const double sceneTop = (0.0 - anchorY - offsetY) / zoom + focusY;
	const double sceneRight = (canvasWidth - anchorX - offsetX) / zoom + focusX;
	const double sceneBottom = (canvasHeight - anchorY - offsetY) / zoom + focusY;

	result.left = (sceneLeft - contentMinX) / contentWidth;
	result.top = (sceneTop - contentMinY) / contentHeight;
	result.right = (sceneRight - contentMinX) / contentWidth;
	result.bottom = (sceneBottom - contentMinY) / contentHeight;
	result.valid = std::isfinite(result.left) && std::isfinite(result.top) && std::isfinite(result.right) &&
		       std::isfinite(result.bottom) && result.right > result.left && result.bottom > result.top;
	return result;
}
