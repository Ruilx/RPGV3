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
	this->timerProcessing = true;
	if(event->timerId() == this->timerId){
		rDebug() << "Killing timer:" << this->timerId;
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
	this->hideDialog();
	this->timerProcessing = false;
}

void RpgSpinItem::keyReleaseEvent(QKeyEvent *event){
	if(event->isAutoRepeat()){
		return;
	}
	if(this->timerProcessing){
		rDebug() << "TimerEvent is processing spins. this key ignored.";
		return;
	}
	if(!this->isRunning()){
		rWarning() << "RpgSpinItem not running.";
		return;
	}
	if(event->modifiers() != Qt::NoModifier){
		return;
	}
	Qt::Key key = (Qt::Key)event->key();
	const QList<Qt::Key> okKeys = QList<Qt::Key>({Qt::Key_Return, Qt::Key_Space});
	const QList<Qt::Key> upKeys = QList<Qt::Key>({Qt::Key_W, Qt::Key_Up});
	const QList<Qt::Key> downKeys = QList<Qt::Key>({Qt::Key_S, Qt::Key_Down});
	const QList<Qt::Key> leftKeys = QList<Qt::Key>({Qt::Key_A, Qt::Key_Left});
	const QList<Qt::Key> rightKeys = QList<Qt::Key>({Qt::Key_D, Qt::Key_Right});
	if(okKeys.contains(key) || upKeys.contains(key) || downKeys.contains(key) ||
			leftKeys.contains(key) || rightKeys.contains(key)){
		if(this->showTextInProgressFlag == true){
			this->quickShowFlag = true;
			return;
		}
	}
	int currentIndex = this->selectingIndex + this->fromIndex;
	if(currentIndex < 0 || currentIndex >= this->spinValues.length()){
		rError() << "CurrentIndex:" << currentIndex << "=" << this->selectingIndex << "(selectingIndex)+" << this->fromIndex << "(fromIndex) is out of spinValues length:" << this->spinValues.length() << ".";
		return;
	}
	if(this->selectingIndex < 0 || this->selectingIndex >= this->spinItems.length()){
		rError() << "SelectingIndex:" << this->selectingIndex << "is out of range [0," << this->spinItems.length() << ").";
		return;
	}
	if(okKeys.contains(key)){
		for(const RpgSpinValue *value: this->spinValues){
			if(!value->getCurrentSpinValueItem().getEnable()){
				// 有项目不可执行
				rWarning() << "RpgSpinItem:" << value->getCurrentSpinValueItem().getValue() << "is not enabled.";
				this->playSound(SoundEffect_Banned);
				return;
			}
		}
		if(this->timerId > 0){
			rDebug() << "Killing timer:" << this->timerId;
			this->killTimer(this->timerId);
			this->timerId = -1;
		}
		this->playSound(SoundEffect_Accept);
		this->hideDialog();
		return;
	}else if(upKeys.contains(key)){
		// 选中上一个选项
		RpgSpinValue *value = this->spinValues.at(currentIndex);
		const RpgSpinValueItem valueItem = value->prev();
		QGraphicsTextItem *item = this->spinItems.at(this->selectingIndex);
		item->setHtml(valueItem.getText());
		item->setDefaultTextColor(valueItem.getEnable() ? this->textColor : this->bannedColor);
	}else if(downKeys.contains(key)){
		// 选中下一个选项
		RpgSpinValue *value = this->spinValues.at(currentIndex);
		const RpgSpinValueItem valueItem = value->next();
		QGraphicsTextItem *item = this->spinItems.at(this->selectingIndex);
		item->setHtml(valueItem.getText());
		item->setDefaultTextColor(valueItem.getEnable() ? this->textColor : this->bannedColor);
	}else if(leftKeys.contains(key)){
		// 选择上一个value
		if(this->spinValues.length() == 1){
			// 如果列表中只有一个spinvalue, 啥也不做
			return;
		}else{
			// 大于一个选项
			if(this->fromIndex == 0){
				// 正在显示 0-N 项
				if(this->selectingIndex == 0){
					// 光标已经在第一个了, 不能再往左了
					rDebug() << "First choose first, cannot up.";
					return;
				}else{
					// 光标不在第一个 (可以往上了)
					this->selectingIndex--;
					this->setSelectBarIndex(this->selectingIndex);
					this->playSound(SoundEffect_Select);
				}
			}else{
				// 正在显示1项以上 (不在第一个)
				if(this->selectingIndex == 0){
					// 光标在第一个 (光标不能往左了, 但From还可以往左)
					// 向上滚动1行显示的内容, 光标需要重绘调整
					this->fromIndex--;
					this->adjustSpinItems(this->fromIndex);
					this->setSpinsText(this->fromIndex);
					this->setSelectBarIndex(this->selectingIndex);
					this->playSound(SoundEffect_Select);
				}else{
					// 光标不在第一个 (可以往上了)
					this->selectingIndex--;
					this->setSelectBarIndex(this->selectingIndex);
					this->playSound(SoundEffect_Select);
				}
			}
		}
	}else if(rightKeys.contains(key)){
		// 选择下一个value
		if(this->spinValues.length() == 1){
			// 如果列表中只有一个spinvalue, 啥也不做
			return;
		}else{
			// 大于一个选项
			int maxFromIndex = this->spinValues.length() - this->spinItems.length();
			if(this->fromIndex >= (maxFromIndex <= 0 ? 0 : maxFromIndex)){
				// 正在显示后N项
				if(this->selectingIndex == qMin(this->spinItems.length() -1, this->spinValues.length() -1)){
					// 光标在最后一个了 (光标和列表不能再往右了)
					rDebug() << "Last choose last, cannot right.";
					return;
				}else{
					// 光标不在最后一个 (需要判断是否处于选项的后几个)
					if(this->fromIndex + this->selectingIndex >= this->spinValues.length() -1){
						// 如果已经到了选项的最下面, 光标不能再往右了
						rDebug() << "Last choice but cursor not at end, (maybe start position too high), cursor cannot moving downward.";
						return;
					}
					this->selectingIndex++;
					this->setSelectBarIndex(this->selectingIndex);
					this->playSound(SoundEffect_Select);
				}
			}else{
				// 正在显示 非后N项 (不在最后几个)
				if(this->selectingIndex == qMin(this->spinItems.length() -1, this->spinValues.length() -1)){
					// 光标不能往右了 (但From还能往右)
					// 向右滚动一行显示的内容, 光标位置需要重绘
					this->fromIndex++;
					this->adjustSpinItems(this->fromIndex);
					this->setSpinsText(this->fromIndex);
					this->setSelectBarIndex(this->selectingIndex);
					this->playSound(SoundEffect_Select);
				}else{
					// 光标不在最后一个选项上 (可以往右了)
					this->selectingIndex++;
					this->setSelectBarIndex(this->selectingIndex);
					this->playSound(SoundEffect_Select);
				}
			}
		}
	}
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
	try{
		rpgSound->play(name, volume, times);
	}catch(RpgRuntimeException exception){
		rError() << "Cannot play sound:" << name;
	}
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
		int currentWidth = fm.boundingRect(item.getText()).width() + 10;
		//int currentWidth = fm.horizontalAdvance(item.getText());
		//int currentWidth = fm.size(Qt::TextSingleLine, item.getText()).width();
		if(currentWidth > maxLength){
			maxLength = currentWidth;
		}
	}
	return maxLength;
}

void RpgSpinItem::adjustSpinItems(int from){
	int currentX = 0;
	qreal maxWidth = this->dialogSize.width() - (2 * RpgDialogBase::PaddingH);
	const int paddingH = RpgDialogBase::PaddingH;
	// Pair.first == pos.x, Pair.second == width
	QList<QPair<int, int>> positions;

	while(currentX < maxWidth && positions.length() + from < this->spinValues.length()){
		RpgSpinValue *currentValue = this->spinValues.at(from + positions.length());
		int maxWidth = this->calSpinValueMaxWidth(*currentValue);
		if(maxWidth <= 0){
			rError() << "There has a spin value has no items.";
			this->end();
			return;
		}
		positions.append(QPair<int, int>(currentX, maxWidth));
		currentX += maxWidth + paddingH;
	}

	int difference = positions.length() - this->spinItems.length();
	if(difference > 0){
		for(int i = 0; i < difference; i++){
			QGraphicsTextItem *item = new QGraphicsTextItem(this->box);
			item->setFont(this->font);
			//item->setPos(i.first, this->spinItemHeight); //下面循环时再设置位置
			item->setZValue(0.2);
			//item->setTextWidth(i.second); //下面循环时再设置位置
			item->setDefaultTextColor(this->textColor);
			item->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
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
	}else if(difference < 0){
		// 选项多了
		for(int i = 0; i > difference; i--){
			QGraphicsTextItem *item = this->spinItems.takeLast();
			if(item != nullptr){
				item->deleteLater();
			}
		}
	}
	if(this->spinItems.length() != positions.length()){
		rWarning() << "spinItems.length:" << this->spinItems.length() << "!= positions.length()" << positions.length();
	}

	for(int i = 0; i < positions.length(); i++){
		QPair<int, int> pos = positions.at(i);
		if(i >= this->spinItems.length()){
			rError() << "i:" << i << "is out of this->spinItems.length():" << this->spinItems.length();
			break;
		}
		QGraphicsTextItem *item = this->spinItems.at(i);
		item->setPos(pos.first + RpgDialogBase::PaddingH, this->spinItemHeight);
		rDebug() << "Item pos:" << item->pos();
		item->setTextWidth(pos.second);
		rDebug() << "Item width:" << item->textWidth();
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

RpgSpinItem::RpgSpinItem(RpgDialogBase *skin, QGraphicsObject *parent): RpgObject(parent){
	this->setTextColor(Qt::white);

	if(skin == nullptr){
		throw RpgNullPointerException("RpgDialogBase not set.");
	}
	this->skin = static_cast<RpgDialogBase*>(skin);

	this->arrowSymbolsTimeLine->setFrameRange(0, this->skin->getContinueSymbolImageCount());
	this->arrowSymbolsTimeLine->setLoopCount(Rpg::Infinity);
	this->connect(this->arrowSymbolsTimeLine, &QTimeLine::frameChanged, this, &RpgSpinItem::arrowSymbolsTimeLineFrameChangedSlot);

	this->setFont(rpgFont->getFont(FontName, 16));
	rDebug() << "Dialog using font:" << this->font.family();

	// MessageBox
	this->messageBox->setFont(this->font);
	this->messageBox->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
	this->messageBox->document()->setUndoRedoEnabled(false);
	QTextOption messageTextOption = this->messageBox->document()->defaultTextOption();{
		messageTextOption.setWrapMode(QTextOption::WrapAnywhere);
		this->messageBox->document()->setDefaultTextOption(messageTextOption);
	}

	// 设置位置和大小
	this->messageBox->setTextWidth(this->dialogSize.width() - (2 * MessageMarginH));
	this->messageBox->setPos(MessageMarginH, MessageMarginV);

	// 设置阴影
	QGraphicsDropShadowEffect *messageBoxShadowEffect = new QGraphicsDropShadowEffect(this);
	messageBoxShadowEffect->setColor(this->textShadowEffectColor);
	messageBoxShadowEffect->setBlurRadius(this->textShadowEffectBlurRadius);
	messageBoxShadowEffect->setOffset(this->textShadowEffectOffect);
	this->messageBox->setGraphicsEffect(messageBoxShadowEffect);

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

	this->selectBarHeight = 35;

	if(!this->spinItems.isEmpty()){
		rError() << "Run while spinItems wasn't emptied, will damage when append new choices.";
		rError() << "It usually caused by 'hide' method not cleared the textItems list items.";
		this->end();
		return;
	}

	this->spinItemHeight = (this->dialogSize.height() - this->selectBarHeight) / 2;
	if(!this->message.isEmpty()){
		QFontMetrics fm(this->font);
		int messageHeight = fm.boundingRect(this->message).height();
		if(this->dialogSize.height() - messageHeight - this->messageBox->pos().y() < this->selectBarHeight){
			// 如果message把上面的空间都占满了, spin没地方显示就直接盖在上面
			this->spinItemHeight = this->dialogSize.height() - RpgDialogBase::PaddingV - this->selectBarHeight;
			rDebug() << "1!";
		}else{
			// 剩余空间的纵向中间
			this->spinItemHeight = ((this->dialogSize.height() - messageHeight - this->messageBox->pos().y()) - this->selectBarHeight) / 2 + messageHeight + this->messageBox->pos().y();
			rDebug() << "2!";
		}
		rDebug() << "SpinHeight:" << this->spinItemHeight;
	}

//	for(const QPair<int, int> &i: positions){
//		QGraphicsTextItem *item = new QGraphicsTextItem(this->box);
//		item->setFont(this->font);
//		item->setPos(i.first, spinItemHeight);
//		rDebug() << "Item pos:" << item->pos();
//		item->setZValue(0.2);
//		item->setTextWidth(i.second);
//		item->setDefaultTextColor(this->textColor);
//		item->document()->setUndoRedoEnabled(false);
//		QTextOption spinTextOption = item->document()->defaultTextOption();{
//			spinTextOption.setWrapMode(QTextOption::NoWrap);
//			item->document()->setDefaultTextOption(spinTextOption);
//		}
//		QGraphicsDropShadowEffect *itemEffect = new QGraphicsDropShadowEffect(this);
//		itemEffect->setColor(this->textShadowEffectColor);
//		itemEffect->setBlurRadius(this->textShadowEffectBlurRadius);
//		itemEffect->setOffset(this->textShadowEffectOffect);
//		// item会得到effect的控制权, 会在item销毁时帮助销毁effect, 也可以将setGraphicsEffect参数设置为nullptr删除当前设定的effect
//		item->setGraphicsEffect(itemEffect);
//		this->spinItems.append(item);
//	}

	// 计算对话框位置
	QPointF dialogPos = RpgUtils::getDialogPos(this->dialogAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

	this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
	this->box->setPos(dialogPos);

	// selectBar
	this->selectBar->hide();

	//this->upArrowSymbol->hide();
	//this->downArrowSymbol->hide();

	this->showDialog();
}

int RpgSpinItem::waitForComplete(){
	if(!this->isRunning()){
		rDebug() << "RpgSpinItem not running.";
		return -1;
	}
	QEventLoop eventLoop(this);
	this->connect(this, &RpgSpinItem::exitDialogMode, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();
	return 0;
}

void RpgSpinItem::end(){
	emit this->exitDialogMode();
	this->clearSpinItems();
	RpgObject::end();
}

const QStringList RpgSpinItem::getValue(){
	QStringList result;
	for(const RpgSpinValue *i: this->spinValues){
		result.append(i->getCurrentSpinValueItem().getValue());
	}
	return result;
}

void RpgSpinItem::showDialog(){
	this->show();
	rpgState->pushState(RpgState::DialogMode);
	this->dialogAnimation->runDialogAnimations(RpgDialogAnimation::Animations(RpgDialogAnimation::AnimationDialogShow));
	this->dialogAnimation->waitAnimationFinish();

	this->showMessage();
	this->adjustSpinItems(0);
	this->setSpinsText(0);
	this->setSelectBarIndex(0);

	this->selectBar->setVisible(true);
	if(this->selectBarAnimation->state() != QPropertyAnimation::Running){
		this->selectBarAnimation->start();
	}

	if(this->arrowSymbolsTimeLine->state() != QTimeLine::Running){
		this->arrowSymbolsTimeLine->start();
	}

	if(this->timeout > 0){
		if(this->timerId >= 0){
			qWarning() << "Handle timerId not reset to -1:" << this->timerId;
		}
		this->timerId = this->startTimer(timeout);
		rDebug() << "Timer started:" << this->timerId;
	}
}

void RpgSpinItem::hideDialog(){
	if(this->arrowSymbolsTimeLine->state() != QTimeLine::NotRunning){
		this->arrowSymbolsTimeLine->stop();
	}
	this->dialogAnimation->runDialogAnimations(RpgDialogAnimation::Animations(RpgDialogAnimation::AnimationDialogHide));
	this->dialogAnimation->waitAnimationFinish();

	this->hide();
	this->clearSpinItems();

	if(rpgState->getTop() == RpgState::DialogMode){
		rpgState->popState();
	}else{
		rDebug() << "RpgState stack top not DialogMode.";
	}
	this->end();
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
				this->messageBox->setHtml(this->message);
				//this->setLineHeight();
				this->quickShowFlag = false;
				break;
			}
		}
	}else{
		this->messageBox->setHtml(this->message);
		//this->setLineHeight();
	}

	this->showTextInProgressFlag = false;

}

void RpgSpinItem::setSpinsText(int from){
	if(from < 0 || from > this->spinValues.length()){
		rError() << "Index is out of range: '" << from << "' not in range [0," << this->spinValues.length() << ").";
		return;
	}
	QList<RpgSpinValue*>::ConstIterator spinIter = this->spinValues.constBegin();
	for(int i = 0; i < from; i++){
		spinIter++;
	}

	for(int i = 0; i < qMin(this->spinItems.length(), this->spinValues.length() - from); i++){
		this->spinItems.at(i)->setHtml(this->spinValues.at(from + i)->getCurrentSpinValueItem().getText());
		if(this->spinValues.at(from + i)->at(0).getEnable()){
			this->spinItems.at(i)->setDefaultTextColor(this->textColor);
		}else{
			this->spinItems.at(i)->setDefaultTextColor(this->bannedColor);
		}
		QTextOption textOption = this->spinItems.at(i)->document()->defaultTextOption();
		if(textOption.alignment() != (Qt::Alignment)this->spinValues.at(from + i)->getCurrentSpinValueItem().getTextAlign()){
			textOption.setAlignment((Qt::Alignment)this->spinValues.at(from + i)->getCurrentSpinValueItem().getTextAlign());
			this->spinItems.at(i)->document()->setDefaultTextOption(textOption);
		}
	}
	for(int i = this->spinItems.length() - from; i < this->spinItems.length(); i++){
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
	rDebug() << "textItem->boundingRect().size() ==" << textItem->boundingRect().size().toSize();
	this->selectBar->setPos(textItem->pos());
	this->selectBar->setPixmap(this->skin->getSelectBarImage(textItem->boundingRect().size().toSize()));

	this->arrowSymbolsTimeLineFrameChangedSlot(this->arrowSymbolsTimeLine->currentFrame());
}

void RpgSpinItem::arrowSymbolsTimeLineFrameChangedSlot(int frameIndex){
	if(frameIndex >= 0 && frameIndex < this->skin->getContinueSymbolImageCount()){
		const QRectF selectBarBoundingRect = this->selectBar->boundingRect();
		const QPixmap upArrowPixmap = this->skin->getUpArrowSymbolImage(frameIndex);
		this->upArrowSymbol->setPixmap(upArrowPixmap);
		this->upArrowSymbol->setPos((selectBarBoundingRect.width() - upArrowPixmap.width()) / 2.0, - upArrowPixmap.size().height());
		const QPixmap downArrowPixmap = this->skin->getDownArrowSymbolImage(frameIndex);
		this->downArrowSymbol->setPixmap(downArrowPixmap);
		this->downArrowSymbol->setPos((selectBarBoundingRect.width() - downArrowPixmap.width()) / 2.0, selectBarBoundingRect.height());
	}
}





