#ifndef RPGVAR_H
#define RPGVAR_H

#include <QtCore>
#include <Rpg/Global.h>
#include <Rpg/core/RpgFileManager.h>

typedef QHash<QString, QVariant> RpgVarHash;

// TODO: 可能需要将文件位置托管至RpgFileManager

/**
 * @brief The RpgVar class
 * RpgVar类, 存储游戏中产生的的变量, 或者读取某个变量
 *
 * Var读取结构: RpgVar = {"Group/Profile1": {...}, "Group/Profile2": {...}}
 * Var存储结构: INI文件:
 * [General]
 * globalKey1 = value1
 * globalKey2 = value2
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
 * 20210919 RpgVar增加从RpgFileManager取出指定位置数据
 */
class RpgVar
{
	static RpgVar *_instance;

	QString group;

	QHash<QString, RpgVarHash*> vars;

	RpgVar(){}

public:
	~RpgVar();

	inline static RpgVar *instance(){
		if(RpgVar::_instance == nullptr){
			RpgVar::_instance = new RpgVar();
		}
		return RpgVar::_instance;
	}

	void beginGroup(const QString &group) NoThrow;
	void endGroup() NoThrow;
	inline const QString &getGroup() const NoThrow;

	void setValue(const QString &group, const QString &key, const QVariant &value);
	void setValue(const QString &key, const QVariant &value);

	inline QVariant getValue(const QString &group, const QString &key) const;
	inline QVariant getValue(const QString &key) const;

	inline int getVarCount(const QString &group) const;
	inline int getVarCount() const;

	bool removeValue(const QString &group, const QString &key);
	bool removeGroup(const QString &group);

	bool loadDataFromFile(const QString &file);
	bool loadData(const QString &name);
	bool saveDataToFile(const QString &file);
	bool saveData(const QString &name);

	void dumpRpgVars() NoThrow;
};

#endif // RPGVAR_H
