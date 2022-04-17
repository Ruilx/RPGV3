#ifndef RPGTILESETBASE_H
#define RPGTILESETBASE_H

#include <Rpg/Rpg.h>

#include <QPixmap>

inline uint qHash(const QPoint &p, uint seed)
Q_DECL_NOEXCEPT_EXPR(noexcept(qHash(p.x(), seed) && noexcept(qHash(p.y(), seed)))) {
	uint h1 = qHash(p.x(), seed);
	uint h2 = qHash(p.y(), seed);
	return ((h1 << 16) | (h1 >> 16)) ^ h2 ^ seed;
}

class RpgTileSetBase
{
	QString name;
	QPixmap oriTile;

	QSize tileSize;
	QHash<QPoint, QList<QPixmap> > tiles;

	const QPixmap nullPixmap;

	void readOriTile();

protected:
	virtual void renderTile(const QPoint &loc) Pure;

public:
	RpgTileSetBase(const QString &name);

	const QPixmap &getTilePixmap(const QPoint &loc, int index);
};

#endif // RPGTILESETBASE_H
