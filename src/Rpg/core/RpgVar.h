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
 * [General](group == ""时的存储空间)
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
 * @date 20210916 RpgVar从com目录迁移到core目录, 去掉QObject继承
 * @date 20210917 RpgVar增加group功能, 可以选择var的profile, 比如Rpg的20个存储框
 * @date 20210919 RpgVar增加从RpgFileManager取出指定位置数据
 */
class RpgVar
{
	static RpgVar *_instance;
	QString group;
	QHash<QString, RpgVarHash*> vars;
	explicit RpgVar(){}
public:
	~RpgVar();

	inline static RpgVar *instance(){
		if(RpgVar::_instance == nullptr){
			RpgVar::_instance = new RpgVar();
		}
		return RpgVar::_instance;
	}

	/**
	 * @brief beginGroup
	 * @param group
	 * 选择(select)配置组, 使用不带有group参数的set/get函数的时候将会默认使用配置的group名中的内容
	 * 因为使用Qt QSettings类, 不妨命名相同.
	 */
	void beginGroup(const QString &group) NoThrow;
	/**
	 * @brief endGroup
	 * 取消(select)配置组, 将组目标变为全局变量组(General)
	 */
	void endGroup() NoThrow;
	/**
	 * @brief getGroup
	 * @return
	 * 获得当前配置的变量组, 返回空字符串相当于全局变量组
	 */
	inline const QString &getGroup() const NoThrow;

	/**
	 * @brief setValue
	 * @param group
	 * @param key
	 * @param value
	 * 设置变量
	 */
	void setValue(const QString &group, const QString &key, const QVariant &value);
	/**
	 * @brief setValue
	 * @param key
	 * @param value
	 * 设置变量 (使用已存储的group)
	 */
	void setValue(const QString &key, const QVariant &value);

	/**
	 * @brief getValue
	 * @param group
	 * @param key
	 * @return 对应值, 如未找到, 返回QVariant(NULL)
	 * 获取变量
	 */
	inline QVariant getValue(const QString &group, const QString &key) const;
	/**
	 * @brief getValue
	 * @param key
	 * @return 对应值, 如未找到, 会在group=""寻找, 如未找到, 返回QVariant(NULL)
	 * 获取变量 (使用已存储的group)
	 */
	inline QVariant getValue(const QString &key) const;

	/**
	 * @brief getVarCount
	 * @param group
	 * @return 指定group中存储的变量数量
	 * 获得指定group内存储的变量数量
	 */
	inline int getVarCount(const QString &group) const;
	/**
	 * @brief getVarCount
	 * @return 当前group中存储的变量数量
	 * 获得当前group内存储的变量数量
	 */
	inline int getVarCount() const;

	/**
	 * @brief removeValue
	 * @param group
	 * @param key
	 * @return 删除成功返回true, 失败返回false
	 * 删除指定group对应指定key
	 */
	bool removeValue(const QString &group, const QString &key);
	/**
	 * @brief removeValue
	 * @param key
	 * @return 删除成功返回true, 失败返回false
	 * 删除当前group指定的key
	 */
	bool removeValue(const QString  &key);
	/**
	 * @brief removeGroup
	 * @param group
	 * @return 删除成功返回true, 失败返回false
	 * 删除指定group内的所有key
	 */
	bool removeGroup(const QString &group);

	/**
	 * @brief loadDataFromFile
	 * @param file
	 * @return 读取成功返回true, 失败返回false
	 * 从指定文件中读取变量
	 */
	bool loadDataFromFile(const QString &file);
	/**
	 * @brief loadData
	 * @param name
	 * @return 读取成功返回true, 失败返回false
	 * 从RpgFileManager中获得文件地址并读取设置
	 */
	bool loadData(const QString &name);
	/**
	 * @brief saveDataToFile
	 * @param file
	 * @return 读取成功返回true, 失败返回false
	 * 将变量存储至指定文件
	 */
	bool saveDataToFile(const QString &file);
	/**
	 * @brief saveData
	 * @param name
	 * @return 读取成功返回true, 失败返回false
	 * 将变量存储至RpgFileManager获取的文件地址对应的文件中
	 */
	bool saveData(const QString &name);

	/**
	 * @brief dumpRpgVars
	 * 打印当前内部的所有变量
	 */
	void dumpRpgVars() NoThrow;
};

#ifndef rpgVar
#	define rpgVar (RpgVar::instance())
#endif

#endif // RPGVAR_H
