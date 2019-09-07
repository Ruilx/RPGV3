#ifndef RPGDIALOGMESSAGE_H
#define RPGDIALOGMESSAGE_H

#include <QtCore>

#include <Rpg/Global.h>
#include <Rpg/Rpg.h>

class RpgDialogMessage
{
	QString name; // 消息主角名
	QString text; // 消息主体
	QString characterName; // 主角立绘或头像, 留空不显示
	Rpg::AvatarMode mode = Rpg::AvatarModeUnknown; // 主角立绘显示模式
	Rpg::AvatarAround around = Rpg::AvatarAroundUnknown; // 主角立绘显示方向
	int pointSize = 18; // 字体大小
	int speed = (int)Rpg::SingleWordSpeedFast; // 消息出字速度, 0表示立即出字
	int waitTime = -1; // 出字之后自动等待时间, 0表示立即返回, -1表示永久等待用户输入并显示继续符号
	qreal lineHeight = 35;
public:

	RpgDialogMessage(const QString &text, const QString &name = QString()){
		this->text = text;
		this->name = name;
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
