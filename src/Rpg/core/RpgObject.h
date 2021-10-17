#ifndef RPGOBJECT_H
#define RPGOBJECT_H

#include <QtCore>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPainter>
#include <Rpg/Global.h>

#include <Rpg/core/RpgUtils.h>

#include <Rpg/exception/RpgNullPointerException.h>


class RpgObject : public QGraphicsObject
{
	Q_OBJECT

	bool running = false;

protected:
	virtual QRectF boundingRect() const{
		return QRectF();
	}

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
		Unused(painter);
		Unused(option);
		Unused(widget);
		return;
	}

	virtual bool event(QEvent *event) override{
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

public:
	RpgObject(QGraphicsObject *parentItem): QGraphicsObject(parentItem){

	}

	inline void setProcessing(bool isRunning){
		this->running = isRunning;
	}

	inline bool getProcessing() const{
		return this->running;
	}

	inline bool isRunning() const{
		return this->getProcessing();
	}

	inline void end(){
		if(this->running != true){
			qDebug() << CodePath << "RPG Module is not running, cannot stop.";
			return;
		}
		this->setProcessing(false);
	}

	virtual void run(){
		if(this->running == true){
			qDebug() << CodePath << "RPG Module is running, please don't call it repeatly.";
			return;
		}
		this->setPos(0.0f, 0.0f);
		this->setProcessing(true);
	}

	// 这两个函数已被Qt的KeyEvent代替, 请使用keyPressEvent和keyReleaseEvent代替(QObject已继承)
//	virtual void receiveKeyPress(int key, Qt::KeyboardModifiers mod){
//		if(!this->running){
//			return;
//		}
//		qDebug() << CodePath << tr("Receive Key Press ↓: ") << RpgUtils::keysToString((Qt::Key)key, mod);
//	}

//	virtual void receiveKeyRelease(int key, Qt::KeyboardModifiers mod){
//		if(!this->running){
//			return;
//		}
//		qDebug() << CodePath << tr("Receive Key Release ↑: ") << RpgUtils::keysToString((Qt::Key)key, mod);
//	}
};

#endif // RPGOBJECT_H
