#ifndef RPGAXISITEM_H
#define RPGAXISITEM_H

#include <Rpg/Global.h>
#include <Rpg/core/RpgObject.h>

/**
 * @brief The RpgAxisItem class
 * Rpg 正交坐标item对象 类
 * 在窗口中绘图一套带网格的直角坐标系, 以便更好的调试使用
 */
class RpgAxisItem : public RpgObject
{
	QSizeF size;

	QSizeF step = QSizeF(MapTileWidth, MapTileHeight);
	QSizeF grid = QSizeF(MapTileWidth * 2, MapTileHeight * 2);

	QPen auxPen1 = QPen(Qt::lightGray);
	QPen auxPen2 = QPen(Qt::darkGray);

	void drawPoint(QPainter &p, const QPointF &pos, const QSizeF &size = QSize(2, 2));
	void draw(QPainter &p);

protected:
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
public:
	RpgAxisItem(const QRectF &area, const QSizeF &step = QSize(), const QSizeF &grid = QSize(), QGraphicsObject *parent = nullptr);
	inline void setStepSize(const QSizeF &step);
	inline void setGridSize(const QSizeF &grid);
};

#endif // RPGAXISITEM_H
