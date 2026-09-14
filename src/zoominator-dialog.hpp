#pragma once

#include <QDialog>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QFrame;
class QKeySequenceEdit;
class QLabel;
class QListWidget;
class QPushButton;
class QSpinBox;
class QTabWidget;
struct calldata;

class ZoominatorDialog final : public QDialog {
	Q_OBJECT

public:
	explicit ZoominatorDialog(QWidget *parent = nullptr);

protected:
	void closeEvent(QCloseEvent *event) override;

private slots:
	void refreshLists();
	void applyToController();
	void testZoom();
	void clearHotkey();
	void clearFollowToggleHotkey();
	void clearWheelZoomShortcut();
	void clearResetZoomHotkey();
	void chooseMarkerColor();
	void populateSourcesTab();

private:
	void buildUi();
	void loadFromController();
	void populateSources();
	void updateMarkerColorButton(const QColor &color);
	void updateWheelZoomUi();
	void updateWheelZoomStatus();
	void updateViewportGuideUi();

	static void obsSourceChanged(void *data, struct calldata *cd);

	QTabWidget *tabWidget = nullptr;

	QComboBox *cmbSource = nullptr;
	QComboBox *cmbMode = nullptr;
	QKeySequenceEdit *editFollowToggleHotkey = nullptr;
	QPushButton *btnClearFollowToggleHotkey = nullptr;

	QComboBox *cmbTrigger = nullptr;
	QKeySequenceEdit *editHotkey = nullptr;
	QPushButton *btnClearHotkey = nullptr;
	QComboBox *cmbMouseBtn = nullptr;
	QCheckBox *chkCtrl = nullptr;
	QCheckBox *chkAlt = nullptr;
	QCheckBox *chkShift = nullptr;
	QCheckBox *chkWin = nullptr;
	QCheckBox *chkLeftCtrl = nullptr;
	QCheckBox *chkRightCtrl = nullptr;
	QCheckBox *chkLeftAlt = nullptr;
	QCheckBox *chkRightAlt = nullptr;
	QCheckBox *chkLeftShift = nullptr;
	QCheckBox *chkRightShift = nullptr;
	QCheckBox *chkLeftWin = nullptr;
	QCheckBox *chkRightWin = nullptr;
	QCheckBox *chkIndependentWheelZoom = nullptr;
	QComboBox *cmbWheelZoomActivation = nullptr;
	QKeySequenceEdit *editWheelZoomShortcut = nullptr;
	QPushButton *btnClearWheelZoomShortcut = nullptr;
	QCheckBox *chkWheelCtrl = nullptr;
	QCheckBox *chkWheelAlt = nullptr;
	QCheckBox *chkWheelShift = nullptr;
	QCheckBox *chkWheelMeta = nullptr;
	QCheckBox *chkWheelLeftCtrl = nullptr;
	QCheckBox *chkWheelRightCtrl = nullptr;
	QCheckBox *chkWheelLeftAlt = nullptr;
	QCheckBox *chkWheelRightAlt = nullptr;
	QCheckBox *chkWheelLeftShift = nullptr;
	QCheckBox *chkWheelRightShift = nullptr;
	QCheckBox *chkWheelLeftMeta = nullptr;
	QCheckBox *chkWheelRightMeta = nullptr;
	QLabel *lblWheelZoomStatus = nullptr;
	QComboBox *cmbResetZoomTrigger = nullptr;
	QKeySequenceEdit *editResetZoomHotkey = nullptr;
	QPushButton *btnClearResetZoomHotkey = nullptr;
	QComboBox *cmbResetZoomMouseBtn = nullptr;
	QCheckBox *chkResetZoomCtrl = nullptr;
	QCheckBox *chkResetZoomAlt = nullptr;
	QCheckBox *chkResetZoomShift = nullptr;
	QCheckBox *chkResetZoomMeta = nullptr;

	QDoubleSpinBox *spZoom = nullptr;
	QDoubleSpinBox *spWheelZoomInStep = nullptr;
	QDoubleSpinBox *spWheelZoomOutStep = nullptr;
	QDoubleSpinBox *spWheelZoomMinimum = nullptr;
	QDoubleSpinBox *spWheelZoomMaximum = nullptr;
	QSpinBox *spIn = nullptr;
	QSpinBox *spOut = nullptr;
	QComboBox *cmbZoomAnchor = nullptr;
	QDoubleSpinBox *spFollowSpeed = nullptr;
	QCheckBox *chkCenterCursorUntilEdge = nullptr;
	QSpinBox *spEdgeOverflowMargin = nullptr;
	QSpinBox *spMouseIdleTimeout = nullptr;
	QCheckBox *chkPortraitCover = nullptr;
	QCheckBox *chkShowCursorMarker = nullptr;
	QCheckBox *chkShowMarkerWhenNotZoomed = nullptr;
	QCheckBox *chkShowViewportBorder = nullptr;
	QCheckBox *chkAllowUnsafeViewportBorder = nullptr;
	QLabel *lblViewportGuideStatus = nullptr;
	QDoubleSpinBox *spViewportBorderZoomThreshold = nullptr;
	QSpinBox *spMarkerSize = nullptr;
	QSpinBox *spMarkerThickness = nullptr;
	QPushButton *btnMarkerColor = nullptr;
	QCheckBox *chkDebug = nullptr;

	QListWidget *lstSources = nullptr;

	QLabel *lblStatus = nullptr;
	QPushButton *btnRefresh = nullptr;
	QPushButton *btnApply = nullptr;
	QPushButton *btnTest = nullptr;

	QWidget *rowHotkeyWidget = nullptr;
	QWidget *rowMouseWidget = nullptr;
	QWidget *rowModifiersWidget = nullptr;
	QWidget *rowWheelModifiersWidget = nullptr;
	QWidget *rowWheelShortcutWidget = nullptr;
	QWidget *rowResetZoomHotkeyWidget = nullptr;
	QWidget *rowResetZoomMouseWidget = nullptr;

	bool loading = false;
};
