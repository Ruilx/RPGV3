#include "RpgSpinItem.h"

#include <Rpg/com/RpgSound.h>
#include <Rpg/com/RpgView.h>

#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgState.h>

#include <Rpg/exception/RpgNullPointerException.h>

void RpgSpinItem::timerEvent(QTimerEvent *event){

}

void RpgSpinItem::keyReleaseEvent(QKeyEvent *event){

}

void RpgSpinItem::clearSpinItems(){
	for(QGraphicsTextItem *i: this->spinItems){
		if(i != nullptr){
			i->deleteLater();
		}
	}
	this->spinItems.clear();
}

void RpgSpinItem::playSound(SoundEffect soundEffect, qreal volume, int times){
	const QString name = this->soundEffects.value(soundEffect);
	if(name.isEmpty()){
		// 如果不设置声音, 就不播放声音
		//rError() << "Try to play a 'Empty name' sound.";
		return;
	}
	rpgSound->play(name, volume, times);
}

int RpgSpinItem::calSpinValueMaxWidth(const RpgSpinValue &value){

}

void RpgSpinItem::setFont(const QString &name, int pointSize, int weight, bool italic){
	this->font = rpgFont->getFont(name, pointSize, weight, italic);
}

void RpgSpinItem::setDialogSize(const QSize &size){
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
			const QRect selectBarBoundingRect = this->selectBar->boundingRect();
			const QPixmap upArrowPixmap = this->skin->getUpArrowSymbolImage(frame);
			this->upArrowSymbol->setPixmap(upArrowPixmap);
			this->upArrowSymbol->setPos((selectBarBoundingRect.width() - upArrowPixmap.width()) / 2.0, -10);
			const QPixmap downArrowSymbol = this->skin->getDownArrowSymbolImage(frame);
			this->downArrowSymbol->setPixmap(downArrowPixmap);
			this->downArrowSymbol->setPos((selectBarBoundingRect.width() - downArrowSymbol.width()) / 2.0, selectBarBoundingRect.height() + 10);
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
	RpgObject::run();

	// 重新设定其Scene
	if(rpgView->scene() == nullptr){
		rError() << "RpgView not loaded scene yet.";
		this->end();
		throw RpgNullPointerException("RpgView::instance()->scene()");
	}else{
		rpgView->scene()->addItem(this);
	}

	qreal maxWidth = this->dialogSize.width() - (2 * RpgDialogBase::PaddingH);

	this->selectBarHeight = 35;

	if(!this->spinItems.isEmpty()){
		rError() << "Run while spinItems wasn't emptied, will damage when append new choices.";
		rError() << "It usually caused by 'hide' method not cleared the textItems list items.";
		this->end();
		return;
	}

	int currentX = 0;
	int paddingH = RpgDialogBase::PaddingH;
	int count = 0;
	while(currentX < maxWidth && count < this->spinValues.length()){
		RpgSpinValue currentValue = this->spinValues.at(count);

	}

}





