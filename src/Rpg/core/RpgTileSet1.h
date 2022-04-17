#ifndef RPGTILESET1_H
#define RPGTILESET1_H

#include <QtCore>
#include <QImage>
#include <QPixmap>
#include <Rpg/Global.h>

#include <Rpg/exception/RpgNullPointerException.h>
#include <Rpg/exception/RpgMapKeyNotFoundException.h>

class RpgTileSetBase1
{
	int rows = 0;
	int cols = 0;
protected:
	QMap<quint64, QImage*> imageList;

	void readHandle(const QString &filename);
	inline quint64 loc2Index(const QPoint &loc) const{ return (quint64)(loc.x()) << 32 | (quint64)(loc.y()); }
public:
	RpgTileSetBase1(const QString &filename = QString());

	QImage *getRpgTile(int x, int y) const{ return this->getRpgTile(QPoint(x, y)); }
	QImage *getRpgTile(const QPoint &loc) const;

	QPixmap getRpgTilePixmap(int x, int y) const;
	QPixmap getRpgTilePixmap(const QPoint &loc) const;

	inline int getRows() const{ return this->rows; }
	inline int getCols() const{ return this->cols; }
};

#endif // RPGTILESET1_H
