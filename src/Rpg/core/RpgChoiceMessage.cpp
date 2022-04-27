#include "RpgChoiceMessage.h"

RpgChoiceMessage::RpgChoiceMessage(const QString &text, bool enabled, const QString &value){
	this->setText(text);
	this->setEnabled(enabled);
	this->setValue(value);
}

RpgChoiceMessage::RpgChoiceMessage(std::initializer_list<std::pair<QString, QVariant> > params){
	std::for_each(params.begin(), params.end(), [this](const std::pair<QString, QVariant> &param){
		if(param.first == "text"){
			if(!param.second.canConvert(QVariant::String)){
				rError() << "Cannot convert 'text' from" << QMetaType::typeName(param.second.type()) << "to String.";
				return;
			}
			this->setText(param.second.toString());
		}else if(param.first == "enabled"){
			if(!param.second.canConvert(QVariant::Bool)){
				rError() << "Cannnot convert 'enabled' from" << QMetaType::typeName(param.second.type()) << "to Bool.";
				return;
			}
			this->setEnabled(param.second.toBool());
		}else if(param.first == "value"){
			if(!param.second.canConvert(QVariant::String)){
				rError() << "Cannot convert 'value' from" << QMetaType::typeName(param.second.type()) << "to String.";
				return;
			}
			this->setValue(param.second.toString());
		}else if(param.first == "lineHeight"){
			if(!param.second.canConvert(QVariant::Double)){
				rError() << "Cannot convert 'lineHeight' from" << QMetaType::typeName(param.second.type()) << "to Double.";
				return;
			}
			this->setLineHeight(param.second.toDouble());
		}else if(param.first == "align" || param.first == "textAlign"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'textAlign' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setTextAlign((Rpg::TextAlign)param.second.toInt());
		}else{
			rError() << "Not a valid RpgChoiceMessage key '" << param.first << "'";
			return;
		}
	});
	if(this->getValue().isEmpty()){
		this->setValue(this->getText());
	}
}
