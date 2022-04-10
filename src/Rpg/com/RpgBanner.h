#ifndef RPGBANNER_H
#define RPGBANNER_H

#include <QGraphicsItem>
#include <QTimeLine>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgItemProperties.h>

class RpgBanner : public RpgObject
{
	Q_OBJECT

	QHash<QString, QGraphicsItem*> items;

	RpgItemProperties *itemProperties = new RpgItemProperties(this, this);
	QPropertyAnimation *enterAnimation = new QPropertyAnimation(this);
	QPropertyAnimation *exitAnimation = new QPropertyAnimation(this);

	QTimeLine *timeLine = nullptr; // constructing by construct function

	int timerId = -1;

	void timerEvent(QTimerEvent *event);

public:
	typedef void(*frameCb)();

private:
	QMap<int, frameCb> frameCbs;
	QMap<int, frameCb>::ConstIterator frameCbI;

	void setup();
	void setTimeLineProps(int durationMs, int fps);

public:
	explicit RpgBanner(int durationMs, int fps = 24, QGraphicsObject *parentObject = nullptr);
	//explicit RpgBanner(int frames, qreal interval, QGraphicsObject *parentObject = nullptr);
	~RpgBanner();

	inline void setDuration(int timeout){ this->timeLine->setDuration(timeout); }
	inline int getDuration() const { return this->timeLine->duration(); }

	void setTimeLineFrameCb(int frame, frameCb cb);
	int setTimeLineTimestampCb(int timestampMs, frameCb cb);
	void removeTimeLineFrameCb(int frame);

	void removeItem(const QString &name);
	void addItem(const QString &name, QGraphicsItem *item);

	const QGraphicsItem *getItem(const QString &name) const{
		if(!this->items.contains(name)){
			rWarning() << "Giving name '" << name << "' found no items.";
			return nullptr;
		}
		return this->items.value(name);
	}

	void run() override;
	int waitForComplete();
	void end() override;
	void showBanner();
	void hideBanner();

signals:
	void enterAutoMode();
	void exitAutoMode();



};

#endif // RPGBANNER_H
