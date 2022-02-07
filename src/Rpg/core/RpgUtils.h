#ifndef RPGUTILS_H
#define RPGUTILS_H

#include <QtCore>

#include <Rpg/Rpg.h>

#define EnumKeyToString(key) #key

class QGraphicsItem;
class RpgUtils
{
public:
	static void msleep(int msec);
	static const QByteArray readFile(const QString &filename, int length = 1048576);
	static const QString detectJsonValue(const QJsonValue &value);
	static const QString detectJsonValue(const QJsonDocument &value);
	static bool iskeyModifierSameToKey(Qt::KeyboardModifiers mod, Qt::Key key);
	static const QString keyModifierToString(Qt::KeyboardModifiers mod);
	static const QString keyToString(Qt::Key key);
	static const QString keysToString(Qt::Key key, Qt::KeyboardModifiers mod);

	static QPointF getBlockPos(Rpg::BlockAlign align, const QSizeF &blockSize, const QSizeF &margin, const QSizeF &parentSize);
	static QPointF getDialogPos(Rpg::BlockAlign align, const QSize &dialogSize, int marginW, int marginH);

//	static QGraphicsItem genViewGrids(qreal paddingWidth = MapTileWidth, qreal paddingHeight = MapTileHeight, qreal radiusWidth = MapTileWidth, qreal radiusHeight = MapTileHeight);
};

#endif // RPGUTILS_H
