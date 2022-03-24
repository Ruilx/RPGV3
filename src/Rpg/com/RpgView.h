
#ifndef RPGVIEW_H
#define RPGVIEW_H

#include <QtCore>
#include <QGraphicsView>

#include <QMouseEvent>

class RpgView : public QGraphicsView
{
	static RpgView *_instance;

	QPointF lastPos;
	qreal currentScale = 1.0f;
	bool mouseMovePressed = false;

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	void wheelEvent(QWheelEvent *event);

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
public:
	static RpgView *instance(QWidget *parent = nullptr);

private:
	RpgView(QWidget *parent = nullptr);
};

#ifndef rpgView
#	define rpgView (RpgView::instance())
#endif

#endif // RPGVIEW_H
