
#ifndef RPGVIEW_H
#define RPGVIEW_H

#include <QtCore>
#include <QGraphicsView>
#include <QScrollBar>

#include <QPropertyAnimation>

#include <Rpg/Global.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgState.h>
#include <Rpg/exception/RpgRuntimeException.h>

#include <QMouseEvent>

class RpgView : public QGraphicsView
{
	static RpgView *_instance;

	QPointF lastPos;
	qreal currentScale = 1.0f;
	bool mouseMovePressed = false;

//	void resizeEvent(QResizeEvent *event){
//		if(!event->oldSize().isValid()){
//			event->ignore();
//			return;
//		}
//		qDebug() << CodePath << "Resize:" << event->oldSize() << "==>" << event->size();
//		QSizeF size = (event->size() - event->oldSize()) / 2.0;
//		this->setDelta(-QPointF(size.width(), size.height()));
//		qDebug() << CodePath << "SceneRect:" << this->sceneRect();
//		event->accept();
//	}

//	void resizeEvent(QResizeEvent *event){
//		if(!event->oldSize().isValid()){
//			event->ignore();
//			return;
//		}
//		int edgeOld = qMin(event->oldSize().width(), event->oldSize().height());
//		int edge = qMin(event->size().width(), event->size().height());
//		QRectF fitInViewSenceRect;
//		if(edge < edgeOld){
//			qDebug().nospace().noquote() << "Calling mapToScene(0, 0, " << event->oldSize().width() << ", " << event->oldSize().height() << ") when edge < edgeOld";
//			fitInViewSenceRect = this->mapToScene(0, 0, event->oldSize().width(), event->oldSize().height()).boundingRect();
//			qDebug() << "MaptoScene when edge < edgeOld:" << fitInViewSenceRect;
//			this->fitInView(fitInViewSenceRect, Qt::KeepAspectRatioByExpanding);
//		}else{
//			qDebug().nospace().noquote() << "Calling mapToScene(0, 0, " << event->oldSize().width() << ", " << event->oldSize().height() << ") when else";
//			fitInViewSenceRect = this->mapToScene(0, 0, event->oldSize().width(), event->oldSize().height()).boundingRect();
//			qDebug() << "MaptoScene when else:" << fitInViewSenceRect;
//			this->fitInView(fitInViewSenceRect, Qt::KeepAspectRatio);
//		}

//		event->accept();
//	}

	void mousePressEvent(QMouseEvent *event){
		if(event->buttons() & Qt::RightButton){
			this->mouseMovePressed = true;
			this->lastPos = event->pos();
			event->accept();
		}
	}

	void mouseReleaseEvent(QMouseEvent *event){
		Q_UNUSED(event)
		this->mouseMovePressed = false;
	}

//	void mouseMoveEvent(QMouseEvent *event){
//		if(this->scene() == nullptr){
//			qDebug() << CodePath << "view has no scene or no valid scene.";
//			event->ignore();
//			return;
//		}
//		if(event->buttons() & Qt::RightButton){
//			QPointF pos = event->pos() - this->lastPos;
//			//qDebug() << CodePath << "Current mouse pos:" << pos;
//			this->lastPos = event->pos();
//			this->setDelta(-pos / this->currentScale);
//			event->accept();
//			return;
//		}else{
//			event->ignore();
//			return;
//		}
//	}

	void mouseMoveEvent(QMouseEvent *event){
		if(this->mouseMovePressed){
			QPoint pos = event->pos();
			int dx = pos.x() - this->lastPos.x();
			int dy = pos.y() - this->lastPos.y();

			this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - dx);
			this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - dy);

			qDebug() << "HOR:" << this->horizontalScrollBar()->value() << "" << "VER:" << this->verticalScrollBar()->value();
			qDebug() << "SceneRect:" << this->scene()->sceneRect();

			this->lastPos = pos;
		}
	}

//	void wheelEvent(QWheelEvent *event){
//		int delta = event->delta();
//		if(delta > 0){
//			if(this->currentScale < 10.0f){
//				this->currentScale += 0.1f;
//			}else{
//				this->currentScale = 10.0f;
//				event->ignore();
//				return;
//			}
//		}else{
//			if(this->currentScale > 0.1f){
//				this->currentScale -= 0.1f;
//			}else{
//				this->currentScale = 0.1f;
//				event->ignore();
//				return;
//			}
//		}
//		this->setAbslouteScale(this->currentScale, this->currentScale);
//		qDebug() << CodePath << "CurrentScale:" << this->currentScale << "SCENE SCALE:" << this->transform().m11() << this->transform().m22();
//	}

	void wheelEvent(QWheelEvent *event){
		int angle = event->angleDelta().y();
		qreal factor = 1.1;
		if(angle < 0){
			factor = 1 / factor;
		}
		this->scale(factor, factor);
	}

//	void setAbslouteScale(qreal scaleX, qreal scaleY){
//		QTransform transform = this->transform();
//		transform.setMatrix(scaleX, transform.m12(), transform.m13(), transform.m21(), scaleY, transform.m23(), transform.m31(), transform.m32(), transform.m33());
//		this->setTransform(transform);
//	}

//	void setDelta(const QPointF &delta){
//		QRectF rect = this->sceneRect();
//		this->scene()->setSceneRect(QRectF(rect.topLeft() + delta.toPoint(), this->size()));
//	}

//	void setDelta(qreal dx, qreal dy){
//		this->setDelta(QPointF(dx, dy));
//	}

	void keyPressEvent(QKeyEvent *event){
//		int key = event->key();
//		Qt::KeyboardModifiers mod = event->modifiers();
		//qDebug() << CodePath << "Received key PRESS [▼]:" << RpgUtils::keysToString((Qt::Key)key, mod);
		if(this->scene() != nullptr){
			RpgState::instance()->keyPressEvent(event, this->scene());
			event->accept();
		}
	}

	void keyReleaseEvent(QKeyEvent *event){
//		int key = event->key();
//		Qt::KeyboardModifiers mod = event->modifiers();
		//qDebug() << CodePath << "Received key RELEASE [▲]:" << RpgUtils::keysToString((Qt::Key)key, mod);
		if(this->scene() != nullptr){
			RpgState::instance()->keyReleaseEvent(event, this->scene());
			event->accept();
		}
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
};

#endif // RPGVIEW_H
