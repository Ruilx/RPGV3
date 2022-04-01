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
	static RpgAvatar *instance();

	explicit RpgAvatar();

	~RpgAvatar();

	void setAvatar(const QString &name, const QPixmap &avatar);

	void setAvatar(const QString &name, const QString &fileManagerName);

	/**
	 * @brief getAvatar
	 * @param name
	 * @return
	 * 自动会从FileManager中自动加载Avatar
	 */
	const QPixmap getAvatar(const QString &name);

	/**
	 * @brief getAvatarAdjusted
	 * @param name
	 * @param mode
	 * @return
	 * 按照对话框大小来调整图片大小, 并缓存至avatarAdjusted
	 */
	const QPixmap getAvatarAdjusted(const QString &name, Rpg::AvatarMode mode, const QSize &size);

	void _dumpAvatars();
};

#ifndef rpgAvatar
#	define rpgAvatar (RpgAvatar::instance())
#endif

#endif // RPGAVATAR_H
