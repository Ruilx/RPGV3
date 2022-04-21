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
	QList<QGraphicsTextItem *> spinItems;

	QGraphicsPixmapItem *selectBar = new QGraphicsPixmapItem(this->box);
	RpgItemProperties *selectItemProperties = new RpgItemProperties(this->selectBar);
	QPropertyAnimation *selectBarAnimation = new QPropertyAnimation(this->selectItemProperties, "opacity", this);

	// 因为上下箭头是跟着selectBar移动的, 所以将parent设置成selectBar
	QGraphicsPixmapItem *upArrowSymbol = new QGraphicsPixmapItem(this->selectBar);
	QGraphicsPixmapItem *downArrowSymbol = new QGraphicsPixmapItem(this->selectBar);

	// effect的配置, 给message和spins文本增加shadow effect
	// @see RpgChoiceItem.h:51
	QColor textShadowEffectColor = QColor(Qt::black);
	qreal textShadowEffectBlurRadius = 5;
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

	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	QTimeLine *arrowSymbolsTimeLine = new QTimeLine(1000, this);

	QHash<SoundEffect, QString> soundEffects = QHash<SoundEffect, QString>({
		{SoundEffect_Select, "select"},
		{SoundEffect_Accept, "accept"},
	});

	void clearSpinItems();

	void playSound(SoundEffect soundEffect, qreal volume = 1.0f, int times = 1);

	// 计算SpinValue最长的文本的宽度
	int calSpinValueMaxWidth(const RpgSpinValue &value);

public:
	// 文字颜色
	inline void setTextColor(const QColor &color){ this->messageBox->setDefaultTextColor(color); this->textColor = color; }
	inline void setTextColor(Qt::GlobalColor color){ this->messageBox->setDefaultTextColor(QColor(color)); this->textColor = QColor(color); }

	// 选项
	void appendSpinValue(const RpgSpinValue &value){ this->spinValues.append(value); }
	void appendSpinValue(const Qlist<RpgSpinValue> &values){ this->spinValues.append(values); }
	void getRpgSpinValueCount() const { return this->spinValues.length(); }

	void clearSpinValues() { this->spinValues.clear(); }

	// 超时时间
	inline void setTimeout(int timeout){ this->timeout = timeout; }
	inline int getTimeout() const { return this->timeout; }

	// 出字速度
	inline void setSpeed(int speed){ this->speed = speed; }
	inline int getSpeed() const { return this->speed; }

	// 字体
	inline void setFont(const QFont &font){ this->font = QFont(font); }
	void setFont(const QString &name, int pointSize = -1, int weight = -1, bool italic = false);
	inline QFont getFont() const { return this->font; }

	void setDialogSize(const QSize &size);
	inline const QSize getDialogSize() const { return this->dialogSize; }

	void setDialogAlign(Rpg::BlockAlign align){ this->dialogAlign = align; }
	inline Rpg::BlockAlign getDialogAlign() const { return this->dialogAlign; }

	void setSoundEffect(SoundEffect soundEffect, const QString &name){ this->soundEffects.insert(soundEffect, name); }
	const QString getSoundEffect(SoundEffect soundEffect) const { return this->soundEffects.value(soundEffect); }

	RpgSpinItem(const RpgDialogBase *skin, QGraphicsItem *parent);
	~RpgSpinItem();

	void run() override;
	int waitForComplete();
	void end() override;

	inline const QStringList getValue();
private:
	void showDialog();
	void hideDialog();

	void setSpinsText(int from);
	void setSelectBarIndex(int index);
signals:
	void enterDialogMode();
	void exitDialogMode();
};

#endif // RPGSPINITEM_H
