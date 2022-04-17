#include "RpgFileManager.h"

RpgFileManager *RpgFileManager::_instance = nullptr;

RpgFileManager::RpgFileManager(){
	for(QHash<QString, QUrl> * file: this->files){
		if(file == nullptr){
			continue;
		}
		file->clear();
		delete file;
		file = nullptr;
	}
	this->files.clear();

	this->files.insert(Unknown, new QHash<QString, QUrl>());
	this->files.insert(SceneFile, new QHash<QString, QUrl>());
	this->files.insert(ScriptFile, new QHash<QString, QUrl>());
	this->files.insert(NormalFile, new QHash<QString, QUrl>());
	this->files.insert(FontFile, new QHash<QString, QUrl>());
	this->files.insert(ImageFile, new QHash<QString, QUrl>());
	this->files.insert(MusicFile, new QHash<QString, QUrl>());
	this->files.insert(LyricFile, new QHash<QString, QUrl>());
	this->files.insert(SoundEffectFile, new QHash<QString, QUrl>());
	this->files.insert(AvatarFile, new QHash<QString, QUrl>());
	this->files.insert(CharacterFile, new QHash<QString, QUrl>());
	this->files.insert(TileSetFile, new QHash<QString, QUrl>());
	this->files.insert(MapFile, new QHash<QString, QUrl>());
	this->files.insert(DataFile, new QHash<QString, QUrl>());
	this->files.insert(AutoTileFile, new QHash<QString, QUrl>());

	this->fileTypeName.insert(Unknown, "Unknown");
	this->fileTypeName.insert(SceneFile, "SceneFile");
	this->fileTypeName.insert(ScriptFile, "ScriptFile");
	this->fileTypeName.insert(NormalFile, "NormalFile");
	this->fileTypeName.insert(FontFile, "FontFile");
	this->fileTypeName.insert(ImageFile, "ImageFile");
	this->fileTypeName.insert(MusicFile, "MusicFile");
	this->fileTypeName.insert(LyricFile, "LyricFile");
	this->fileTypeName.insert(SoundEffectFile, "SoundEffectFile");
	this->fileTypeName.insert(AvatarFile, "AvatarFile");
	this->fileTypeName.insert(CharacterFile, "CharacterFile");
	this->fileTypeName.insert(TileSetFile, "MapBlockFile");
	this->fileTypeName.insert(MapFile, "MapFile");
	this->fileTypeName.insert(DataFile, "DataFile");
	this->fileTypeName.insert(AutoTileFile, "AutoTileFile");
}

void RpgFileManager::addFile(RpgFileManager::FileType type, const QString &name, const QUrl &url){
	if(!this->files.contains(type)){
		throw RpgMapKeyNotFoundException(this->fileTypeName.value(type, "unknown"));
	}
	QHash<QString, QUrl> *file = this->files.value(type);
	if(file == nullptr){
		throw RpgNullPointerException("file");
	}
	file->insert(name, url);
}

void RpgFileManager::addFile(RpgFileManager::FileType type, const QString &name, const QString &filename){
	if(!QFile::exists(filename)){
		throw RpgResourceNotFoundException(filename);
	}
	this->addFile(type, name, QUrl::fromLocalFile(filename));
}

void RpgFileManager::addFile(RpgFileManager::FileType type, const QMap<QString, QUrl> &dict){
	for(QMap<QString, QUrl>::ConstIterator i = dict.constBegin(); i != dict.constEnd(); i++){
		this->addFile(type, i.key(), i.value());
	}
}

void RpgFileManager::addFile(RpgFileManager::FileType type, const QMap<QString, QString> &dict){
	for(QMap<QString, QString>::ConstIterator i = dict.constBegin(); i != dict.constEnd(); i++){
		this->addFile(type, i.key(), i.value());
	}
}

void RpgFileManager::removeFile(RpgFileManager::FileType type, const QString &name){
	if(!this->files.contains(type)){
		throw RpgMapKeyNotFoundException(this->fileTypeName.value(type, "unknown"));
	}
	QHash<QString, QUrl> *file = this->files.value(type);
	if(file == nullptr){
		throw RpgNullPointerException("file");
	}
	file->remove(name);
}

QUrl RpgFileManager::getFile(RpgFileManager::FileType type, const QString &name) const{
	if(!this->files.contains(type)){
		throw RpgMapKeyNotFoundException(this->fileTypeName.value(type, "unknown"));
	}
	QHash<QString, QUrl> *file = this->files.value(type);
	if(file == nullptr){
		throw RpgNullPointerException("file");
	}
	return file->value(name);
}

QString RpgFileManager::getFileString(RpgFileManager::FileType type, const QString &name) const{
	QUrl url = this->getFile(type, name);
	if(!url.isValid()){
		throw RpgMapKeyNotFoundException(name);
	}
	return url.url(QUrl::PreferLocalFile);
}

void RpgFileManager::dumpFiles(){
	rDebug() << "=================== RpgFileManager ====================";
	for(QHash<FileType, QHash<QString, QUrl>* >::const_iterator i = this->files.constBegin(); i != this->files.constEnd(); i++){
		rDebug() << "[" << this->fileTypeName.value(i.key(), "UNKNOWN") << "]";
		QHash<QString, QUrl> *file = i.value();
		if(file == nullptr){
			rDebug() << "<nullptr pointer>";
			continue;
		}
		for(QHash<QString, QUrl>::const_iterator ii = file->constBegin(); ii != file->constEnd(); ii++){
			rDebug() << "    " << ii.key() << "=" << ii.value();
		}
	}
	rDebug() << "=======================================================";
}
