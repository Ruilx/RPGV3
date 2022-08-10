#include "RpgMovie.h"

#include <Rpg/core/RpgState.h>
#include <Rpg/com/RpgView.h>

void RpgMovie::keyReleaseEvent(QKeyEvent *event){
	if(event->isAutoRepeat()){
		return;
	}
	if(!this->isRunning()){
		rWarning() << "RpgMovie not running.";
		return;
	}
	if(!this->musicInstance->isRunning()){
		rDebug() << "music is stopped, this key ignored.";
		return;
	}
	if(event->modifiers() != Qt::NoModifier){
		return;
	}
	int key = event->key();
	if(key == Qt::Key_Return || key == Qt::Key_Space){
		this->hideMovie();
	}
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

	this->movie->setPos(rpgView->mapToScene(0, 0));
	this->movie->resizeRenderer(Rpg::ScreenSize.toSize());

	this->setZValue(Rpg::ZValueFront);

	this->hide();

	rpgState->registerRpgObject(this, RpgState::AutoMode);
}

RpgMovie::~RpgMovie(){

}

void RpgMovie::run(){
	RpgObject::run();

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

	// show widget and play
	this->showMovie();
}

void RpgMovie::end(){
	this->hide();
	emit this->exitAutoMode();
	RpgObject::end();
}

void RpgMovie::showMovie(){
	rpgState->pushState(RpgState::AutoMode);
	emit this->enterAutoMode();
	this->show();
	QEventLoop eventLoop;
	if(this->enterAnimation->state() == QPropertyAnimation::Stopped){
		this->enterAnimation->start();
	}
	if(this->enterAnimation->state() == QPropertyAnimation::Running){
		eventLoop.exec();
	}
	this->musicInstance->playMusic(this->movieName);
}

void RpgMovie::hideMovie(){
	if(this->musicInstance->isRunning()){
		this->musicInstance->stopMusic();
	}
	QEventLoop eventLoop;
	QObject::connect(this->exitAnimation, &QPropertyAnimation::finished, &eventLoop, &QEventLoop::quit);
	if(this->exitAnimation->state() == QPropertyAnimation::Stopped){
		this->exitAnimation->start();
	}
	if(this->exitAnimation->state() == QPropertyAnimation::Running){
		eventLoop.exec();
	}

	if(rpgState->getTop() == RpgState::AutoMode){
		rpgState->popState();
	}else{
		rDebug() << "RpgState stack top is not AutoMode.";
	}
	this->end();
}


