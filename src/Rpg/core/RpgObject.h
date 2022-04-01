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
	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual bool event(QEvent *event) override;

public:
	RpgObject(QGraphicsObject *parentItem): QGraphicsObject(parentItem){}

	inline void setRunning(bool isRunning){ this->running = isRunning; }
	inline bool getRunning() const{ return this->running; }
	inline bool isRunning() const{ return this->getRunning(); }

	virtual void end();
	virtual void run();

};

#endif // RPGOBJECT_H
