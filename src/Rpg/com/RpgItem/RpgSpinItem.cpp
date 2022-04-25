#include "RpgSpinItem.h"

#include <QFontMetrics>
#include <QTextDocument>

#include <Rpg/com/RpgSound.h>
#include <Rpg/com/RpgView.h>

#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgHtmlSplit.h>

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
	int maxLength = 0;
	QFontMetrics fm(this->font);
	if(value.getChoicesCount() <= 0){
		rDebug() << "Cannot get spin width, spin has no values.";
		return 0;
	}
	for(int i = 0; i < value.getChoicesCount(); i++){
		const RpgSpinValueItem item = value.at(i);
		int currentWidth = fm.boundingRect(item.getText()).width();
		if(currentWidth > maxLength){
			maxLength = currentWidth;
		}
	}
	return maxLength;
}

void RpgSpinItem::adjustSpinItems(int from){
	int currentX = 0;
	const int paddingH = RpgDialogBase::PaddingH;
	// Pair.first == pos.x, Pair.second == width
	QList<QPair<int, int>> positions;

	while(currentX < maxWidth && positions.length() + from < this->spinValues.length()){
		RpgSpinValue currentValue = this->spinValues.at(from + positions.length());
		int maxWidth = this->calSpinValueMaxWidth(currentValue);
		if(maxWidth <= 0){
			rError() << "There has a spin value has no items.";
			this->end();
			return;
		}
		positions.append(QPair<int, int>(currentX, maxWidth));
		currentX += maxWidth + paddingH;
	}

	for(const QPair<int, int> &i: positions){
		QGraphicsTextItem *item = new QGraphicsTextItem(this->box);
		item->setFont(this->font);
		item->setPos(i.first, this->spinItemHeight);
		rDebug() << "Item pos:" << item->pos();
		item->setZValue(0.2);
		item->setTextWidth(i.second);
		item->setDefaultTextColor(this->textColor);
		item->document()->setUndoRedoEnabled(false);
		QTextOption spinTextOption = item->document()->defaultTextOption();{
			spinTextOption.setWrapMode(QTextOption::NoWrap);
			item->document()->setDefaultTextOption(spinTextOption);
		}
		QGraphicsDropShadowEffect *itemEffect = new QGraphicsDropShadowEffect(this);
		itemEffect->setColor(this->textShadowEffectColor);
		itemEffect->setBlurRadius(this->textShadowEffectBlurRadius);
		itemEffect->setOffset(this->textShadowEffectOffect);
		// item会得到effect的控制权, 会在item销毁时帮助销毁effect, 也可以将setGraphicsEffect参数设置为nullptr删除当前设定的effect
		item->setGraphicsEffect(itemEffect);
		this->spinItems.append(item);
	}

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
	// Pair.first == pos.x, Pair.second == width
	QList<QPair<int, int>> positions;
	while(currentX < maxWidth && count < this->spinValues.length()){
		RpgSpinValue currentValue = this->spinValues.at(count);
		int maxWidth = this->calSpinValueMaxWidth(currentValue);
		if(maxWidth <= 0){
			rError() << "There has a spin value has no items.";
			this->end();
			return;
		}
		positions.append(QPair<int, int>(currentX, maxWidth));
		currentX += maxWidth + paddingH;
	}

	this->spinItemHeight = (this->dialogSize.height() - this->selectBarHeight) / 2;
	if(!this->message.isEmpty()){
		QFontMetrics fm(this->font);
		int messageHeight = fm.boundingRect(this->message).height();
		if(this->dialogSize.height() - messageHeight < this->selectBarHeight){
			// 如果message把上面的空间都占满了, spin没地方显示就直接盖在上面
			this->spinItemHeight = this->dialogSize.height() - RpgDialogBase::PaddingV - this->selectBarHeight;
		}else{
			// 剩余空间的纵向中间
			this->spinItemHeight = ((this->dialogSize.height() - messageHeight) - this->selectBarHeight) / 2;
		}
	}

	for(const QPair<int, int> &i: positions){
		QGraphicsTextItem *item = new QGraphicsTextItem(this->box);
		item->setFont(this->font);
		item->setPos(i.first, spinItemHeight);
		rDebug() << "Item pos:" << item->pos();
		item->setZValue(0.2);
		item->setTextWidth(i.second);
		item->setDefaultTextColor(this->textColor);
		item->document()->setUndoRedoEnabled(false);
		QTextOption spinTextOption = item->document()->defaultTextOption();{
			spinTextOption.setWrapMode(QTextOption::NoWrap);
			item->document()->setDefaultTextOption(spinTextOption);
		}
		QGraphicsDropShadowEffect *itemEffect = new QGraphicsDropShadowEffect(this);
		itemEffect->setColor(this->textShadowEffectColor);
		itemEffect->setBlurRadius(this->textShadowEffectBlurRadius);
		itemEffect->setOffset(this->textShadowEffectOffect);
		// item会得到effect的控制权, 会在item销毁时帮助销毁effect, 也可以将setGraphicsEffect参数设置为nullptr删除当前设定的effect
		item->setGraphicsEffect(itemEffect);
		this->spinItems.append(item);
	}

	// 计算对话框位置
	QPointF dialogPos = RpgUtils::getDialogPos(this->dialogAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

	this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
	this->box->setPos(dialogPos);

	// selectBar
	this->selectBar->hide();

	this->upArrowSymbol->hide();
	this->downArrowSymbol->hide();

	this->showDialog();
}

void RpgSpinItem::showDialog(){
	this->show();
	rpgState->pushState(RpgState::DialogMode);
	this->dialogAnimation->runDialogAnimations(RpgDialogAnimation::Animations(RpgDialogAnimation::AnimationDialogShow));
	this->dialogAnimation->waitAnimationFinish();
	if(this->arrowSymbolsTimeLine->state() != QTimeLine::Running){
		this->arrowSymbolsTimeLine->start();
	}
	this->adjustSpinItems(0);
	this->showMessage();
	this->setSpinsText(0);
	// todo...
}

void RpgSpinItem::showMessage(){
	if(this->message.isEmpty()){
		return;
	}
	this->showTextInProgressFlag = true;
	this->messageBox->document()->clear();
	// 设定textalign
	QTextOption textOption = this->messageBox->document()->defaultTextOption();{
		if(textOption.alignment() != (Qt::Alignment)this->messageAlign){
			textOption.setAlignment((Qt::Alignment)this->messageAlign);
			this->messageBox->document()->setDefaultTextOption(textOption);
		}
	}

	if(this->speed > 0){
		this->quickShowFlag = false;
		RpgHtmlSplit htmlSplit(this->message);
		while(htmlSplit.hasNext()){
			QString wordLeft = htmlSplit.chopLeft();
			this->messageBox->setHtml(wordLeft);
			//this->setLineHeight();
			RpgUtils::msleep(this->speed);
			if(this->quickShowFlag == true){
				this->messageBox->setHtml(text);
				//this->setLineHeight();
			}
			this->quickShowFlag = false;
			break;
		}
	}else{
		this->messageBox->setHtml(this->message);
		//this->setLineHeight();
	}

}

void RpgSpinItem::setSpinsText(int from){
	if(from < 0 || from > this->spinValues.length()){
		rError() >> "Index is out of range: '" << from << "' not in range [0," << this->spinValues.length() << ").";
		return;
	}
	QList<RpgSpinValue>::ConstIterator spinIter = this->spinValues.constBegin();
	for(int i = 0; i < from; i++){
		spinIter++;
	}

	for(int i = 0; i < qMin(this->spinItems.length(), this->spinValues.length() - from); i++){
		this->spinItems.at(i)->setHtml(this->spinValues.at(from + i).at(0));
		if(this->spinValues.at(from + i).at(0).getEnable()){
			this->spinItems.at(i)->setDefaultTextColor(this->textColor);
		}else{
			this->spinItems.at(i)->setDefaultTextColor(this->bannedColor);
		}
	}
	for(int i = this->spinItems.length() - from; i < this->spinValues.length(); i++){
		this->spinItems.at(i)->document()->clear();
	}
}

void RpgSpinItem::setSelectBarIndex(int index){
	if(index < 0 || index >= qMin(this->spinItems.length(), this->spinValues.length())){
		rDebug() << "Index is out of range: '" << index << "' not in range [0," << qMin(this->spinItems.length(), this->spinValues.length()) << ")";
		rDebug() << "this->spinItems.length() ==" << this->spinItems.length() << ", this->spinValues.length() ==" << this->spinValues.length() << ", index ==" << index;
		return;
	}
	QGraphicsTextItem *textItem = this->spinItems.at(index);
	this->selectBar->setPos(textItem->pos());
	this->selectBar->setPixmap(this->skin->getSelectBarImage(textItem->boundingRect().size().toSize()));
}





