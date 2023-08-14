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
 * RpgDialog由背景图片, 内容框, 继续三角形组和立绘框组成
 * 背景图片box由skin提供, skin提供相应大小的背景图片
 * -> 背景图片的大小不能超过RpgDialogBase::maxDialogSize(), 默认大小:RpgDialogBase::dialogSize(),
 *    有9个位置可以选择Rpg::DialogAlign, 配合宽度显示对话框
 * -> 内容框建立在背景框之上, 宽度是背景图片宽度-2*MessageMarginH, 高度是背景图片高度-2*MessageMarginV,
 *    但是有立绘在前面, 会适量减小宽度, 若内容依然不足, 可能需要去掉立绘(特指前端立绘)
 * -> 继续框是一个列表, 不保证每一个继续图标大小都相同, 也没有确定继续框列表中图片的数量,
 *    所以需要在运行时确定其位置, 一般位置是在对话框正下方, 位置需要重新计算
 * -> 立绘框分为6种, 分别是:
 *    1, 2: 主角立绘在框后面, 呈半身状, 宽高自动适应到(0.8xScreenHeight, 0.25xScreenWidth), 在绝对的屏幕左侧或右侧:
 *          # 在左侧显示, 动画淡入淡出+向左移动
 *          # 在右侧显示, 动画淡入淡出+向右移动
 *    3, 4: 主角立绘在框前面, 呈半身状, 宽高自动适应到(0.8xScreenHeight, 0.25xScreenWidth), 在绝对的屏幕左侧或右侧:
 *          # 在左侧显示, 动画淡入淡出+向左移动
 *          # 在右侧显示, 动画淡入淡出+向右移动
 *          注: 宽度有可能会盖住后面的字, 所以后面的框需要按需要缩小宽度, 还需要计算
 *    5, 6: 主角头像框, 只显示头像, 呈方形, 宽高自动适应(min(min(框高, 框宽), 屏幕宽度/4))
 *          # 在左侧显示, 动画淡入淡出
 *          # 在右侧显示, 动画淡入淡出
 *          注: 对话框一定要比头像宽度宽, 如果没有这么宽, 头像可能会去掉
 *    如果头像位置相同, 则下一句变化不做动画, 若不相同则更换位置运行动画
 *    这些要分开计算, 自动适应的结果建议缓存
 *    (我想把AvatarAround中mode和Left/Right分开了...) 左侧或是右侧已经被分开, 加入至Message中
 *
 * RpgAvatar维护显示的头像
 * RpgDialogMessage维护显示的内容
 * 每一段内容都可能分配一个头像, 如果下段头像相同, 则不执行动画, 如果有变化, 则装载进入/退出动画
 *
 * CurrentBug:
 * 过渡显示阶段, 不会运行以下动画
 * 相同位置(Around)的头像模式转换(头像转立绘, 立绘转立绘等)
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
	void timerEvent(QTimerEvent *event) override;
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
