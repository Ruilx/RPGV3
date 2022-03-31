#include "RpgDialogItem.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlockFormat>

#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgFont.h>

void RpgDialogItem::setLineHeight(qreal pixel, int lineHeightType){
	QTextCursor textCursor = this->messageBox->textCursor();
	QTextBlockFormat textBlockFormat = textCursor.blockFormat();
	textBlockFormat.setLineHeight(pixel, lineHeightType);
	textCursor.setBlockFormat(textBlockFormat);
	this->messageBox->setTextCursor(textCursor);
}

RpgDialogItem::RpgDialogItem(RpgDialogBase *dialogBase, QGraphicsObject *parent): RpgObject(parent){
	this->setTextColor(Qt::white);
	this->messageBox->setFont(rpgFont->getFont(FontName));
	rDebug() << "Dialog using font:" << this->messageBox->font().family();

	if(dialogBase == nullptr){
		throw RpgNullPointerException("RpgDialogBase not set");
	}
	this->skin = static_cast<RpgDialogBase*>(dialogBase);

	this->messageBox->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
	rDebug() << "Dialog CSS:" << this->messageBox->document()->defaultStyleSheet();
	this->messageBox->document()->setUndoRedoEnabled(false);
	QTextOption messageTextOption = this->messageBox->document()->defaultTextOption();{
		messageTextOption.setWrapMode(QTextOption::WrapAnywhere);
		this->messageBox->document()->setDefaultTextOption(messageTextOption);
	}

	this->continueSymbolTimeLine->setFrameRange(0, this->skin->getContinueSymbolImageCount());
	this->continueSymbolTimeLine->setLoopCount(Rpg::Infinity);

	this->connect(this->continueSymbolTimeLine, &QTimeLine::frameChanged, [this](int frameIndex){
		if(frameIndex >= 0 && frameIndex < this->skin->getContinueSymbolImageCount()){
			const QPixmap framePixmap = this->skin->getContinueSymbolImage(frameIndex);
			this->continueSymbol->setPos((this->dialogSize.width() - framePixmap.width()) / 2,
										 this->dialogSize.height() - framePixmap.height());
			this->continueSymbol->setPixmap(framePixmap);
		}
	});
	// Z-Value
	this->setZValue(Rpg::ZValueDialog);
	this->box->setZValue(0);
	this->messageBox->setZValue(0.2);
	this->continueSymbol->setZValue(0.3);
	this->avatarBoxRight->setZValue(-0.1);
	this->avatarBoxLeft->setZValue(-0.1);

	// 默认位置和大小
	this->setMessageTextWidth(this->dialogSize.width() - (2 * MessageMarginH));
	this->messageBox->setPos(MessageMarginH, MessageMarginV);

	// 设置阴影
	this->messageShadowEffect->setColor(QColor(Qt::black));
	this->messageShadowEffect->setBlurRadius(5.0);
	this->messageShadowEffect->setOffset(2.0, 2.0);
	this->messageBox->setGraphicsEffect(this->messageShadowEffect);

	// 立绘初始位置
	this->avatarBoxLeft->setPos(0, 0);
	this->avatarBoxLeft->hide();
	this->avatarBoxRight->setPos(0, 0);
	this->avatarBoxRight->hide();

	// 默认不可见
	this->hide();

	rpgState->registerRpgObject(this, RpgState::DialogMode);
}

RpgDialogItem::~RpgDialogItem(){
	rpgState->unregisterRpgObject(this, RpgState::DialogMode);
}
