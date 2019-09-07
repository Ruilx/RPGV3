#ifndef RPGVIEW_H
#define RPGVIEW_H

#include <QtCore>
#include <QGraphicsView>

#include <QPropertyAnimation>

#include <Rpg/Global.h>
#include <Rpg/exception/RpgRuntimeException.h>

#include <QMouseEvent>

class RpgView : public QGraphicsView
{
	static RpgView *_instance;

	QPointF lastPos;
	qreal currentScale = 1.0f;

	void mousePressEvent(QMouseEvent *event){
		this->lastPos = event->pos();
	}

	void mouseMoveEvent(QMouseEvent *event){
		if(this->scene() == nullptr){
			return;
		}
		if(event->buttons() & Qt::LeftButton){
			QPointF pos = event->pos() - this->lastPos;
			this->lastPos = event->pos();
			this->setSceneRect((this->sceneRect().x() - pos.x()), (this->sceneRect().y() - pos.y()), this->sceneRect().width(), this->sceneRect().height());
		}
	}

	void wheelEvent(QWheelEvent *event){
		int delta = event->delta();
		if(delta > 0){
			if(this->currentScale < 10.0f){
				this->currentScale += 0.1f;
			}else{
				this->currentScale = 10.0f;
				event->ignore();
				return;
			}
		}else{
			if(this->currentScale > 0.1f){
				this->currentScale -= 0.1f;
			}else{
				this->currentScale = 0.1f;
				event->ignore();
				return;
			}
		}
		this->setAbslouteScale(this->currentScale, this->currentScale);
		qDebug() << CodePath << "CurrentScale:" << this->currentScale << "SCENE SCALE:" << this->transform().m11() << this->transform().m22();
	}

	void setAbslouteScale(qreal scaleX, qreal scaleY){
		QTransform transform = this->transform();
		transform.setMatrix(scaleX, transform.m12(), transform.m13(), transform.m21(), scaleY, transform.m23(), transform.m31(), transform.m32(), transform.m33());
		this->setTransform(transform);
	}
public:
	static RpgView *instance(QWidget *parent = nullptr){
		if(_instance == nullptr){
			if(parent != nullptr){
				_instance = new RpgView(parent);
			}else{
				throw RpgRuntimeException("RpgView need implemented from construct function of RpgWidget!");
			}
		}
		return _instance;
	}

private:
	RpgView(QWidget *parent = nullptr): QGraphicsView(parent){
		QPalette pal;{
			pal.setColor(QPalette::Base, Qt::black);
			pal.setColor(QPalette::Background, Qt::black);
		}
		this->setPalette(pal);
//		this->setFixedSize(ScreenWidth + 2, ScreenHeight + 2);
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
};

#endif // RPGVIEW_H
