#include "RpgSpinItem.h"

#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgState.h>

#include <Rpg/exception/RpgNullPointerException.h>

void RpgSpinItem::timerEvent(QTimerEvent *event){

}

void RpgSpinItem::keyReleaseEvent(QKeyEvent *event){

}

void RpgSpinItem::clearSpinItems(){
	for(QGraphicsTextItem *i: this->textItems){

	}
}

RpgSpinItem::RpgSpinItem(const RpgDialogBase *skin, QGraphicsItem *parent): RpgObject(parent){
	this->setTextColor(Qt::white);

	if(skin == nullptr){
		throw RpgNullPointerException("RpgDialogBase not set.");
	}
	this->skin = static_cast<RpgDialogBase*>(skin);

	this->arrowSymbolsTimeLine->setFrameRange(0, this->skin->getContinueSymbolImageCount());
	this->arrowSymbolsTimeLine->setLoopCount(Rpg::Infinity);
	this->connect(this->arrowSymbolsTimeLine, &QTimeLine::frameChanged, [this](int frame){
		if(frameIndex >= 0 && frameIndex < this->skin->getContinueSymbolImageCount()){
			const QPixmap upArrowPixmap = this->skin->getUpArrowSymbolImage(frame);
			this->upArrowSymbol->setPixmap(upArrowPixmap);
			const QPixmap downArrowSymbol = this->skin->getDownArrowSymbolImage(frame);
			this->downArrowSymbol->setPixmap(downArrowPixmap);
		}
	});

	// Z-Value
	this->setZValue(Rpg::ZValueDialog);
	this->box->setZValue(0);
	this->messageBox->setZValue(0.1);

	this->selectBar->setZValue(0.2);

	this->upArrowSymbol->setZValue(0); // sence SelectBar ZValue
	this->downArrowSymbol->setZValue(0); // scene SelectBar ZValue

	// 阴影设置
	QGraphicsDropShadowEffect *textShadowEffect = new QGraphicsDropShadowEffect(this->messageBox);
	textShadowEffect->setColor(this->textShadowEffectColor);
	textShadowEffect->setBlurRadius(this->textShadowEffectBlurRadius);
	textShadowEffect->setOffset(this->textShadowEffectOffect);

	// selectBar动画设置
	this->selectBarAnimation->setEasingCurve(QEasingCurve::OutQuad);
	this->selectBarAnimation->setStartValue(1);
	this->selectBarAnimation->setEndValue(1);
	this->selectBarAnimation->setKeyValueAt(0.5, 0);
	this->selectBarAnimation->setDuration(1500);
	this->selectBarAnimation->setLoopCount(Rpg::AnimationLoopInfinity);

	this->setFont(rpgFont->getFont(FontName, 16));
	rDebug() << "Dialog using font:" << this->font.family();

	// 默认不可见
	this->hide();

	rpgState->registerRpgObject(this, RpgState::DialogMode);
}

RpgSpinItem::~RpgSpinItem(){
	this->clearSpinItems();
	rpgState->unregisterRpgObject(this, RpgState::DialogMode);
}

void RpgSpinItem::run(){

}





