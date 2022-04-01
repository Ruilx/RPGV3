#include "RpgAvatar.h"

RpgAvatar *RpgAvatar::_instance = nullptr;

RpgAvatar *RpgAvatar::instance(){
	if(_instance == nullptr){
		_instance = new RpgAvatar();
	}
	return _instance;
}

RpgAvatar::RpgAvatar(){
	avatars.clear();
}

RpgAvatar::~RpgAvatar(){
	if(RpgAvatar::_instance != nullptr){
		delete RpgAvatar::_instance;
	}
}

void RpgAvatar::setAvatar(const QString &name, const QPixmap &avatar){
	if(avatar.isNull()){
		rDebug() << CodePath << "The name: " << name << " of pixmap is null";
		return;
	}
	this->avatars.insert(name, avatar);
}

void RpgAvatar::setAvatar(const QString &name, const QString &fileManagerName){
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::AvatarFile, fileManagerName);
	QPixmap pixmap(filename);
	if(pixmap.isNull()){
		rDebug() << CodePath << "The name: " << name << " of filename: " << filename << " isn't a pixmap";
		return;
	}
	this->avatars.insert(name, pixmap);
}

const QPixmap RpgAvatar::getAvatar(const QString &name){
	if(this->avatars.contains(name)){
		return this->avatars.value(name);
	}else{
		this->setAvatar(name, name);
		if(this->avatars.contains(name)){
			return this->avatars.value(name);
		}
		rDebug() << CodePath << "Cannot found avatar pixmap with name: " << name << ".";
		throw RpgMapKeyNotFoundException(name);
	}
}

const QPixmap RpgAvatar::getAvatarAdjusted(const QString &name, Rpg::AvatarMode mode, const QSize &size){
	QString key = QString("%1-%2,%3-%4").arg(name).arg(size.width()).arg(size.height()).arg(mode);
	QPixmap pixmap = this->avatarAdjusted.value(key);
	if(pixmap.isNull()){
		pixmap = this->getAvatar(name);
		switch(mode){
			case Rpg::AvatarHalfBodyFront:
			case Rpg::AvatarHalfBodyBehind:
				pixmap = pixmap.scaledToHeight(ScreenHeight * RpgAvatar::avatarMaxScaleRatio, Qt::SmoothTransformation);
				if(pixmap.width() >= ScreenWidth * RpgAvatar::avatarMaxScaleRatio){
					rDebug() << CodePath << "pixmap size:" << pixmap.size() << " width is bigger than ScreenWidth * RpgAvatar::avatarMaxScaleRatio(" << RpgAvatar::avatarMaxScaleRatio << "):" << (ScreenWidth * 0.8f);
					pixmap = pixmap.scaledToWidth(ScreenWidth * RpgAvatar::avatarMaxScaleRatio, Qt::SmoothTransformation);
				}
				break;
			case Rpg::AvatarFrame:
			{
				// 边长 = min(min(对话框宽(已经减去了隔离宽度的), 对话框高(已经减去了隔离高度的)), 屏幕宽度 / 3)
				qreal side = qMin((const double)qMin(size.width(), size.height()), ScreenWidth * RpgAvatar::avatarMaxWidth);
				pixmap = pixmap.scaled(side, side, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
				break;
			case Rpg::AvatarModeUnknown:
			default:
				throw RpgRuntimeException("Touched Rpg::AvatarAroundUnknown or default!");
		}
		this->avatarAdjusted.insert(key, pixmap);
	}
	return pixmap;
}

void RpgAvatar::_dumpAvatars(){
	for(QHash<QString, QPixmap>::ConstIterator i = this->avatars.constBegin(); i != this->avatars.constEnd(); i++){
		rDebug() << i.key() << "=>" << i.value();
	}
}
