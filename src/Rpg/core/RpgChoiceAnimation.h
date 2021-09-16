#ifndef RPGCHOICEANIMATION_H
#define RPGCHOICEANIMATION_H

#include <Rpg/Rpg.h>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

#include <Rpg/core/RpgItemProperties.h>

#include <QPropertyAnimation>

#include <Rpg/exception/RpgNullPointerException.h>
#include <Rpg/exception/RpgRuntimeException.h>

class RpgChoiceAnimation
{
	int duration = 300;
	QEasingCurve::Type easingCurveType = QEasingCurve::OutQuad;

	QGraphicsItem *dialog = nullptr;

	RpgItemProperties *dialogProperties = nullptr;

	QParallelAnimationGroup *group = nullptr;

	enum AnimationMode{
		AnimationEnter = 0,
		AnimationExit = 1,
	};

	QPropertyAnimation* makeDialogAnimation(RpgChoiceAnimation::AnimationMode mode){
		if(this->dialog == nullptr){
			qDebug() << CodePath << "=>this->dialog is nullptr.";
			return nullptr;
		}
		QPropertyAnimation *dialogAnimation = new QPropertyAnimation(this->dialogProperties, "opacity");
		dialogAnimation->setDuration(this->duration);
		dialogAnimation->setEasingCurve(this->easingCurveType);
		if(mode == AnimationEnter){
			dialogAnimation->setStartValue(0.0);
			dialogAnimation->setEndValue(1.0);
		}else{
			dialogAnimation->setStartValue(1.0);
			dialogAnimation->setEndValue(0.0);
		}
		return dialogAnimation;
	}

public:
	RpgChoiceAnimation();
};

#endif // RPGCHOICEANIMATION_H
