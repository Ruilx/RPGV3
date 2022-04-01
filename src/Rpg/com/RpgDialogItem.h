#ifndef RPGDIALOGITEM_H
#define RPGDIALOGITEM_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsPixmapItem>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgDialogAnimation.h>
#include <Rpg/core/RpgDialogMessage.h>

class RpgDialogItem : public RpgObject
{
	Q_OBJECT
	// 皮肤基类
	RpgDialogBase *skin = nullptr;

	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);
	QGraphicsTextItem *messageBox = new QGraphicsTextItem(this->box);
	QGraphicsPixmapItem *continueSymbol = new QGraphicsPixmapItem(this->box);
	// 立绘
	QGraphicsPixmapItem *avatarBoxRight = new QGraphicsPixmapItem(this);
	QGraphicsPixmapItem *avatarBoxLeft = new QGraphicsPixmapItem(this);
	// 效果
	QGraphicsDropShadowEffect *messageShadowEffect = new QGraphicsDropShadowEffect(this);
	// 动画组成
	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, this->avatarBoxLeft, this->avatarBoxRight, 300, QEasingCurve::OutQuad, this);
	// 对话框大小
	QSize dialogSize = RpgDialogBase::dialogSize();
	// 对话框对齐
	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;

	// 继承函数
//	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
//	void boundingRect() override;

	void timerEvent(QTimerEvent *event);

	// timer
	int timerId = -1;
	bool timerProcessing = false;

	// 按键
	void keyReleaseEvent(QKeyEvent *event) override;
public:
	// 消息框内部间距
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	// 消息使用内部字体
	const char *FontName = "dialog";
private:

	// 消息列表
	QList<RpgDialogMessage> messages;
	int messageIndex = 0;
	QList<RpgDialogMessage>::ConstIterator lastDialogMessage;

	// 正在运行的Flag
	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	// 继续三角形 时间轴
	QTimeLine *continueSymbolTimeLine = new QTimeLine(1000, this);


	inline void setMessageTextWidth(qreal width){
		this->messageBox->setTextWidth(width);
	}

public:
	// 文字颜色(不含CSS标签颜色)
	inline void setTextColor(const QColor &color){ this->messageBox->setDefaultTextColor(color); }
	inline void setTextColor(Qt::GlobalColor color){ this->messageBox->setDefaultTextColor(QColor(color)); }

	// 增加消息
	void appendMessage(const RpgDialogMessage &message);
	void appendMessage(const QList<RpgDialogMessage> &messages);
	void appendMessage(const QString &text, const QString &name = QString());

	// 清除消息
	inline void clearMessages(){ this->messages.clear(); }

	// 字体
	inline void setFont(const QFont &font){ this->messageBox->setFont(font); }
	inline QFont getFont() const{ return this->messageBox->font(); }

	// 行高
	void setLineHeight(qreal pixel, int lineHeightType = QTextBlockFormat::FixedHeight);

	// 窗口大小
	void setDialogSize(const QSize &size);
	inline const QSize getDialogSize() const{ return this->dialogSize; }

	// 窗口位置
	void setDialogAlign(Rpg::BlockAlign align){ this->dialogAlign = align; }
	inline Rpg::BlockAlign getDialogAlign() const { return this->dialogAlign; }

	// 构造
	RpgDialogItem(RpgDialogBase *dialogBase, QGraphicsObject *parent = nullptr);
	~RpgDialogItem();

	// 执行
	void run() override;
private:
	void showDialog();
	void hideDialog();

	void showNextMessage();
	void showMessage(index);
	void showText(const QString &text, int speed = 0, int pointSize = -1, const QString &name = QString(), qreal lineHeight = 35);
};

#endif // RPGDIALOGITEM_H
