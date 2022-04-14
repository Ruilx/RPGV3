#ifndef RPGUTILS_H
#define RPGUTILS_H

#include <QtCore>

#include <Rpg/Rpg.h>

#define EnumKeyToString(key) #key

class QGraphicsItem;
class RpgUtils {
public:
	static void msleep(int msec);
	static const QByteArray readFile(const QString &filename, int length = 1048576);
	static const QString detectJsonValue(const QJsonValue &value);
	static const QString detectJsonValue(const QJsonDocument &value);
	static bool iskeyModifierSameToKey(Qt::KeyboardModifiers mod, Qt::Key key);
	static const QString keyModifierToString(Qt::KeyboardModifiers mod);
	static const QString keyToString(Qt::Key key);
	static const QString keysToString(Qt::Key key, Qt::KeyboardModifiers mod);

	static QPointF getBlockPos(Rpg::BlockAlign align, const QSizeF &blockSize, const QSizeF &margin = QSizeF(0, 0), const QSizeF &parentSize = Rpg::ScreenSize);
	static QPointF getDialogPos(Rpg::BlockAlign align, const QSizeF &dialogSize, int marginW = 0, int marginH = 0);

};

#endif // RPGUTILS_H
