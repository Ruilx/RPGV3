#ifndef RPGAXISITEM_H
#define RPGAXISITEM_H

#include <Rpg/Global.h>
#include <Rpg/core/RpgObject.h>

class RpgAxisItem : public RpgObject
{
	QPixmap axisPixmap;

	QSize step = QSize(MapTileWidth, MapTileHeight);
	QSize grid = QSize(MapTileWidth * 2, MapTileHeight * 2);
	bool redrawFlag = true;

	const QPen auxPen1 = QPen(Qt::lightGray);
	const QPen auxPen2 = QPen(Qt::darkGray);

	void drawPoint(QPainter &p, const QPointF &pos, const QSize &size = QSize(2, 2)){
		if(!p.isActive()){
			return;
		}
		p.drawEllipse(QRectF(QPointF(pos.x() - size.width() / 2.0, pos.y() - size.height() / 2.0), size));
	}

	void draw(){
		rDebug() << "Redraw!";
		QPainter p(&this->axisPixmap);
		const QRectF area = QRectF(this->pos(), this->axisPixmap.size());
		const qreal width = area.width();
		const qreal height = area.height();
		p.setPen(this->auxPen1);
		p.drawLine(QLineF(width / 2.0, 0, width / 2.0, height));
		p.drawLine(QLineF(0, height / 2.0, width, height / 2.0));

		for(qreal i = area.center().x(); i > area.left(); i -= this->step.width()){
			this->drawPoint(p, QPointF(i, height / 2.0));
			if(int(area.center().x() - i) % grid.width() == 0){
				p.drawLine(QLineF(i, 0, i, height));
			}
		}
		for(qreal i = area.center().x() + this->step.width(); i < area.width(); i += step.width()){
			this->drawPoint(p, QPointF(i, height / 2.0));
			if(int(area.center().x() - i) % grid.width() == 0){
				p.drawLine(QLineF(i, 0, i, height));
			}
		}
		for(qreal i = area.center().y(); i > area.top(); i -= this->step.height()){
			this->drawPoint(p, QPoint(width / 2.0, i));
			if(int(i - area.center().y()) % grid.height() == 0){
				p.drawLine(QLineF(0, i, width, i));
			}
		}
		for(qreal i = area.center().y() + this->step.height(); i < area.height(); i += step.height()){
			this->drawPoint(p, QPointF(width / 2.0, i));
			if(int(i - area.center().y()) % grid.height() == 0){
				p.drawLine(QLineF(0, i, width, i));
			}
		}
		p.end();
		if(this->redrawFlag){
			this->redrawFlag = false;
		}
	}

protected:
	QRectF boundingRect() const Override {
		return QRectF(this->pos(), this->axisPixmap.size());
	}

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) Override{
		Unused(option);
		Unused(widget);
		if(!painter->isActive()){
			return;
		}
		if(this->redrawFlag){
			this->draw();
		}
		painter->drawPixmap(0, 0, this->axisPixmap);
	}
public:
	RpgAxisItem(const QRectF &area, const QSize &step = QSize(), const QSize &grid = QSize(), QGraphicsObject *parent = nullptr): RpgObject(parent){
		if(step.isValid()){
			this->step = step;
		}
		if(grid.isValid()){
			this->grid = grid;
		}
		if(!area.isValid()){
			rWarning() << "rect area is not valid.";
			return;
		}
		this->axisPixmap = QPixmap(area.size().toSize());
		this->axisPixmap.fill(Qt::transparent);
		this->setPos(area.topLeft());
	}

	void setStepSize(const QSize &step){
		if(step.isValid()){
			this->step = step;
			this->redrawFlag = true;
		}
	}

	void setGridSize(const QSize &grid){
		if(grid.isValid()){
			this->grid = grid;
			this->redrawFlag = true;
		}
	}
};

#endif // RPGAXISITEM_H
