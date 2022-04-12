
#ifndef RPGVIEW_H
#define RPGVIEW_H

#include <QtCore>
#include <QGraphicsView>

#include <QMouseEvent>

/**
 * @brief The RpgView class
 * RpgView为RPG组件的展示区, 并接收各种外设的输入
 */
class RpgView : public QGraphicsView
{
	static RpgView *_instance;

	QPointF lastPos;
	qreal currentScale = 1.0f;
	bool mouseMovePressed = false;

	/**
	 * @brief ignoreKeyAutoRepeat
	 * 接收按键时有可能会频繁接收按下和抬起的事件, 即便用户只是按下了按键并未抬起
	 * 但这个就会导致人物接收的事件太多难于处理
	 * 设置一个flag, 将中间的按键脉冲去除, 使进入RpgState的按键事件 比较纯净
	 */
	bool ignoreKeyAutoRepeat = true;

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	void wheelEvent(QWheelEvent *event);

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
public:
	static RpgView *instance(QWidget *parent = nullptr);

	inline void setIgnoreKeyAutoRepeat(bool enable){ this->ignoreKeyAutoRepeat = enable; }
	inline bool getIgnoreKeyAutoRepeat() const { return this->ignoreKeyAutoRepeat; }

private:
	RpgView(QWidget *parent = nullptr);
};

#ifndef rpgView
#	define rpgView (RpgView::instance())
#endif

#endif // RPGVIEW_H
