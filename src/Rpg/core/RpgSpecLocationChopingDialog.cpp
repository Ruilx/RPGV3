#include "RpgSpecLocationChopingDialog.h"


const QPixmap RpgSpecLocationChopingDialog::renderBackground(const QSize &dialogSize){
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

RpgSpecLocationChopingDialog::RpgSpecLocationChopingDialog(const QString &dialogSkinName): RpgDialogBase(dialogSkinName){
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

void RpgSpecLocationChopingDialog::renderDialog(const QSize &dialogSize) NoThrow{
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

void RpgSpecLocationChopingDialog::renderSelectBar(const QSize &selectBarSize) NoThrow{
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

