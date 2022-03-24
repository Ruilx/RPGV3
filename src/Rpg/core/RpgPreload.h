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

	bool handlePreload(const QByteArray &loadingJson);

	bool parseInit(const QJsonValue &value){
		if(!value.isString()){
			rError() << "JSON value not a STRING," << RpgUtils::detectJsonValue(value) << "found.";
			return false;
		}
		this->bootScriptSceneName = value.toString();
		return true;
	}
	bool parseMaps(const QJsonValue &value){
		if(!value.isObject()){
			rError() << "JSON value not an OBJECT," << RpgUtils::detectJsonValue(value) << "found.";
			return false;
		}
		QJsonObject obj = value.toObject();
		for(const QString &key: obj.keys()){
			QJsonValue v = obj.value(key);
			if(!v.isObject()){
				rError() << "Map scene value not an OBJECT," << RpgUtils::detectJsonValue(v) << "found.";
				continue;
			}
			QJsonObject sceneNameObj = value.toObject();
			do{
				if(!sceneNameObj.contains("map") || !sceneNameObj.value("map").isString()){
					rWarning() << "JSON node map name not a STRING," << RpgUtils::detectJsonValue(sceneNameObj.value("map")) << "found.";
					break;
				}
				if(!sceneNameObj.contains("script") || !sceneNameObj.value("script").isString()){
					rWarning() << "JSON node map script not a STRING," << RpgUtils::detectJsonValue(sceneNameObj.value("script")) << "found.";
					break;
				}
				QString mapName = sceneNameObj.value("map").toString();
				QString scriptName = sceneNameObj.value("script").toString();
				if(mapName.isEmpty() || scriptName.isEmpty()){
					rError() << "Map file name or script name empty, scene '" << key << "' is invalid.";
					break;
				}

			}while(false);
		}
	}
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
};

#endif // RPGPRELOAD_H
