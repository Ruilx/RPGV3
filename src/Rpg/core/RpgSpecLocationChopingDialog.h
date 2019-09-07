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

	const QPixmap renderBackground(const QSize &dialogSize){
		QPixmap background = QPixmap(dialogSize);
		background.fill(Qt::transparent);
		QPixmap backgroundCopy = this->backgroundOrigin.scaled(dialogSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		QPainter p(&background);{
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			QRectF area(2, 2, background.width() - 4, background.height() - 4);
			p.drawPixmap(area, backgroundCopy, area);
			p.end();
		}
		return background;
	}

	void renderDialog(const QSize &dialogSize) NoThrow Override Final{
		QPixmap background = this->renderBackground(dialogSize);
		QPainter p(&background);{
			int width = dialogSize.width();
			int height = dialogSize.height();
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			for(int i = 16; i < width - 16; i += 16){
				p.drawPixmap(QRect(i, 0, 16, 16), this->backgroundBorder[Top]);
				p.drawPixmap(QRect(i, height - 16, 16, 16), this->backgroundBorder[Bottom]);
			}
			for(int i = 16; i < height - 16; i += 16){
				p.drawPixmap(QRect(0, i, 16, 16), this->backgroundBorder[Left]);
				p.drawPixmap(QRect(width - 16, i, 16, 16), this->backgroundBorder[Right]);
			}
			p.drawPixmap(QRect(0, 0, 16, 16), this->backgroundBorder[LeftTop]);
			p.drawPixmap(QRect(width - 16, 0, 16, 16), this->backgroundBorder[RightTop]);
			p.drawPixmap(QRect(0, height - 16, 16, 16), this->backgroundBorder[LeftBottom]);
			p.drawPixmap(QRect(width - 16, height - 16, 16, 16), this->backgroundBorder[RightBottom]);
			p.end();
		}
		this->dialogBackground.insert(RpgSpecLocationChopingDialog::generateQSizePair(dialogSize), background);
	}

	void renderSelectBar(const QSize &selectBarSize) NoThrow Override Final{
		QPixmap selectBarImage = QPixmap(RpgDialogBase::selectBarSize());
		selectBarImage.fill(Qt::transparent);
		QPainter p(&selectBarImage);{
			int width = selectBarSize.width();
			int height = selectBarSize.height();
			p.setCompositionMode(QPainter::CompositionMode_Source);
			// 背景绘图
			for(qreal i = 0; i < width - 16; i += 16){
				for(qreal j = 0; j < height - 16; j += 16){
					p.drawPixmap(QRect(i, j, 16, 16), this->selectBar[Center]);
				}
			}
			// 边框绘图
			for(int i = 16; i < width - 16; i += 16){
				p.drawPixmap(QRect(i, 0, 16, 16), this->selectBar[Top]);
				p.drawPixmap(QRect(i, height - 16, 16, 16), this->selectBar[Bottom]);
			}
			for(int i = 16; i < height - 16; i += 16){
				p.drawPixmap(QRect(0, i, 16, 16), this->selectBar[Left]);
				p.drawPixmap(QRect(width - 16, i, 16, 16), this->selectBar[Right]);
			}
			int minWidth = qMin(width, 16);
			int minHeight = qMin(height, 16);
			p.drawPixmap(QRect(0, 0, 16, 16), this->selectBar[LeftTop]);
			p.drawPixmap(QRect(width - 16 + (16 - minWidth), 0, 16 - (16 - minWidth), 16), this->selectBar[RightTop]);
			p.drawPixmap(QRect(0, height - 16 + (16 - minHeight), 16, 16 - (16 - minHeight)), this->selectBar[LeftBottom]);
			p.drawPixmap(QRect(width - 16 + (16 - minWidth), height - 16 + (16 - minHeight), 16 - (16 - minWidth), 16 - (16 - minHeight)), this->selectBar[RightBottom]);
			p.end();
		}
		this->selectBarBackground.insert(RpgSpecLocationChopingDialog::generateQSizePair(selectBarSize), selectBarImage);
	}

	void renderContinueSymbols() NoThrow Override Final{}
public:
	RpgSpecLocationChopingDialog(const QString &dialogSkinName): RpgDialogBase(dialogSkinName){
		this->chopingMode = RpgMakerMode;
		if(this->base.isNull()){
			throw RpgRuntimeException("Base image is not valid.");
		}
		if(this->base.width() < 192 || this->base.height() < 128){
			qDebug() << CodePath << "Loaded skin pixmap not fit.";
			throw RpgRuntimeException("Loaded skin pixmap not fit.");
		}

		QPixmap background = this->base.copy(this->backgroundRect);
		QPixmap backgroundBorder = this->base.copy(this->backgroundBorderRect);
		QPixmap selectBar = this->base.copy(this->selectBarRect);
		QPixmap continueSymbol = this->base.copy(this->continueSymbolRect);

		this->backgroundOrigin				= background;
		this->backgroundBorder[LeftTop]		= backgroundBorder.copy(this->backgroundBorderLeftTopRect);
		this->backgroundBorder[Top]			= backgroundBorder.copy(this->backgroundBorderTopRect);
		this->backgroundBorder[RightTop]	= backgroundBorder.copy(this->backgroundBorderRightTopRect);
		this->backgroundBorder[Left]		= backgroundBorder.copy(this->backgroundBorderLeftRect);
		this->backgroundBorder[Center]		= backgroundBorder.copy(this->backgroundBorderCenterRect);
		this->backgroundBorder[Right]		= backgroundBorder.copy(this->backgroundBorderRightRect);
		this->backgroundBorder[LeftBottom]	= backgroundBorder.copy(this->backgroundBorderLeftBottomRect);
		this->backgroundBorder[Bottom]		= backgroundBorder.copy(this->backgroundBorderBottomRect);
		this->backgroundBorder[RightBottom]	= backgroundBorder.copy(this->backgroundBorderRightBottomRect);
		this->selectBar[LeftTop]			= selectBar.copy(this->selectBarLeftTopRect);
		this->selectBar[Top]				= selectBar.copy(this->selectBarTopRect);
		this->selectBar[RightTop]			= selectBar.copy(this->selectBarRightTopRect);
		this->selectBar[Left]				= selectBar.copy(this->selectBarLeftRect);
		this->selectBar[Center]				= selectBar.copy(this->selectBarCenterRect);
		this->selectBar[Right]				= selectBar.copy(this->selectBarRightRect);
		this->selectBar[LeftBottom]			= selectBar.copy(this->selectBarLeftBottomRect);
		this->selectBar[Bottom]				= selectBar.copy(this->selectBarBottomRect);
		this->selectBar[RightBottom]		= selectBar.copy(this->selectBarRightBottomRect);

		this->continueSymbolList.append(continueSymbol.copy(this->continueSymbol1Rect));
		this->continueSymbolList.append(continueSymbol.copy(this->continueSymbol2Rect));
		this->continueSymbolList.append(continueSymbol.copy(this->continueSymbol3Rect));
		this->continueSymbolList.append(continueSymbol.copy(this->continueSymbol4Rect));
	}


};

#endif // RPGSPECLOCATIONCHOPINGDIALOG_H
