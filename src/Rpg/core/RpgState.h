#ifndef RPGSTATE_H
#define RPGSTATE_H

#include <QtCore>
#include <QKeyEvent>
#include <Rpg/Global.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>

#include <Rpg/exception/RpgNullPointerException.h>

class RpgState : public QObject
{
	Q_OBJECT
	const int MaxStates = 10;

	static RpgState *_instance;

public:

	static RpgState *instance();

	enum Mode{
		UnknowMode = -1,
		AutoMode = 0,
		NormalMode,
		SystemMenuMode,
		DialogMode,
		ItemMode,
	};

private:
	QHash<Mode, const char*> stateModeString = QHash<Mode, const char*>({
		{UnknowMode, "Unknown"},
		{AutoMode, "Auto"},
		{NormalMode, "Normal"},
		{SystemMenuMode, "SystemMenu"},
		{DialogMode, "Dialog"},
		{ItemMode, "Item"},
	});

	QStack<Mode> modeStack;
	QHash<Mode, QVector<RpgObject*> > modeObjects;

public:
	explicit RpgState(QObject *parent = nullptr);

	inline const QString getModeStr(Mode mode) const{ return this->stateModeString.value(mode, "Undefined"); }

	inline Mode getTop() const{ return this->modeStack.top(); }
	void pushState(Mode mode);
	Mode popState();
	void registerRpgObject(RpgObject *obj, Mode mode);
	void unregisterRpgObject(RpgObject *obj, Mode mode);

public slots:
	void keyPressEvent(QKeyEvent *event, const QGraphicsScene *scene);
	void keyReleaseEvent(QKeyEvent *event, const QGraphicsScene *scene);
};

#ifndef rpgState
#	define rpgState (RpgState::instance())
#endif

#endif // RPGSTATE_H
