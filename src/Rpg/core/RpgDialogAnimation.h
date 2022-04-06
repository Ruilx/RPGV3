#ifndef RPGDIALOGANIMATION_H
#define RPGDIALOGANIMATION_H

#include <Rpg/Rpg.h>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

#include <Rpg/core/RpgItemProperties.h>

#include <QPropertyAnimation>

#include <Rpg/exception/RpgNullPointerException.h>
#include <Rpg/exception/RpgRuntimeException.h>

class RpgDialogAnimation
{
	QObject *parent = nullptr;

	int duration = 300;
	QEasingCurve::Type easingCurveType = QEasingCurve::OutQuad;

	QGraphicsItem *dialog = nullptr;
	QGraphicsPixmapItem *avatarItemLeft = nullptr;
	QGraphicsPixmapItem *avatarItemRight = nullptr;

	RpgItemProperties *dialogProperties = nullptr;
	RpgItemProperties *avatarItemLeftProperties = nullptr;
	RpgItemProperties *avatarItemRightProperties = nullptr;

	QParallelAnimationGroup *group = nullptr;

	enum AnimationMode{
		AnimationEnter = 0,
		AnimationExit = 1,
	};

	/**
	 * @brief makeDialogAnimation
	 * @param mode
	 * @return
	 * 创造一个窗口的淡入/淡出的动画, 返回已new的一个动画指针, group运行结束之后会自动销毁
	 */
	QPropertyAnimation* makeDialogAnimation(RpgDialogAnimation::AnimationMode mode){
		if(this->dialog == nullptr){
			rWarning() << "=>this->dialog is nullptr.";
			return nullptr;
			//throw RpgNullPointerException("RpgDialogAnimtion::dialog");
		}
		QPropertyAnimation *dialogAnimation = new QPropertyAnimation(this->dialogProperties, "opacity", this->parent);
		dialogAnimation->setDuration(this->duration);
		dialogAnimation->setEasingCurve(this->easingCurveType);
		if(mode == AnimationEnter){
			dialogAnimation->setStartValue(0.0f);
			dialogAnimation->setEndValue(1.0f);
		}else{
			dialogAnimation->setStartValue(1.0f);
			dialogAnimation->setEndValue(0.0f);
		}
		return dialogAnimation;
	}

	/**
	 * @brief makeAvatarAnimation
	 * @param target
	 * @param mode
	 * @param avatarMode
	 * @param avatarAround
	 * @return
	 * 创造一个左/右头像的移动动画, 返回已new的一个动画指针, group运行结束之后会自动销毁
	 */
	QPropertyAnimation* makeAvatarAnimation(QGraphicsPixmapItem *target, RpgDialogAnimation::AnimationMode mode, Rpg::AvatarMode avatarMode, Rpg::AvatarAround avatarAround){
		QPropertyAnimation *avatarAnimation = nullptr;
		if(target == nullptr){
			rWarning() << "target is nullptr.";
			return nullptr;
		}
		if(!target->pixmap().isNull() && (avatarMode == Rpg::AvatarHalfBodyFront || avatarMode == Rpg::AvatarHalfBodyBehind)){
			RpgItemProperties *properties = nullptr;
			if(target == this->avatarItemLeft){
				// target 必不是nullptr, 如果avatarItemLeft是空的情况, avatarItemLeftProperties不会复制到properties中.
				properties = this->avatarItemLeftProperties;
			}else if(target == this->avatarItemRight){
				// target 必不是nullptr, 如果avatarItemRight是空的情况, avatarItemRightProperties不会复制到properties中.
				properties = this->avatarItemRightProperties;
			}else{
				throw RpgRuntimeException("Target is not fit weather left avatar or right avatar.");
			}
			avatarAnimation = new QPropertyAnimation(properties, "pos", this->parent);
			avatarAnimation->setDuration(this->duration);
			avatarAnimation->setEasingCurve(this->easingCurveType);
			if(mode == AnimationEnter){
				if(avatarAround == Rpg::AvatarAroundLeft){
					avatarAnimation->setStartValue(this->leftAvatarOutsidePos(target->pixmap().size()));
					avatarAnimation->setEndValue(this->leftAvatarInsidePos(target->pixmap().size()));
				}else{
					avatarAnimation->setStartValue(this->rightAvatarOutsidePos(target->pixmap().size()));
					avatarAnimation->setEndValue(this->rightAvatarInsidePos(target->pixmap().size()));
				}
			}else{
				if(avatarAround == Rpg::AvatarAroundLeft){
					avatarAnimation->setStartValue(this->leftAvatarInsidePos(target->pixmap().size()));
					avatarAnimation->setEndValue(this->leftAvatarOutsidePos(target->pixmap().size()));
				}else{
					avatarAnimation->setStartValue(this->rightAvatarInsidePos(target->pixmap().size()));
					avatarAnimation->setEndValue(this->rightAvatarOutsidePos(target->pixmap().size()));
				}
			}
		}else{
			rDebug() << "touched!";
			rDebug() << target->pixmap().isNull();
			rDebug() << avatarMode;
		}
		return avatarAnimation;
	}

	/**
	 * @brief makeAvatarOpacityAnimation
	 * @param target
	 * @param mode
	 * @return
	 * 创建一个左/右头像的淡入淡出动画, 返回已new的一个动画指针, group运行结束之后会自动销毁
	 */
	QPropertyAnimation* makeAvatarOpacityAnimation(QGraphicsPixmapItem *target, RpgDialogAnimation::AnimationMode mode){
		QPropertyAnimation *avatarAnimation = nullptr;
		if(target == nullptr){
			rWarning() << "target is nullptr.";
			return nullptr;
		}
		if(!target->pixmap().isNull()){
			RpgItemProperties *properties = nullptr;
			if(target == this->avatarItemLeft){
				properties = this->avatarItemLeftProperties;
			}else if(target == this->avatarItemRight){
				properties = this->avatarItemRightProperties;
			}else{
				throw RpgRuntimeException("Target is not fit weather left avatar or right avatar.");
			}
			avatarAnimation = new QPropertyAnimation(properties, "opacity", this->parent);
			avatarAnimation->setDuration(this->duration);
			avatarAnimation->setEasingCurve(this->easingCurveType);
			if(mode == AnimationEnter){
				avatarAnimation->setStartValue(0.0f);
				avatarAnimation->setEndValue(1.0f);
			}else{
				avatarAnimation->setStartValue(1.0f);
				avatarAnimation->setEndValue(0.0f);
			}
		}
		return avatarAnimation;
	}

	/**
	 * @brief appendToGroup
	 * @param animation
	 * 载入至group中, 会自动排除动画返回nullptr的结果
	 */
	void appendToGroup(QAbstractAnimation *animation){
		// 确定返回的动画不是空值, 然后载入列表
		if(animation != nullptr){
			this->group->addAnimation(animation);
		}
	}

	/**
	 * @brief getState
	 * @return
	 * 返回现在的状态
	 */
	inline QAbstractAnimation::State getState() const{
		return this->group->state();
	}

	/**
	 * @brief isRunning
	 * @return
	 * 是否在运行
	 */
	inline bool isRunning() const{
		return this->group->state() == QAbstractAnimation::Running;
	}

	/**
	 * @brief start
	 * 开始动画
	 */
	inline void start(){
		if(this->group->animationCount() <= 0){
			rDebug() << "No animations found.";
			return;
		}
		if(this->isRunning()){
			this->group->pause();
			this->group->setCurrentTime(0);
		}
		this->group->start(QAbstractAnimation::KeepWhenStopped);
	}

	/**
	 * @brief stop
	 * 停止动画
	 */
	inline void stop(){
		if(!this->isRunning()){
			return;
		}
		this->group->stop();
		this->group->setCurrentTime(this->group->totalDuration());
	}

	/**
	 * @brief clear
	 * 清理动画
	 */
	inline void clear(){
		if(this->isRunning()){
			this->stop();
		}
		this->group->clear();
	}

public:
	enum Animation{
//		AnimationShowFromLeft = 0,  // 对话框进入动画, 如果左侧有设置头像, 则加上左侧进入动画
//		AnimationShowFromRight = 1, // 对话框进入动画, 如果右侧有设置头像, 则加上右侧进入动画
//		AnimationLeftToRight = 2,   // 对话框不动, 头像从左侧过渡至右侧
//		AnimationRightToLeft = 3,   // 对话框不动, 头像从右侧过渡至左侧
//		AnimationExitToRight = 4,   // 对话框退出动画, 如果右侧有设置头像, 则加上右侧退出动画
//		AnimationExitToLeft = 5,    // 对话框退出动画, 如果左侧有设置头像, 则加上左侧退出动画

		AnimationDialogShow = 1,		// 对话框进入动画
		AnimationDialogHide = 2,		// 对话框退出动画
		AnimationLeftAvatarShow = 4,	// 左侧立绘入场
		AnimationLeftAvatarHide = 8,	// 左侧立绘离场
		AnimationRightAvatarShow = 16,	// 右侧立绘入场
		AnimationRightAvatarHide = 32,	// 右侧立绘离场
	};
	Q_DECLARE_FLAGS(Animations, Animation)

	static const QPointF leftAvatarOutsidePos(const QSize &pixmapSize){
		return QPointF(-pixmapSize.width(), ScreenHeight - pixmapSize.height());
	}

	static const QPointF leftAvatarInsidePos(const QSize &pixmapSize){
		return QPointF(0, ScreenHeight - pixmapSize.height());
	}

	static const QPointF rightAvatarOutsidePos(const QSize &pixmapSize){
		return QPointF(ScreenWidth, ScreenHeight - pixmapSize.height());
	}

	static const QPointF rightAvatarInsidePos(const QSize &pixmapSize){
		return QPointF(ScreenWidth - pixmapSize.width(), ScreenHeight - pixmapSize.height());
	}

	RpgDialogAnimation(QGraphicsItem *dialog, QGraphicsPixmapItem *avatarItemLeft = nullptr, QGraphicsPixmapItem *avatarItemRight = nullptr, int duration = 300, QEasingCurve::Type easingCurveType = QEasingCurve::OutQuad, QObject *parent = nullptr): parent(parent), group(new QParallelAnimationGroup(parent)){
		this->dialog = dialog;
		this->avatarItemLeft = avatarItemLeft;
		this->avatarItemRight = avatarItemRight;
		this->duration = duration;
		this->easingCurveType = easingCurveType;

		this->dialogProperties = new RpgItemProperties(this->dialog, parent);
		if(this->avatarItemLeft != nullptr){
			this->avatarItemLeftProperties = new RpgItemProperties(this->avatarItemLeft, parent);
		}
		if(this->avatarItemRight != nullptr){
			this->avatarItemRightProperties = new RpgItemProperties(this->avatarItemRight, parent);
		}

//		QObject::connect(this->group, &QParallelAnimationGroup::finished, [this](){
//			qDebug() << "Dialog:" << this->dialog->pos() << this->dialog->boundingRect() << "Visible:" << this->dialog->isVisible() << "Opacity:" << this->dialog->opacity();
//			qDebug() << "Left:" << this->avatarItemLeft->pos() << this->avatarItemLeft->pixmap().size() << "Visible:" << this->avatarItemLeft->isVisible() << "Opacity:" << this->avatarItemLeft->opacity();
//			qDebug() << "Right:" << this->avatarItemRight->pos() << this->avatarItemRight->pixmap().size() << "Visible:" << this->avatarItemRight->isVisible() << "Opacity:" << this->avatarItemRight->opacity();
//		});
	}

	RpgDialogAnimation(QGraphicsItem *dialog, int duration, QEasingCurve::Type easingCurveType, QObject *parent): parent(parent), group(new QParallelAnimationGroup(parent)){
		this->dialog = dialog;
		this->avatarItemLeft = nullptr;
		this->avatarItemRight = nullptr;
		this->duration = duration;
		this->easingCurveType = easingCurveType;

		this->dialogProperties = new RpgItemProperties(this->dialog, parent);
	}

	~RpgDialogAnimation(){
		if(this->dialogProperties != nullptr){
			delete this->dialogProperties;
		}
		if(this->avatarItemLeftProperties != nullptr){
			delete this->avatarItemLeftProperties;
		}
		if(this->avatarItemRightProperties != nullptr){
			delete this->avatarItemRightProperties;
		}
	}

	inline void setDuration(int duration){
		this->duration = duration;
	}

	inline int getDuration() const{
		return this->duration;
	}

	inline void setEasingCurveType(QEasingCurve::Type type){
		this->easingCurveType = type;
	}

	inline QEasingCurve::Type getEasingCurveType() const{
		return this->easingCurveType;
	}

//	void runDialogAvatarAnimations(Rpg::AvatarMode mode, AnimationType type){
//		if(this->dialog == nullptr){
//			rDebug() << "Specific dialog is nullptr";
//			throw RpgNullPointerException("=>this->dialog:QGraphicsItem");
//		}
//		if(this->avatarItemLeft == nullptr){
//			rDebug() << "Specific avatarItemLeft is nullptr";
//			throw RpgNullPointerException("=>this->avatarItemLeft:QGraphicsPixmapItem");
//		}
//		if(this->avatarItemRight == nullptr){
//			rDebug() << "Specific avatarItemRight is nullptr";
//			throw RpgNullPointerException("=>this->avatarItemRight:QGraphicsPixmapItem");
//		}
//		this->clear();
//		switch(type){
//			case AnimationShowFromLeft:
//				// 对话框淡入显示, 头像框(若有, 半身), 则增加动画从左进入
//				this->appendToGroup(this->makeDialogAnimation(AnimationEnter));
//				if(this->avatarItemLeft->isVisible()){
//					// 设置了可见就表示需要进行动画, 如果不可见, 说明没有图像
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemLeft, AnimationEnter, mode, Rpg::AvatarAroundLeft));
//				}
//				break;
//			case AnimationShowFromRight:
//				// 对话框淡入显示, 头像框(若有, 半身), 则增加动画从右进入
//				this->appendToGroup(this->makeDialogAnimation(AnimationEnter));
//				if(this->avatarItemRight->isVisible()){
//					// 设置了可见就表示需要进行动画, 如果不可见, 说明没有图像
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemRight, AnimationEnter, mode, Rpg::AvatarAroundRight));
//				}
//				break;
//			case AnimationLeftToRight:
//				// 对话框不动, 头像框(必须有, 半身), 从左侧退出, 从右侧进入动画
//				if(this->avatarItemLeft->isVisible()){
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemLeft, AnimationExit, mode, Rpg::AvatarAroundLeft));
//					this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemLeft, AnimationExit));
//				}
//				if(this->avatarItemRight->isVisible()){
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemRight, AnimationEnter, mode, Rpg::AvatarAroundRight));
//					this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemRight, AnimationEnter));
//				}
//				break;
//			case AnimationRightToLeft:
//				// 对话框不动, 头像框(必须有, 半身), 从右侧退出, 从左侧进入动画
//				if(this->avatarItemLeft->isVisible()){
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemLeft, AnimationEnter, mode, Rpg::AvatarAroundLeft));
//					this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemLeft, AnimationEnter));
//				}
//				if(this->avatarItemRight->isVisible()){
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemRight, AnimationExit, mode, Rpg::AvatarAroundRight));
//					this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemRight, AnimationExit));
//				}
//				break;
//			case AnimationExitToRight:
//				// 对话框淡出隐藏, 头像框(若有, 半身), 则向右侧退出
//				this->appendToGroup(this->makeDialogAnimation(AnimationExit));
//				if(this->avatarItemRight->isVisible()){
//					// 设置了可见就表示需要进行动画, 如果不可见, 说明没有图像
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemRight, AnimationExit, mode, Rpg::AvatarAroundRight));
//				}
//				break;
//			case AnimationExitToLeft:
//				// 对话框淡出隐藏, 头像框(若有, 半身), 则向左侧退出
//				this->appendToGroup(this->makeDialogAnimation(AnimationExit));
//				if(this->avatarItemLeft->isVisible()){
//					// 设置了可见就表示需要进行动画, 如果不可见, 说明没有图像
//					this->appendToGroup(this->makeAvatarAnimation(this->avatarItemLeft, AnimationExit, mode, Rpg::AvatarAroundLeft));
//				}
//				break;
//		}
//		this->start();
//	}

	void runDialogAnimations(Animations animations){
		if(this->dialog == nullptr){
			rDebug() << "Specific dialog is nullptr";
			throw RpgNullPointerException("=>this->dialog:QGraphicsItem");
		}
		this->clear();
		if(animations.testFlag(AnimationDialogShow)){
			this->appendToGroup(this->makeDialogAnimation(AnimationEnter));
		}
		if(animations.testFlag(AnimationDialogHide)){
			if(animations.testFlag(AnimationDialogShow)){
				rDebug() << "Cannot deal both same target showing and hiding. Default run 'show' way.";
			}else{
				this->appendToGroup(this->makeDialogAnimation(AnimationExit));
			}
		}
		this->start();
	}

	void runDialogAvatarAnimations(Rpg::AvatarMode mode, Rpg::AvatarMode lastMode, Animations animations){
		if(this->dialog == nullptr){
			rDebug() << "Specific dialog is nullptr";
			throw RpgNullPointerException("=>this->dialog:QGraphicsItem");
		}
		if(this->avatarItemLeft == nullptr){
			rDebug() << "Specific avatarItemLeft is nullptr";
			throw RpgNullPointerException("=>this->avatarItemLeft:QGraphicsPixmapItem");
		}
		if(this->avatarItemRight == nullptr){
			rDebug() << "Specific avatarItemRight is nullptr";
			throw RpgNullPointerException("=>this->avatarItemRight:QGraphicsPixmapItem");
		}
		this->clear();
		if(animations.testFlag(AnimationDialogShow)){
			this->appendToGroup(this->makeDialogAnimation(AnimationEnter));
		}
		if(animations.testFlag(AnimationDialogHide)){
			if(animations.testFlag(AnimationDialogShow)){
				rDebug() << "Cannot deal both same target showing and hiding. Default run 'show' way.";
			}else{
				this->appendToGroup(this->makeDialogAnimation(AnimationExit));
			}
		}
		if(animations.testFlag(AnimationLeftAvatarShow)){
			this->appendToGroup(this->makeAvatarAnimation(this->avatarItemLeft, AnimationEnter, mode, Rpg::AvatarAroundLeft));
//			if(!animations.testFlag(AnimationDialogShow)){
				this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemLeft, AnimationEnter));
//			}
		}
		if(animations.testFlag(AnimationLeftAvatarHide)){
			if(animations.testFlag(AnimationDialogShow)){
				rDebug() << "Cannot deal both same target showing and hiding. Default run 'show' way.";
			}else{
				this->appendToGroup(this->makeAvatarAnimation(this->avatarItemLeft, AnimationExit, lastMode, Rpg::AvatarAroundLeft));
//				if(!animations.testFlag(AnimationDialogHide)){
					this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemLeft, AnimationExit));
//				}
			}
		}
		if(animations.testFlag(AnimationRightAvatarShow)){
			this->appendToGroup(this->makeAvatarAnimation(this->avatarItemRight, AnimationEnter, mode, Rpg::AvatarAroundRight));
//			if(!animations.testFlag(AnimationDialogShow)){
				this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemRight, AnimationEnter));
//			}
		}
		if(animations.testFlag(AnimationRightAvatarHide)){
			if(animations.testFlag(AnimationRightAvatarShow)){
				rDebug() << "Cannot deal both same target showing and hiding. Default run 'show' way.";
			}else{
				this->appendToGroup(this->makeAvatarAnimation(this->avatarItemRight, AnimationExit, lastMode, Rpg::AvatarAroundRight));
//				if(!animations.testFlag(AnimationDialogHide)){
					this->appendToGroup(this->makeAvatarOpacityAnimation(this->avatarItemRight, AnimationExit));
//				}
			}
		}
		this->start();
	}

	void waitAnimationFinish(){
		if(!this->isRunning()){
			return;
		}
		if(this->group->currentTime() + 2 >= this->group->totalDuration()){
			return;
		}
		QEventLoop eventLoop;
		QObject::connect(this->group, &QParallelAnimationGroup::finished, &eventLoop, &QEventLoop::quit);
		eventLoop.exec();
	}
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RpgDialogAnimation::Animations)

#endif // RPGDIALOGANIMATION_H
