#pragma once

struct ViewportBorderState {
	bool valid = false;
	double left = 0.0;
	double top = 0.0;
	double right = 0.0;
	double bottom = 0.0;
};

ViewportBorderState computeViewportBorderState(double canvasWidth, double canvasHeight, double contentMinX,
					       double contentMinY, double contentMaxX, double contentMaxY, double zoom,
					       double focusX, double focusY, double anchorX, double anchorY,
					       double offsetX, double offsetY);
