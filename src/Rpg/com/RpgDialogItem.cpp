#include "RpgDialogItem.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlockFormat>

#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgAvatar.h>

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

RpgDialogItem::RpgDialogItem(RpgDialogBase *dialogBase, QGraphicsObject *parent): RpgObject(parent){
	this->setTextColor(Qt::white);
	this->messageBox->setFont(rpgFont->getFont(FontName));
	rDebug() << "Dialog using font:" << this->messageBox->font().family();

	if(dialogBase == nullptr){
		throw RpgNullPointerException("RpgDialogBase not set");
	}
	this->skin = static_cast<RpgDialogBase*>(dialogBase);

	this->messageBox->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
	rDebug() << "Dialog CSS:" << this->messageBox->document()->defaultStyleSheet();
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

void RpgDialogItem::showDialog(){
	this->show();
	rpgState->pushState(RpgState::DialogMode);
	this->showMessage(0);
}

void RpgDialogItem::showMessage(index){
	if(index < 0 || index > this->messages.length()){
		rWarning() << "Index out of range: [0," << this->messages.length() << ")";
		return;
	}
	const RpgDialogMessage current = this->messages.at(index);
	QString characterName = current.getCharacterName();
	Rpg::AvatarMode mode = current.getAvatarMode(); // 前半身立绘, 后半身立绘, 头像
	Rpg::AvatarAround around = current.getAvatarAround(); // 左, 右
	RpgDialogAnimation::Animations animations = 0;
	if(index = 0){
		animations.setFlag(RpgDialogAnimation::AnimationDialogShow);
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
					}
				}
			}
		}
	}


}
