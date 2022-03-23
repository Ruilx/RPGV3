#include "RpgDialog.h"

void RpgDialog::showMessage(int index){
	if(index < 0 || index > this->messagesReady.length()){
		rDebug() << "index out of bound: [0," << this->messagesReady.length() << ")";
		return;
	}
	const RpgDialogMessage current = this->messagesReady.at(index);
	QString avatarString = current.getCharacterName();
	Rpg::AvatarMode mode = current.getAvatarMode(); // 前半身立绘, 后半身立绘, 头像
	Rpg::AvatarAround around = current.getAvatarAround(); // 左/右
	RpgDialogAnimation::Animations animations = 0;
	if(index == 0){
		animations.setFlag(RpgDialogAnimation::AnimationDialogShow);
	}
	// 这里只管dialog显示, 不管dialog隐藏, 隐藏由hideDialog()函数进行
	//		else if(index >= this->messagesReady.length() -1){
	//			animations.setFlag(RpgDialogAnimation::AnimationDialogHide);
	//		}

	if(!avatarString.isEmpty()){
		// 如果有头像的话, 与前一次相对比, 如果:
		// 头像从没有或头像变成立绘, 立绘头像做进入
		// 头像从立绘变到头像或没有, 立绘头像做退出
		// 头像从立绘左侧变到右侧, 左侧立绘头像做退出, 右侧立绘头像做进入
		// 头像从立绘右侧变到左侧, 右侧立绘头像做退出, 左侧立绘头像做进入
		QPixmap avatarPixmap = RpgAvatar::instance()->getAvatarAdjusted(avatarString, mode, this->dialogSize - (2 * QSize(MessageMarginH, MessageMarginV)));
		if(mode == Rpg::AvatarHalfBodyFront || mode == Rpg::AvatarHalfBodyBehind){
			// 如果这次是半身立绘
			if(around == Rpg::AvatarAroundLeft){
				if(around != this->lastDialogMessage.getAvatarAround()){
					// 左, 需要动, 与之前不一样
					animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarShow);
					if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundRight){
						animations.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
					}
					this->avatarBoxLeft->setPixmap(avatarPixmap);
					this->avatarBoxLeft->setPos(RpgDialogAnimation::leftAvatarOutsidePos(avatarPixmap.size()));
					this->avatarBoxLeft->setOpacity(0.0f);
				}else{
					// 左, 不用动, 只换头像即可
					this->avatarBoxLeft->setPixmap(avatarPixmap);
					this->avatarBoxLeft->setPos(RpgDialogAnimation::leftAvatarInsidePos(avatarPixmap.size()));
				}
				if(mode == Rpg::AvatarHalfBodyFront){
					// 重新定位消息框位置和宽度, 立绘在左侧
//					QPointF dialogPos = this->box->pos();
//					this->messageBox->setPos(QPointF(qMax(avatarPixmap.width() - dialogPos.x(), dialogPos.x()) + MessageMarginH, MessageMarginV));
//					qreal textWidth = qMin(dialogSize.width() - avatarPixmap.width() + dialogPos.x(), qreal(dialogSize.width())) - 2 * MessageMarginH;
//					if(textWidth > 0){
//						this->setMessageTextWidth(textWidth);
//					}else{
//						this->setMessageTextWidth(dialogSize.width() - 2 * MessageMarginH);
//					}

					qreal x = (qreal)avatarPixmap.width() - this->box->pos().x();
					if(x > 0){
						this->messageBox->setPos(x + MessageMarginH, MessageMarginV);
					}else{
						this->messageBox->setPos(MessageMarginH, MessageMarginV);
					}
					this->setMessageTextWidth(dialogSize.width() - x - MessageMarginH * 2);

					this->avatarBoxLeft->setZValue(0.1f);
				}else if(mode == Rpg::AvatarHalfBodyBehind){
					// 立绘在后面, 宽度还原成对话框宽度 - 2 * Margin
					this->messageBox->setPos(QPointF(MessageMarginH, MessageMarginV));
					this->setMessageTextWidth(dialogSize.width() - 2 * MessageMarginH);
					this->avatarBoxLeft->setZValue(-0.1f);
				}
				if(!this->avatarBoxLeft->isVisible()){
					this->avatarBoxLeft->show();
				}
			}else if(around == Rpg::AvatarAroundRight){
				if(around != this->lastDialogMessage.getAvatarAround()){
					// 右, 需要动, 与之前不一样
					animations.setFlag(RpgDialogAnimation::AnimationRightAvatarShow);
					if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundLeft){
						animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
					}
					this->avatarBoxRight->setPixmap(avatarPixmap);
					this->avatarBoxRight->setPos(RpgDialogAnimation::rightAvatarOutsidePos(avatarPixmap.size()));
					qDebug() << this->avatarBoxRight->pos() << this->avatarBoxRight->isVisible() << this->avatarBoxRight->opacity();
					this->avatarBoxRight->setOpacity(0.0f);
				}else{
					// 右, 不用动, 只换头像即可
					this->avatarBoxRight->setPixmap(avatarPixmap);
					this->avatarBoxRight->setPos(RpgDialogAnimation::rightAvatarInsidePos(avatarPixmap.size()));
				}
				if(mode == Rpg::AvatarHalfBodyFront){
					// 重新定位消息框位置和宽度, 立绘在右侧
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
					// 立绘在后面, 宽度还原成对话框宽度 - 2 * Margin
					this->messageBox->setPos(QPointF(MessageMarginH, MessageMarginV));
					this->setMessageTextWidth(dialogSize.width() - 2 * MessageMarginH);
					this->avatarBoxRight->setZValue(-0.1f);
				}
				if(!this->avatarBoxRight->isVisible()){
					this->avatarBoxRight->show();
				}
			}
		}else if(mode == Rpg::AvatarFrame){
			// 如果这次是头像立绘
			if(around == Rpg::AvatarAroundLeft){
				// 左, 需要调整位置, 如果之前是立绘, 将立绘头像退出
				if(this->lastDialogMessage.getAvatarMode() != Rpg::AvatarFrame){
					// 不是同一个模式, 需要强制迁出立绘
					if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundLeft){
						// 相同位置, 需要等待立绘退出之后才能转换显示成头像模式
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, this->lastDialogMessage.getAvatarMode(), animationsTemp);
						this->dialogAnimation->waitAnimationFinish();
						this->avatarBoxLeft->setPixmap(avatarPixmap);
						this->avatarBoxLeft->setPos(QPointF(MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxLeft->setOpacity(1.0f);
						this->avatarBoxLeft->setZValue(0.1f);
						this->avatarBoxLeft->show();
					}else if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundRight){
						// 不同位置, (可以不等待) 但是左边已经有了等待, 所以也加上等待
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, this->lastDialogMessage.getAvatarMode(), animationsTemp);
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
					if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundLeft){
						this->avatarBoxLeft->setPixmap(avatarPixmap);
						this->avatarBoxLeft->setPos(QPointF(MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxLeft->setOpacity(1.0f);
						this->avatarBoxLeft->setZValue(0.1f);
						this->avatarBoxLeft->show();
					}else if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundRight){
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
				if(this->lastDialogMessage.getAvatarMode() != Rpg::AvatarFrame){
					// 不是同一个模式, 需要强制迁出立绘
					if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundLeft){
						// 不同位置, (可以不等待) 但是右边已经有了等待, 所以也加上等待
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, this->lastDialogMessage.getAvatarMode(), animationsTemp);
						this->dialogAnimation->waitAnimationFinish();
						this->avatarBoxLeft->hide();
						this->avatarBoxRight->setPixmap(avatarPixmap);
						this->avatarBoxRight->setPos(QPointF(this->dialogSize.width() - avatarPixmap.width() - MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxRight->setOpacity(1.0f);
						this->avatarBoxRight->setZValue(0.1f);
						this->avatarBoxRight->show();
					}else if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundRight){
						// 相同位置, 需要等待立绘退出之后才能转换显示成头像模式
						RpgDialogAnimation::Animations animationsTemp = 0;
						animationsTemp.setFlag(RpgDialogAnimation::AnimationDialogHide);
						this->dialogAnimation->runDialogAvatarAnimations(mode, this->lastDialogMessage.getAvatarMode(), animationsTemp);
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
					if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundLeft){
						this->avatarBoxLeft->hide();
						this->avatarBoxRight->setPixmap(avatarPixmap);
						this->avatarBoxRight->setPos(QPointF(this->dialogSize.width() - avatarPixmap.width() - MessageMarginH, MessageMarginV) + this->box->pos());
						this->avatarBoxRight->setOpacity(1.0f);
						this->avatarBoxRight->setZValue(0.1f);
						this->avatarBoxRight->show();
					}else if(this->lastDialogMessage.getAvatarAround() == Rpg::AvatarAroundRight){
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
		bool lastHasAvatar = !this->lastDialogMessage.getCharacterName().isEmpty();
		Rpg::AvatarMode lastMode = this->lastDialogMessage.getAvatarMode();
		Rpg::AvatarAround lastAround = this->lastDialogMessage.getAvatarAround();
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
	this->dialogAnimation->runDialogAvatarAnimations(mode, this->lastDialogMessage.getAvatarMode(), animations);
	this->showText(current.getText(), current.getSpeed(), current.getPointSize(), current.getName(), current.getLineHeight());
	this->lastDialogMessage = current;
}
