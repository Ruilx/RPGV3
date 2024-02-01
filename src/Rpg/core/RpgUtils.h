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

	/**
	 * @brief RpgUtils::optmize2dCoordGaps
	 * @param ori
	 * @return
	 *
	 * 这个函数用来将散列的二维坐标中间的空白没用的位置删除掉, 并保证每个坐标的相对位置不变, 以此来减少存储容量
	 * pair = ((1, 1), (5, 20), (3, 15), (4, 7), (2, 5), (6, 18), (1, 15), (3, 18))
	 * map =
	 *     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * 0 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * 1 |  | X|  |  |  |  |  |  |  |  |  |  |  |  |  | X|  |  |  |  |  |
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * 2 |  |  |  |  |  | X|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * 3 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | X|  |  | X|  |  |
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * 4 |  |  |  |  |  |  |  | X|  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * 5 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | X|
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * 6 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | X|  |  |
	 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 * after:
	 *     0  1  2  3  4  5
	 *   +--+--+--+--+--+--+
	 * 0 | X|  |  | X|  |  |
	 *   +--+--+--+--+--+--+
	 * 1 |  | X|  |  |  |  |
	 *   +--+--+--+--+--+--+
	 * 2 |  |  |  | X| X|  |
	 *   +--+--+--+--+--+--+
	 * 3 |  |  | X|  |  |  |
	 *   +--+--+--+--+--+--+
	 * 4 |  |  |  |  |  | X|
	 *   +--+--+--+--+--+--+
	 * 5 |  |  |  |  | X|  |
	 *   +--+--+--+--+--+--+
	 * result = [(0, 0), (0, 3), (1, 1), (2, 3), (2, 4), (3, 2), (4, 5), (5, 4)]
	 */
	static QList<QPair<int, int>> optmize2dCoordGaps(const QList<QPair<int, int>> &points);
};

#endif // RPGUTILS_H
