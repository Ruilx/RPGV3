#ifndef RPGCHOICEITEM_H
#define RPGCHOICEITEM_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsPixmapItem>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgDialogAnimation.h>

#include <Rpg/core/RpgChoiceMessage.h>

/**
 * @brief The RpgChoiceItem class
 * RpgChoice 组件
 * 从指定选项组中选择一项作为结果返回
 * RpgChoice由背景框, 内容框*N(在运行时通过指定高度计算出来), 下/上三角形
 * 选项中不显示立绘框.
 *
 * 图片由Skin提供, 选项内容由RpgChoicesMessage提供
 * ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ △ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 * ┃  ┌──────────────────────────────────────────────────────────┐  ┃
 * ┃  │ 第一个选项                                                 │  ┃
 * ┃  └──────────────────────────────────────────────────────────┘  ┃
 * ┃    第二个选项                                                    ┃
 * ┃                                                                ┃
 * ┃    第三个选项                                                    ┃
 * ┃                                                                ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ ▼ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 *
 */
class RpgChoiceItem : public RpgObject
{
	Q_OBJECT

	RpgDialogBase *skin = nullptr;

	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);
	QList<QGraphicsTextItem *> textItems;

	QGraphicsPixmapItem *upArrowSymbol = new QGraphicsPixmapItem(this->box);
	QGraphicsPixmapItem *downArrowSymbol = new QGraphicsPixmapItem(this->box);

	QGraphicsPixmapItem *selectBar = new QGraphicsPixmapItem(this->box);
	RpgItemProperties *selectItemProperties = new RpgItemProperties(this->selectBar, this);
	QPropertyAnimation *selectBarAnimation = new QPropertyAnimation(this->selectItemProperties, "opacity", this);

	// effect只能对一个text item增加effect, 所以这里做成配置式的
	// 在设置effect的时候item会take ownership of the effect, 所以对其中直接new
	//QGraphicsDropShadowEffect *textShadowEffect = new QGraphicsDropShadowEffect(this);
	QColor textShadowEffectColor = QColor(Qt::black);
	qreal textShadowEffectBlurRadius = 5;
	QPointF textShadowEffectOffset = QPointF(2, 2);

	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, nullptr, nullptr, 300, QEasingCurve::OutQuad, this);

	QSize dialogSize = RpgDialogBase::dialogSize();

	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;

	// timer
	void timerEvent(QTimerEvent *event);
	int timerId = -1;
	bool timerProcessing = false;

	int timeout = 0;

	// 按键
	void keyReleaseEvent(QKeyEvent *event) override;

	// 因为动态生成列表, 所以在run之前一些配置在这里存储, 在run的时候动态设置上
	//qreal textWidth = 0;
	QColor textColor = QColor(Qt::white);
	QFont font;
	// 计算出的选项与选项之间的间距(run计算出来的)
	qreal innerPaddingV = 0;
	// 计算出第一个选项距离顶增加的距离(run计算出来的)
	qreal addingPaddingV = 0;
	// 计算出的选项上的select bar的高度
	int selectBarHeight = RpgDialogBase::selectBarSize().height();

	int speed = Rpg::SingleWordSpeedFast;

	// 选择框在屏幕上的消息位置
	int selectingIndex = 0;
	// 展示第一个选项在列表中的哪一个
	int fromIndex = 0;

	// 返回结果
	int chosenIndex = 0;

public:
	// 消息框内部间距
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	// 消息使用内部字体名
	const char *FontName = "dialog";
	const QColor bannedColor = QColor(Qt::darkGray);

	enum SoundEffect{
		SoundEffect_Select = 1,
		SoundEffect_Banned,
		SoundEffect_Accept,
	};

private:
	// 选择列表
	QList<RpgChoiceMessage> choices;
	//int mesasgeIndex = 0;

	// 默认选中的index
	int defaultChoiceIndex = 0;

	// 正在运行的Flag
	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	// 上下箭头三角形 时间轴
	QTimeLine *arrowSymbolsTimeLine = new QTimeLine(1000, this);

	// 记录soundEffects对应的声音名称
	QHash<SoundEffect, QString> soundEffects = QHash<SoundEffect, QString>({
		{SoundEffect_Select, "select"},
		{SoundEffect_Banned, "banned"},
		{SoundEffect_Accept, "accepted"},
	});

	//inline void setChoiceTextWidth(qreal width){ this->textWidth = width; }
	void clearTextItems();

	void playSound(SoundEffect soundEffect, qreal volume = 1.0f, int times = 1);
public:
	// 文字颜色(不含CSS标签颜色)
	inline void setTextColor(const QColor &color){ this->textColor = color; }
	inline void setTextColor(Qt::GlobalColor color){ this->textColor = QColor(color); }

	inline void appendChoice(const RpgChoiceMessage &choice);
	inline void appendChoice(const QList<RpgChoiceMessage> &choices);
	inline void appendChoice(const QString &text, bool enabled = true){
		this->choices.append(RpgChoiceMessage(text, enabled));
	}

	inline void clearChoices(){ this->choices.clear(); }

	void setDefaultChoice(int index);
	void setDefaultChoice(const QString &value);
	inline int getDefaultChoice() const { return this->defaultChoiceIndex; }

	inline void setTimeout(int timeout){ this->timeout = timeout; }
	inline int getTimeout() const { return this->timeout; }

	// 载入前出字速度
	inline void setSpeed(int speed){ this->speed = speed; }
	inline int getSpeed() const { return this->speed; }

	// 字体 (借当前父系font的值)
	inline void setFont(const QFont &font) { this->font = QFont(font); }
	inline QFont getFont() const { return this->font; }

	// 窗口大小
	void setDialogSize(const QSize &size);
	inline const QSize getDialogSize() const { return this->dialogSize; }

	// 窗口位置
	void setDialogAlign(Rpg::BlockAlign align){ this->dialogAlign = align; }
	inline Rpg::BlockAlign getDialogAlign() const { return this->dialogAlign; }

	// 设置音效
	void setSoundEffect(SoundEffect soundEffect, const QString &name){ this->soundEffects.insert(soundEffect, name); }
	const QString getSoundEffect(SoundEffect soundEffect) const { return this->soundEffects.value(soundEffect); }

	// 构造
	RpgChoiceItem(RpgDialogBase *dialogBase, QGraphicsObject *parent = nullptr);
	~RpgChoiceItem();

	// 执行
	void run() override;
	int waitForComplete();
	void end() override;

	// 获得选项值
	inline const QString &getValue() const { return this->choices.at(this->chosenIndex).getValue(); }

private:
	void showDialog();
	void hideDialog();

	void setChoicesText(int from, bool withSpeed = false);
	void setSelectBarIndex(int index);
signals:
	void enterDialogMode();
	void exitDialogMode();

};

#endif // RPGCHOICEITEM_H
