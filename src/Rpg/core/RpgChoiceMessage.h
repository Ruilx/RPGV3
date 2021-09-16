#ifndef RPGCHOICEMESSAGE_H
#define RPGCHOICEMESSAGE_H

#include <Rpg/Rpg.h>

class RpgChoiceMessage
{
	QString text;
	bool enabled = true;
	qreal lineHeight = 35;
public:
	RpgChoiceMessage(const QString &text, bool enabled = true){
		this->text = text;
		this->enabled = enabled;
	}

	inline void setText(const QString &text){ this->text = text; }
	inline const QString getText() const{ return this->text; }
	inline void setEnabled(bool enabled){ this->enabled = enabled; }
	inline bool getEnabled() const{ return this->enabled; }
	inline void setLineHeight(qreal lineHeight){ this->lineHeight = lineHeight; }
	inline qreal getLineHeight() const{ return this->lineHeight; }
};

#endif // RPGCHOICEMESSAGE_H
