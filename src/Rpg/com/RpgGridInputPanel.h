#ifndef RPGGRIDINPUTPANEL_H
#define RPGGRIDINPUTPANEL_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QTextBlockFormat>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgItemProperties.h>
#include <Rpg/core/RpgDialogAnimation.h>

/**
 * @brief The RpgGridInputPanel class
 * RpgGridInputPanel组件
 * 有一个提示语和一个输入框, 输入框的内容可以在下方上下左右选择, 如同掌机游戏中的输入样式相似
 */
class RpgGridInputPanel : public RpgObject
{
	Q_OBJECT

	// 皮肤基类
	RpgDialogBase *skin = nullptr;

	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);
	QGraphicsTextItem *mesasgeBox = new QGraphicsTextItem(this->box);
	QString message;
	QList<QList<QGraphicsTextItem *>> panelItems;

	QGraphicsPixmapItem *selectBar = new QGraphicsPixmapItem(this->box);
	RpgItemProperties *selectItemProperties = new RpgItemProperties(this->selectBar);
	QPropertyAnimation *selectBarAnimation = new QPropertyAnimation(this->selectItemProperties, "opacity", this);

	//因为上下左右箭头要跟着selectBar移动的, 所以将parent设置位为selectBar
	QGraphicsPixmapItem *upArrowSymbol = new QGraphicsPixmapItem(this->selectBar);
	QGraphicsPixmapItem *downArrowSymbol = new QGraphicsPixmapItem(this->selectBar);
	QGraphicsPixmapItem *leftArrowSymbol = new QGraphicsPixmapItem(this->selectBar);
	QGraphicsPixmapItem *rightArrowSymbol = new QGraphicsPixmapItem(this->selectBar);

	// effect的配置
	QColor textShadowEffectColor = QColor(Qt::black);
	qreal textShadowEffectBlurRedius = 5;
	QPointF textShadowEffectOffset = QPointF(2, 2);

	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, nullptr, nullptr, 300, QEasingCurve::OutQuad, this);

	// 对话框大小
	QSize dialogSize = RpgDialogBase::maxDialogSize();
	// 对话框对齐
	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;
	Rpg::TextAlign messageAlign = Rpg::AlignLeft;

	// timer
	void timerEvent(QTimerEvent *event) override;
	int timerId = -1;
	bool timerProcessing = false;

	int timeout = 0;

	// 按键
	void keyReleaseEvent(QKeyEvent *event) override;

	QColor textColor = QColor(Qt::white);
	QColor bannedColor = QColor(Qt::darkGray);
	QFont font;

	// 计算出选项与选项之间的间距(由run计算出来)
	qreal innerPaddingH = 0;
	// 计算出第一个选项距离左边的距离(由run计算出来)
	qreal addingPaddingH = 0;
	// 计算选项上的select bar的高度
	int selectBarHeight = RpgDialogBase::selectBarSize().height();

	// 计算出的spin距离dialog top的距离
	int gridItemTop = 0;

	int lineHeight = 35;

	// 选择框在屏幕上的位置
	QPoint selectingIndex = QPoint(0, 0);
	// 展示的左上角选项在第几个
	QPoint fromIndex = QPoint(0, 0);

public:
	// 消息内部间距
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	// 消息使用内部字体名
	const char *FontName = "dialog";

	enum SoundEffect{
		SoundEffect_Select = 1,
		SoundEffect_Accept,
		SoundEffect_Banned,
	};

private:
	// 选择列表
	QList<QList<QString>> gridItems;

	int defaultGridIndex = 0;

	QTimeLine *arrowSymbolsTimeLine = new QTimeLine(1000, this);

	QHash<SoundEffect, QString> soundEffects = QHash<SoundEffect, QString>({
		{SoundEffect_Select, "select"},
		{SoundEffect_Accept, "accept"},
		{SoundEffect_Banned, "banned"},
	});

	void clearGridItems();

	void playSound(SoundEffect soundEffect, qreal volume = 1.0f, int times = 1);

	// 计算GridValue最长的文本的宽度
	int calGridValueMaxWidth(const QString &value);

	// 调整GraphicsItem gridItems
	void adjustGridItems(const QPoint &from);

public:

	// 提示文本
	inline void setMessage(const QString &message){ this->message = message; }

	RpgGridInputPanel(RpgDialogBase *dialogBase, QGraphicsObject *parent = nullptr): RpgObject(parent){

	}
};

#endif // RPGGRIDINPUTPANEL_H
