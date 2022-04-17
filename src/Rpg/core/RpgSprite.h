#ifndef RPGSPRITE_H
#define RPGSPRITE_H

#include <QMutex>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>

#define MotionSeqFrames 4

class RpgSprite : public RpgObject
{
	Q_OBJECT

	// 角色名称
	QString name;

	QGraphicsPixmapItem *spritePixmap = new QGraphicsPixmapItem(this);

	// 当前状态
	QString motion;
	int motionIndex = 0;

	// 状态锁
	QMutex motionLocker;

	int timerId = -1;
	// timerEvent间隔时间(ms)
	int timerInterval = 200;

	void timerEvent(QTimerEvent *event);

public:
	typedef QVarLengthArray<QPixmap, MotionSeqFrames> MotionPixmapList;

private:
	QHash<QString, MotionPixmapList*> motions;

	bool updateSpriteMotion();

	// TODO: RpgMap ...
	// TODO: setMapPos ...
	// TODO: setMap ...

	// TODO: RpgSpriteEffect ...

public:
	// 角色名称
	inline void setName(const QString &name){ this->name = name; }
	inline const QString &getName() const { return this->name; }

	// 定时器间隔设置
	inline void setTimerInterval(int ms){ this->timerInterval = ms; }
	inline int getTimerInterval() const { return this->timerInterval; }

	// 动作设置
	void setMotion(const QString &motion);
	const QString &getMotion() const{ return this->motion; }

	void addMotionPixmap(const QString &motion, const QPixmap &frame);
	const MotionPixmapList *getMotionPixmaps(const QString &motion) const;
	int getMotionPixmapCount(const QString &motion) const;

	RpgSprite(const QString &name, QGraphicsObject *parent = nullptr);

	void run();
	void end();
};

#endif // RPGSPRITE_H
