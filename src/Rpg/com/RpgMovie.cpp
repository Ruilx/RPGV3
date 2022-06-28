#include "RpgMovie.h"

#include <Rpg/core/RpgState.h>

void RpgMovie::keyReleaseEvent(QKeyEvent *event)
{

}

void RpgMovie::setup(){
	this->enterAnimation->setTargetObject(this);
	this->enterAnimation->setPropertyName("opacity");
	this->enterAnimation->setDuration(300);
	this->enterAnimation->setEasingCurve(QEasingCurve::OutQuad);
	this->enterAnimation->setStartValue(0);
	this->enterAnimation->setEndValue(1);

	this->exitAnimation->setTargetObject(this);
	this->exitAnimation->setPropertyName("opacity");
	this->exitAnimation->setDuration(300);
	this->exitAnimation->setEasingCurve(QEasingCurve::OutQuad);
	this->exitAnimation->setStartValue(1);
	this->exitAnimation->setEndValue(0);

	this->setZValue(Rpg::ZValueFront);

	this->hide();

	rpgState->registerRpgObject(this, RpgState::AutoMode);
}

RpgMovie::~RpgMovie()
{

}


