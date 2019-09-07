#ifndef RPGDIALOG_H
#define RPGDIALOG_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgAvatar.h>
#include <Rpg/com/RpgView.h>
#include <Rpg/core/RpgState.h>

#include <Rpg/core/RpgDialogBase.h>

#include <Rpg/core/RpgDialogMessage.h>
#include <Rpg/core/RpgHtmlSplit.h>
#include <Rpg/core/RpgDialogAnimation.h>

#include <Rpg/exception/RpgNullPointerException.h>

/**
 * @brief The RpgDialog class
 * RpgDialog Rpg对话显示模块
 * RpgDialog由背景图片, 内容框, 继续三角形组和立绘框组成
 * 背景图片box由skin提供, skin提供相应大小的背景图片
 * -> 背景图片的大小不能超过RpgDialogBase::maxDialogSize(), 默认大小:RpgDialogBase::dialogSize(),
 *    有9个位置可以选择Rpg::DialogAlign, 配合宽度显示对话框
 * -> 内容框建立在背景框之上, 宽度是背景图片宽度-2*MessageMarginH, 高度是背景图片高度-2*MessageMarginV,
 *    但是有立绘在前面, 会适量减小宽度, 若内容依然不足, 可能需要去掉立绘(特指前端立绘)
 * -> 继续框是一个列表, 不保证每一个继续图标大小都相同, 也没有确定继续框列表中图片的数量,
 *    所以需要在运行时确定其位置, 一般位置是在对话框正下方, 位置需要重新计算
 * -> 立绘框分为6种, 分别是:
 *    1, 2: 主角立绘在框后面, 呈半身状, 宽高自动适应到(0.8xScreenHeight, 0.25xScreenWidth), 在绝对的屏幕左侧或右侧:
 *          # 在左侧显示, 动画淡入淡出+向左移动
 *          # 在右侧显示, 动画淡入淡出+向右移动
 *    3, 4: 主角立绘在框前面, 呈半身状, 宽高自动适应到(0.8xScreenHeight, 0.25xScreenWidth), 在绝对的屏幕左侧或右侧:
 *          # 在左侧显示, 动画淡入淡出+向左移动
 *          # 在右侧显示, 动画淡入淡出+向右移动
 *          注: 宽度有可能会盖住后面的字, 所以后面的框需要按需要缩小宽度, 还需要计算
 *    5, 6: 主角头像框, 只显示头像, 呈方形, 宽高自动适应(min(min(框高, 框宽), 屏幕宽度/4))
 *          # 在左侧显示, 动画淡入淡出
 *          # 在右侧显示, 动画淡入淡出
 *          注: 对话框一定要比头像宽度宽, 如果没有这么宽, 头像可能会去掉
 *    如果头像位置相同, 则下一句变化不做动画, 若不相同则更换位置运行动画
 *    这些要分开计算, 自动适应的结果建议缓存
 *    (我想把AvatarAround中mode和Left/Right分开了...) 左侧或是右侧已经被分开, 加入至Message中
 *
 * RpgAvatar维护显示的头像
 * RpgDialogMessage维护显示的内容
 * 每一段内容都可能分配一个头像, 如果下段头像相同, 则不执行动画, 如果有变化, 则装载进入/退出动画
 *
 * CurrentBug:
 * 过渡显示阶段, 不会运行以下动画
 * 相同位置(Around)的头像模式转换(头像转立绘, 立绘转立绘等)
 *
 */
class RpgDialog : public RpgObject
{
	Q_OBJECT

	RpgDialogBase *skin = nullptr;

	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this); // 基础框
	QGraphicsTextItem *messageBox = new QGraphicsTextItem(this->box); // 内容框
	QGraphicsPixmapItem *continueSymbol = new QGraphicsPixmapItem(this->box); // 继续框
	QGraphicsPixmapItem *avatarBoxRight = new QGraphicsPixmapItem(this); // 右侧立绘框
	QGraphicsPixmapItem *avatarBoxLeft = new QGraphicsPixmapItem(this); // 左侧立绘框

	// 特效
	QGraphicsDropShadowEffect *messageShadowEffect = new QGraphicsDropShadowEffect(this);

	// 动画
	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, this->avatarBoxLeft, this->avatarBoxRight, 300, QEasingCurve::OutQuad, this);

	// 对话框大小
	QSize dialogSize = RpgDialogBase::dialogSize();
	// 对话框位置
	//QPointF dialogPos = QPointF((ScreenWidth - RpgDialogBase::dialogSize().width()) / 2.0f, (ScreenHeight - RpgDialogBase::MarginV - RpgDialogBase::dialogSize().height()));
	// 内容大小
	//QRectF messageRect = QRect(RpgDialog::MessageMarginH, RpgDialog::MessageMarginV, RpgDialogBase::dialogSize().width() - (RpgDialog::MessageMarginH << 1), RpgDialogBase::dialogSize().height() - (RpgDialog::MessageMarginV << 1));

	Rpg::DialogAlign dialogaAlign = Rpg::AlignBottom;
public:
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	const int MinDialogWidth = 125;
	const int MinDialogHeight = 125;

private:
	QList<RpgDialogMessage> messages;
	QList<RpgDialogMessage> messagesReady;
	int messageIndex = 0;
	RpgDialogMessage lastDialogMessage = RpgDialogMessage("");

	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	QTimeLine *continueSymbolTimeLine = new QTimeLine(1000, this);

	inline void setMessageTextWidth(qreal width){
		this->messageBox->setTextWidth(width);
	}

	void showText(const QString &text, int speed = 0, int pointSize = -1, const QString &name = QString(), qreal lineHeight = 35){
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
//				QString word = htmlSplit.getOne();
//				this->messageBox->textCursor().insertHtml(word);
				QString wordLeft = htmlSplit.chopLeft();
				if(!name.isEmpty()){
					wordLeft.prepend(name + "<br>");
				}
				this->messageBox->setHtml(wordLeft);
				this->setLineHeight(lineHeight);
				RpgUtils::msleep(speed);
				if(this->quickShowFlag == true){
					if(!name.isEmpty()){
						this->messageBox->setHtml(name + "<br>" + text);
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
				this->messageBox->setHtml(name + "<br>" + text);
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

//	void showMessage1(int index){
//		if(index < 0 || index > this->messagesReady.length()){
//			qDebug() << CodePath << "Index out of range: [0," << this->messagesReady.length() << ")";
//			return;
//		}
//		const RpgDialogMessage currentMessage = this->messagesReady.at(index);
//		const QString currentAvatarString = currentMessage.getCharacterName();
//		const Rpg::AvatarMode currentMode = currentMessage.getAvatarMode();
//		const Rpg::AvatarAround currentAround = currentMessage.getAvatarAround();
//		const QString lastAvatarString = this->lastDialogMessage.getCharacterName();
//		const Rpg::AvatarMode lastMode = this->lastDialogMessage.getAvatarMode();
//		const Rpg::AvatarAround lastAround = this->lastDialogMessage.getAvatarAround();

//		RpgDialogAnimation::Animations animations = 0;
//		if(index == 0){
//			animations.setFlag(RpgDialogAnimation::AnimationDialogShow);
//		}

//		if(currentAvatarString.isEmpty()){
//			// 当前立绘为空

//		}else{
//			// 当前立绘非空
//			QGraphicsPixmapItem *currentAvatarBox = nullptr;
//			QGraphicsPixmapItem *oppositeAvatarBox = nullptr;
//			QPixmap avatarPixmap = RpgAvatar::instance()->getAvatarAdjusted(currentAvatarString, currentMode, this->dialogSize - (2 * QSize(MessageMarginH, MessageMarginV)));
//			if(currentAround == Rpg::AvatarAroundLeft){
//				// 当前立绘处于左侧
//				currentAvatarBox = this->avatarBoxLeft;
//				oppositeAvatarBox = this->avatarBoxRight;
//				this->avatarBoxLeft->setPixmap(avatarPixmap);
//				if(!lastAvatarString.isEmpty() && lastAround == Rpg::AvatarAroundRight){

//				}
//			}else if(currentAround == Rpg::AvatarAroundRight){
//				// 当前立绘处于右侧
//				currentAvatarBox = this->avatarBoxRight;
//				oppositeAvatarBox = this->avatarBoxLeft;
//				this->avatarBoxRight->setPixmap(avatarPixmap);
//			}else{
//				qDebug() << CodePath << "CurrentAround is not valid.";
//				goto ShowMessage;
//			}

//			if(currentMode == Rpg::AvatarFrame){

//			}else if(currentMode == Rpg::AvatarHalfBodyFront || currentMode == Rpg::AvatarHalfBodyBehind){


//			}
//		}

//ShowMessage:

//	}

	void showMessage(int index){
		if(index < 0 || index > this->messagesReady.length()){
			qDebug() << CodePath << "index out of bound: [0," << this->messagesReady.length() << ")";
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
						QPointF dialogPos = this->box->pos();
						this->messageBox->setPos(QPointF(qMax(avatarPixmap.width() - dialogPos.x(), dialogPos.x()) + MessageMarginH, MessageMarginV));
						qreal textWidth = qMin(dialogSize.width() - avatarPixmap.width() + dialogPos.x(), qreal(dialogSize.width())) - 2 * MessageMarginH;
						if(textWidth > 0){
							this->setMessageTextWidth(textWidth);
						}else{
							this->setMessageTextWidth(dialogSize.width() - 2 * MessageMarginH);
						}
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
		qDebug() << animations;
		this->dialogAnimation->runDialogAvatarAnimations(mode, this->lastDialogMessage.getAvatarMode(), animations);
		this->showText(current.getText(), current.getSpeed(), current.getPointSize(), current.getName(), current.getLineHeight());
		this->lastDialogMessage = current;
	}

	void showDialog(){
		this->show();
		emit this->enterDialogMode();
		RpgState::instance()->pushMode(RpgState::DialogMode);
		this->showMessage(0);
	}

	void hideDialog(){
		if(this->continueSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->continueSymbolTimeLine->stop();
		}
		RpgDialogAnimation::Animations animations;
		animations.setFlag(RpgDialogAnimation::AnimationDialogHide);
		if(this->avatarBoxLeft->opacity() != 0.0f){
			animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
		}
		if(this->avatarBoxRight->opacity() != 0.0f){
			animations.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
		}
		this->dialogAnimation->runDialogAvatarAnimations(this->lastDialogMessage.getAvatarMode(), this->lastDialogMessage.getAvatarMode(), animations);
		this->dialogAnimation->waitAnimationFinish();
		this->hide();
		this->clearMessages();
		emit this->exitDialogMode();
		if(RpgState::instance()->getTop() == RpgState::DialogMode){
			RpgState::instance()->popMode();
		}else{
			qDebug() << CodePath << "RpgState: Current top mode isn't DialogMode.";
		}
	}

	void receiveKeyPress(int key, Qt::KeyboardModifiers mod){
		if(!this->isRunning()){
			return;
		}
		if(mod != Qt::NoModifier){
			return;
		}
		qDebug() << CodePath << QString("Receive key Release: %1 + %2").arg(RpgUtils::keyModifierToString(mod)).arg(RpgUtils::keyToString((Qt::Key)key));
		if(key == Qt::Key_Return || key == Qt::Key_Space){
			if(this->showTextInProgressFlag == true){
				this->quickShowFlag = true;
			}else{
				this->messageIndex++;
				if(this->messageIndex >= this->messagesReady.length()){
					// 会话全部完成
					this->hideDialog();
					this->messageIndex = 0;
					this->lastDialogMessage = RpgDialogMessage("");
					this->end();
					return;
				}else{
					// 如果还没有, 则显示接下来的对话
					this->showMessage(this->messageIndex);
					// TODO: 设置超时时间
				}
			}
		}
	}

public:
	RpgDialog(RpgDialogBase *dialogBase, QGraphicsObject *parentItem = nullptr): RpgObject(parentItem){
		this->setTextColor(Qt::white);
		this->messageBox->setFont(RpgFont::instance()->getFont("dialog"));
		qDebug() << this->messageBox->font().family();

		if(dialogBase == nullptr){
			throw RpgNullPointerException("RpgDialogBase not set");
		}
		this->skin = static_cast<RpgDialogBase*>(dialogBase);

		this->messageBox->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
		qDebug() << CodePath << "Default CSS:" << this->messageBox->document()->defaultStyleSheet();
		this->messageBox->document()->setUndoRedoEnabled(false);
		QTextOption messageTextOption = this->messageBox->document()->defaultTextOption();{
			messageTextOption.setWrapMode(QTextOption::WrapAnywhere);
			this->messageBox->document()->setDefaultTextOption(messageTextOption);
		}


		// 继续三角形动画
		this->continueSymbolTimeLine->setFrameRange(0, this->skin->getContinueSymbolImageLength());
		this->continueSymbolTimeLine->setLoopCount(Rpg::Infinity);
		this->connect(this->continueSymbolTimeLine, &QTimeLine::frameChanged, [this](int frameIndex){
			if(frameIndex >= 0 && frameIndex < this->skin->getContinueSymbolImageLength()){
				QPixmap framePixmap = this->skin->getContinueSymbolImage(frameIndex);
				this->continueSymbol->setPos((this->dialogSize.width() - framePixmap.width()) / 2.0f, this->dialogSize.height() - (framePixmap.height() / 2.0f));
				this->continueSymbol->setPixmap(framePixmap);
			}
		});

		// Z-Value
		this->setZValue(Rpg::ZValueDialog);
		this->box->setZValue(0.0f);
		this->messageBox->setZValue(0.2f);
		this->continueSymbol->setZValue(0.3f);
		this->avatarBoxRight->setZValue(-0.1f); // 默认先置后
		this->avatarBoxLeft->setZValue(-0.1f); // 默认先置后

		// 默认消息位置和大小
		this->setMessageTextWidth(this->dialogSize.width() - (2.0f * MessageMarginH));
		this->messageBox->setPos(MessageMarginH, MessageMarginV);

		// 字符下的阴影
		this->messageShadowEffect->setColor(QColor(Qt::black));
		this->messageShadowEffect->setBlurRadius(5.0f);
		this->messageShadowEffect->setOffset(2.0f, 2.0f);
		this->messageBox->setGraphicsEffect(this->messageShadowEffect);

		// 立绘初始位置
		this->avatarBoxLeft->setPos(0, 0);
		this->avatarBoxLeft->hide();
		this->avatarBoxRight->setPos(ScreenWidth, 0);
		this->avatarBoxRight->hide();

//		QTextCursor textCursor = this->messageBox->textCursor();
//		QTextBlockFormat blockFormat = textCursor.blockFormat();{
//			blockFormat.setLineHeight(40, QTextBlockFormat::FixedHeight);
//			textCursor.setBlockFormat(blockFormat);
//			this->messageBox->setTextCursor(textCursor);
//		}
		this->setLineHeight(50.0f);

		// 坐标放大不影响显示
		//this->setFlag(QGraphicsItem::ItemIgnoresTransformations);

		// 默认不可见
		this->hide();
		this->avatarBoxLeft->hide();
		this->avatarBoxRight->hide();

		RpgState::instance()->registerRpgObject(this, RpgState::DialogMode);
	}

	~RpgDialog(){
		RpgState::instance()->unregisterRpgObject(this, RpgState::DialogMode);
	}

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认字符颜色
	 */
	inline void setTextColor(const QColor &color){
		this->messageBox->setDefaultTextColor(color);
	}

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认字符颜色
	 */
	inline void setTextColor(Qt::GlobalColor color){
		this->messageBox->setDefaultTextColor(QColor(color));
	}

	/**
	 * @brief appendMessage
	 * @param message
	 * 添加消息
	 */
	inline void appendMessage(const RpgDialogMessage &message){
		this->messages.append(message);
	}

	/**
	 * @brief appendMessage
	 * @param list
	 * 添加一段消息
	 */
	inline void appendMessage(const QList<RpgDialogMessage> &list){
		this->messages.append(list);
	}

	/**
	 * @brief setFont
	 * @param font
	 * 设置字体
	 */
	inline void setFont(const QFont &font){
		this->messageBox->setFont(font);
	}

	QFont getFont() const{
		return this->messageBox->font();
	}

	/**
	 * @brief setLineHeight
	 * @param pixel
	 * 这个函数在输入时没有对其进行高度处理, 我在思考是否能够使用defaultxxx来设置
	 * Solving: 在设置Html之后, 立即执行setLineHeight, document clear之后lineHeight会还原默认
	 * 所以此函数暂时注释, 换成简单函数处理
	 */
//	void setLineHeight(qreal pixel){
//		QTextDocument *document = this->messageBox->document();
//		QTextCursor textCursor = this->messageBox->textCursor();
//		for(QTextBlock i = document->begin(); i != document->end(); i = i.next()){
//			QTextBlockFormat blockFormat = i.blockFormat();
//			blockFormat.setLineHeight(pixel, QTextBlockFormat::FixedHeight);
//			textCursor.setPosition(i.position());
//			textCursor.setBlockFormat(blockFormat);
//			this->messageBox->setTextCursor(textCursor);
//		}
//	}

	void setLineHeight(qreal pixel, int lineHeightType = QTextBlockFormat::FixedHeight){
		QTextCursor textCursor = this->messageBox->textCursor();
		QTextBlockFormat textBlockFormat = textCursor.blockFormat();
		textBlockFormat.setLineHeight(pixel, lineHeightType);
		textCursor.setBlockFormat(textBlockFormat);
		this->messageBox->setTextCursor(textCursor);
	}

	void setLineHeight1(qreal pixel, int lineHeightType = QTextBlockFormat::FixedHeight){
		QTextDocument *doc = this->messageBox->document();
		QTextFrame *rootFrame = doc->rootFrame();
		QTextCursor textCursor = rootFrame->firstCursorPosition();
		QTextBlockFormat textBlockFormat = textCursor.blockFormat();
		textBlockFormat.setLineHeight(pixel, lineHeightType);
		textCursor.setBlockFormat(textBlockFormat);
		this->messageBox->setTextCursor(textCursor);
	}

	/**
	 * @brief setDialogWidth
	 * @param size
	 * 设置对话框大小
	 */
	void setDialogSize(const QSize &size){
		int width = size.width();
		int height = size.height();
		if(width < RpgDialog::MinDialogWidth || width > RpgDialogBase::maxDialogSize().width()){
			qDebug() << CodePath << "Given width: " << width << "is out of range: (" << RpgDialog::MinDialogWidth << "," << RpgDialogBase::maxDialogSize().width() << ")";
			return;
		}
		if(height < RpgDialog::MinDialogHeight || height > RpgDialogBase::maxDialogSize().height()){
			qDebug() << CodePath << "Given height: " << height << "is out of range: (" << RpgDialog::MinDialogHeight << "," << RpgDialogBase::maxDialogSize().height() << ")";
			return;
		}
		this->dialogSize.setWidth(width);
		this->dialogSize.setHeight(height);
	}

	/**
	 * @brief clearMessages
	 * 清理消息内容
	 */
	inline void clearMessages(){
		this->messages.clear();
		this->messagesReady.clear();
	}

	/**
	 * @brief setDialogAlign
	 * @param align
	 * 设置对话框位置
	 */
	inline void setDialogAlign(Rpg::DialogAlign align){
		this->dialogaAlign = align;
	}

	/**
	 * @brief getDialogAlign
	 * @return
	 * 返回对话框位置
	 */
	inline Rpg::DialogAlign getDialogAlign() const{
		return this->dialogaAlign;
	}

	void run() Override{
		RpgObject::run();
		// 重新设定其Scene
		if(RpgView::instance()->scene() == nullptr){
			qDebug() << CodePath << "RpgView not loaded scene yet.";
			this->end();
			throw RpgNullPointerException("RpgView::instance()->scene()");
		}else{
			// 如果该对象已装载至其他的scene的时候, 这个函数能自动将这个对象在其他scene删除后添加至该scene
			RpgView::instance()->scene()->addItem(this);
		}

		// 确定继续三角形存在
		for(int i = 0; i < this->skin->getContinueSymbolImageLength(); i++){
			if(this->skin->getContinueSymbolImage(i).isNull()){
				qDebug() << CodePath << "Continue symbol frame " << i << " is null.";
				this->end();
				return;
			}
		}
		// 复制消息组件
		// 在这里Ready是用来显示的, messages是用来事先设置的
		if(this->messages.isEmpty()){
			qDebug() << CodePath << "Detected message is empty, exitted";
			this->end();
			return;
		}else{
			this->messagesReady.clear();
			this->messagesReady = this->messages;
		}

		// 设置对话框背景, 支持每次对话框形状不同
		QPointF dialogPos;
		switch(this->dialogaAlign){
			case Rpg::AlignTopLeft:
				dialogPos = QPointF(RpgDialogBase::MarginH, RpgDialogBase::MarginV);
				break;
			case Rpg::AlignTop:
				dialogPos = QPointF((ScreenWidth - this->dialogSize.width()) / 2.0f, RpgDialogBase::MarginV);
				break;
			case Rpg::AlignTopRight:
				dialogPos = QPointF(ScreenWidth - this->dialogSize.width() - RpgDialogBase::MarginH, RpgDialogBase::MarginV);
				break;
			case Rpg::AlignLeft:
				dialogPos = QPointF(RpgDialogBase::MarginH, (ScreenHeight - this->dialogSize.height()) / 2.0f);
				break;
			case Rpg::AlignCenter:
				dialogPos = QPointF((ScreenWidth - this->dialogSize.width()) / 2.0f, (ScreenHeight - this->dialogSize.height()) / 2.0f);
				break;
			case Rpg::AlignRight:
				dialogPos = QPointF(ScreenWidth - this->dialogSize.width() - RpgDialogBase::MarginH, (ScreenHeight - this->dialogSize.height()) / 2.0f);
				break;
			case Rpg::AlignBottomLeft:
				dialogPos = QPointF(RpgDialogBase::MarginH, ScreenHeight - this->dialogSize.height() - RpgDialogBase::MarginV);
				break;
			case Rpg::AlignBottom:
				dialogPos = QPointF((ScreenWidth - this->dialogSize.width()) / 2.0f, ScreenHeight - this->dialogSize.height() - RpgDialogBase::MarginV);
				break;
			case Rpg::AlignBottomRight:
				dialogPos = QPointF(ScreenWidth - this->dialogSize.width() - RpgDialogBase::MarginH, ScreenHeight - this->dialogSize.height() - RpgDialogBase::MarginV);
				break;
		}

		this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
		this->box->setPos(dialogPos);

		QPixmap continueSymbol0 = this->skin->getContinueSymbolImage(0);
		QPointF continueSymbolPos((this->dialogSize.width() - continueSymbol0.width()) / 2, this->dialogSize.height() - continueSymbol0.height());
		this->continueSymbol->setVisible(false);

		this->showDialog();
	}

signals:
	void enterDialogMode();
	void exitDialogMode();
};

#endif // RPGDIALOG_H
