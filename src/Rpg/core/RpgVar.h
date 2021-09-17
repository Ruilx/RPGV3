#ifndef RPGVAR_H
#define RPGVAR_H

#include <QtCore>
#include <Rpg/Global.h>

typedef QHash<QString, QVariant> RpgVarHash;

// TODO: 可能需要将文件位置托管至RpgFileManager

/**
 * @brief The RpgVar class
 * RpgVar类, 存储游戏中产生的的变量, 或者读取某个变量
 *
 * Var读取结构: RpgVar = {"Group/Profile1": {...}, "Group/Profile2": {...}}
 * Var存储结构: INI文件:
 * [Group/Profile1]
 * key1 = value1
 * key2 = value2
 * [Group/Profile2]
 * key1 = value1
 * key2 = value2
 *
 * 当group/profile名为空字符串时, 对应的INI文件不属于任何一个[Group/Profile]
 * Group/Profile的作用是分隔如每个进度的存储栏目, Group/Profile是空的位置可存储游戏系统跨进度的一些信息.
 * 存储信息暂未加密保存
 *
 * 20210916 RpgVar从com目录迁移到core目录, 去掉QObject继承
 * 20210917 RpgVar增加group功能, 可以选择var的profile, 比如Rpg的20个存储框
 */
class RpgVar
{
	static RpgVar *_instance;

	QHash<QString, RpgVarHash*> vars;

	RpgVar();
public:
	static RpgVar *instance(){
		if(RpgVar::_instance == nullptr){
			RpgVar::_instance = new RpgVar();
		}
		return RpgVar::_instance;
	}

	void setValue(const QString &group, const QString &key, const QVariant &value){
		if(this->vars.contains(group)){
			RpgVarHash *var = this->vars.value(group);
			if(var != nullptr){
				if(var->contains(key)){
					var->remove(key);
				}
				var->insert(key, value);
			}else{
				qDebug() << CodePath << "RpgVar group:" << group << "returns a null hash";
				return;
			}
		}else{
			RpgVarHash *var = new RpgVarHash();
			this->vars.insert(group, var);
			var->insert(key, value);

		}
	}

	inline QVariant getValue(const QString &group, const QString &key) const{
		if(this->vars.contains(group)){
			RpgVarHash *var = this->vars.value(group);
			if(var != nullptr){
				if(var->contains(key)){
					return var->value(key);
				}else{
					qDebug() << CodePath << "No such key name:" << group << "." << key;
					return QVariant();
				}
			}
		}else{
			qDebug() << CodePath << "No such group name:" << group;
			return QVariant();
		}
	}

	inline int getVarCount(const QString &group) const{
		if(this->vars.contains(group)){
			RpgVarHash *var = this->vars.value(group);
			if(var != nullptr){
				qDebug() << CodePath << "RpgVar group:" << group << "returns a null hash";
				return 0;
			}else{
				return var->count();
			}
		}else{
			qDebug() << CodePath << "No such group name:" << group;
			return 0;
		}
	}

	bool removeValue(const QString &group, const QString &key){
		if(this->vars.contains(group)){
			RpgVarHash *var = this->vars.value(group);
			if(var != nullptr){
				if(var->contains(key)){
					var->remove(key);
					return true;
				}else{
					qDebug() << CodePath << "No such key name:" << group << "." << key;
					return false;
				}
			}else{
				qDebug() << CodePath << "RpgVar group:" << group << "returns a null hash";
				return false;
			}
		}else{
			qDebug() << CodePath << "Try to remove a null group key:" << group;
			return false;
		}
	}

	bool loadData(const QString &file){
		if(!QFile::exists(file)){
			qDebug() << CodePath << "Specific saving file:" << file << "not exists.";
			return false;
		}
		QSettings slot(file, QSettings::IniFormat);
		QStringList groups = slot.childGroups();
		int count = 0;
		for(const QString &group: groups){
			slot.beginGroup(group);
			QStringList keys = slot.childKeys();
			count += keys.length();
			for(const QString &key: keys){
				this->setValue(group, key, slot.value(key));
			}
			slot.endGroup();
		}
		QStringList keys = slot.childKeys();
		count += keys.length();
		for(const QString &key: keys){
			this->setValue("", key, slot.value(key));
		}
		qDebug() << CodePath << "Loading RpgVar successful. Loaded" << count << "keys.";
		return true;
	}

//	bool loadData(const QString &file){
//		if(!QFile::exists(file)){
//			qDebug() << CodePath << "Specific file:" << file << "not exists.";
//			return false;
//		}
//		QSettings slot(file, QSettings::IniFormat);
//		slot.beginGroup("Vars");
//		QStringList keys = slot.childKeys();
//		for(const QString &key: keys){
//			this->setValue(key, slot.value(key));
//		}
//		slot.endGroup();
//		qDebug() << CodePath << "Load var values successful. Loaded:" << keys.length() << "items.";
//		return true;
//	}

	bool saveData(const QString &file){
		if(QFile::exists(file)){
			qDebug() << CodePath << "Specific saving file:" << file << "not exists.";
			return false;
		}
		QSettings slot(file, QSettings::IniFormat);
		QStringList groups = this->vars.keys();
		int count = 0;
		for(const QString &group: groups){
			if(group != ""){
				slot.beginGroup(group);
			}
			RpgVarHash *var = this->vars.value(group);
			QStringList keys = var->keys();
			count += keys.length();
			for(const QString &key: keys){
				slot.setValue(key, var->value(key));
			}
			if(group != ""){
				slot.endGroup();
			}
		}
		qDebug() << CodePath << "Saving RpgVar successful. Saved" << count << "keys.";
		return true;
	}

	bool saveData(const QString &file){
		if(QFile::exists(file)){
			qDebug() << CodePath << "Specific file exists, will clear the data before save. file:" << file;
			return false;
		}
		QSettings slot(file, QSettings::IniFormat);
		slot.clear();
		slot.beginGroup("Vars");
		QStringList keys = this->var.keys();
		for(const QString &key: keys){
			slot.setValue(key, this->getValue(key));
		}
		slot.endGroup();
		qDebug() << CodePath << "Save var values successful. Saved:" << keys.length() << "items.";
		return true;
	}
};

#endif // RPGVAR_H
