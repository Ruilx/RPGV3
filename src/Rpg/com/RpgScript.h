#ifndef RPGSCRIPT_H
#define RPGSCRIPT_H

#include <QObject>
#include <QJSEngine>

class RpgScript : public QObject
{
	Q_OBJECT
public:
	explicit RpgScript(QObject *parent = nullptr);

signals:

public slots:
};

#endif // RPGSCRIPT_H
