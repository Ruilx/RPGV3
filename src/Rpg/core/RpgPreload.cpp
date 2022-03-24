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
			dict.insert(key, QUrl(stringValue));
		}
	}
	return dict;
}

bool RpgPreload::handlePreload(const QByteArray &loadingJson){
	if(loadingJson.isEmpty()){
		rDebug() << "Initialization file is Empty.";
		return false;
	}
	QJsonParseError error;
	QJsonDocument initDoc = QJsonDocument::fromJson(loadingJson, &error);
	if(error.error != QJsonParseError::NoError){
		rDebug() << "Initialization configure json parse error: " << error.errorString();
		return false;
	}
	if(!initDoc.isObject()){
		rDebug() << "Root object must be object, " << RpgUtils::detectJsonValue(initDoc) << " found.";
		return false;
	}
	QJsonObject root = initDoc.object();
	QStringList keys = root.keys();
	for(const QString &key: keys){
		QJsonValue value = root.value(key);
		if(key == "init"){
			// Execute Script Scene
			if(!value.isString()){
				rDebug() << "Init value is not a String, " << RpgUtils::detectJsonValue(value) << " found.";
				return false;
			}
			this->bootScriptSceneName = value.toString();
		}else if(key == "map"){
			// Map List
			if(!value.isObject()){
				rDebug() << "Map Object is not an Object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject mapObj = value.toObject();
			QStringList keys = mapObj.keys();
			for(const QString &key: keys){
				QJsonValue value = mapObj.value(key);
				if(!value.isObject()){
					rDebug() << "Map Scene is not an object, " << RpgUtils::detectJsonValue(value) << " found.";
					continue;
				}
				QString mapName;
				QString scriptName;
				QJsonObject sceneNameObj = value.toObject();
				QStringList keys = sceneNameObj.keys();
				for(const QString &key: keys){
					QJsonValue value = sceneNameObj.value(key);
					if(key == "map"){
						mapName = value.toString();
					}else if(key == "script"){
						scriptName = value.toString();
					}else{
						continue;
					}
				}
				if(mapName.isEmpty() || scriptName.isEmpty()){
					rDebug() << "Map file name or script name is/are empty, the scene " << key << " invalid";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::MapFile, key, QUrl(mapName));
					RpgFileManager::instance()->addFile(RpgFileManager::ScriptFile, key, QUrl(scriptName));

					// TODO: Setup RpgScene Below
					// ...
				}
			}
		}else if(key == "music"){
			// Music List
			if(!value.isObject()){
				rDebug() << "Music object is not an object, " << RpgUtils::detectJsonValue(value) << " found";
				continue;
			}
			QJsonObject musicObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(musicObj);
			RpgFileManager::instance()->addFile(RpgFileManager::MusicFile, dict);
		}else if(key == "soundEffect" || key == "se"){
			// Sound effect List
			if(!value.isObject()){
				rDebug() << "Sound effect object is not an object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject soundObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(soundObj);
			RpgFileManager::instance()->addFile(RpgFileManager::SoundEffectFile, dict);
		}else if(key == "image"){
			// Image List
			if(!value.isObject()){
				rDebug() << "Image object is not an object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject imageObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(imageObj);
			RpgFileManager::instance()->addFile(RpgFileManager::ImageFile, dict);
		}else if(key == "lyric"){
			// Lyric List
			if(!value.isObject()){
				rDebug() << "Lyric object is not an Object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject lyricObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(lyricObj);
			RpgFileManager::instance()->addFile(RpgFileManager::LyricFile, dict);
		}else if(key == "font"){
			// Font List
			if(!value.isObject()){
				rDebug() << "Font object is not an Object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject fontObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(fontObj);
			RpgFileManager::instance()->addFile(RpgFileManager::FontFile, dict);
		}else if(key == "avatar"){
			// Avatar List
			if(!value.isObject()){
				rDebug() << "Avatar object is not an Object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject avatarObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(avatarObj);
			RpgFileManager::instance()->addFile(RpgFileManager::AvatarFile, dict);
		}else if(key == "character"){
			// Character List
			if(!value.isObject()){
				rDebug() << "Character object is not an Object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject characterObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(characterObj);
			RpgFileManager::instance()->addFile(RpgFileManager::CharacterFile, dict);
		}else if(key == "mapBlock"){
			// Map Block List
			if(!value.isObject()){
				rDebug() << "Map Block object is not an Object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject mapBlockObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(mapBlockObj);
			RpgFileManager::instance()->addFile(RpgFileManager::MapBlockFile, dict);
		}

		else if(key == "file"){
			// File List
			if(!value.isObject()){
				rDebug() << "File object is not an Object, " << RpgUtils::detectJsonValue(value) << " found.";
				continue;
			}
			QJsonObject fileObj = value.toObject();
			QMap<QString, QUrl> dict = this->parseDict(fileObj);
			RpgFileManager::instance()->addFile(RpgFileManager::NormalFile, dict);
		}else{
			// Extra
			continue;
		}
	}
	return true;
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
