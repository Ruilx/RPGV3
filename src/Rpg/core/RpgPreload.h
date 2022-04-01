#ifndef RPGPRELOAD_H
#define RPGPRELOAD_H

#include <QtCore>

#include <Rpg/Global.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/core/RpgFileManager.h>

#include <Rpg/exception/RpgRuntimeException.h>

/**
 * @brief The RpgPreload class
 * Preload 组件
 * 用来解析脚本启动时的配置文件信息, 载入的内容等
 * 程序启动时需要检测:
 * 1. 配置文件是否存在
 * 2. 配置文件中Json是否合法
 * 3. 配置文件中的相关信息是否全面:
 * 3.1. data, 包括map, mapscene, music, sound, image, tale等
 * 3.2. 程序启动时的一些配置(暂时没有固化)
 */

class RpgPreload
{
	QString bootScriptSceneName;

	const QMap<QString, QUrl> parseDict(const QJsonObject &object);

	bool parseValueToDict(const QJsonValue &value, void(*cb)(const QMap<QString, QUrl> &));

	bool handlePreload(const QByteArray &loadingJson);

	bool parseInit(const QJsonValue &value);
	bool parseMaps(const QJsonValue &value);
	bool parseMusics(const QJsonValue &value);
	bool parseSoundEffects(const QJsonValue &value);
	bool parseImages(const QJsonValue &value);
	bool parseLyrics(const QJsonValue &value);
	bool parseFonts(const QJsonValue &value);
	bool parseAvatars(const QJsonValue &value);
	bool parseCharacters(const QJsonValue &value);
	bool parseMapBlocks(const QJsonValue &value);
	bool parseFile(const QJsonValue &value);
public:
	RpgPreload(const QString &filename);

	inline const QString &getBootScriptSceneName() const{ return this->bootScriptSceneName; }
};

#endif // RPGPRELOAD_H
