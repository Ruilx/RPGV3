#ifndef RPGFONT_H
#define RPGFONT_H

#include <QHash>
#include <QString>
#include <QFont>
#include <QFontDatabase>

#include <Rpg/core/RpgFileManager.h>

/**
 * @brief The RpgFont class
 * RpgFont 字体管理类
 * 存储并维护所用RPG的TTF/TTC/OTF字体, 可从文件中提取字体而不用提前安装指定字体
 *
 * @date 2022-04-12 整理逻辑内容至CPP
 */
class RpgFont
{
	static RpgFont *_instance;

	QHash<QString, int> fontMap;

	void setupFontInFileManager(const QString &name, const QString &fileManagerName);

protected:
	explicit RpgFont();
	~RpgFont();

public:
	static RpgFont *instance();

	/**
	 * @brief setFallbackFont 设置兜底字体
	 * @param fontName 目标字体名
	 * @param fallbackFontName 兜底字体名
	 * 设置兜底字体, 如果目标字体没有该字形, 则从兜底字体中取得, 兜底字体选用时尽量选择字库大的字体
	 */
	static void setFallbackFont(const QString &fontName, const QString &fallbackFontName);
	const QString getDefaultFontIndex(const QString &name);
	const QFont getDefaultFont(const QString &name, int pointSize = -1, int weight = -1, bool italic = false);

	/**
	 * @brief getFont 获取字体在文件中或系统中
	 * @param name 设置的字体名或系统中字体名
	 * @param pointSize 字体大小
	 * @param weight 是否加粗
	 * @param italic 是否倾斜
	 * @return QFont非修改实例
	 * 寻找字体, 如果在选项中, 返回选项中的字体, 如果没有, 返回对应系统字体
	 */
	const QFont getFont(const QString &name, int pointSize = -1, int weight = -1, bool italic = false);

	void _dumpFonts(){
		for(QHash<QString, int>::ConstIterator i = this->fontMap.constBegin(); i != this->fontMap.constEnd(); i++){
			rDebug() << i.key() << "=>" << i.value();
		}
	}
};

#ifndef rpgFont
#	define rpgFont (RpgFont::instance())
#endif

#endif // RPGFONT_H
