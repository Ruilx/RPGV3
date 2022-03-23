#include "RpgAxisItem.h"

void RpgAxisItem::drawPoint(QPainter &p, const QPointF &pos, const QSizeF &size){
	if(!p.isActive()){
		return;
	}
	p.drawEllipse(QRectF(QPointF(pos.x() - size.width() / 2.0, pos.y() - size.height() / 2.0), size));
}

void RpgAxisItem::draw(QPainter &p){
	if(!p.isActive()){
		return;
	}
	const QRectF area = QRectF(this->pos(), this->size);
	const qreal width = area.width();
	const qreal height = area.height();
	p.setPen(this->auxPen1);
	p.drawLine(QLineF(width / 2.0, 0, width / 2.0, height));
	p.drawLine(QLineF(0, height / 2.0, width, height / 2.0));
	this->drawPoint(p, QPointF(width / 2.0, height / 2.0), QSizeF(5.0, 5.0));

	p.setPen(this->auxPen2);
	for(qreal i = area.center().x() - this->step.width(); i >= area.left(); i -= this->step.width()){
		this->drawPoint(p, QPointF(i, height / 2.0));
		qreal quotient = (area.center().x() - i) / grid.width();
		if(quotient - int(quotient) <= 1e-5){
			p.drawLine(QLineF(i, 0, i, height));
		}
	}
	for(qreal i = area.center().x() + this->step.width(); i <= area.width(); i += step.width()){
		this->drawPoint(p, QPointF(i, height / 2.0));
		qreal quotient = (area.center().x() - i) / grid.width();
		if(quotient - int(quotient) <= 1e-5){
			p.drawLine(QLineF(i, 0, i, height));
		}
	}
	for(qreal i = area.center().y() - this->step.height(); i >= area.top(); i -= this->step.height()){
		this->drawPoint(p, QPoint(width / 2.0, i));
		qreal quotient = (i - area.center().y()) / grid.height();
		if(quotient - int(quotient) <= 1e-5){
			p.drawLine(QLineF(0, i, width, i));
		}
	}
	for(qreal i = area.center().y() + this->step.height(); i <= area.height(); i += step.height()){
		this->drawPoint(p, QPointF(width / 2.0, i));
		qreal quotient = (i - area.center().y()) / grid.height();
		if(quotient - int(quotient) <= 1e-5){
			p.drawLine(QLineF(0, i, width, i));
		}
	}
}

QRectF RpgAxisItem::boundingRect() const{
	if(this->size.isValid()){
		return QRectF(this->pos(), this->size);
	}else{
		return QRectF(this->pos(), QSize(0, 0));
	}
}

void RpgAxisItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Unused(option);
	Unused(widget);
	if(!painter->isActive()){
		return;
	}
	this->draw(*painter);
}

RpgAxisItem::RpgAxisItem(const QRectF &area, const QSizeF &step, const QSizeF &grid, QGraphicsObject *parent): RpgObject(parent){
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
	this->size = area.size();
	this->setPos(area.topLeft());

	this->auxPen1.setWidth(1);
	this->auxPen2.setWidth(1);

	this->setZValue(Rpg::ZValueMapFront);
}

void RpgAxisItem::setStepSize(const QSizeF &step){
	if(step.isValid()){
		this->step = step;
	}
}

void RpgAxisItem::setGridSize(const QSizeF &grid){
	if(grid.isValid()){
		this->grid = grid;
	}
}
