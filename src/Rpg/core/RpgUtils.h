#ifndef RPGUTILS_H
#define RPGUTILS_H

#include <QtCore>

#include <Rpg/Rpg.h>

#define EnumKeyToString(key) #key

class QGraphicsItem;

class RpgUtils {
public:
	typedef std::function<void()> waitStartCb;

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

	template <typename T>
	static QString toString(const T &from, const T &to, bool includeFrom = true, bool includeTo = true);
	static QString toString(const QSize &size);
	static QString toString(const QSizeF &sizef, int prec = 6);
	static QString toString(const QPoint &point);
	static QString toString(const QPointF &pointf, int prec = 6);

	template <typename Func>
	static void waitFor(QObject *object, Func signal, waitStartCb cb, int timeoutMs = -1);
};

#endif // RPGUTILS_H
