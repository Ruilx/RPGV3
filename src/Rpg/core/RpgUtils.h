#ifndef RPGUTILS_H
#define RPGUTILS_H

#include <QtCore>

#define EnumKeyToString(key) #key

class RpgUtils
{
public:
	static void msleep(int msec);
	static const QByteArray readFile(const QString &filename, int length = 1048576);
	static const QString detectJsonValue(const QJsonValue &value);
	static const QString detectJsonValue(const QJsonDocument &value);
	static const QString keyModifierToString(Qt::KeyboardModifiers mod);
	static const QString keyToString(Qt::Key key);
	static const QString keysToString(Qt::Key key, Qt::KeyboardModifiers mod);
};

#endif // RPGUTILS_H
