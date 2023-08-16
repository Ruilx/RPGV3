#ifndef RPGDIALOGOBJECT_H
#define RPGDIALOGOBJECT_H

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgDialogBase.h>
#include <Rpg/core/RpgDialogAnimation.h>

#include <QGraphicsDropShadowEffect>

class RpgDialogObject : public RpgObject
{

protected:
	RpgDialogBase *skin = nullptr;

	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(this);

	RpgDialogAnimation *dialogAnimation = new RpgDialogAnimation(this->box, nullptr, nullptr, 300, QEasingCurve::OutQuad, this);

	QSize dialogSize = RpgDialogBase::dialogSize();

	Rpg::BlockAlign dialogAlign = Rpg::AlignBottom;

	QFont font;

	QHash<Rpg::SoundEffect, QString> soundEffects = QHash<Rpg::SoundEffect, QString>({
		{Rpg::SoundEffect_Select, ""},
		{Rpg::SoundEffect_Accept, ""},
		{Rpg::SoundEffect_Banned, ""},
		{Rpg::SoundEffect_Close, ""},
		{Rpg::SoundEffect_Continuous, ""},
	});
	void playSound(Rpg::SoundEffect soundEffect, qreal volume = 1.0f, int times = 1);


public:
	const int MessageMarginH = 10;
	const int MessageMarginV = 5;

	const char *fontName = "dialog";

	// 构造
	RpgDialogObject(RpgDialogBase *dialogBase, QGraphicsObject *parent = nullptr);
	~RpgDialogObject();

	// 预置字体
	inline void setFont(const QFont &font){ this->font = font; }
	inline const QFont &getFont() const { return this->font; }

	// 窗口大小
	inline void setDialogSize(const QSize &size){ this->dialogSize = size; }
	inline const QSize &getSize() const { return this->dialogSize; }

	// 窗口对齐
	inline void setDialogAlign(Rpg::BlockAlign align){ this->dialogAlign = align; }
	inline Rpg::BlockAlign getDialogAlign() const { return this->dialogAlign; }

	void run() override;
	int wait();
	void end() override;

private:
	inline int maxTextWidth(){
		return qMax(this->dialogSize.width() - (2 * this->MessageMarginH), 0);
	}

	void showDialog();
	void hideDialog();

signals:
	void enterDialogMode();
	void exitDialogMode();
};

#endif // RPGDIALOGOBJECT_H
