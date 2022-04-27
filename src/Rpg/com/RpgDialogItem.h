#ifndef RPGDIALOGITEM_H
#define RPGDIALOGITEM_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsPixmapItem>
#include <QTextBlockFormat>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgDialogAnimation.h>
#include <Rpg/core/RpgDialogMessage.h>

/**
 * @brief The RpgDialogItem class
 * RpgDialog 组件
 * 展示一系列有序的会话消息, 和会话者立绘
 * RpgDialog由背景框, 内容框, 下三角形(继续图标), 左立绘和右立绘(显示只能选其一)组成
 * 展示时可根据立绘状态调整入/出动画, 立绘大小自适应
 *
 * 图片由Skin提供, 选项内容由RpgDialogMessage提供
 *
 */
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

	// timer
	void timerEvent(QTimerEvent *event);
	int timerId = -1;
	bool timerProcessing = false;

	// 按键
	void keyReleaseEvent(QKeyEvent *event) override;
public:
	// 消息框内部间距
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	// 消息使用内部字体名
	const char *FontName = "dialog";

	enum SoundEffect{
		SoundEffect_Continuous = 1,
		SoundEffect_Close,
	};

private:

	// 消息列表
	QList<RpgDialogMessage> messages;
	int messageIndex = 0;

	QList<RpgDialogMessage> emptyMessage = {RpgDialogMessage("", "")};
	QList<RpgDialogMessage>::ConstIterator lastDialogMessage;
	/* 关于lastDialogMessage的补丁
	 * lastDialogMessage从index改成iterator之后, 读取lastDialogMessage的第一项可能会出问题
	 * 读取模式时要从"永远不同"开始, 但List并无"空"项, 所以在此定义一个emptyMessage List用来存
	 * 储空项, 从第二项开始将lastDialogMessage导至messages的第0项, 由此向后继续
	 */

	// 正在运行的Flag
	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	// 继续三角形 时间轴
	QTimeLine *continueSymbolTimeLine = new QTimeLine(1000, this);

	// 记录soundEffect对应的声音名称
	QHash<SoundEffect, QString> soundEffects = QHash<SoundEffect, QString>({
		{SoundEffect_Continuous, ""},
		{SoundEffect_Close, ""},
	});

	void playSound(SoundEffect soundEffect, qreal volume = 1.0f, int times = 1);

	inline void setMessageTextWidth(qreal width){ this->messageBox->setTextWidth(width); }

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
	int waitForComplete();
	void end() override;
private:
	void showDialog();
	void hideDialog();

	void showNextMessage();
	void showMessage(int index);
	void showText(const QString &text, int speed = 0, int pointSize = -1, const QString &name = QString(), qreal lineHeight = 35, Rpg::TextAlign textAlign = Rpg::AlignLeft);
signals:
	void enterDialogMode();
	void exitDialogMode();
};

#endif // RPGDIALOGITEM_H
