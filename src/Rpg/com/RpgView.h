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

	void resizeEvent(QResizeEvent *event){
		if(!event->oldSize().isValid()){
			event->ignore();
			return;
		}
		qDebug() << CodePath << "Resize:" << event->oldSize() << "==>" << event->size();
		QSizeF size = (event->size() - event->oldSize()) / 2.0;
		this->setDelta(-QPointF(size.width(), size.height()));
		qDebug() << CodePath << "SceneRect:" << this->sceneRect();
		event->accept();
	}

	void mousePressEvent(QMouseEvent *event){
		if(event->buttons() & Qt::RightButton){
			this->lastPos = event->pos();
		}
	}

//	void mouseMoveEvent(QMouseEvent *event){
//		if(this->scene() == nullptr){
//			return;
//		}
//		if(event->buttons() & Qt::LeftButton){
//			QPointF pos = (event->pos() - this->lastPos) * this->currentScale;
//			this->lastPos = event->pos();
////			qDebug() << CodePath << QString("ScreenRect(%1, %2, %3, %4)")
////						.arg(this->sceneRect().x() - pos.x())
////						.arg(this->sceneRect().y() - pos.y())
////						.arg(this->sceneRect().width())
////						.arg(this->sceneRect().height());
////			this->setSceneRect((this->sceneRect().x() - pos.x()), (this->sceneRect().y() - pos.y()), this->sceneRect().width(), this->sceneRect().height());
////			qDebug() << CodePath << "NOW ScreneRect" << this->sceneRect();
//			this->centerOn(this->sceneRect().x() - pos.x() + (this->sceneRect().width() / 2), this->sceneRect().y() - pos.y() + (this->sceneRect().height() / 2));
//		}
//	}

	void mouseMoveEvent(QMouseEvent *event){
		if(this->scene() == nullptr){
			qDebug() << CodePath << "view has no scene or no valid scene.";
			event->ignore();
			return;
		}
		if(event->buttons() & Qt::RightButton){
			QPointF pos = event->pos() - this->lastPos;
			//qDebug() << CodePath << "Current mouse pos:" << pos;
			this->lastPos = event->pos();
			this->setDelta(-pos / this->currentScale);
			event->accept();
			return;
		}else{
			event->ignore();
			return;
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

	void setDelta(const QPointF &delta){
		QRectF rect = this->sceneRect();
		this->scene()->setSceneRect(QRectF(rect.topLeft() + delta.toPoint(), this->size()));
	}

	void setDelta(qreal dx, qreal dy){
		this->setDelta(QPointF(dx, dy));
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
