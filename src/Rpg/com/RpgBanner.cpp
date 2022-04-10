#include "RpgBanner.h"

#include <Rpg/core/RpgState.h>
#include <Rpg/com/RpgView.h>

#include <Rpg/exception/RpgNullPointerException.h>

void RpgBanner::timerEvent(QTimerEvent *event){

}

void RpgBanner::setup(){
	this->items.clear();

	this->enterAnimation->setTargetObject(this);
	this->enterAnimation->setPropertyName("opacity");
	this->enterAnimation->setDuration(300);
	this->enterAnimation->setEasingCurve(QEasingCurve::OutQuad);
	this->enterAnimation->setStartValue(0.0f);
	this->enterAnimation->setEndValue(1.0f);

	this->exitAnimation->setTargetObject(this);
	this->exitAnimation->setPropertyName("opacity");
	this->exitAnimation->setDuration(300);
	this->exitAnimation->setEasingCurve(QEasingCurve::OutQuad);
	this->exitAnimation->setStartValue(1.0f);
	this->exitAnimation->setEndValue(0.0f);

	this->connect(this->timeLine, &QTimeLine::frameChanged, [this](int frame){
		if(*this->frameCbI == nullptr){
			rError() << "Map frameCbs' iterator is nullptr.";
			throw RpgNullPointerException("RpgBanner=>frameCbs::ConstIterator");
		}
		// TODO: continue...
	});

	this->hide();

	rpgState->registerRpgObject(this, RpgState::AutoMode);
}

void RpgBanner::setTimeLineProps(int durationMs, int fps){
	if(this->timeLine == nullptr){
		rError() << "timeLine is nullptr.";
		throw RpgNullPointerException("=> RpgBanner::timeline");
	}
	this->timeLine->setDuration(durationMs);
	this->timeLine->setFrameRange(0, qCeil((qreal)durationMs * fps / 1000));
	this->timeLine->setUpdateInterval(qFloor(1000 / (qreal)fps));
}

RpgBanner::RpgBanner(int durationMs, int fps, QGraphicsObject *parentObject): RpgObject(parentObject){
	if(this->timeLine == nullptr){
		this->timeLine = new QTimeLine(durationMs, this);
		this->setTimeLineProps(durationMs, fps);
	}
	this->setup();
}

//RpgBanner::RpgBanner(int duration, QGraphicsObject *parentObject): RpgObject(parentObject){
//	if(this->timeLine == nullptr){
//		this->timeLine = new QTimeLine(duration, this);
//	}
//	this->setup();
//}

RpgBanner::~RpgBanner(){
	rpgState->unregisterRpgObject(this, RpgState::AutoMode);
}

void RpgBanner::setTimeLineFrameCb(int frame, RpgBanner::frameCb cb){
	if(this->frameCbs.contains(frame)){
		rWarning() << "Frame already has callback function, will update 'No." << frame << "' callback function.";
		this->removeTimeLineFrameCb(frame);
	}
	this->frameCbs.insert(frame, cb);
}

int RpgBanner::setTimeLineTimestampCb(int timestampMs, RpgBanner::frameCb cb){
	int frame = this->timeLine->frameForTime(timestampMs);
	this->setTimeLineFrameCb(frame, cb);
	return frame;
}

void RpgBanner::removeTimeLineFrameCb(int frame){
	if(!this->frameCbs.contains(frame)){
		rWarning() << "Frame '" << frame << "' not found in frameCbs.";
		return;
	}
	this->frameCbs.remove(frame);
}

void RpgBanner::removeItem(const QString &name){
	if(!this->items.contains(name)){
		rDebug() << "Given name: '" << name << "' is not in items.";
		return;
	}
	QGraphicsItem *item = this->items.value(name);
	if(item == nullptr){
		rDebug() << "item name: '" << name << "' in items is nullptr already.";
		this->items.remove(name);
	}else if(item->scene() == nullptr){
		// 这个item没有绑定到任何一个scene
		delete item;
		rDebug() << "item: '" << item << "' deleted.";
		this->items.remove(name);
	}else{
		this->items.remove(name);
	}
}

void RpgBanner::addItem(const QString &name, QGraphicsItem *item){
	if(item == nullptr){
		rDebug() << "Giving item obj is nullptr.";
		throw RpgNullPointerException("item", item);
	}
	if(this->items.contains(name)){
		QGraphicsItem *itemInHash = this->items.value(name);
		if(itemInHash == item){
			rDebug() << "Item already in items: '" << name << "' with value: '" << itemInHash << "'.";
			return;
		}else{
			rDebug() << "Items will update '" << name << "' new item '" << item << "' to replace old item: '" << itemInHash << "'.";
			this->removeItem(name);
			item->setParentItem(this);
			this->items.insert(name, item);
		}
	}else{
		this->setParentItem(this);
		this->items.insert(name, item);
	}
}

void RpgBanner::run(){
	RpgObject::run();
	emit this->enterAutoMode();

	// move to scene
	if(rpgView->scene() == nullptr){
		rError() << "RpgView not loaded any scene yet.";
		this->end();
		throw RpgNullPointerException("rpgView->scene()");
	}else{
		// 如果该对象已装载至其他的scene的时候, 这个函数能自动将这个对象在其他scene删除后添加至该scene
		rpgView->scene()->addItem(this);
	}

	this->frameCbI = this->frameCbs.constBegin();

	this->showBanner();
}

int RpgBanner::waitForComplete(){
	if(!this->isRunning()){
		rError() << "RpgBanner not running.";
		return -1;
	}
}

void RpgBanner::end(){

}

void RpgBanner::showBanner(){
	this->show();
	rpgState->pushState(RpgState::AutoMode);
	if(this->timeLine->state() != QTimeLine::NotRunning){
		rWarning() << "RpgBanner timeline is running, will restart timeline";
	}
	this->timeLine->start();
}
