#ifndef RPGCHOICE_H
#define RPGCHOICE_H

#include <Rpg/Rpg.h>

#include <QtWidgets>

#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgFont.h>
#include <Rpg/com/RpgView.h>
#include <Rpg/core/RpgState.h>

#include <Rpg/core/RpgDialogBase.h>

#include <Rpg/core/RpgChoiceMessage.h>
#include <Rpg/core/RpgDialogAnimation.h>
#include <Rpg/core/RpgHtmlSplit.h>

#include <Rpg/exception/RpgNullPointerException.h>

#define ChoiceBuff 3

/**
 * @brief The RpgChoice class
 * RpgChoice Rpg对话选择模块
 * RpgChoice由背景图片, 内容框*3, 下三角形, 上三角形组成 (没有立绘框?)
 * 背景图片box由skin提供, skin提供相应大小的背景图片
 * -> 背景图片大小不能超过RpgDialogBase::maxDialogSize(), 默认大小:RpgDialogBase::dialogSize(),
 *    有9个位置可以选择Rpg::DialogAlign, 配合宽度显示对话框
 * -> 内容框建立在背景框之上, 宽度是背景图片宽度-2*MessageMarginH, 高度是背景图片高度-2*MessageMarginV,
 *    选择框中没有立绘, 但有ChoiceBuff个消息框, 按上下键选取, 确定键选择并返回
 * -> 若超出ChoiceBuff的内容, 在列表中隐藏
 */
class RpgChoice : public RpgObject
{
	Q_OBJECT

	RpgDialogBase *skin = nullptr;
	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this); // 基础框
	QGraphicsTextItem *messageBox[ChoiceBuff] = {nullptr}; // 内容框
	QGraphicsPixmapItem *upSymbol = new QGraphicsPixmapItem(this->box); // 上箭头框
	QGraphicsPixmapItem *downSymbol = new QGraphicsPixmapItem(this->box); // 下箭头框
	QGraphicsPixmapItem *choiceSymbol = new QGraphicsPixmapItem(this->box); // 选择框

	// 特效(阴影)
	QGraphicsDropShadowEffect *messageShadowEffect = new QGraphicsDropShadowEffect(this);

	// 并行动画
	//QParallelAnimationGroup *entryAniGroup = new QParallelAnimationGroup(this);
	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, 300, QEasingCurve::OutQuad, this);

	// 选择框的闪烁动画
	//QGraphicsOpacityEffect *choiceBarOpacityEffect = new QGraphicsOpacityEffect(this);
	RpgItemProperties *choiceBarProperties = new RpgItemProperties(this->choiceSymbol, this);
	QPropertyAnimation *choiceBarAnimation = new QPropertyAnimation(this->choiceBarProperties, "opacity", this);

	// 对话框大小
	QSize dialogSize = RpgDialogBase::dialogSize();

	// 对话框位置
	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;
public:
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;
	const int MessageSpecingV = 2;

	const int MinDialogWidth = 125;
	const int MinDialogHeight = 125;

private:
	QList<RpgChoiceMessage> messages;
	QList<RpgChoiceMessage> messagesReady;
	int messageIndex = 0;

	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	QTimeLine *arrowSymbolTimeLine = new QTimeLine(1000, this);

	QList<QPixmap> upPixmap;

	inline void setMessageTextWidth(qreal width){
		for(QGraphicsTextItem *message: this->messageBox){
			if(message != nullptr){
				message->setTextWidth(width);
			}
		}
	}

	void showDialog(){
		this->show();
		emit this->enterDialogMode();
		RpgState::instance()->pushState(RpgState::DialogMode);
		RpgDialogAnimation::Animations animations = RpgDialogAnimation::AnimationDialogShow;
//		this->dialogAnimation->runDialogAvatarAnimations();

	}
public:
	RpgChoice(RpgDialogBase *dialogBase, QGraphicsObject *parentItem = nullptr): RpgObject(parentItem){
		this->setTextColor(Qt::white);

		if(dialogBase == nullptr){
			throw RpgNullPointerException("RpgDialogBase not set.");
		}
		this->skin = static_cast<RpgDialogBase*>(dialogBase);

		for(QGraphicsTextItem *message: this->messageBox){
			if(message != nullptr){
				delete message;
			}
			message = new QGraphicsTextItem(this);
			message->setFont(RpgFont::instance()->getFont("dialog"));
			rDebug() << message->font().family();
			message->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
			rDebug() << message->document()->defaultStyleSheet();
			message->document()->setUndoRedoEnabled(false);
			QTextOption messageTextOption = message->document()->defaultTextOption();{
				// Dialog用的QTextOption是QTextOption::WrapAnywhere
				messageTextOption.setWrapMode(QTextOption::NoWrap);
				message->document()->setDefaultTextOption(messageTextOption);
			}
		}

		// 上三角的形成
		QMatrix upDownMirror;
		upDownMirror.rotate(180);
		for(int i = 0; i < this->skin->getContinueSymbolImageLength(); i++){
			QPixmap triangle = this->skin->getContinueSymbolImage(i).transformed(upDownMirror, Qt::SmoothTransformation);
			this->upPixmap.append(triangle);
		}


		// 上三角形和下三角形的动画(公用一个Timeline)
		this->arrowSymbolTimeLine->setFrameRange(0, this->skin->getContinueSymbolImageLength());
		this->arrowSymbolTimeLine->setLoopCount(Rpg::Infinity);
		this->connect(this->arrowSymbolTimeLine, &QTimeLine::frameChanged, [this](int frameIndex){
			if(frameIndex >= 0 && frameIndex < this->skin->getContinueSymbolImageLength()){
				QPixmap framePixmap = this->skin->getContinueSymbolImage(frameIndex);
				this->downSymbol->setPos((this->dialogSize.width() - framePixmap.width()) / 2.0, this->dialogSize.height() - (framePixmap.height() / 2.0));
				this->downSymbol->setPixmap(framePixmap);
				this->upSymbol->setPos((this->dialogSize.width() - framePixmap.width()) / 2.0, framePixmap.height() / 2.0);
				this->upSymbol->setPixmap(this->upPixmap.at(frameIndex));
			}
		});

		// Z-Value
		this->setZValue(Rpg::ZValueDialog);
		this->box->setZValue(0);
		for(QGraphicsTextItem *message: this->messageBox){
			message->setZValue(0.2);
		}
		this->upSymbol->setZValue(0.3);
		this->downSymbol->setZValue(0.3);

		// 默认消息位置和大小
		this->setMessageTextWidth(this->dialogSize.width() - (2.0 * MessageMarginH));
		for(int i = 0; i < ChoiceBuff; i++){
			QFontMetrics metrics(this->messageBox[i]->font());
			this->messageBox[i]->setPos(MessageMarginH, MessageMarginV + (i * (MessageSpecingV + metrics.height())));
		}

		// 字符下的阴影
		this->messageShadowEffect->setColor(QColor(Qt::black));
		this->messageShadowEffect->setBlurRadius(5.0);
		this->messageShadowEffect->setOffset(2.0, 2.0);
		for(QGraphicsTextItem *message: this->messageBox){
			message->setGraphicsEffect(this->messageShadowEffect);
		}

		// 默认不可见
		this->hide();

		RpgState::instance()->registerRpgObject(this, RpgState::DialogMode);
	}

	~RpgChoice(){
		RpgState::instance()->unregisterRpgObject(this, RpgState::DialogMode);
	}

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认字符颜色
	 */
	inline void setTextColor(const QColor &color){
		for(QGraphicsTextItem *message: this->messageBox){
			if(message != nullptr){
				message->setDefaultTextColor(color);
			}
		}
	}

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认字符颜色
	 */
	inline void setTextColor(Qt::GlobalColor color){
		for(QGraphicsTextItem *message: this->messageBox){
			if(message != nullptr){
				message->setDefaultTextColor(QColor(color));
			}
		}
	}

	/**
	 * @brief appendChoice
	 * @param message
	 * 增加选项
	 */
	inline void appendChoice(const RpgChoiceMessage &message){
		this->messages.append(message);
	}

	/**
	 * @brief appendChoice
	 * @param list
	 * 增加选项
	 */
	inline void appendChoice(const QList<RpgChoiceMessage> &list){
		this->messages.append(list);
	}

	/**
	 * @brief setFont
	 * @param font
	 * 设置字体
	 */
	inline void setFont(const QFont &font){
		for(QGraphicsTextItem *message: this->messageBox){
			if(message != nullptr){
				message->setFont(font);
			}
		}
	}

	/**
	 * @brief getFont
	 * @return
	 * 获得当前字体
	 */
	QFont getFont() const{
		if(this->messageBox[0] != nullptr){
			return this->messageBox[0]->font();
		}else{
			return QFont();
		}
	}

	/**
	 * @brief setDialogSize
	 * @param size
	 * 设置对话框大小
	 */
	void setDialogSize(const QSize &size){
		int width = size.width();
		int height = size.height();
		if(width < RpgChoice::MinDialogWidth || width > RpgDialogBase::maxDialogSize().width()){
			rDebug() << "Given width:" << width << "is out of range: (" << RpgChoice::MinDialogWidth << "," << RpgDialogBase::maxDialogSize().width() << ")";
			return;
		}
		// height按理来说应该大于ChoiceBuff加前后的padding, margin之后的结果, 但调大之后会导致很奇怪的显示方式, 之后希望调大之后能够出更多的选项等
		// Todo: height值的优化
		if(height < RpgChoice::MinDialogHeight || height > RpgDialogBase::maxDialogSize().height()){
			rDebug() << "Given height:" << height << "is out of range: (" << RpgChoice::MinDialogHeight << "," << RpgDialogBase::maxDialogSize().height() << ")";
			return;
		}
		this->dialogSize.setWidth(width);
		this->dialogSize.setHeight(height);
	}

	/**
	 * @brief getDialogSize
	 * @return
	 * 获得对话框大小
	 */
	QSize getDialogSize() const{
		return this->dialogSize;
	}

	/**
	 * @brief clearMessages
	 * 清除选项内容
	 */
	inline void clearMessages(){
		this->messages.clear();
		this->messagesReady.clear();
	}

	/**
	 * @brief setDialogAlign
	 * @param align
	 * 设置对话框位置
	 */
	inline void setDialogAlign(Rpg::BlockAlign align){
		this->dialogAlign = align;
	}

	/**
	 * @brief getDialogAlign
	 * @return
	 * 获得对话框位置
	 */
	inline Rpg::BlockAlign getDialogAlign() const{
		return this->dialogAlign;
	}

	void run() Override{
		RpgObject::run();
		// 重新设定其Scene
		if(RpgView::instance()->scene() == nullptr){
			rDebug() << "RpgView not loaded seene yet.";
			this->end();
			throw RpgNullPointerException("RpgView::instance()->scene()");
		}else{
			// 如果该对象已装载至其他的scene的时候, 这个函数能自动将这个对象在其他scene删除后添加至该scene
			RpgView::instance()->scene()->addItem(this);
		}

		// 确定上下三角形的存在
		for(int i = 0; i < this->skin->getContinueSymbolImageLength(); i++){
			if(this->skin->getContinueSymbolImage(i).isNull()){
				rDebug() << "UpDown symbol frame" << i << "is null";
				this->end();
				return;
			}
		}

		// 复制消息组件
		// 在这里Ready是用来显示的, messages是事先设置的
		if(this->messages.isEmpty()){
			rDebug() << "Detached message is empty, exitted";
			this->end();
			return;
		}else{
			this->messagesReady.clear();
			this->messagesReady = this->messages;
		}

		// 设置对话框背景, 支持每次对话框形状不同
		QPointF dialogPos = RpgUtils::getDialogPos(this->dialogAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

		this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
		this->box->setPos(dialogPos);

		this->upSymbol->setPixmap(this->upPixmap.at(0));
		this->upSymbol->setVisible(false);

		this->downSymbol->setPixmap(this->skin->getContinueSymbolImage(0));
		this->downSymbol->setVisible(false);

		this->choiceSymbol->setVisible(false);

		this->showDialog();
	}

signals:
	void enterDialogMode();
	void exitDialogMode();
};

#endif // RPGCHOICE_H
