#include "RpgChoiceItem.h"

#include <Rpg/com/RpgView.h>
#include <Rpg/core/RpgState.h>

void RpgChoiceItem::appendChoice(const RpgChoiceMessage &choice){
	this->choices.append(choice);
}

void RpgChoiceItem::appendChoice(const QList<RpgChoiceMessage> &choices){
	this->choices.append(choices);
}

void RpgChoiceItem::appendChoice(const QString &text, bool enabled){
	this->choices.append(RpgChoiceMessage(text, enabled));
}

void RpgChoiceItem::setDialogSize(const QSize &size){
	int width = size.width();
	int height = size.height();
	if(width < RpgDialogBase::MinDialogWidth || width > RpgDialogBase::maxDialogSize().width()){
		rDebug() << "Given width:" << width << "is out of range: (" << RpgDialogBase::MinDialogWidth << "," << RpgDialogBase::maxDialogSize().width() << ").";
		return;
	}
	if(height < RpgDialogBase::MinDialogHeight || height > RpgDialogBase::maxDialogSize().height()){
		rDebug() << "Given height:" << height << "is out of range: (" << RpgDialogBase::MinDialogHeight << "," << RpgDialogBase::maxDialogSize().height() << ").";
		return;
	}
	this->dialogSize.setWidth(width);
	this->dialogSize.setHeight(height);
}

RpgChoiceItem::RpgChoiceItem(RpgDialogBase *dialogBase, QGraphicsObject *parent){
	this->setTextColor(Qt::white);

	if(dialogBase == nullptr){
		throw RpgNullPointerException("RpgDialogBase not set.");
	}
	this->skin = static_cast<RpgDialogBase*>(dialogBase);

	this->arrowSymbolsTimeLine->setFrameRange(0, this->skin->getContinueSymbolImageCount());
	this->arrowSymbolsTimeLine->setLoopCount(Rpg::Infinity);
	this->connect(this->arrowSymbolsTimeLine, &QTimeLine::frameChanged, [this](int frameIndex){
		if(frameIndex >= 0 && frameIndex < this->skin->getContinueSymbolImageCount()){
			const QPixmap upArrowPixmap = this->skin->getUpArrowSymbolImage(frameIndex);
			this->upArrowSymbol->setPos((this->dialogSize.width() - upArrowPixmap.width()) / 2.0, upArrowPixmap.height() / 2.0);
			this->upArrowSymbol->setPixmap(upArrowPixmap);
			const QPixmap downArrowPixmap = this->skin->getDownArrowSymbolImage(frameIndex);
			this->downArrowSymbol->setPos((this->dialogSize.width() - downArrowPixmap.width()) / 2.0, this->dialogSize.height() - (downArrowPixmap.height() / 2.0));
			this->downArrowSymbol->setPixmap(downArrowPixmap);
		}
	});

	// Z-Value
	this->setZValue(Rpg::ZValueDialog);
	this->box->setZValue(0);

	this->upArrowSymbol->setZValue(0.3);
	this->downArrowSymbol->setZValue(0.3);

	this->setMessageTextWidth(this->dialogSize.width() - (2.0 * MessageMarginH));

	// 阴影设置
	this->textShadowEffect.setColor(QColor(Qt::black));
	this->textShadowEffect.setBlurRadius(5.0);
	this->textShadowEffect.setOffset(2.0, 2.0);

	// 默认不可见
	this->hide();

	rpgState->registerRpgObject(this, RpgState::DialogMode);
}

RpgChoiceItem::~RpgChoiceItem(){
	rpgState->unregisterRpgObject(this, RpgState::DialogMode);
}

void RpgChoiceItem::run(){
	RpgObject::run();
	// 重新设定其Scene
	if(rpgView->scene() == nullptr){
		rDebug() << "RpgView not loaded any scene yet.";
		this->end();
		throw RpgNullPointerException("RpgView::instance()->scene()");
	}else{
		rpgView->scene()->addItem(this);
	}

	int innerDialogHeight = this->dialogSize.height() - (RpgDialogBase::PaddingV * 2);
	int selectBarHeight = this->skin->selectBarSize().height();
	qreal fRatio = qreal(innerDialogHeight) / selectBarHeight;
	int iRatio = innerDialogHeight / selectBarHeight;
	if(fRatio < 1){
		rWarning() << "dialog size cannot fit even 1 choice.";
		this->end();
		return;
	}
	if(fRatio - iRatio <= 1e-5){

	}


	if(this->choices.isEmpty()){
		rDebug() << "Detected choices is empty, exitted";
		this->end();
		return;
	}else if(this->choices.length())

	}

}


