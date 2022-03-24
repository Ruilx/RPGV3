#ifndef RPGAVATAR_H
#define RPGAVATAR_H

#include <QHash>
#include <QString>
#include <QPixmap>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgFileManager.h>

#include <Rpg/exception/RpgRuntimeException.h>

/**
 * @brief The RpgAvatar class
 * RpgAvatar维护着全局头像的内容, 使用名字来进行提取
 * 自动会在FileManager中进行缓存
 */
class RpgAvatar
{
	static RpgAvatar *_instance;

	/**
	 * @brief avatars
	 * 存的头像源
	 */
	QHash<QString, QPixmap> avatars;

	/**
	 * @brief avatarAdjusted
	 * 这个是在生成大小相似的图像的时候, 为了防止每一次取得内容都需要处理
	 * 特将已处理的图片缓存至此, 使用的时候若已取得了该内容, 直接返回缓存
	 * key = name + "-" + size.width() + "," + size.height() + "-" + Rpg::AvatarMode
	 */
	QHash<QString, QPixmap> avatarAdjusted;

	const qreal avatarMaxScaleRatio = 0.8f;
	const qreal avatarMaxWidth = (1.0f / 3);

public:
	static RpgAvatar *instance(){
		if(_instance == nullptr){
			_instance = new RpgAvatar();
		}
		return _instance;
	}

	explicit RpgAvatar(){
		avatars.clear();
	}

	~RpgAvatar(){
		if(RpgAvatar::_instance != nullptr){
			delete RpgAvatar::_instance;
		}
	}

	void setAvatar(const QString &name, const QPixmap &avatar){
		if(avatar.isNull()){
			rDebug() << CodePath << "The name: " << name << " of pixmap is null";
			return;
		}
		this->avatars.insert(name, avatar);
	}

	void setAvatar(const QString &name, const QString &fileManagerName){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::AvatarFile, fileManagerName);
		QPixmap pixmap(filename);
		if(pixmap.isNull()){
			rDebug() << CodePath << "The name: " << name << " of filename: " << filename << " isn't a pixmap";
			return;
		}
		this->avatars.insert(name, pixmap);
	}

	/**
	 * @brief getAvatar
	 * @param name
	 * @return
	 * 自动会从FileManager中自动加载Avatar
	 */
	const QPixmap getAvatar(const QString &name){
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

	/**
	 * @brief getAvatarAdjusted
	 * @param name
	 * @param mode
	 * @return
	 * 按照对话框大小来调整图片大小, 并缓存至avatarAdjusted
	 */
	const QPixmap getAvatarAdjusted(const QString &name, Rpg::AvatarMode mode, const QSize &size){
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

	void _dumpAvatars(){
		for(QHash<QString, QPixmap>::ConstIterator i = this->avatars.constBegin(); i != this->avatars.constEnd(); i++){
			rDebug() << i.key() << "=>" << i.value();
		}
	}
};

#ifndef rpgAvatar
#	define rpgAvatar (RpgAvatar::instance())
#endif

#endif // RPGAVATAR_H
