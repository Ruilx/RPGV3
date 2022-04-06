#ifndef RPGCHOICEMESSAGE_H
#define RPGCHOICEMESSAGE_H

#include <Rpg/Rpg.h>

class RpgChoiceMessage
{
	QString text;
	bool enabled = true;
	qreal lineHeight = 35;
	QString value;
	//bool selected = false;
public:
	RpgChoiceMessage(const QString &text, bool enabled = true, const QString &value = QString()){
		this->text = text;
		this->enabled = enabled;
		this->value = value.isEmpty() ? text : value;
	}

	inline void setText(const QString &text){ this->text = text; }
	inline const QString &getText() const{ return this->text; }
	inline void setEnabled(bool enabled){ this->enabled = enabled; }
	inline bool getEnabled() const{ return this->enabled; }
	inline void setValue(const QString &value){ this->value = value; }
	inline const QString &getValue() const{ return this->value; }
//	inline void setSelected(bool selected){ this->selected = selected; }
//	inline bool getSelected() const { return this->selected; }
	inline void setLineHeight(qreal lineHeight){ this->lineHeight = lineHeight; }
	inline qreal getLineHeight() const{ return this->lineHeight; }
};

#endif // RPGCHOICEMESSAGE_H
