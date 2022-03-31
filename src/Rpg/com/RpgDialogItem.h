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

	RpgDialogBase *skin = nullptr;

	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);
	QGraphicsTextItem *messageBox = new QGraphicsTextItem(this->box);
	QGraphicsPixmapItem *continueSymbol = new QGraphicsPixmapItem(this->box);

	QGraphicsPixmapItem *avatarBoxRight = new QGraphicsPixmapItem(this);
	QGraphicsPixmapItem *avatarBoxLeft = new QGraphicsPixmapItem(this);

	QGraphicsDropShadowEffect *messageShadowEffect = new QGraphicsDropShadowEffect(this);

	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, this->avatarBoxLeft, this->avatarBoxRight, 300, QEasingCurve::OutQuad, this);

	QSize dialogSize = RpgDialogBase::dialogSize();

	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void boundingRect();

	void keyReleaseEvent(QKeyEvent *event) override;
public:
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	const char *FontName = "dialog";
private:
	QList<RpgDialogMessage> messages;
	int messageIndex = 0;
	QList<RpgDialogMessage>::ConstIterator lastDialogMessage;

	bool showTextInProgressFlag = false;
	bool quickShowFlag = false;

	QTimeLine *continueSymbolTimeLine = new QTimeLine(1000, this);

	inline void setMessageTextWidth(qreal width){
		this->messageBox->setTextWidth(width);
	}

public:
	inline void setTextColor(const QColor &color){
		this->messageBox->setDefaultTextColor(color);
	}

	inline void setTextColor(Qt::GlobalColor color){
		this->messageBox->setDefaultTextColor(QColor(color));
	}

	inline void appendMessage(const RpgDialogMessage &message){
		this->messages.append(message);
	}

	inline void appendMessage(const QList<RpgDialogMessage> &messages){
		this->messages.append(messages);
	}

	inline void appendMessage(const QString &text, const QString &name = QString()){
		this->messages.append(RpgDialogMessage(text, name));
	}

	inline void setFont(const QFont &font){
		this->messageBox->setFont(font);
	}

	inline QFont getFont() const{
		return this->messageBox->font();
	}

	void setLineHeight(qreal pixel, int lineHeightType = QTextBlockFormat::FixedHeight);


	RpgDialogItem(RpgDialogBase *dialogBase, QGraphicsObject *parent = nullptr);
	~RpgDialogItem();
};

#endif // RPGDIALOGITEM_H
