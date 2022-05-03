#include "RpgInputItem.h"

#include <Rpg/com/RpgView.h>
#include <Rpg/com/RpgSound.h>

#include <Rpg/core/RpgFileManager.h>
#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgHtmlSplit.h>

#include <QTextDocument>
#include <QTextBlock>

void RpgInputItem::timerEvent(QTimerEvent *event){
	this->timerProcessing = true;
	if(event->timerId() == this->timerId){
		rDebug() << "Killing timer:" << this->timerId;
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
	this->hideDialog();
	this->timerProcessing = false;
}

void RpgInputItem::keyPressEvent(QKeyEvent *event){
	if(this->timerProcessing){
		rDebug() << "TimerEvent is processing input. this key ignored.";
		return;
	}
	if(!this->isRunning()){
		rWarning() << "RpgInputItem not running.";
		return;
	}
	Qt::Key key = (Qt::Key)event->key();
	Qt::KeyboardModifiers mod = event->modifiers();
	const QList<Qt::Key> okKeys = QList<Qt::Key>({Qt::Key_Return, Qt::Key_Space});
	if(mod != Qt::NoModifier && okKeys.contains(key)){
		// 按下了Enter
		return;
	}else{
		qApp->postEvent(this->input, new QKeyEvent(*event));
	}
}

void RpgInputItem::keyReleaseEvent(QKeyEvent *event){
	if(this->timerProcessing){
		rDebug() << "TimerEvent is processing input. this key ignored.";
		return;
	}
	if(!this->isRunning()){
		rWarning() << "RpgInputItem not running.";
		return;
	}
	Qt::Key key = (Qt::Key)event->key();
	Qt::KeyboardModifiers mod = event->modifiers();
	const QList<Qt::Key> okKeys = QList<Qt::Key>({Qt::Key_Return, Qt::Key_Space});
	if(mod == Qt::NoModifier && okKeys.contains(key)){
		// 按下了Enter
		if(this->showTextInProgressFlag == true){
			this->quickShowFlag = true;
			return;
		}else{
			if(this->timerId > 0){
				rDebug() << "Killing timer:" << this->timerId;
				this->killTimer(this->timerId);
				this->timerId = -1;
			}
			this->playSound(SoundEffect_Accept);
			this->hideDialog();
			return;
		}
	}else{
		qApp->postEvent(this->input, new QKeyEvent(*event));
	}
}

void RpgInputItem::playSound(RpgInputItem::SoundEffect soundEffect, qreal volume, int times){
	const QString name = this->soundEffects.value(soundEffect);
	if(name.isEmpty()){
		// 如果不设置声音, 就不播放声音
		//rError() << "Try to play a 'Empty name' sound.";
		return;
	}
	try{
		rpgSound->play(name, volume, times);
	}catch(const RpgRuntimeException &exception){
		rError() << "Cannot play sound:" << name;
	}
}

void RpgInputItem::setFont(const QString &name, int pointSize, int weight, bool italic){
	this->setFont(rpgFont->getFont(name, pointSize, weight, italic));
}

void RpgInputItem::setLineHeight(qreal pixel, int lineHeightType){
	QTextCursor textCursor = this->messageBox->textCursor();
	QTextBlockFormat textBlockFormat = textCursor.blockFormat();
	textBlockFormat.setLineHeight(pixel, lineHeightType);
	textCursor.setBlockFormat(textBlockFormat);
	this->messageBox->setTextCursor(textCursor);
}

RpgInputItem::RpgInputItem(RpgDialogBase *skin, RpgObject *parent): RpgObject(parent){
	this->setTextColor(Qt::white);

	if(skin == nullptr){
		throw RpgNullPointerException("RpgDialogBase not set.");
	}
	this->skin = static_cast<RpgDialogBase*>(skin);

	//this->input->setStyleSheet(Rpg::getDefaultCss());
	this->input->setAutoFillBackground(false);
	this->input->setStyleSheet("QLineEdit{color: white; background-color: transparent; border: 0 none;}");
	this->input->setContextMenuPolicy(Qt::NoContextMenu);
	//QPalette pal = this->input->palette();{
	//	pal.setColor(QPalette::Background, Qt::transparent);
	//	pal.setColor(QPalette::Foreground, Qt::white);
	//	this->input->setPalette(pal);
	//}

	this->setFont(FontName, 16);
	rDebug() << "Input using font:" << this->getFont();

	// MessageBox
	this->messageBox->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
	this->messageBox->document()->setUndoRedoEnabled(false);
	QTextOption messageTextOption = this->messageBox->document()->defaultTextOption();{
		messageTextOption.setWrapMode(QTextOption::WrapAnywhere);
		this->messageBox->document()->setDefaultTextOption(messageTextOption);
	}

	// 设置位置和大小
	this->messageBox->setTextWidth(this->dialogSize.width() - (2 * MessageMarginH));
	this->messageBox->setPos(MessageMarginH, MessageMarginV);

	// 阴影设置
	this->textShadowEffect->setColor(Qt::black);
	this->textShadowEffect->setBlurRadius(5.0f);
	this->textShadowEffect->setOffset(2, 2);
	this->messageBox->setGraphicsEffect(this->textShadowEffect);

	this->inputItem->setWidget(this->input);

	// Z-Value
	this->setZValue(Rpg::ZValueDialog);
	this->box->setZValue(0);
	this->messageBox->setZValue(0.1);
	this->inputItem->setZValue(0.2);

	// 默认不可见
	this->hide();

	rpgState->registerRpgObject(this, RpgState::DialogMode);
}

RpgInputItem::~RpgInputItem(){
	this->inputItem->deleteLater();
	this->input->deleteLater();
	rpgState->unregisterRpgObject(this, RpgState::DialogMode);
}

void RpgInputItem::run(){
	RpgObject::run();

	// 重新设定其Scene
	if(rpgView->scene() == nullptr){
		rError() << "RpgView not loaded scene yet.";
		this->end();
		throw RpgNullPointerException("RpgView::instance()->scene()");
	}else{
		rpgView->scene()->addItem(this);
	}

	int inputTop = (this->dialogSize.height() - this->input->size().height()) / 2;
	if(!this->message.isEmpty()){
		QFontMetrics fm(this->getFont());
		int messageHeight = fm.boundingRect(this->message).height();
		if(this->dialogSize.height() - messageHeight - this->messageBox->pos().y() < this->input->height()){
			// 如果message把上面的空间都占满了, input没地方显示就直接盖在上面
			inputTop = this->dialogSize.height() - RpgDialogBase::PaddingV - messageHeight;
			rDebug() << "1!";
		}else{
			// 剩余空间的纵向中间
			inputTop = ((this->dialogSize.height() - messageHeight - this->messageBox->pos().y()) - this->input->height()) / 2 + messageHeight + this->messageBox->pos().y();
			rDebug() << "2!";
		}
	}
	this->inputItem->setPos(MessageMarginH * 1.5, inputTop);
	this->input->resize(this->dialogSize.width() - (MessageMarginH * 3), this->input->height());
	this->inputItem->hide();

	QPointF dialogPos = RpgUtils::getDialogPos(this->dialogAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

	this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
	this->box->setPos(dialogPos);

	this->showDialog();
}

int RpgInputItem::waitingForComplete(){
	if(!this->isRunning()){
		rDebug() << "RpgInputItem not running.";
		return -1;
	}
	QEventLoop eventLoop(this);
	this->connect(this, &RpgInputItem::exitDialogMode, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();
	return 0;
}

void RpgInputItem::end(){
	emit this->exitDialogMode();
	RpgObject::end();
}

const QString RpgInputItem::getValue(){
	return this->getInputText();
}

void RpgInputItem::showDialog(){
	this->show();
	rpgState->pushState(RpgState::DialogMode);
	this->dialogAnimation->runDialogAnimations(RpgDialogAnimation::Animations(RpgDialogAnimation::AnimationDialogShow));
	this->dialogAnimation->waitAnimationFinish();

	this->showMessage();

	this->inputItem->show();
	this->input->setFocus();

	if(this->timeout > 0){
		if(this->timerId >= 0){
			rWarning() << "Handle timerId not reset to -1:" << this->timerId;
		}
		this->timerId = this->startTimer(timeout);
		rDebug() << "Timer started:" << this->timerId;
	}
}

void RpgInputItem::hideDialog(){
	if(this->input->hasFocus()){
		this->input->clearFocus();
	}

	this->dialogAnimation->runDialogAnimations(RpgDialogAnimation::Animations(RpgDialogAnimation::AnimationDialogHide));
	this->dialogAnimation->waitAnimationFinish();

	this->hide();

	if(rpgState->getTop() == RpgState::DialogMode){
		rpgState->popState();
	}else{
		rDebug() << "RpgState stack top not DialogMode.";
	}
	this->end();
}

void RpgInputItem::showMessage(){
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
			this->setLineHeight(this->lineHeight);
			RpgUtils::msleep(this->speed);
			if(this->quickShowFlag == true){
				this->messageBox->setHtml(this->message);
				this->setLineHeight(this->lineHeight);
				this->quickShowFlag = false;
				break;
			}
		}
	}else{
		this->messageBox->setHtml(this->message);
	}
	this->showTextInProgressFlag = false;
}


