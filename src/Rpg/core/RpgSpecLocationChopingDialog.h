#ifndef RPGSPECLOCATIONCHOPINGDIALOG_H
#define RPGSPECLOCATIONCHOPINGDIALOG_H

#include <QtCore>
#include <QtWidgets>

#include <Rpg/Global.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgUtils.h>

#include <Rpg/exception/RpgRuntimeException.h>

class RpgSpecLocationChopingDialog : public RpgDialogBase
{
	// 对话框背景原作(128*128) 未变形的时候
	QPixmap backgroundOrigin;
	// 对话框背景成品(已变形)
	QPixmap background;
	// 对话框边9个图
	QPixmap backgroundBorder[9];
	// 选择框边的9个图
	QPixmap selectBar[9];


	// 各个图片在原图中的位置坐标
	const QRect backgroundRect					= QRect(  0,  0, 128, 128);
	const QRect backgroundBorderRect			= QRect(128,  0,  64,  64);
	const QRect selectBarRect					= QRect(128, 64,  32,  32);
	const QRect continueSymbolRect				= QRect(160, 64,  32,  32);

	const QRect backgroundBorderLeftTopRect		= QRect( 0,  0, 16, 16);
	const QRect backgroundBorderTopRect			= QRect(16,  0, 16, 16);
	const QRect backgroundBorderRightTopRect	= QRect(48,  0, 16, 16);
	const QRect backgroundBorderLeftRect		= QRect( 0, 16, 16, 16);
	const QRect backgroundBorderCenterRect		= QRect(24, 24, 16, 16);
	const QRect backgroundBorderRightRect		= QRect(48, 16, 16, 16);
	const QRect backgroundBorderLeftBottomRect	= QRect( 0, 48, 16, 16);
	const QRect backgroundBorderBottomRect		= QRect(16, 48, 16, 16);
	const QRect backgroundBorderRightBottomRect	= QRect(48, 48, 16, 16);

	const QRect selectBarLeftTopRect			= QRect( 0,  0, 16, 16);
	const QRect selectBarTopRect				= QRect( 8,  0, 16, 16);
	const QRect selectBarRightTopRect			= QRect(16,  0, 16, 16);
	const QRect selectBarLeftRect				= QRect( 0,  8, 16, 16);
	const QRect selectBarCenterRect				= QRect( 8,  8, 16, 16);
	const QRect selectBarRightRect				= QRect(16,  8, 16, 16);
	const QRect selectBarLeftBottomRect			= QRect( 0, 16, 16, 16);
	const QRect selectBarBottomRect				= QRect( 8, 16, 16, 16);
	const QRect selectBarRightBottomRect		= QRect(16, 16, 16, 16);

	const QRect continueSymbol1Rect				= QRect( 0,  0, 16, 16);
	const QRect continueSymbol2Rect				= QRect(16,  0, 16, 16);
	const QRect continueSymbol3Rect				= QRect( 0, 16, 16, 16);
	const QRect continueSymbol4Rect				= QRect(16, 16, 16, 16);

protected:
	enum SplitBlock{
		LeftTop = 0,		// 左上角 ↖
		Top = 1,			// 上边  ↑
		RightTop = 2,		// 右上角 ↗
		Left = 3,			// 左边  ←
		Center = 4,			// 中间  ·
		Right = 5,			// 右边  →
		LeftBottom = 6,		// 左下角 ↙
		Bottom = 7,			// 下边  ↓
		RightBottom = 8		// 右下角 ↘
	};

	const QPixmap renderBackground(const QSize &dialogSize);

	void renderDialog(const QSize &dialogSize) NoThrow Override Final;
	void renderSelectBar(const QSize &selectBarSize) NoThrow Override Final;
	void renderContinueSymbols() NoThrow Override Final{}
	void renderUpArrowSymbols() NoThrow Override Final{}
	void renderDownArrowSymbols() NoThrow Override Final{}

public:
	RpgSpecLocationChopingDialog(const QString &dialogSkinName);

	const QPixmap &getDownArrowSymbolImage(int index) const;
};

#endif // RPGSPECLOCATIONCHOPINGDIALOG_H
