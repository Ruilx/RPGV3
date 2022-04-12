#include "RpgDialogItem.h"

#include <QTextDocument>
#include <QTextCursor>

#include <Rpg/com/RpgView.h>

#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgAvatar.h>
#include <Rpg/core/RpgHtmlSplit.h>

void RpgDialogItem::timerEvent(QTimerEvent *event){
	this->timerProcessing = true;
	if(event->timerId() == this->timerId){
		rDebug() << "Killing timer" << this->timerId;
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
	this->showNextMessage();
	this->timerProcessing = false;
}

void RpgDialogItem::keyReleaseEvent(QKeyEvent *event){
	if(event->isAutoRepeat()){
		return;
	}
	if(this->timerProcessing){
		rDebug() << "TimerEvent is processing messages, this key ignored.";
		return;
	}
	if(this->timerId > 0){
		rDebug() << "Killing timer:" << this->timerId;
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
	if(!this->isRunning()){
		rWarning() << "RpgDialogItem not running.";
		return;
	}
	if(event->modifiers() != Qt::NoModifier){
		return;
	}
	int key = event->key();
	if(key == Qt::Key_Return || key == Qt::Key_Space){
		if(this->showTextInProgressFlag == true){
			this->quickShowFlag = true;
		}else{
			this->showNextMessage();
		}
	}
}

void RpgDialogItem::appendMessage(const RpgDialogMessage &message){
	if(this->isRunning()){
		rWarning() << "Cannot append message when dialog running.";
		return;
	}
	this->messages.append(message);
}

void RpgDialogItem::appendMessage(const QList<RpgDialogMessage> &messages){
	if(this->isRunning()){
		rWarning() << "Cannot append messages when dialog running.";
		return;
	}
	this->messages.append(messages);
}

void RpgDialogItem::appendMessage(const QString &text, const QString &name){
	if(this->isRunning()){
		rWarning() << "Cannot append message when dialog is running.";
		return;
	}
	this->messages.append(RpgDialogMessage(text, name));
}

void RpgDialogItem::setLineHeight(qreal pixel, int lineHeightType){
	QTextCursor textCursor = this->messageBox->textCursor();
	QTextBlockFormat textBlockFormat = textCursor.blockFormat();
	textBlockFormat.setLineHeight(pixel, lineHeightType);
	textCursor.setBlockFormat(textBlockFormat);
	this->messageBox->setTextCursor(textCursor);
}

void RpgDialogItem::setDialogSize(const QSize &size){
	int w = size.width();
	int h = size.height();
	if(w < RpgDialogBase::MinDialogWidth || w > RpgDialogBase::maxDialogSize().width()){
		rWarning() << "Dialog size width:" << w << "is out of range: (" << RpgDialogBase::MinDialogWidth << "," << RpgDialogBase::maxDialogSize().width() << ")";
		return;
	}
	if(h < RpgDialogBase::MinDialogHeight || w > RpgDialogBase::maxDialogSize().height()){
		rWarning() << "Dialog size height:" << h << "is out of range: (" << RpgDialogBase::MinDialogHeight << "," << RpgDialogBase::maxDialogSize().height() << ")";
		return;
	}
	this->dialogSize.setWidth(w);
	this->dialogSize.setHeight(h);
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
	//rDebug() << "Dialog CSS:" << this->messageBox->document()->defaultStyleSheet();
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

void RpgDialogItem::run(){
	RpgObject::run();
	emit this->enterDialogMode();
	// move to scene
	if(rpgView->scene() == nullptr){
		rError() << "RpgView not loaded any scene yet.";
		this->end();
		throw RpgNullPointerException("rpgView->scene()");
	}else{
		// 如果该对象已装载至其他的scene的时候, 这个函数能自动将这个对象在其他scene删除后添加至该scene
		rpgView->scene()->addItem(this);
	}

	// 确定继续三角形存在
	for(int i = 0; i < this->skin->getContinueSymbolImageCount(); i++){
		if(this->skin->getContinueSymbolImage(i).isNull()){
			rWarning() << "Continue symbol frame" << i << "is null.";
			this->end();
			return;
		}
	}

	// 判断是否有messages
	if(this->messages.isEmpty()){
		rError() << "Messages is empty, exitted.";
		this->end();
		return;
	}

	// 计算对话框位置
	QPointF dialogPos = RpgUtils::getDialogPos(this->dialogAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

	this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
	this->box->setPos(dialogPos);

	// 设置lastDialogMessage为空, 在showMessage中切换至message::constBegin
	this->lastDialogMessage = this->emptyMessage.constBegin();

	this->continueSymbol->setVisible(false);
	this->showDialog();
}

int RpgDialogItem::waitForComplete(){
	if(!this->isRunning()){
		rDebug() << "RpgDialog not running.";
		return -1;
	}
	QEventLoop eventLoop(this);
	this->connect(this, &RpgDialogItem::exitDialogMode, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();
	return 0;
}

void RpgDialogItem::end(){
	emit this->exitDialogMode();
	RpgObject::end();
}

void RpgDialogItem::showDialog(){
	this->show();
	rpgState->pushState(RpgState::DialogMode);
	this->showMessage(0);
}

void RpgDialogItem::hideDialog(){
	if(this->continueSymbolTimeLine->state() != QTimeLine::NotRunning){
		this->continueSymbolTimeLine->stop();
	}
	RpgDialogAnimation::Animations animations;
	animations.setFlag(RpgDialogAnimation::AnimationDialogHide);
	if(this->avatarBoxLeft->opacity() != 0){
		animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
	}
	if(this->avatarBoxRight->opacity() != 0){
		animations.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
	}
	this->dialogAnimation->runDialogAvatarAnimations((*this->lastDialogMessage).getAvatarMode(), (*this->lastDialogMessage).getAvatarMode(), animations);
	this->dialogAnimation->waitAnimationFinish();

	this->hide();
	this->clearMessages();

	if(rpgState->getTop() == RpgState::DialogMode){
		rpgState->popState();
	}else{
		rDebug() << "RpgState stack top not DialogMode.";
	}
	this->end();
}

void RpgDialogItem::showNextMessage(){
	this->messageIndex++;
	if(this->messageIndex >= this->messages.length()){
		// 会话全部完成
		this->hideDialog();
		this->messageIndex = 0;
		this->lastDialogMessage = this->messages.constBegin();
		return;
	}else{
		this->showMessage(this->messageIndex);

		// 设置超时时间
		if(this->timerId > 0){
			rDebug() << "Killing timer:" << this->timerId;
			this->killTimer(this->timerId);
			this->timerId = -1;
		}
		int waitTime = this->messages.at(this->messageIndex).getWaitTime();
		if(waitTime >= 0){
			if(this->timerId >= 0){
				qWarning() << "Handle timerId not reset to -1: " << this->timerId;
			}
			// startTimer时长为0时, 程序可能会反复post timerEvent, 可能会导致爆炸, 选择100ms进行延迟假装立即返回(用户按键也没那么快)
			this->timerId = this->startTimer(waitTime <= 100? 100: waitTime);
			rDebug() << "Timer started:" << this->timerId;
		}
	}
}

void RpgDialogItem::showMessage(int index){
	if(index < 0 || index > this->messages.length()){
		rWarning() << "Index out of range: [0," << this->messages.length() << ")";
		return;
	}
	const RpgDialogMessage current = this->messages.at(index);
	QString characterName = current.getCharacterName();
	Rpg::AvatarMode mode = current.getAvatarMode(); // 前半身立绘, 后半身立绘, 头像
	Rpg::AvatarAround around = current.getAvatarAround(); // 左, 右
	RpgDialogAnimation::Animations animations = 0;
	if(index == 0){
		animations.setFlag(RpgDialogAnimation::AnimationDialogShow);
	}
	if(index == 1){
		// 设置lastDialogMessage到messages::constBegin
		this->lastDialogMessage = this->messages.constBegin();
	}

	if(!characterName.isEmpty()){
		// 有立绘/头像
		// 从 无/头像 -> 立绘 => 立绘头像进入动画
		// 从 立绘 -> 头像/无 => 立绘头像退出动画
		// 从 立绘左 -> 立绘右 => 左侧退出, 右侧进入
		// 从 立绘右 -> 立绘左 => 右侧退出, 左侧进入
		QPixmap avatarPixmap = rpgAvatar->getAvatarAdjusted(characterName, mode, this->dialogSize - (2 * QSize(MessageMarginH, MessageMarginV)));
		if(mode == Rpg::AvatarHalfBodyFront || mode == Rpg::AvatarHalfBodyBehind){
			// 本次半身立绘
			if(around == Rpg::AvatarAroundLeft){
				if(around != (*this->lastDialogMessage).getAvatarAround()){
					// Left, not same as last
					animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarShow);
					if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundRight){
						animations.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
					}
					this->avatarBoxLeft->setPixmap(avatarPixmap);
					this->avatarBoxLeft->setPos(RpgDialogAnimation::leftAvatarOutsidePos(avatarPixmap.size()));
					this->avatarBoxLeft->setOpacity(0);
				}else{
					// Left, only change pixmap
					this->avatarBoxLeft->setPixmap(avatarPixmap);
					this->avatarBoxLeft->setPos(RpgDialogAnimation::leftAvatarInsidePos(avatarPixmap.size()));
				}

				if(mode == Rpg::AvatarHalfBodyFront){
					qreal x = (qreal)avatarPixmap.width() - this->box->pos().x();
					if(x > 0){
						this->messageBox->setPos(x + MessageMarginH, MessageMarginV);
					}else{
						this->messageBox->setPos(MessageMarginH, MessageMarginV);
					}
					this->setMessageTextWidth(dialogSize.width() - x - MessageMarginH * 2);
					this->avatarBoxLeft->setZValue(0.1f);
				}

				if(!this->avatarBoxLeft->isVisible()){
					this->avatarBoxLeft->show();
				}
			}else if(around == Rpg::AvatarAroundRight){
				if(around != (*this->lastDialogMessage).getAvatarAround()){
					// Right, not same as last
					animations.setFlag(RpgDialogAnimation::AnimationRightAvatarShow);
					if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundLeft){
						animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
					}
					this->avatarBoxRight->setPixmap(avatarPixmap);
					this->avatarBoxRight->setPos(RpgDialogAnimation::rightAvatarOutsidePos(avatarPixmap.size()));
					this->avatarBoxRight->setOpacity(0);
				}else{
					// Right, only change pixmap
					this->avatarBoxRight->setPixmap(avatarPixmap);
					this->avatarBoxRight->setPos(RpgDialogAnimation::rightAvatarInsidePos(avatarPixmap.size()));
				}

				if(mode == Rpg::AvatarHalfBodyFront){
					QPointF dialogPos = this->box->pos();
					this->messageBox->setPos(QPointF(MessageMarginH, MessageMarginV));
					qreal textWidth = qMin(dialogSize.width() - avatarPixmap.width() + (ScreenWidth - dialogPos.x() - dialogSize.width()), qreal(dialogSize.width()));
					if(textWidth > 0){
						this->setMessageTextWidth(textWidth);
					}else{
						this->setMessageTextWidth(dialogSize.width() - 2 * MessageMarginH);
					}
					this->avatarBoxRight->setZValue(0.1f);
				}else if(mode == Rpg::AvatarHalfBodyBehind){
					// Avatar behind
					this->messageBox->setPos(QPointF(MessageMarginH, MessageMarginV));
					this->setMessageTextWidth(dialogSize.width() - 2 * MessageMarginH);
					this->avatarBoxRight->setZValue(-0.1f);
				}

				if(!this->avatarBoxRight->isVisible()){
					this->avatarBoxRight->show();
				}
			}
		}else if(mode == Rpg::AvatarFrame){
			// header avatar
			if(around == Rpg::AvatarAroundLeft){
				// Left, need repos
				if((*this->lastDialogMessage).getAvatarMode() != Rpg::AvatarFrame){
					// mode not same, force to remove avatars
					if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundLeft){
						// same position, waiting to avatar exit
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, (*this->lastDialogMessage).getAvatarMode(), animationsTemp);
						this->dialogAnimation->waitAnimationFinish();

						this->avatarBoxLeft->setPixmap(avatarPixmap);
						this->avatarBoxLeft->setPos(QPointF(MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxLeft->setOpacity(1.0f);
						this->avatarBoxLeft->setZValue(0.1f);
						this->avatarBoxLeft->show();
					}else if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundRight){
						// 不同位置, (可以不等待) 但是左边已经有了等待, 所以也加上等待
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, (*this->lastDialogMessage).getAvatarMode(), animationsTemp);
						this->dialogAnimation->waitAnimationFinish();
						this->avatarBoxRight->hide(); // 右侧隐藏
						this->avatarBoxLeft->setPixmap(avatarPixmap);
						this->avatarBoxLeft->setPos(QPointF(MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxLeft->setOpacity(1.0f);
						this->avatarBoxLeft->setZValue(0.1f);
						this->avatarBoxLeft->show();
					}else{
						// 上一次为没有
						this->avatarBoxLeft->setPixmap(avatarPixmap);
						this->avatarBoxLeft->setPos(QPointF(MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxLeft->setOpacity(1.0f);
						this->avatarBoxLeft->setZValue(0.1f);
						this->avatarBoxLeft->show();
					}
				}else{
					// 是同一个模式, 只需要调整位置和hide就行
					if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundLeft){
						this->avatarBoxLeft->setPixmap(avatarPixmap);
						this->avatarBoxLeft->setPos(QPointF(MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxLeft->setOpacity(1.0f);
						this->avatarBoxLeft->setZValue(0.1f);
						this->avatarBoxLeft->show();
					}else if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundRight){
						this->avatarBoxRight->hide(); // 右侧隐藏
						this->avatarBoxLeft->setPixmap(avatarPixmap);
						this->avatarBoxLeft->setPos(QPointF(MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxLeft->setOpacity(1.0f);
						this->avatarBoxLeft->setZValue(0.1f);
						this->avatarBoxLeft->show();
					}
				}
				this->messageBox->setPos(QPointF(this->avatarBoxLeft->pixmap().width() + 2 * MessageMarginH, MessageMarginV));
				this->setMessageTextWidth(this->dialogSize.width() - this->messageBox->pos().x() - MessageMarginH);
			}else if(around == Rpg::AvatarAroundRight){
				// 右, 需要调整位置, 如果之前是立绘, 将立绘头像退出
				if((*this->lastDialogMessage).getAvatarMode() != Rpg::AvatarFrame){
					// 不是同一个模式, 需要强制迁出立绘
					if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundLeft){
						// 不同位置, (可以不等待) 但是右边已经有了等待, 所以也加上等待
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, (*this->lastDialogMessage).getAvatarMode(), animationsTemp);
						this->dialogAnimation->waitAnimationFinish();
						this->avatarBoxLeft->hide();
						this->avatarBoxRight->setPixmap(avatarPixmap);
						this->avatarBoxRight->setPos(QPointF(this->dialogSize.width() - avatarPixmap.width() - MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxRight->setOpacity(1.0f);
						this->avatarBoxRight->setZValue(0.1f);
						this->avatarBoxRight->show();
					}else if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundRight){
						// 相同位置, 需要等待立绘退出之后才能转换显示成头像模式
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationDialogHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, (*this->lastDialogMessage).getAvatarMode(), animationsTemp);
						this->avatarBoxRight->setPixmap(avatarPixmap);
						this->avatarBoxRight->setPos(QPointF(this->dialogSize.width() - avatarPixmap.width() - MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxRight->setOpacity(1.0f);
						this->avatarBoxRight->setZValue(0.1f);
						this->avatarBoxRight->show();
					}else{
						// 上一次为没有
						this->avatarBoxRight->setPixmap(avatarPixmap);
						this->avatarBoxRight->setPos(QPointF(this->dialogSize.width() - avatarPixmap.width() - MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxRight->setOpacity(1.0f);
						this->avatarBoxRight->setZValue(0.1f);
						this->avatarBoxRight->show();
					}
				}else{
					// 是同一个模式, 只需要调整位置和hide就行
					if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundLeft){
						this->avatarBoxLeft->hide();
						this->avatarBoxRight->setPixmap(avatarPixmap);
						this->avatarBoxRight->setPos(QPointF(this->dialogSize.width() - avatarPixmap.width() - MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxRight->setOpacity(1.0f);
						this->avatarBoxRight->setZValue(0.1f);
						this->avatarBoxRight->show();
					}else if((*this->lastDialogMessage).getAvatarAround() == Rpg::AvatarAroundRight){
						this->avatarBoxRight->setPixmap(avatarPixmap);
						this->avatarBoxRight->setPos(QPointF(this->dialogSize.width() - avatarPixmap.width() - MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxRight->setOpacity(1.0f);
						this->avatarBoxRight->setZValue(0.1f);
						this->avatarBoxRight->show();
					}
				}
				this->messageBox->setPos(QPointF(MessageMarginH, MessageMarginV));
				this->setMessageTextWidth(this->dialogSize.width() - this->avatarBoxRight->pixmap().width() - MessageMarginH);
			}
		}
	}else{
		// 没有立绘
		bool lastHasAvatar = !(*this->lastDialogMessage).getCharacterName().isEmpty();
		Rpg::AvatarMode lastMode = (*this->lastDialogMessage).getAvatarMode();
		Rpg::AvatarAround lastAround = (*this->lastDialogMessage).getAvatarAround();
		if(lastHasAvatar){
			// 上一次有立绘
			if(lastMode == Rpg::AvatarHalfBodyFront || lastMode == Rpg::AvatarHalfBodyBehind){
				// 上一次立绘是半身立绘
				if(lastAround == Rpg::AvatarAroundLeft){
					// 从左边退出
					animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
				}else if(lastAround == Rpg::AvatarAroundRight){
					// 从右边退出
					animations.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
				}
			}else if(lastMode == Rpg::AvatarFrame){
				// 上一次立绘是头像
				if(lastAround == Rpg::AvatarAroundLeft){
					this->avatarBoxLeft->hide();
				}else if(lastAround == Rpg::AvatarAroundRight){
					this->avatarBoxRight->hide();
				}
			}
		}
		// 还原消息宽度
		this->messageBox->setPos(QPointF(MessageMarginH, MessageMarginV));
		this->setMessageTextWidth(dialogSize.width() - 2 * MessageMarginH);
	}
	rDebug() << "Animations:" << animations;
	this->dialogAnimation->runDialogAvatarAnimations(mode, (*this->lastDialogMessage).getAvatarMode(), animations);
	this->showText(current.getText(), current.getSpeed(), current.getPointSize(), current.getName(), current.getLineHeight());
	if(index != 0){
		this->lastDialogMessage++;
	}
}

void RpgDialogItem::showText(const QString &text, int speed, int pointSize, const QString &name, qreal lineHeight){
	if(text.isEmpty()){
		return;
	}
	this->showTextInProgressFlag = true;
	this->continueSymbolTimeLine->stop();
	this->continueSymbol->setVisible(false);
	this->messageBox->document()->clear();
	if(pointSize > 0 && this->messageBox->font().pointSize() != pointSize){
		QFont font = this->messageBox->font();
		font.setPointSize(pointSize);
		this->messageBox->setFont(font);
	}
	if(speed > 0){
		this->quickShowFlag = false;
		RpgHtmlSplit htmlSplit(text);
		while(htmlSplit.hasNext()){
			QString wordLeft = htmlSplit.chopLeft();
			if(!name.isEmpty()){
				wordLeft.prepend(name % "<br>");
			}
			this->messageBox->setHtml(wordLeft);
			this->setLineHeight(lineHeight);
			RpgUtils::msleep(speed);
			if(this->quickShowFlag == true){
				if(!name.isEmpty()){
					this->messageBox->setHtml(name % "<br>" % text);
					this->setLineHeight(lineHeight);
				}else{
					this->messageBox->setHtml(text);
					this->setLineHeight(lineHeight);
				}
				this->quickShowFlag = false;
				break;
			}
		}
	}else{
		if(!name.isEmpty()){
			this->messageBox->setHtml(name % "<br>" % text);
			this->setLineHeight(lineHeight);
		}else{
			this->messageBox->setHtml(text);
			this->setLineHeight(lineHeight);
		}
	}
	this->showTextInProgressFlag = false;
	this->continueSymbol->setVisible(true);
	this->continueSymbolTimeLine->start();
}
