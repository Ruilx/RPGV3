#include "RpgPreload.h"

const QMap<QString, QUrl> RpgPreload::parseDict(const QJsonObject &object){
	QMap<QString, QUrl> dict;
	QStringList keys = object.keys();
	for(const QString &key: keys){
		QJsonValue value = object.value(key);
		QString stringValue = value.toString();
		if(stringValue.isEmpty()){
			rDebug() << "The value of the Key: " << key << " is empty.";
			continue;
		}else{
			QString lowerStr = stringValue.toLower();
			if(lowerStr.startsWith("http:") || lowerStr.startsWith("https:") ||
					lowerStr.startsWith("ftp:") || lowerStr.startsWith("tcp:") ||
					lowerStr.startsWith("file:") || lowerStr.startsWith("qrc:")){
				dict.insert(key, QUrl(stringValue));
			}else{
				dict.insert(key, QUrl::fromLocalFile(stringValue));
			}
		}
	}
	return dict;
}

bool RpgPreload::parseValueToDict(const QJsonValue &value, void (*cb)(const QMap<QString, QUrl> &)) {
	if(!value.isObject()){
		rWarning() << "JSON value not an OBJECT," << RpgUtils::detectJsonValue(value) << "found.";
		return false;
	}
	QJsonObject obj = value.toObject();
	QMap<QString, QUrl> dict = this->parseDict(obj);
	if(cb != nullptr){
		cb(dict);
	}
	return true;
}

bool RpgPreload::handlePreload(const QByteArray &loadingJson){
	if(loadingJson.isEmpty()){
		rError() << "Initialization file is Empty.";
		return false;
	}
	QJsonParseError error;
	QJsonDocument initDoc = QJsonDocument::fromJson(loadingJson, &error);
	if(error.error != QJsonParseError::NoError){
		rError() << "Initialization configure json parse error: " << error.errorString();
		return false;
	}
	if(!initDoc.isObject()){
		rError() << "Root object must be object, " << RpgUtils::detectJsonValue(initDoc) << " found.";
		return false;
	}
	QJsonObject root = initDoc.object();
	QStringList keys = root.keys();
	for(const QString &key: keys){
		QJsonValue value = root.value(key);
		if(key == "init"){
			// Execute Script Scene
			this->parseInit(value);
		}else if(key == "map"){
			// Map List
			this->parseMaps(value);
		}else if(key == "music"){
			// Music List
			this->parseMusics(value);
		}else if(key == "soundEffect" || key == "se"){
			// Sound effect List
			this->parseSoundEffects(value);
		}else if(key == "image"){
			// Image List
			this->parseImages(value);
		}else if(key == "lyric"){
			// Lyric List
			this->parseLyrics(value);
		}else if(key == "font"){
			// Font List
			this->parseFonts(value);
		}else if(key == "avatar"){
			// Avatar List
			this->parseAvatars(value);
		}else if(key == "character"){
			// Character List
			this->parseCharacters(value);
		}else if(key == "mapBlock"){
			// Map Block List
			this->parseMapBlocks(value);
		}else if(key == "file"){
			// File List
			this->parseFile(value);
		}else{
			// Extra
			rWarning() << "Cannot parse" << key << "with any parse rules.";
			continue;
		}
	}
	return true;
}

bool RpgPreload::parseInit(const QJsonValue &value){
	if(!value.isString()){
		rError() << "JSON value not a STRING," << RpgUtils::detectJsonValue(value) << "found.";
		return false;
	}
	this->bootScriptSceneName = value.toString();
	return true;
}

bool RpgPreload::parseMaps(const QJsonValue &value){
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
	return true;
}

bool RpgPreload::parseMusics(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::MusicFile, dict);
	});
}

bool RpgPreload::parseSoundEffects(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::SoundEffectFile, dict);
	});
}

bool RpgPreload::parseImages(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::ImageFile, dict);
	});
}

bool RpgPreload::parseLyrics(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::LyricFile, dict);
	});
}

bool RpgPreload::parseFonts(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::FontFile, dict);
	});
}

bool RpgPreload::parseAvatars(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::AvatarFile, dict);
	});
}

bool RpgPreload::parseCharacters(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::CharacterFile, dict);
	});
}

bool RpgPreload::parseMapBlocks(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::TileSetFile, dict);
	});
}

bool RpgPreload::parseFile(const QJsonValue &value){
	return this->parseValueToDict(value, [](const QMap<QString, QUrl> &dict){
		rpgFileManager->addFile(RpgFileManager::NormalFile, dict);
	});
}

RpgPreload::RpgPreload(const QString &filename){
	QByteArray loadingJson = RpgUtils::readFile(filename);
	if(loadingJson.isEmpty()){
		throw RpgRuntimeException("Read initialization file failed.");
	}
	if(!this->handlePreload(loadingJson)){
		throw RpgRuntimeException("Preload failed.");
	}
}
