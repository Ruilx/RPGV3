#include "RpgDialogMessage.h"

RpgDialogMessage::RpgDialogMessage(const QString &text, const QString &name){
	this->text = text;
	this->name = name;
}

RpgDialogMessage::RpgDialogMessage(std::initializer_list<std::pair<QString, QVariant> > params){
	std::for_each(params.begin(), params.end(), [this](const std::pair<QString, QVariant> &param){
		if(param.first == "name"){
			if(!param.second.canConvert(QVariant::String)){
				rError() << "Cannot convert 'name' from" << QMetaType::typeName(param.second.type()) << "to String.";
				return;
			}
			this->setName(param.second.toString());
		}else if(param.first == "text"){
			if(!param.second.canConvert(QVariant::String)){
				rError() << "Cannot convert 'text' from" << QMetaType::typeName(param.second.type()) << "to String.";
				return;
			}
			this->setText(param.second.toString());
		}else if(param.first == "characterName"){
			if(!param.second.canConvert(QVariant::String)){
				rError() << "Cannot convert 'characterName' from" << QMetaType::typeName(param.second.type()) << "to String.";
				return;
			}
			this->setCharacterName(param.second.toString());
		}else if(param.first == "avatarMode" || param.first == "mode"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'avatarMode' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setAvatarMode((Rpg::AvatarMode)param.second.toInt());
		}else if(param.first == "avatarAround" || param.first == "around"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'avatarAround' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setAvatarAround((Rpg::AvatarAround)param.second.toInt());
		}else if(param.first == "avatarMirroringMode" || param.first == "mirroring"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'avatarMirroringMode' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setAvatarMirroringMode((Rpg::AvatarMirroringMode)param.second.toInt());
		}else if(param.first == "pointSize" || param.first == "point"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'pointSize' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setPointSize(param.second.toInt());
		}else if(param.first == "speed"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'speed' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setSpeed(param.second.toInt());
		}else if(param.first == "waitTime" || param.first == "wait"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'waitTime' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setWaitTime(param.second.toInt());
		}else if(param.first == "lineHeight"){
			if(!param.second.canConvert(QVariant::Double)){
				rError() << "Cannot convert 'lineHeight' from" << QMetaType::typeName(param.second.type()) << "to Double.";
				return;
			}
			this->setLineHeight(param.second.toDouble());
		}else if(param.first == "textAlign" || param.first == "align"){
			if(!param.second.canConvert(QVariant::Int)){
				rError() << "Cannot convert 'textAlign' from" << QMetaType::typeName(param.second.type()) << "to Int.";
				return;
			}
			this->setTextAlign((Rpg::TextAlign)param.second.toInt());
		}else{
			rError() << "Not a valid RpgDialogMessage key '" << param.first << "'";
			return;
		}
	});
}
