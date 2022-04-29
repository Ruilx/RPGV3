#ifndef RPGINPUTITEM_H
#define RPGINPUTITEM_H

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgDialogAnimation.h>

#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>

class RpgInputItem : public RpgObject
{
	Q_OBJECT

	RpgDialogBase *skin = nullptr;

	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);
	QGraphicsTextItem *messageBox = new QGraphicsTextItem(this->box);
	QString message;

	QGraphicsDropShadowEffect *textShadowEffect = new QGraphicsDropShadowEffect(this);

	QLineEdit *input = new QLineEdit();
	QGraphicsProxyWidget *inputItem = new QGraphicsProxyWidget(this->box);

	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, nullptr, nullptr, 300, QEasingCurve::OutQuad, this);

	QSize dialogSize = RpgDialogBase::dialogSize();

	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;
	Rpg::TextAlign messageAlign = Rpg::AlignLeft;

	// timer
	void timerEvent(QTimerEvent *event) override;
	int timerId = -1;
	bool timerProcessing = false;

	int timeout = 0;

	// 按键
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;

	QColor textColor = QColor(Qt::white);

	int inputItemTop = 0;

	int speed = Rpg::SingleWordSpeedFast;
public:
	// 消息内部间距
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	// 消息使用内部自体名
	const char *FontName = "dialog";

	enum SoundEffect{
		SoundEffect_Accept = 1,
		SoundEffect_Banned,
	};

private:
	//
	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	//
	QHash<SoundEffect, QString> soundEffects = QHash<SoundEffect, QString>({
		{SoundEffect_Accept, "accept"},
		{SoundEffect_Banned, "banned"},
	});

	void playSound(SoundEffect soundEffect, qreal volume = 1.0f, int times = 1);

public:
	// 提示文本
	inline void setMessage(const QString &message){ this->message = message; }
	inline const QString &getMessage() const{ return this->message; }

	// 文字颜色
	inline void setTextColor(const QColor &color){ this->messageBox->setDefaultTextColor(color); this->textColor = color; }
	inline void setTextColor(Qt::GlobalColor color){ this->messageBox->setDefaultTextColor(QColor(color)); this->textColor = QColor(color); }

	// 输入框文本
	inline void setInputText(const QString &text){ this->input->setText(text); }
	inline QString getInputText() const { return this->input->text(); }

	// 超时时间
	inline void setTimeout(int timeout){ this->timeout = timeout; }
	inline int getTimeout() const { return this->timeout; }

	// 出字速度
	inline void setSpeed(int speed){ this->speed = speed; }
	inline int getSpeed() const { return this->speed; }

	// 字体
	inline void setFont(const QFont &font){ this->messageBox->setFont(font); this->input->setFont(font); }
	void setFont(const QString &name, int pointSize = -1, int weight = -1, bool italic = false);
	inline QFont getFont() const { return this->messageBox->font(); }

	void setDialogSize(const QSize &size);
	inline const QSize getDialogSize() const { return this->dialogSize; }

	void setDialogAlign(Rpg::BlockAlign align){ this->dialogAlign = align; }
	inline Rpg::BlockAlign getDialogAlign() const { return this->dialogAlign; }

	void setSoundEffect(SoundEffect soundEffect, const QString &name){ this->soundEffects.insert(soundEffect, name); }
	const QString getSoundEffect(SoundEffect soundEffect) const { return this->soundEffects.value(soundEffect); }

	RpgInputItem(RpgDialogBase *skin, RpgObject *parent = nullptr);
	~RpgInputItem();

	void run() override;
	int waitingForComplete();
	void end() override;

	const QString getValue();
private:
	void showDialog();
	void hideDialog();

	void showMessage();

signals:
	void enterDialogMode();
	void exitDialogMode();
};

#endif // RPGINPUTITEM_H
