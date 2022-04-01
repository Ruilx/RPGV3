#ifndef RPGDIALOGMESSAGE_H
#define RPGDIALOGMESSAGE_H

#include <Rpg/Rpg.h>

/**
 * @brief The RpgDialogMessage class
 * RPG 对话框消息类
 * 管理对话框消息的结构体, 存储着
 * 1. 主角名
 * 2. 消息主体
 * 3. 立绘名
 * 4. 立绘显示模式
 * 5. 立绘方向(左/右)
 * 6. 立绘镜像(正常/镜像)
 * 7. 字体大小
 * 8. 出字速度(Rpg::SingleWordSpeed)
 * 9. 完成出字后等待用户时间
 * 10. lineheight
 */
class RpgDialogMessage
{
	QString name; // 消息主角名
	QString text; // 消息主体
	QString characterName; // 主角立绘或头像, 留空不显示
	Rpg::AvatarMode mode = Rpg::AvatarModeUnknown; // 主角立绘显示模式
	Rpg::AvatarAround around = Rpg::AvatarAroundUnknown; // 主角立绘显示方向
	Rpg::AvatarMirroringMode mirroring = Rpg::AvatarNormal; // 主角立绘显示镜像(左右)
	int pointSize = 16; //18; // 字体大小
	int speed = (int)Rpg::SingleWordSpeedFast; // 消息出字速度, 0表示立即出字
	int waitTime = -1; // 出字之后自动等待时间, 0表示立即返回, -1表示永久等待用户输入并显示继续符号
	qreal lineHeight = 35;
public:

	RpgDialogMessage(const QString &text, const QString &name = QString()){
		this->text = text;
		this->name = name;
	}
	RpgDialogMessage(std::initializer_list<std::pair<QString, QVariant>> params){
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
			}else{
				rError() << "Not a valid RpgDialogMessage key '" << param.first << "'";
				return;
			}
		});
	}

	inline void setName(const QString &name){ this->name = name; }
	inline const QString getName() const{ return this->name; }
	inline void setText(const QString &text){ this->text = text; }
	inline const QString getText() const{ return this->text; }
	inline void setCharacterName(const QString &characterName){ this->characterName = characterName; }
	inline const QString getCharacterName() const{ return this->characterName; }
	inline void setAvatarMode(Rpg::AvatarMode mode){ this->mode = mode; }
	inline Rpg::AvatarMode getAvatarMode() const{ return this->mode; }
	inline void setAvatarAround(Rpg::AvatarAround around){ this->around = around; }
	inline Rpg::AvatarAround getAvatarAround() const{ return this->around; }
	inline void setAvatarMirroringMode(Rpg::AvatarMirroringMode mirroring){ this->mirroring = mirroring; }
	inline Rpg::AvatarMirroringMode getAvatarMirroringMode() const{ return this->mirroring; }
	inline void setPointSize(int pointSize){ this->pointSize = pointSize; }
	inline int getPointSize() const{ return this->pointSize; }
	inline void setSpeed(int speed){ this->speed = speed; }
	inline int getSpeed() const{ return this->speed; }
	inline void setWaitTime(int waitTime){ this->waitTime = waitTime; }
	inline int getWaitTime() const{ return this->waitTime; }
	inline void setLineHeight(qreal lineHeight){ this->lineHeight = lineHeight; }
	inline qreal getLineHeight() const{ return this->lineHeight; }
};

#endif // RPGDIALOGMESSAGE_H
