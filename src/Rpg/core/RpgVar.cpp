#include "RpgVar.h"

RpgVar *RpgVar::_instance = nullptr;

RpgVar::~RpgVar(){
	for(const QString &group: vars.keys()){
		RpgVarHash *var = this->vars.value(group);
		if(var != nullptr){
			var->clear();
			this->vars.remove(group);
			delete var;
		}
	}
}

void RpgVar::beginGroup(const QString &group) NoThrow{
	this->group = group;
}

void RpgVar::endGroup() NoThrow{
	this->group.clear();
}

const QString &RpgVar::getGroup() const NoThrow{
	return this->group;
}

void RpgVar::setValue(const QString &group, const QString &key, const QVariant &value){
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

void RpgVar::setValue(const QString &key, const QVariant &value){
	return this->setValue(this->group, key, value);
}

QVariant RpgVar::getValue(const QString &group, const QString &key) const{
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

QVariant RpgVar::getValue(const QString &key) const{
	QVariant res = this->getValue(this->group, key);
	if(res.isNull()){
		return this->getValue("", key);
	}
}

int RpgVar::getVarCount(const QString &group) const{
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

int RpgVar::getVarCount() const{
	int count = 0;
	for(RpgVarHash *var: this->vars){
		if(var != nullptr){
			count += var->count();
		}
	}
	return count;
}

bool RpgVar::removeValue(const QString &group, const QString &key){
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

bool RpgVar::removeValue(const QString &key){
	return this->removeValue(this->group, key);
}

bool RpgVar::removeGroup(const QString &group){
	if(this->vars.contains(group)){
		RpgVarHash *var = this->vars.value(group);
		if(var != nullptr){
			var->clear();
			this->vars.remove(group);
			delete var;
			return true;
		}else{
			return false;
		}
	}
	return false;
}

bool RpgVar::loadDataFromFile(const QString &file){
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

bool RpgVar::loadData(const QString &name){
	if(name.isEmpty()){
		qDebug() << CodePath << "Given name is empty.";
		return false;
	}
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::DataFile, name);
	if(filename.isEmpty()){
		qDebug() << CodePath << "FileManager has no file named '" << name << "'.";
		return false;
	}
	return this->loadDataFromFile(filename);
}

bool RpgVar::saveDataToFile(const QString &file){
	if(!QFile::exists(file)){
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

bool RpgVar::saveData(const QString &name){
	if(name.isEmpty()){
		qDebug() << CodePath << "Given name is empty.";
		return false;
	}
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::DataFile, name);
	if(filename.isEmpty()){
		qDebug() << CodePath << "FileManager has no file named '" << name << "'.";
		return false;
	}
	return this->saveDataToFile(filename);
}

void RpgVar::dumpRpgVars() NoThrow{
	qDebug() << "============ RpgVar ==============";
	for(QHash<QString, RpgVarHash*>::const_iterator i = this->vars.constBegin(); i != this->vars.constEnd(); i++){
		QString key = i.key();
		if(key == ""){
			key.append("General");
		}
		qDebug().noquote().nospace() << "[" << key << "]";
		RpgVarHash *value = i.value();
		if(value == nullptr){
			qDebug() << "<nullptr pointer>";
			continue;
		}
		for(RpgVarHash::const_iterator ii = value->constBegin(); ii != value->constEnd(); ii++){
			qDebug().noquote() << ii.key() << "=" << ii.value();
		}
		qDebug();
	}
	qDebug() << "==================================";
}
