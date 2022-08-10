#include "RpgMovie.h"

#include <Rpg/core/RpgState.h>
#include <Rpg/com/RpgView.h>

void RpgMovie::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
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

RpgMovie::~RpgMovie(){

}

void RpgMovie::run(){
    RpgObject::run();
    rpgState->pushState(RpgState::AutoMode);
    emit this->enterAutoMode();

    // reset the scene for item
    if(rpgView->scene() == nullptr){
        rError() << "RpgView not loaded scene yet";
        this->end();
        throw RpgNullPointerException("RpgView::instance()->scene()");
    }else{
        rpgView->scene()->addItem(this);
    }

    if(this->musicInstance == nullptr){
        throw RpgNullPointerException("RpgMusic::instance()");
    }

    // stop music
    this->musicInstance->stopMusic();

    // set renderer
    this->musicInstance->setRenderer(this->movie);

    // show widget
    this->showMovie();

    // play movie
    this->musicInstance->playMusic();

}

void RpgMovie::end(){


    emit this->exitAutoMode();
    RpgObject::end();
}

void RpgMovie::showMovie(){
    this->show();
    this->enterAnimation->start();
}


