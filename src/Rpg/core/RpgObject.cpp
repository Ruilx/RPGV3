#include "RpgObject.h"

QRectF RpgObject::boundingRect() const{
	return QRectF();
}

void RpgObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Unused(painter);
	Unused(option);
	Unused(widget);
	return;
}

bool RpgObject::event(QEvent *event){
	switch(event->type()){
		case QEvent::KeyPress:
			this->keyPressEvent(reinterpret_cast<QKeyEvent*>(event));
			break;
		case QEvent::KeyRelease:
			this->keyReleaseEvent(reinterpret_cast<QKeyEvent*>(event));
			break;
		default:
			return QGraphicsObject::event(event);
	}
	return event->isAccepted();
}

void RpgObject::end(){
	if(!this->isRunning()){
		rDebug() << "RPG Module is not running, cannot stop.";
		return;
	}
	this->setRunning(false);
}

void RpgObject::run(){
	if(this->isRunning()){
		qDebug() << CodePath << "RPG Module is running, please don't call it repeatly.";
		return;
	}
	this->setPos(0.0f, 0.0f);
	this->setRunning(true);
}
