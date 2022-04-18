#include "RpgSpinValue.h"


RpgSpinValue::RpgSpinValue(const QString &text, bool enable, const QString &value){
	this->setText(text);
	this->setEnable(enable);
	this->setValue(value);
}

RpgSpinValue::RpgSpinValue(std::initializer_list<std::pair<QString, QVariant> > params){
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
}
