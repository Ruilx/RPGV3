#ifndef RPGITEMPROPERTIES_H
#define RPGITEMPROPERTIES_H

#include <QObject>
#include <QGraphicsItem>

#include <Rpg/Global.h>

#include <Rpg/Rpg.h>
#include <Rpg/exception/RpgNullPointerException.h>

class RpgItemProperties : public QObject
{
	Q_OBJECT
	QGraphicsItem *target = nullptr;

	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
	Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
public:
	explicit RpgItemProperties(QGraphicsItem *target, QObject *parent = nullptr): QObject(parent){
		if(target == nullptr){
			throw RpgNullPointerException("target");
		}
		this->target = target;
	}

	inline qreal opacity() const{
		return this->target->opacity();
	}

	inline void setOpacity(qreal opacity){
		this->target->setOpacity(opacity);
	}

	inline QPointF pos() const{
		return this->target->pos();
	}

	inline void setPos(const QPointF &pos){
		this->target->setPos(pos);
	}

signals:
	void opacityChanged(qreal opacity);
	void posChanged(const QPointF &pos);

public slots:
};

#endif // RPGITEMPROPERTIES_H
