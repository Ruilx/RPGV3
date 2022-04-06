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
	this->selectBar->setZValue(0.4);

	//this->setMessageTextWidth(this->dialogSize.width() - (2.0 * MessageMarginH));

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

	qreal textWidth = this->dialogSize.width() - (2 * RpgDialogBase::PaddingH);
	qreal innerPaddingV = 0;

	int innerDialogHeight = this->dialogSize.height() - (RpgDialogBase::PaddingV * 2);
	int selectBarHeight = this->skin->selectBarSize().height();
	qreal fRatio = qreal(innerDialogHeight) / selectBarHeight;
	int iRatio = innerDialogHeight / selectBarHeight; // 选择项目数
	if(fRatio < 1){
		rWarning() << "dialog size cannot fit even 1 choice.";
		this->end();
		return;
	}
	if(iRatio == 1){
		innerPaddingV = 0;
	}else if(iRatio > 1){
		if(fRatio - iRatio <= 1){
			innerPaddingV = 0;
		}else{
			innerPaddingV = (this->dialogSize.height() - (selectBarHeight * iRatio)) / (iRatio - 1);
		}
	}

	// 判断是否有choices
	if(this->choices.isEmpty()){
		rDebug() << "Detected choices is empty, exitting";
		this->end();
		return;
	}

	// 判断是否被重新设定textItems了
	if(!this->textItems.isEmpty()){
		rError() << "Run while textItems wasn't emptied, will damage when append new choices.";
		rError() << "It usually caused by 'hide' method not cleared the textItems list items.";
		this->end();
		return;
	}

	int displayChoices = qMin(this->choices.length(), iRatio);
	int curHeight = RpgDialogBase::PaddingV;
	for(int i = 0; i < displayChoices; i++){
		QGraphicsTextItem *item = new QGraphicsTextItem(this->box);
		item->setFont(this->font);
		item->setPos(RpgDialogBase::MarginH, curHeight);
		item->setZValue(0.2);
		item->setTextWidth(textWidth);
		item->setGraphicsEffect(this->textShadowEffect);
		this->textItems.append(item);

		curHeight += selectBarHeight + innerPaddingV;
	}

	// 计算对话框位置
	QPointF dialogPos = RpgUtils::getDialogPos(this->dialogAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

	this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
	this->box->setPos(dialogPos);

	this->upArrowSymbol->setVisible(false);
	this->downArrowSymbol->setVisible(false);

	this->showDialog();

}

void RpgChoiceItem::end(){
	emit this->exitDialogMode();
	// 回收选项中新增的TextItem的内存
	for(QList<QGraphicsTextItem*>::ConstIterator i: this->textItems){
		if((*i) != nullptr){
			(*i)->deleteLater();
		}
	}
	this->textItems.clear();
	RpgObject::end();
}

void RpgChoiceItem::showDialog(){
	this->show();
	rpgState->pushState(RpgState::DialogMode);
	this->dialogAnimation->runDialogAnimations();

}


