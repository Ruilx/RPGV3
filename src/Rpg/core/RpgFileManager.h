#ifndef RPGFILEMANAGER_H
#define RPGFILEMANAGER_H

#include <QtCore>
#include <Rpg/Global.h>
#include <Rpg/exception/RpgMapKeyNotFoundException.h>
#include <Rpg/exception/RpgNullPointerException.h>
#include <Rpg/exception/RpgResourceNotFoundException.h>

/**
 * @brief The RpgFileManager class
 * RpgFileManager类, 是进行RPG资源存储映射的组件
 * 在游戏启动初会检查每一个映射的文件是否存在以及定义名称使其他组件能够按照这个名称载入并执行文件
 * 因为全局只能有一个资源管理, 所以作为单例模式存在
 *
 * 文件载入默认在执行初期, 通过设置的json文件进行处理, 在每个组件中进行取出及使用
 *
 */
class RpgFileManager
{
	static RpgFileManager *_instance;
public:
	enum FileType{
		Unknown = 0,	// 不使用
		SceneFile,		// 场景文件
		ScriptFile,		// 脚本文件
		NormalFile,		// 普通文件
		FontFile,		// 字体文件
		ImageFile,		// 图片文件
		MusicFile,		// 音乐文件(背景音乐)
		LyricFile,		// 歌词文件
		SoundEffectFile,// 音效文件(SE)
		AvatarFile,		// 头像文件
		CharacterFile,	// 角色文件
		MapBlockFile,	// 地图块文件
		MapFile,		// 地图文件
	};

	static RpgFileManager *instance(){
		if(_instance == nullptr){
			_instance = new RpgFileManager();
		}
		return _instance;
	}

private:
	QHash<FileType, QHash<QString, QUrl>* > files;
	QHash<FileType, QString> fileTypeName;

public:
	RpgFileManager(){
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
		this->files.insert(MapBlockFile, new QHash<QString, QUrl>());
		this->files.insert(MapFile, new QHash<QString, QUrl>());

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
		this->fileTypeName.insert(MapBlockFile, "MapBlockFile");
		this->fileTypeName.insert(MapFile, "MapFile");
	}

	void addFile(FileType type, const QString &name, const QUrl &url){
		if(!this->files.contains(type)){
			throw RpgMapKeyNotFoundException(this->fileTypeName.value(type, "unknown"));
		}
		QHash<QString, QUrl> *file = this->files.value(type);
		if(file == nullptr){
			throw RpgNullPointerException("file");
		}
		file->insert(name, url);
	}

	void addFile(FileType type, const QString &name, const QString &filename){
		if(!QFile::exists(filename)){
			throw RpgResourceNotFoundException(filename);
		}
		this->addFile(type, name, QUrl::fromLocalFile(filename));
	}

	void addFile(FileType type, const QMap<QString, QUrl> &dict){
		for(QMap<QString, QUrl>::ConstIterator i = dict.constBegin(); i != dict.constEnd(); i++){
			this->addFile(type, i.key(), i.value());
		}
	}

	void addFile(FileType type, const QMap<QString, QString> &dict){
		for(QMap<QString, QString>::ConstIterator i = dict.constBegin(); i != dict.constEnd(); i++){
			this->addFile(type, i.key(), i.value());
		}
	}

	void removeFile(FileType type, const QString &name){
		if(!this->files.contains(type)){
			throw RpgMapKeyNotFoundException(this->fileTypeName.value(type, "unknown"));
		}
		QHash<QString, QUrl> *file = this->files.value(type);
		if(file == nullptr){
			throw RpgNullPointerException("file");
		}
		file->remove(name);
	}

	QUrl getFile(FileType type, const QString &name) const{
		if(!this->files.contains(type)){
			throw RpgMapKeyNotFoundException(this->fileTypeName.value(type, "unknown"));
		}
		QHash<QString, QUrl> *file = this->files.value(type);
		if(file == nullptr){
			throw RpgNullPointerException("file");
		}
		return file->value(name);
	}

	QString getFileString(FileType type, const QString &name) const{
		QUrl url = this->getFile(type, name);
		if(!url.isValid()){
			throw RpgMapKeyNotFoundException(name);
		}
		return url.url(QUrl::PreferLocalFile);
	}

	void dumpFiles() NoThrow{
		for(QHash<FileType, QHash<QString, QUrl>* >::const_iterator i = this->files.constBegin(); i != this->files.constEnd(); i++){
			qDebug() << "=========================" << this->fileTypeName.value(i.key(), "UNKNOWN") << "=========================";
			QHash<QString, QUrl> *file = i.value();
			if(file == nullptr){
				qDebug() << "NULL POINTER OCCURRED!";
				continue;
			}
			for(QHash<QString, QUrl>::const_iterator ii = file->constBegin(); ii != file->constEnd(); ii++){
				qDebug() << ii.key() << " => " << ii.value();
			}
		}
	}

};

#endif // RPGFILEMANAGER_H
