#ifndef RPGDIALOG_H
#define RPGDIALOG_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgAvatar.h>
#include <Rpg/com/RpgView.h>
#include <Rpg/core/RpgState.h>

#include <Rpg/core/RpgDialogBase.h>

#include <Rpg/core/RpgDialogMessage.h>
#include <Rpg/core/RpgHtmlSplit.h>
#include <Rpg/core/RpgDialogAnimation.h>

#include <Rpg/exception/RpgNullPointerException.h>

/**
 * @brief The RpgDialog class
 * RpgDialog Rpg对话显示模块
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
class RpgDialog : public RpgObject
{
	Q_OBJECT

	RpgDialogBase *skin = nullptr;

	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this); // 基础框
	QGraphicsTextItem *messageBox = new QGraphicsTextItem(this->box); // 内容框
	QGraphicsPixmapItem *continueSymbol = new QGraphicsPixmapItem(this->box); // 继续框
	QGraphicsPixmapItem *avatarBoxRight = new QGraphicsPixmapItem(this); // 右侧立绘框
	QGraphicsPixmapItem *avatarBoxLeft = new QGraphicsPixmapItem(this); // 左侧立绘框

	// 特效
	QGraphicsDropShadowEffect *messageShadowEffect = new QGraphicsDropShadowEffect(this);

	// 动画
	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, this->avatarBoxLeft, this->avatarBoxRight, 300, QEasingCurve::OutQuad, this);

	// 对话框大小
	QSize dialogSize = RpgDialogBase::dialogSize();
	// 对话框位置
	//QPointF dialogPos = QPointF((ScreenWidth - RpgDialogBase::dialogSize().width()) / 2.0f, (ScreenHeight - RpgDialogBase::MarginV - RpgDialogBase::dialogSize().height()));
	// 内容大小
	//QRectF messageRect = QRect(RpgDialog::MessageMarginH, RpgDialog::MessageMarginV, RpgDialogBase::dialogSize().width() - (RpgDialog::MessageMarginH << 1), RpgDialogBase::dialogSize().height() - (RpgDialog::MessageMarginV << 1));

	Rpg::BlockAlign dialogaAlign = Rpg::AlignBottom;
public:
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	const int MinDialogWidth = 125;
	const int MinDialogHeight = 125;

private:
	QList<RpgDialogMessage> messages;
	QList<RpgDialogMessage> messagesReady;
	int messageIndex = 0;
	RpgDialogMessage lastDialogMessage = RpgDialogMessage("");

	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	QTimeLine *continueSymbolTimeLine = new QTimeLine(1000, this);

	inline void setMessageTextWidth(qreal width){
		this->messageBox->setTextWidth(width);
	}

	void showText(const QString &text, int speed = 0, int pointSize = -1, const QString &name = QString(), qreal lineHeight = 35){
		if(text.isEmpty()){
			return;
		}
		this->showTextInProgressFlag = true;
		this->continueSymbolTimeLine->stop();
		this->continueSymbol->setVisible(false);
		this->messageBox->document()->clear();
		if(pointSize > 0 && this->messageBox->font().pointSize() != pointSize){
			QFont font = this->messageBox->font();
			font.setPointSize(pointSize);
			this->messageBox->setFont(font);
		}
		if(speed > 0){
			this->quickShowFlag = false;
			RpgHtmlSplit htmlSplit(text);
			while(htmlSplit.hasNext()){
//				QString word = htmlSplit.getOne();
//				this->messageBox->textCursor().insertHtml(word);
				QString wordLeft = htmlSplit.chopLeft();
				if(!name.isEmpty()){
					wordLeft.prepend(name + "<br>");
				}
				this->messageBox->setHtml(wordLeft);
				this->setLineHeight(lineHeight);
				RpgUtils::msleep(speed);
				if(this->quickShowFlag == true){
					if(!name.isEmpty()){
						this->messageBox->setHtml(name + "<br>" + text);
						this->setLineHeight(lineHeight);
					}else{
						this->messageBox->setHtml(text);
						this->setLineHeight(lineHeight);
					}
					this->quickShowFlag = false;
					break;
				}
			}
		}else{
			if(!name.isEmpty()){
				this->messageBox->setHtml(name + "<br>" + text);
				this->setLineHeight(lineHeight);
			}else{
				this->messageBox->setHtml(text);
				this->setLineHeight(lineHeight);
			}
		}
		this->showTextInProgressFlag = false;
		this->continueSymbol->setVisible(true);
		this->continueSymbolTimeLine->start();
	}

//	void showMessage1(int index){
//		if(index < 0 || index > this->messagesReady.length()){
//			rDebug() << "Index out of range: [0," << this->messagesReady.length() << ")";
//			return;
//		}
//		const RpgDialogMessage currentMessage = this->messagesReady.at(index);
//		const QString currentAvatarString = currentMessage.getCharacterName();
//		const Rpg::AvatarMode currentMode = currentMessage.getAvatarMode();
//		const Rpg::AvatarAround currentAround = currentMessage.getAvatarAround();
//		const QString lastAvatarString = this->lastDialogMessage.getCharacterName();
//		const Rpg::AvatarMode lastMode = this->lastDialogMessage.getAvatarMode();
//		const Rpg::AvatarAround lastAround = this->lastDialogMessage.getAvatarAround();

//		RpgDialogAnimation::Animations animations = 0;
//		if(index == 0){
//			animations.setFlag(RpgDialogAnimation::AnimationDialogShow);
//		}

//		if(currentAvatarString.isEmpty()){
//			// 当前立绘为空

//		}else{
//			// 当前立绘非空
//			QGraphicsPixmapItem *currentAvatarBox = nullptr;
//			QGraphicsPixmapItem *oppositeAvatarBox = nullptr;
//			QPixmap avatarPixmap = RpgAvatar::instance()->getAvatarAdjusted(currentAvatarString, currentMode, this->dialogSize - (2 * QSize(MessageMarginH, MessageMarginV)));
//			if(currentAround == Rpg::AvatarAroundLeft){
//				// 当前立绘处于左侧
//				currentAvatarBox = this->avatarBoxLeft;
//				oppositeAvatarBox = this->avatarBoxRight;
//				this->avatarBoxLeft->setPixmap(avatarPixmap);
//				if(!lastAvatarString.isEmpty() && lastAround == Rpg::AvatarAroundRight){

//				}
//			}else if(currentAround == Rpg::AvatarAroundRight){
//				// 当前立绘处于右侧
//				currentAvatarBox = this->avatarBoxRight;
//				oppositeAvatarBox = this->avatarBoxLeft;
//				this->avatarBoxRight->setPixmap(avatarPixmap);
//			}else{
//				rDebug() << "CurrentAround is not valid.";
//				goto ShowMessage;
//			}

//			if(currentMode == Rpg::AvatarFrame){

//			}else if(currentMode == Rpg::AvatarHalfBodyFront || currentMode == Rpg::AvatarHalfBodyBehind){


//			}
//		}

//ShowMessage:

//	}

	void showMessage(int index);

	void showDialog(){
		this->show();
		emit this->enterDialogMode();
		RpgState::instance()->pushMode(RpgState::DialogMode);
		this->showMessage(0);
	}

	void hideDialog(){
		if(this->continueSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->continueSymbolTimeLine->stop();
		}
		RpgDialogAnimation::Animations animations;
		animations.setFlag(RpgDialogAnimation::AnimationDialogHide);
		if(this->avatarBoxLeft->opacity() != 0.0f){
			animations.setFlag(RpgDialogAnimation::AnimationLeftAvatarHide);
		}
		if(this->avatarBoxRight->opacity() != 0.0f){
			animations.setFlag(RpgDialogAnimation::AnimationRightAvatarHide);
		}
		this->dialogAnimation->runDialogAvatarAnimations(this->lastDialogMessage.getAvatarMode(), this->lastDialogMessage.getAvatarMode(), animations);
		this->dialogAnimation->waitAnimationFinish();
		this->hide();
		this->clearMessages();
		emit this->exitDialogMode();
		if(RpgState::instance()->getTop() == RpgState::DialogMode){
			RpgState::instance()->popMode();
		}else{
			rDebug() << "RpgState: Current top mode isn't DialogMode.";
		}
	}

	void keyReleaseEvent(QKeyEvent *event) Override{
		if(!event->isAutoRepeat()){
			if(!this->isRunning()){
				return;
			}
			if(event->modifiers() != Qt::NoModifier){
				return;
			}
			int key = event->key();
			if(key == Qt::Key_Return || key == Qt::Key_Space){
				if(this->showTextInProgressFlag == true){
					this->quickShowFlag = true;
				}else{
					this->messageIndex++;
					if(this->messageIndex >= this->messagesReady.length()){
						// 会话全部完成
						this->hideDialog();
						this->messageIndex = 0;
						this->lastDialogMessage = RpgDialogMessage("");
						this->end();
						return;
					}else{
						// 如果还没有, 则显示接下来的对话
						this->showMessage(this->messageIndex);
						// TODO: 设置超时时间
					}
				}
			}
		}
	}

//	void receiveKeyPress(int key, Qt::KeyboardModifiers mod){
//		if(!this->isRunning()){
//			return;
//		}
//		if(mod != Qt::NoModifier){
//			return;
//		}
//		rDebug() << QString("Receive key Release: %1 + %2").arg(RpgUtils::keyModifierToString(mod)).arg(RpgUtils::keyToString((Qt::Key)key));
//		if(key == Qt::Key_Return || key == Qt::Key_Space){
//			if(this->showTextInProgressFlag == true){
//				this->quickShowFlag = true;
//			}else{
//				this->messageIndex++;
//				if(this->messageIndex >= this->messagesReady.length()){
//					// 会话全部完成
//					this->hideDialog();
//					this->messageIndex = 0;
//					this->lastDialogMessage = RpgDialogMessage("");
//					this->end();
//					return;
//				}else{
//					// 如果还没有, 则显示接下来的对话
//					this->showMessage(this->messageIndex);
//					// TODO: 设置超时时间
//				}
//			}
//		}
//	}

public:
	RpgDialog(RpgDialogBase *dialogBase, QGraphicsObject *parentItem = nullptr): RpgObject(parentItem){
		this->setTextColor(Qt::white);
		this->messageBox->setFont(RpgFont::instance()->getFont("dialog"));
		rDebug() << this->messageBox->font().family();

		if(dialogBase == nullptr){
			throw RpgNullPointerException("RpgDialogBase not set");
		}
		this->skin = static_cast<RpgDialogBase*>(dialogBase);

		this->messageBox->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
		rDebug() << "Default CSS:" << this->messageBox->document()->defaultStyleSheet();
		this->messageBox->document()->setUndoRedoEnabled(false);
		QTextOption messageTextOption = this->messageBox->document()->defaultTextOption();{
			messageTextOption.setWrapMode(QTextOption::WrapAnywhere);
			this->messageBox->document()->setDefaultTextOption(messageTextOption);
		}


		// 继续三角形动画
		this->continueSymbolTimeLine->setFrameRange(0, this->skin->getContinueSymbolImageLength());
		this->continueSymbolTimeLine->setLoopCount(Rpg::Infinity);

		this->connect(this->continueSymbolTimeLine, &QTimeLine::frameChanged, [this](int frameIndex){
			if(frameIndex >= 0 && frameIndex < this->skin->getContinueSymbolImageLength()){
				const QPixmap framePixmap = this->skin->getContinueSymbolImage(frameIndex);
				//this->continueSymbol->setPos((this->dialogSize.width() - framePixmap.width()) / 2.0, this->dialogSize.height() - (framePixmap.height() / 2.0));
				//this->continueSymbol->setPos((this->dialogSize.width() - framePixmap.width()) / 2.0, this->dialogSize.height() - (framePixmap.height() * 0.75f));
				this->continueSymbol->setPos((this->dialogSize.width() - framePixmap.width()) / 2.0, this->dialogSize.height() - framePixmap.height());
				this->continueSymbol->setPixmap(framePixmap);
			}
		});

		// Z-Value
		this->setZValue(Rpg::ZValueDialog);
		this->box->setZValue(0);
		this->messageBox->setZValue(0.2);
		this->continueSymbol->setZValue(0.3);
		this->avatarBoxRight->setZValue(-0.1); // 默认先置后
		this->avatarBoxLeft->setZValue(-0.1); // 默认先置后

		// 默认消息位置和大小
		this->setMessageTextWidth(this->dialogSize.width() - (2.0 * MessageMarginH));
		this->messageBox->setPos(MessageMarginH, MessageMarginV);

		// 字符下的阴影
		this->messageShadowEffect->setColor(QColor(Qt::black));
		this->messageShadowEffect->setBlurRadius(5.0);
		this->messageShadowEffect->setOffset(2.0, 2.0);
		this->messageBox->setGraphicsEffect(this->messageShadowEffect);

		// 立绘初始位置
		this->avatarBoxLeft->setPos(0, 0);
		this->avatarBoxLeft->hide();
		this->avatarBoxRight->setPos(ScreenWidth, 0);
		this->avatarBoxRight->hide();

		// 坐标放大不影响显示
		//this->setFlag(QGraphicsItem::ItemIgnoresTransformations);

		// 默认不可见
		this->hide();
		this->avatarBoxLeft->hide();
		this->avatarBoxRight->hide();

		RpgState::instance()->registerRpgObject(this, RpgState::DialogMode);
	}

	~RpgDialog(){
		RpgState::instance()->unregisterRpgObject(this, RpgState::DialogMode);
	}

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认字符颜色
	 */
	inline void setTextColor(const QColor &color){
		this->messageBox->setDefaultTextColor(color);
	}

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认字符颜色
	 */
	inline void setTextColor(Qt::GlobalColor color){
		this->messageBox->setDefaultTextColor(QColor(color));
	}

	/**
	 * @brief appendMessage
	 * @param message
	 * 添加消息
	 */
	inline void appendMessage(const RpgDialogMessage &message){
		this->messages.append(message);
	}

	/**
	 * @brief appendMessage
	 * @param list
	 * 添加一段消息
	 */
	inline void appendMessage(const QList<RpgDialogMessage> &list){
		this->messages.append(list);
	}

	/**
	 * @brief appendMessage
	 * @param text
	 * @param name
	 * 添加一段消息
	 */
	inline void appendMessage(const QString &text, const QString &name = QString()){
		this->messages.append(RpgDialogMessage(text, name));
	}

	/**
	 * @brief setFont
	 * @param font
	 * 设置字体
	 */
	inline void setFont(const QFont &font){
		this->messageBox->setFont(font);
	}

	/**
	 * @brief getFont
	 * @return
	 * 获取当前字体
	 */
	QFont getFont() const{
		return this->messageBox->font();
	}

	/**
	 * @brief setLineHeight
	 * @param pixel
	 * 这个函数在输入时没有对其进行高度处理, 我在思考是否能够使用defaultxxx来设置
	 * Solving: 在设置Html之后, 立即执行setLineHeight, document clear之后lineHeight会还原默认
	 * 所以此函数暂时注释, 换成简单函数处理
	 */
//	void setLineHeight(qreal pixel){
//		QTextDocument *document = this->messageBox->document();
//		QTextCursor textCursor = this->messageBox->textCursor();
//		for(QTextBlock i = document->begin(); i != document->end(); i = i.next()){
//			QTextBlockFormat blockFormat = i.blockFormat();
//			blockFormat.setLineHeight(pixel, QTextBlockFormat::FixedHeight);
//			textCursor.setPosition(i.position());
//			textCursor.setBlockFormat(blockFormat);
//			this->messageBox->setTextCursor(textCursor);
//		}
//	}

	void setLineHeight(qreal pixel, int lineHeightType = QTextBlockFormat::FixedHeight){
		QTextCursor textCursor = this->messageBox->textCursor();
		QTextBlockFormat textBlockFormat = textCursor.blockFormat();
		textBlockFormat.setLineHeight(pixel, lineHeightType);
		textCursor.setBlockFormat(textBlockFormat);
		this->messageBox->setTextCursor(textCursor);
	}

	void setLineHeight1(qreal pixel, int lineHeightType = QTextBlockFormat::FixedHeight){
		QTextDocument *doc = this->messageBox->document();
		QTextFrame *rootFrame = doc->rootFrame();
		QTextCursor textCursor = rootFrame->firstCursorPosition();
		QTextBlockFormat textBlockFormat = textCursor.blockFormat();
		textBlockFormat.setLineHeight(pixel, lineHeightType);
		textCursor.setBlockFormat(textBlockFormat);
		this->messageBox->setTextCursor(textCursor);
	}

	/**
	 * @brief setDialogWidth
	 * @param size
	 * 设置对话框大小
	 */
	void setDialogSize(const QSize &size){
		int width = size.width();
		int height = size.height();
		if(width < RpgDialog::MinDialogWidth || width > RpgDialogBase::maxDialogSize().width()){
			rDebug() << "Given width:" << width << "is out of range: (" << RpgDialog::MinDialogWidth << "," << RpgDialogBase::maxDialogSize().width() << ")";
			return;
		}
		if(height < RpgDialog::MinDialogHeight || height > RpgDialogBase::maxDialogSize().height()){
			rDebug() << "Given height:" << height << "is out of range: (" << RpgDialog::MinDialogHeight << "," << RpgDialogBase::maxDialogSize().height() << ")";
			return;
		}
		this->dialogSize.setWidth(width);
		this->dialogSize.setHeight(height);
	}

	QSize getDialogSize() const{
		return this->dialogSize;
	}

	/**
	 * @brief clearMessages
	 * 清理消息内容
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
		this->dialogaAlign = align;
	}

	/**
	 * @brief getDialogAlign
	 * @return
	 * 返回对话框位置
	 */
	inline Rpg::BlockAlign getDialogAlign() const{
		return this->dialogaAlign;
	}

	void run() Override{
		RpgObject::run();
		// 重新设定其Scene
		if(RpgView::instance()->scene() == nullptr){
			rDebug() << "RpgView not loaded scene yet.";
			this->end();
			throw RpgNullPointerException("RpgView::instance()->scene()");
		}else{
			// 如果该对象已装载至其他的scene的时候, 这个函数能自动将这个对象在其他scene删除后添加至该scene
			RpgView::instance()->scene()->addItem(this);
		}

		// 确定继续三角形存在
		for(int i = 0; i < this->skin->getContinueSymbolImageLength(); i++){
			if(this->skin->getContinueSymbolImage(i).isNull()){
				rDebug() << "Continue symbol frame " << i << " is null.";
				this->end();
				return;
			}
		}
		// 复制消息组件
		// 在这里Ready是用来显示的, messages是用来事先设置的
		if(this->messages.isEmpty()){
			rDebug() << "Detected message is empty, exitted";
			this->end();
			return;
		}else{
			this->messagesReady.clear();
			this->messagesReady = this->messages;
		}

		// 设置对话框背景, 支持每次对话框形状不同
		QPointF dialogPos = RpgUtils::getDialogPos(this->dialogaAlign, this->dialogSize, RpgDialogBase::MarginH, RpgDialogBase::MarginV);

		this->box->setPixmap(this->skin->getDialogImage(this->dialogSize));
		this->box->setPos(dialogPos);

		QPixmap continueSymbol0 = this->skin->getContinueSymbolImage(0);
		QPointF continueSymbolPos((this->dialogSize.width() - continueSymbol0.width()) / 2, this->dialogSize.height() - continueSymbol0.height());
		this->continueSymbol->setVisible(false);

		this->showDialog();
	}

	int waitForComplete(){
		if(!this->isRunning()){
			rDebug() << "RpgDialog is not running.";
			return -1;
		}
		QEventLoop eventLoop(this);
		this->connect(this, &RpgDialog::exitDialogMode, &eventLoop, &QEventLoop::quit);
		eventLoop.exec();
		return 0;
	}

signals:
	void enterDialogMode();
	void exitDialogMode();
};

#endif // RPGDIALOG_H
