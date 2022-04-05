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


class RpgChoiceItem : public RpgObject
{
	Q_OBJECT

	RpgDialogBase *skin = nullptr;

	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);
	QList<QGraphicsTextItem *> textItems;

	QGraphicsPixmapItem *upArrowSymbol = new QGraphicsPixmapItem(this->box);
	QGraphicsPixmapItem *downArrowSymbol = new QGraphicsPixmapItem(this->box);

	QGraphicsDropShadowEffect textShadowEffect = new QGraphicsDropShadowEffect(this);

	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, nullptr, nullptr, 300, QEasingCurve::OutQuad, this);

	QSize dialogSize = RpgDialogBase::dialogSize();

	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;

	// timer
	void timerEvent(QTimerEvent *event);
	int timerId = -1;
	bool timerProcessing = false;

	// 按键
	void keyReleaseEvent(QKeyEvent *event) override;

	// 因为动态生成列表, 所以在run之前一些配置在这里存储, 在run的时候动态设置上
	qreal textWidth = 0;
	QColor textColor = QColor(Qt::white);

public:
	// 消息框内部间距
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	// 消息使用内部字体名
	const char *FontName = "dialog";
private:
	// 选择列表
	QList<RpgChoiceMessage> choices;
	int mesasgeIndex = 0;

	// 正在运行的Flag
	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	// 上下箭头三角形 时间轴
	QTimeLine *arrowSymbolsTimeLine = new QTimeLine(1000, this);

	inline void setMessageTextWidth(qreal width){ this->textWidth = width; }
public:
	// 文字颜色(不含CSS标签颜色)
	inline void setTextColor(const QColor &color){ this->textColor = color; }
	inline void setTextColor(Qt::GlobalColor color){ this->textColor = QColor(color); }

	inline void appendChoice(const RpgChoiceMessage &choice);
	inline void appendChoice(const QList<RpgChoiceMessage> &choices);
	inline void appendChoice(const QString &text, bool enabled = true);

	inline void clearChoices(){ this->choices.clear(); }

	// 字体 (借当前父系font的值)
	inline void setFont(const QFont &font){ this->setFont(font); }
	inline QFont getFont() const { return this->font(); }

	// 窗口大小
	void setDialogSize(const QSize &size);
	inline const QSize getDialogSize() const { return this->dialogSize; }

	// 窗口位置
	void setDialogAlign(Rpg::BlockAlign align){ this->dialogAlign = align; }
	inline Rpg::BlockAlign getDialogAlign() const { return this->dialogAlign; }

	// 构造
	RpgChoiceItem(RpgDialogBase *dialogBase, QGraphicsObject *parent = nullptr);
	~RpgChoiceItem();

	// 执行
	void run() override;
	int waitForComplete();
	void end() override;

private:
	void showDialog();
	void hideDialog();

signals:
	void enterDialogMode();
	void exitDialogMode();

};

#endif // RPGCHOICEITEM_H
