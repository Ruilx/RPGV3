#ifndef RPGSPINITEM_H
#define RPGSPINITEM_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsPixmapItem>

#include <QComboBox>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgDialogAnimation.h>
#include <Rpg/core/RpgSpinValue.h>
#include <Rpg/core/RpgItemProperties.h>

class RpgSpinItem : public RpgObject
{
	Q_OBJECT
	// 皮肤
	RpgDialogBase *skin = nullptr;

	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);
	QGraphicsTextItem *messageBox = new QGraphicsTextItem(this->box);

	QGraphicsPixmapItem *upArrowSymbol = new QGraphicsPixmapItem(this->box);
	QGraphicsPixmapItem *downArrowSymbol = new QGraphicsPixmapItem(this->box);

	QGraphicsPixmapItem *selectBar = new QGraphicsPixmapItem(this->box);
	RpgItemProperties *selectItemProperties = new RpgItemProperties(this->selectBar);
	QPropertyAnimation *selectBarAnimation = new QPropertyAnimation(this->selectItemProperties, "opacity", this);

	// effect的配置, 给message和spins文本增加shadow effect
	// @see RpgChoiceItem.h:51
	QColor textShadowEffectColor = QColor(Qt::black);
	qreal textShadowEffectColor = 5;
	QPointF textShadowEffectOffect = QPointF(2, 2);

	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, nullptr, nullptr, 300, QEasingCurve::OutQuad, this);

	QSize dialogSize = RpgDialogBase::dialogSize();

	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;

	// timer
	void timerEvent(QTimerEvent *event) override;
	int timerId = -1;
	bool timerProcessing = false;

	int timeout = 0;

	// 按键
	void keyReleaseEvent(QKeyEvent *event) override;

	QColor textColor = QColor(Qt::white);
	QFont font;
	// 计算出选项与选项之间的间距(由run计算出来)
	qreal innerPaddingH = 0;
	// 计算出第一个选项距离左边的距离(由run计算出来)
	qreal addingPaddingH = 0;
	// 计算选项上的select bar的高度
	int selectBarHeight = RpgDialogBase::selectBarSize().height();

	int speed = Rpg::SingleWordSpeedFast;

	// 选择框在屏幕上的位置
	int selectingIndex = 0;

public:
	// 消息内部间距
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	// 消息使用内部自体名
	const char *FontName = "dialog";

	enum SoundEffect{
		SoundEffect_Select = 1,
		SoundEffect_Accept,
	};

private:
	// 选择列表
	QList<RpgSpinValue> spinValues;

	// 默认选中的index
	int defaultSpinIndex = 0;



	RpgSpinItem(const RpgDialogBase *skin, QGraphicsItem *parent): RpgObject(parent){

	}
};

#endif // RPGSPINITEM_H
