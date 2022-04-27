#ifndef RPGSPINVALUE_H
#define RPGSPINVALUE_H

#include <Rpg/Rpg.h>

class RpgSpinValueItem
{
	QString text;
	QString value;
	bool enable = true;

public:
	explicit RpgSpinValueItem(const QString &text, bool enable = true, const QString &value = QString());
	RpgSpinValueItem(std::initializer_list<std::pair<QString, QVariant>> params);
	RpgSpinValueItem(){
		this->setText("");
		this->setValue("");
	}

	inline void setText(const QString &text){ this->text = text.isEmpty() ? "<nil>" : text; }
	inline const QString &getText() const { return this->text; }
	inline void setEnable(bool enable){ this->enable = enable; }
	inline bool getEnable() const { return this->enable; }
	inline void setValue(const QString &value){ this->value = value.isEmpty() ? this->getText() : value; }
	inline const QString getValue() const { return this->value; }
};

class RpgSpinValue{
	QList<RpgSpinValueItem> choices;

	int currentChoiceIndex = 0;

	RpgSpinValueItem nullItem;

	bool indexCirculation = true;
public:
	RpgSpinValue(const QList<RpgSpinValueItem> &choices){
		if(choices.length() > 0){
			this->appendChoice(choices);
		}
	}

	RpgSpinValue(std::initializer_list<std::initializer_list<std::pair<QString, QVariant>>> params){
		std::for_each(params.begin(), params.end(), [this](const std::initializer_list<std::pair<QString, QVariant>> &param){
			this->appendChoice(RpgSpinValueItem(param));
		});
	}

	inline void appendChoice(const QString &text, bool enable = true, const QString &value = QString()){ this->appendChoice(RpgSpinValueItem(text, enable, value)); }
	inline void appendChoice(const RpgSpinValueItem &item){ this->choices.append(item); }
	inline void appendChoice(const QList<RpgSpinValueItem> &items){ this->choices.append(items); }
	inline int getChoicesCount() const { return this->choices.length(); }
	inline void clearChoices() { this->choices.clear(); }

	const RpgSpinValueItem &at(int i) const{
		if(i >= this->choices.length()){
			rError() << "Given index:" << i << "is out of range: [0," << this->choices.length() << ").";
			return this->nullItem;
		}
		return this->choices.at(i);
	}

	inline void setCurrentChoiceIndex(int i){
		if(i >= this->choices.length()){
			rError() << "Given index:" << i << "is out of range: [0," << this->choices.length() << ").";
			return;
		}
		this->currentChoiceIndex = i;
	}

	inline int getCurrentChoiceIndex() const { return this->currentChoiceIndex; }

	inline const RpgSpinValueItem &getCurrentSpinValueItem() const {
		if(this->currentChoiceIndex < 0 || this->currentChoiceIndex >= this->choices.length()){
			rError() << "this->currentChoiceIndex:" << this->currentChoiceIndex << "is out of range: [0," << this->choices.length() << ").";
			return this->nullItem;
		}
		return this->choices.at(this->currentChoiceIndex);
	}

	const RpgSpinValueItem &next(bool *ok = nullptr);
	const RpgSpinValueItem &prev(bool *ok = nullptr);
};

#endif // RPGSPINVALUE_H
