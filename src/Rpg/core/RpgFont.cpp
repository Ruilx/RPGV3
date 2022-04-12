#include "RpgFont.h"

RpgFont *RpgFont::_instance = nullptr;

void RpgFont::setupFontInFileManager(const QString &name, const QString &fileManagerName){
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::FontFile, fileManagerName);
	int fontId = QFontDatabase::addApplicationFont(filename);
	if(fontId == -1){
		rDebug() << CodePath << "The name of font:" << fileManagerName << "is not valid from font database.";
		return;
	}
	this->fontMap.insert(name, fontId);
}

RpgFont::RpgFont(){
	this->fontMap.clear();
}

RpgFont::~RpgFont(){
	if(RpgFont::_instance != nullptr){
		delete RpgFont::_instance;
	}
}

RpgFont *RpgFont::instance(){
	if(_instance == nullptr){
		_instance = new RpgFont();
	}
	return _instance;
}

void RpgFont::setFallbackFont(const QString &fontName, const QString &fallbackFontName){
	const QString setupFont = RpgFont::instance()->getDefaultFontIndex(fontName);
	const QString setupFallbackFont = RpgFont::instance()->getDefaultFontIndex(fallbackFontName);
	QFont::insertSubstitution(setupFont, setupFallbackFont);
}

const QString RpgFont::getDefaultFontIndex(const QString &name){
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

const QFont RpgFont::getDefaultFont(const QString &name, int pointSize, int weight, bool italic){
	const QString internalFontName = this->getDefaultFontIndex(name);
	if(internalFontName.isEmpty()){
		throw RpgMapKeyNotFoundException(name);
	}
	return QFont(internalFontName, pointSize, weight, italic);
}

const QFont RpgFont::getFont(const QString &name, int pointSize, int weight, bool italic){
	try{
		return this->getDefaultFont(name, pointSize, weight, italic);
	}catch(RpgMapKeyNotFoundException e){
		return QFont(name, pointSize, weight, italic);
	}
}
