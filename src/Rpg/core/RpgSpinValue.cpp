#include "RpgSpinValue.h"


RpgSpinValueItem::RpgSpinValueItem(const QString &text, bool enable, const QString &value){
	this->setText(text);
	this->setEnable(enable);
	this->setValue(value);
}

RpgSpinValueItem::RpgSpinValueItem(std::initializer_list<std::pair<QString, QVariant> > params){
	std::for_each(params.begin(), params.end(), [this](const std::pair<QString, QVariant> &param){
		if(param.first == "text"){
			if(!param.second.canConvert(QVariant::String)){
				rError() << "Cannot convert 'text' from" << QMetaType::typeName(param.second.type()) << "to String.";
				return;
			}
			this->setText(param.second.toString());
		}else if(param.first == "enable"){
			if(!param.second.canConvert(QVariant::Bool)){
				rError() << "Cannot convert 'enable' from" << QMetaType::typeName(param.second.type()) << "to Bool.";
				return;
			}
			this->setEnable(param.second.toBool());
		}else if(param.first == "value"){
			if(!param.second.canConvert(QVariant::String)){
				rError() << "Cannot convert 'value' from" << QMetaType::typeName(param.second.type()) << "to String.";
				return;
			}
			this->setValue(param.second.toString());
		}else{
			rError() << "Not a valid RpgSpinValue key '" << param.first << "'";
			return;
		}
	});
	if(this->getValue().isEmpty()){
		this->setValue(this->getText());
	}
}

const RpgSpinValueItem &RpgSpinValue::next(bool *ok){
	if(ok){
		*ok = false;
	}
	if(this->choices.isEmpty()){
		rError() << "Calling next when choicesList is empty.";
		return this->nullItem;
	}
	this->currentChoiceIndex++;
	rDebug() << "CurrentChoiceIndex:" << currentChoiceIndex;
	if(this->currentChoiceIndex >= this->choices.length()){
		if(this->indexCirculation == false){
			return this->nullItem;
		}
		this->currentChoiceIndex = 0;
	}
	rDebug() << "CurrentChoiceIndex:" << currentChoiceIndex;
	if(ok){
		*ok = true;
	}
	return this->at(this->currentChoiceIndex);
}

const RpgSpinValueItem &RpgSpinValue::prev(bool *ok){
	if(ok){
		*ok = false;
	}
	if(this->choices.isEmpty()){
		rError() << "Calling prev when choicesList is empty.";
		return this->nullItem;
	}
	this->currentChoiceIndex--;
	rDebug() << "CurrentChoiceIndex:" << currentChoiceIndex;
	if(this->currentChoiceIndex < 0){
		if(this->indexCirculation == false){
			return this->nullItem;
		}
		this->currentChoiceIndex = this->choices.length() -1;
	}
	rDebug() << "CurrentChoiceIndex:" << currentChoiceIndex;
	if(ok){
		*ok = true;
	}
	return this->at(this->currentChoiceIndex);
}
