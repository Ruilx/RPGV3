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
		DataFile,		// 数据文件(Variables)
		AutoTileFile,   // 自动瓦片文件
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
	RpgFileManager();

	void addFile(FileType type, const QString &name, const QUrl &url);
	void addFile(FileType type, const QString &name, const QString &filename);
	void addFile(FileType type, const QMap<QString, QUrl> &dict);
	void addFile(FileType type, const QMap<QString, QString> &dict);

	void removeFile(FileType type, const QString &name);

	QUrl getFile(FileType type, const QString &name) const;
	QString getFileString(FileType type, const QString &name) const;

	void dumpFiles();

	// 考虑一下
	inline QString operator ()(FileType type, const QString &name) const{
		return this->getFileString(type, name);
	}
};

#ifndef rpgFileManager
#	define rpgFileManager (RpgFileManager::instance())
#endif

#endif // RPGFILEMANAGER_H
