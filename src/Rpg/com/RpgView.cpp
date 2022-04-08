#include "RpgView.h"


#include <QScrollBar>
#include <QPropertyAnimation>

#include <Rpg/Global.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgState.h>

#include <Rpg/exception/RpgRuntimeException.h>

RpgView *RpgView::_instance = nullptr;

void RpgView::mousePressEvent(QMouseEvent *event){
	if(event->buttons() & Qt::RightButton){
		this->mouseMovePressed = true;
		this->lastPos = event->pos();
		return;
	}
	QGraphicsView::mousePressEvent(event);
}

void RpgView::mouseReleaseEvent(QMouseEvent *event){
	this->mouseMovePressed = false;
	QGraphicsView::mouseReleaseEvent(event);
}

void RpgView::mouseMoveEvent(QMouseEvent *event){
	if(this->mouseMovePressed){
		QPoint pos = event->pos();
		int dx = pos.x() - this->lastPos.x();
		int dy = pos.y() - this->lastPos.y();

		this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - dx);
		this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - dy);

		//rDebug() << "HOR:" << this->horizontalScrollBar()->value() << "" << "VER:" << this->verticalScrollBar()->value();
		//rDebug() << "SceneRect:" << this->scene()->sceneRect();

		this->lastPos = pos;
	}
	QGraphicsView::mouseMoveEvent(event);
}

void RpgView::wheelEvent(QWheelEvent *event){
	int angle = event->angleDelta().y();
	qreal factor = 1.1;
	if(angle < 0){
		factor = 1 / factor;
	}
	this->scale(factor, factor);
	QGraphicsView::wheelEvent(event);
}

void RpgView::keyPressEvent(QKeyEvent *event){
	//		int key = event->key();
	//		Qt::KeyboardModifiers mod = event->modifiers();
	//rDebug() << "Received key PRESS [▼]:" << RpgUtils::keysToString((Qt::Key)key, mod);
	if(this->scene() != nullptr){
		RpgState::instance()->keyPressEvent(event, this->scene());
	}
}

void RpgView::keyReleaseEvent(QKeyEvent *event){
	//		int key = event->key();
	//		Qt::KeyboardModifiers mod = event->modifiers();
	//rDebug() << "Received key RELEASE [▲]:" << RpgUtils::keysToString((Qt::Key)key, mod);
	if(this->scene() != nullptr){
		RpgState::instance()->keyReleaseEvent(event, this->scene());
	}
}

RpgView *RpgView::instance(QWidget *parent){
	if(_instance == nullptr){
		if(parent != nullptr){
			_instance = new RpgView(parent);
		}else{
			throw RpgRuntimeException("RpgView need implemented from construct function of RpgWidget!");
		}
	}
	return _instance;
}

RpgView::RpgView(QWidget *parent): QGraphicsView(parent){
	QPalette pal;{
		pal.setColor(QPalette::Base, Qt::black);
#if QT_VERSION_MAJOR < 6
		pal.setColor(QPalette::Background, Qt::black);
#endif
	}
	this->setPalette(pal);
	//		this->setFixedSize(ScreenWidth + 2, ScreenHeight + 2);
	//		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	this->setRenderHint(QPainter::TextAntialiasing);

	this->setDragMode(QGraphicsView::RubberBandDrag);
	this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

	this->setInteractive(false);
}
