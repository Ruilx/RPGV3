#ifndef RPGFONT_H
#define RPGFONT_H

#include <QHash>
#include <QString>
#include <QFont>
#include <QFontDatabase>

#include <Rpg/core/RpgFileManager.h>

class RpgFont
{
	static RpgFont *_instance;

	QHash<QString, int> fontMap;

	void setupFontInFileManager(const QString &name, const QString &fileManagerName){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::FontFile, fileManagerName);
		int fontId = QFontDatabase::addApplicationFont(filename);
		if(fontId == -1){
			rDebug() << CodePath << "The name of font:" << fileManagerName << "is not valid from font database.";
			return;
		}
		this->fontMap.insert(name, fontId);
	}
public:
	RpgFont(){
		this->fontMap.clear();
	}

	~RpgFont(){
		if(RpgFont::_instance != nullptr){
			delete RpgFont::_instance;
		}
	}

	static RpgFont *instance(){
		if(_instance == nullptr){
			_instance = new RpgFont();
		}
		return _instance;
	}

	static void setFallbackFont(const QString &fontName, const QString &fallbackFontName){
		const QString setupFont = RpgFont::instance()->getDefaultFontIndex(fontName);
		const QString setupFallbackFont = RpgFont::instance()->getDefaultFontIndex(fallbackFontName);
		QFont::insertSubstitution(setupFont, setupFallbackFont);
	}

	const QString getDefaultFontIndex(const QString &name){
		if(!this->fontMap.contains(name)){
			this->setupFontInFileManager(name, name);
			if(!this->fontMap.contains(name)){
				throw RpgMapKeyNotFoundException(name);
			}
		}
		QStringList fontList = QFontDatabase::applicationFontFamilies(this->fontMap.value(name));
		//rDebug() << "Detected font:" << name << "with:" << fontList;
		if(fontList.length() >= 1){
			return fontList.at(0);
		}else{
			return QString();
		}
	}

	const QFont getDefaultFont(const QString &name, int pointSize = -1, int weight = -1, bool italic = false){
		const QString internalFontName = this->getDefaultFontIndex(name);
		if(internalFontName.isEmpty()){
			throw RpgMapKeyNotFoundException(name);
		}
		return QFont(internalFontName, pointSize, weight, italic);
	}

	/**
	 * @brief getFont
	 * @param name
	 * @param pointSize
	 * @param weight
	 * @param italic
	 * @return
	 * 寻找字体, 如果在选项中, 返回选项中的字体, 如果没有, 返回对应系统字体
	 */
	const QFont getFont(const QString &name, int pointSize = -1, int weight = -1, bool italic = false){
		try{
			return this->getDefaultFont(name, pointSize, weight, italic);
		}catch(RpgMapKeyNotFoundException e){
			return QFont(name, pointSize, weight, italic);
		}
	}

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
