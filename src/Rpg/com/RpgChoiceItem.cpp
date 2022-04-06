#include "RpgChoiceItem.h"

#include <Rpg/com/RpgView.h>
#include <Rpg/com/RpgSound.h>
#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgHtmlSplit.h>

#include <QTextDocument>

void RpgChoiceItem::timerEvent(QTimerEvent *event){
	this->timerProcessing = true;
	if(event->timerId() == this->timerId){
		rDebug() << "Killing timer:" << this->timerId;
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
	this->chosenIndex = this->defaultChoiceIndex;
	this->hideDialog();
	this->timerProcessing = false;
}

void RpgChoiceItem::keyReleaseEvent(QKeyEvent *event){
	if(event->isAutoRepeat()){
		return;
	}
	if(this->timerProcessing){
		rDebug() << "TimerEvent is processing choices. this key ignored.";
		return;
	}
	if(this->timerId > 0){
		rDebug() << "Killing timer:" << this->timerId;
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
	if(!this->isRunning()){
		qWarning() << "RpgChoiceItem not running.";
		return;
	}
	if(event->modifiers() != Qt::NoModifier){
		return;
	}
	int key = event->key();
	const QList<Qt::Key> okKeys = QList<Qt::Key>({Qt::Key_Return, Qt::Key_Space});
	const QList<Qt::Key> controlKeys = QList<Qt::Key>({Qt::Key_W, Qt::Key_S, Qt::Key_Up, Qt::Key_Down});
	if(okKeys.contains((Qt::Key)key) || controlKeys.contains((Qt::Key)key)){
		if(this->showTextInProgressFlag == true){
			this->quickShowFlag = true;
			return;
		}
		if(okKeys.contains((Qt::Key)key)){
			this->chosenIndex = this->fromIndex + this->selectingIndex;
			if(this->choices.at(this->chosenIndex).getEnabled()){
				// 选中项目可执行
				this->hideDialog();
				return;
			}else{
				// 选中项目不可执行
				rpgSound->play("banned");
				rDebug() << "Choice is not enabled.";
				return;
			}
		}else if(controlKeys.contains((Qt::Key)key)){
			if(key == Qt::Key_Up || key == Qt::Key_W){
				// 显示上一个项目 (光标希望向上走)
				if(this->choices.length() == 1){
					// 如果列表中只有一个选项, 啥也不做
					return;
				}else{
					// 大于一个选项
					if(this->fromIndex == 0){
						// 正在显示0-2项
						if(this->selectingIndex == 0){
							// 光标已经在第一个了 (不能再往上了)
							qDebug() << "First choose first, cannot up.";
							return;
						}else{
							// 光标不在第一个 (可以往上了)
							this->selectingIndex--;
							this->setSelectBarIndex(this->selectingIndex);
							rpgSound->play("select");
						}
					}else{
						// 正在显示1项以上 (不在第一个)
						if(this->selectingIndex == 0){
							// 光标在第一个 (光标不能往上了, 但from还可以往上)
							// 向上滚动一行显示的内容, 光标位置不变
							this->fromIndex--;
							this->setChoicesText(this->fromIndex);
							rpgSound->play("select");
						}else{
							// 光标不在第一个 (可以往上了)
							this->selectingIndex--;
							this->setSelectBarIndex(this->selectingIndex);
							rpgSound->play("select");
						}
					}
				}
			}
		}else if(key == Qt::Key_Down || key == Qt::Key_S){
			// 显示下一个项目 (光标希望向下走)
			if(this->choices.length() == 1){
				// 如果列表中只有一个选项, 啥也不做
				return;
			}else{
				// 大于一个选项
				int maxFromIndex = this->choices.length() - this->textItems.length();
				if(this->fromIndex >= (maxFromIndex <= 0 ? 0 : maxFromIndex)){
					// 正在显示后N项
					if(this->selectingIndex == qMin(this->textItems.length() -1, this->choices.length() -1)){
						// 光标在最后一个了 (光标和列表都不能往下了)
						rDebug() << "Last choose last, cannot down.";
						return;
					}else{
						// 光标不在最后一个 (可以正常往下了)
						this->selectingIndex++;
						this->setSelectBarIndex(this->selectingIndex);
						rpgSound->play("select");
					}
				}else{
					// 正在显示 非后N项 (不在最后几个)
					if(this->selectingIndex == qMin(this->textItems.length() -1, this->choices.length() -1)){
						// 光标不能往下了 (光标不能往下了, 但from还可以往下)
						// 向下滚动一行显示的内容, 光标位置不变
						this->fromIndex++;
						this->setChoicesText(this->fromIndex);
						rpgSound->play("select");
					}else{
						// 光标不在最后一个选项上 (可以往上了)
						this->selectingIndex++;
						this->setSelectBarIndex(this->selectingIndex);
						rpgSound->play("select");
					}
				}
			}
		}
	}
}

void RpgChoiceItem::clearTextItems(){
	for(QGraphicsTextItem *i: this->textItems){
		if(i != nullptr){
			i->deleteLater();
		}
	}
	this->textItems.clear();
}

void RpgChoiceItem::appendChoice(const RpgChoiceMessage &choice){
	this->choices.append(choice);
}

void RpgChoiceItem::appendChoice(const QList<RpgChoiceMessage> &choices){
	this->choices.append(choices);
}

void RpgChoiceItem::appendChoice(const QString &text, bool enabled){
	this->choices.append(RpgChoiceMessage(text, enabled));
}

void RpgChoiceItem::setDefaultChoice(int index){
	if(index < 0 || index >= this->choices.length()){
		rError() << "Set default choice to index: '" << index << "' is out of range [0," << this->choices.length() << ")";
		return;
	}
	this->defaultChoiceIndex = index;
}

void RpgChoiceItem::setDefaultChoice(const QString &value){
	int index = 0;
	for(const RpgChoiceMessage &i: this->choices){
		if(i.getValue() == value){
			this->setDefaultChoice(index);
		}else{
			index++;
		}
	}
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

RpgChoiceItem::RpgChoiceItem(RpgDialogBase *dialogBase, QGraphicsObject *parent): RpgObject(parent){
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
	this->textShadowEffect->setColor(QColor(Qt::black));
	this->textShadowEffect->setBlurRadius(5.0);
	this->textShadowEffect->setOffset(2.0, 2.0);

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
		this->innerPaddingV = 0;
	}else if(iRatio > 1){
		if(fRatio - iRatio <= 1){
			this->innerPaddingV = 0;
		}else{
			this->innerPaddingV = (this->dialogSize.height() - (selectBarHeight * iRatio)) / (iRatio - 1);
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
		item->document()->setUndoRedoEnabled(false);
		QTextOption choiceTextOption = item->document()->defaultTextOption();{
			choiceTextOption.setWrapMode(QTextOption::NoWrap);
			item->document()->setDefaultTextOption(choiceTextOption);
		}
		item->setGraphicsEffect(this->textShadowEffect);
		this->textItems.append(item);

		curHeight += selectBarHeight + this->innerPaddingV;
	}

	// 计算对话框位置
	QPointF dialogPos = RpgUtils::getDialogPos(this->dialogAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

	this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
	this->box->setPos(dialogPos);

	this->upArrowSymbol->setVisible(false);
	this->downArrowSymbol->setVisible(false);

	this->showDialog();

}

int RpgChoiceItem::waitForComplete(){
	if(!this->isRunning()){
		rDebug() << "RpgChoice not running.";
		return -1;
	}
	QEventLoop eventLoop(this);
	this->connect(this, &RpgChoiceItem::exitDialogMode, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();
	return this->chosenIndex;
}

void RpgChoiceItem::end(){
	emit this->exitDialogMode();
	// 回收选项中新增的TextItem的内存
	for(QGraphicsTextItem *i: this->textItems){
		if(i != nullptr){
			i->deleteLater();
		}
	}
	this->textItems.clear();
	RpgObject::end();
}

void RpgChoiceItem::showDialog(){
	this->show();
	rpgState->pushState(RpgState::DialogMode);
	this->dialogAnimation->runDialogAnimations(RpgDialogAnimation::Animations(RpgDialogAnimation::AnimationDialogShow));
	this->dialogAnimation->waitAnimationFinish();
	if(this->arrowSymbolsTimeLine->state() != QTimeLine::Running){
		this->arrowSymbolsTimeLine->start();
	}
	this->setChoicesText(this->defaultChoiceIndex, this->speed);
}

void RpgChoiceItem::hideDialog(){
	if(this->arrowSymbolsTimeLine->state() != QTimeLine::NotRunning){
		this->arrowSymbolsTimeLine->stop();
	}
	this->dialogAnimation->runDialogAnimations(RpgDialogAnimation::Animations(RpgDialogAnimation::AnimationDialogHide));
	this->dialogAnimation->waitAnimationFinish();

	this->hide();
	this->clearTextItems();
	this->clearChoices();

	if(rpgState->getTop() == RpgState::DialogMode){
		rpgState->popState();
	}else{
		rDebug() << "RpgState stack top not DialogMode.";
	}
	this->end();
}

void RpgChoiceItem::setChoicesText(int from, bool withSpeed){
	if(from < 0 || from > this->choices.length()){
		rError() << "Index is out of range: '" << from << "' not in range [0," << this->choices.length() << ").";
		return;
	}
	QList<RpgChoiceMessage>::ConstIterator choiceIter = this->choices.constBegin();
	for(int i = 0; i < from; i++){
		choiceIter++;
	}

	if(!withSpeed || this->speed <= 0){
		for(int i = 0; i < qMin(this->textItems.length(), this->choices.length() - from); i++){
			this->textItems.at(i)->setHtml(this->choices.at(from + i).getText());
		}
		for(int i = this->choices.length() - from; i < this->textItems.length(); i++){
			this->textItems.at(i)->document()->clear();
		}
	}else{
		this->showTextInProgressFlag = true;
		if(this->arrowSymbolsTimeLine->state() != QTimeLine::NotRunning){
			this->arrowSymbolsTimeLine->stop();
		}
		if(this->speed > 0){
			this->quickShowFlag = false;
			for(int i = 0; i < qMin(this->textItems.length(), this->choices.length() - from); i++){
				RpgHtmlSplit htmlSplit(this->choices.at(from + i).getText());
				while(htmlSplit.hasNext()){
					QString wordLeft = htmlSplit.chopLeft();
					this->textItems.at(i)->setHtml(wordLeft);
					RpgUtils::msleep(speed);
					if(this->quickShowFlag == true){
						this->textItems.at(i)->setHtml(this->choices.at(from + i).getText());
						this->quickShowFlag = false;
						break;
					}
				}
			}
		}else{
			qWarning() << "here variable speed < 0 judged before, here cannot be executed.";
		}
		if(this->arrowSymbolsTimeLine->state() != QTimeLine::Running){
			this->arrowSymbolsTimeLine->start();
		}
		this->showTextInProgressFlag = false;
	}

	// 判断上三角和下三角的可见性
	if(from > 0){
		this->upArrowSymbol->setVisible(true);
	}else{
		this->upArrowSymbol->setVisible(false);
	}
	int maxFromIndex = this->choices.length() - this->textItems.length();
	if(from < (maxFromIndex <= 0 ? 0 : maxFromIndex)){
		this->downArrowSymbol->setVisible(true);
	}else{
		this->downArrowSymbol->setVisible(false);
	}
}

void RpgChoiceItem::setSelectBarIndex(int index){
	if(index < 0 || index >= qMin(this->textItems.length(), this->choices.length() - index)){
		rDebug() << "Index is out of range: '" << index << "' not in range [0," << qMin(this->textItems.length(), this->choices.length() - index) << ")";
		return;
	}
	qreal selectBarHeight = this->skin->selectBarSize().height();
	this->selectBar->setPos(RpgDialogBase::PaddingH, (selectBarHeight + this->innerPaddingV) * this->selectingIndex);
}


