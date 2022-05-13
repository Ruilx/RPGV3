#ifndef RPGBANNER_H
#define RPGBANNER_H

#include <QGraphicsItem>
#include <QTimeLine>

#include <functional>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgItemProperties.h>

/**
 * @brief The RpgBanner class
 * RpgBanner类, 可以在窗口中展示一段实时播放的演示, 信息或图片等
 * 将item设置好后可加入组件中, 并设置好时间轴关键帧回调, 可以用于JS函数回调(需要配合JsBanner回调)
 * 创建: 给定参数duration:时长ms, fps:帧率(关键帧/秒), parent
 * 增加Item: addItem(名称, *item), item的parent将会设置为banner
 * 增加CB: setTimeLineFrameCb(frame, CB) 或 setTimeLineTimestampCb(ms, CB)
 * 如果增加的CB超出了Timeline的长度, 则这个CB不会被执行
 *
 * 注意: 设置到timestamp的CB会被换算成frame, 所以其执行可能滞后于当前帧对应的timestamp.
 */
class RpgBanner : public RpgObject
{
	Q_OBJECT

	QHash<QString, QGraphicsItem*> items;

	RpgItemProperties *itemProperties = new RpgItemProperties(this, this);
	QPropertyAnimation *enterAnimation = new QPropertyAnimation(this);
	QPropertyAnimation *exitAnimation = new QPropertyAnimation(this);

	QTimeLine *timeLine = nullptr; // constructing by construct function

	bool canSkip = false;
	bool willWaitKeyPress = false;

	void keyReleaseEvent(QKeyEvent *event);
public:
	//typedef void(*frameCb)();
	typedef std::function<void()> frameCb;
private:
	QMap<int, frameCb> frameCbs;
	QMap<int, frameCb>::ConstIterator frameCbI;

	void setup();
	void setTimeLineProps(int durationMs, int fps);

public:
	RpgBanner(int durationMs, int fps = 24, QGraphicsObject *parentObject = nullptr);
	//explicit RpgBanner(int frames, qreal interval, QGraphicsObject *parentObject = nullptr);
	~RpgBanner();

	inline void setDuration(int timeout){ this->timeLine->setDuration(timeout); }
	inline int getDuration() const { return this->timeLine->duration(); }

	void setTimeLineFrameCb(int frame, frameCb cb);
	int setTimeLineTimestampCb(int timestampMs, frameCb cb);
	void removeTimeLineFrameCb(int frame);

	void removeItem(const QString &name);
	void addItem(const QString &name, QGraphicsItem *item);

	inline const QGraphicsItem *getItem(const QString &name) const{
		if(!this->items.contains(name)){
			rWarning() << "Giving name '" << name << "' found no items.";
			return nullptr;
		}
		return this->items.value(name);
	}

	inline void setCanSkip(bool canSkip){ this->canSkip = canSkip; }
	inline bool getCanSkip() const { return this->canSkip; }

	inline void setWillWaitKeyPress(bool willWaitKey);
	inline bool getWillWaitKeyPress() const { return this->willWaitKeyPress; }

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
