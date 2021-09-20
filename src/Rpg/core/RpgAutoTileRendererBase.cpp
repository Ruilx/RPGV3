#include "RpgAutoTileRendererBase.h"


RpgAutoTileRendererBase::RpgAutoTileRendererBase(const QString &name){
	if(name.isEmpty()){
		qDebug() << CodePath << "Given name is empty.";
		return;
	}
	this->name = name;
	QString filename = RpgFileManager::getFileString(RpgFileManager::AutoTileFile, name);
	if(filename.isEmpty()){
		qDebug() << CodePath << "RpgFileManager has no registation name:" << name;
		return;
	}
	this->tilePixmap = QPixmap(filename);
	if(this->tilePixmap.isEmpty()){
		qDebug() << CodePath << "tilePixmap not load successful, empty content:" << this->tilePixmap;
		return;
	}
}

void RpgAutoTileRendererBase::insertTile(Rpg::AutoTileBlockType type, const QPixmap &pixmap){
	if(pixmap.isNull()){
		qDebug() << CodePath << "Given pixmap is null.";
		return;
	}
	this->tilePixmap.insert(type, pixmap);
}

const QPixmap &RpgAutoTileRendererBase::getTile(Rpg::AutoTileBlockLocation location) const{
	if(this->tilePixmap.contains(location)){
		return this->tilePixmap.value(location);
	}else{
		qDebug() << CodePath << "TileSet has no location named:" << location;
		return this->emptyPixmap;
	}
}

const QPixmap &RpgAutoTileRendererBase::getTileByType(Rpg::AutoTileBlockType type) const{
	if(this->tilePixmap.contains(type)){
		return this->tilePixmap.value(type);
	}else{
		qDebug() << CodePath << "TileSet has no type named:" << type;
		return this->emptyPixmap;
	}
}

void RpgAutoTileRendererBase::dumpTiles(const QDir &savingDir){
	if(!savingDir.exists()){
		savingDir.makeAbsolute();
	}else{
		if(!savingDir.isEmpty()){
			qDebug() << CodePath << "saving path has files in it. please delete those first. path:" << savingDir.absolutePath();
			return;
		}
	}
	int count = 0;
	for(QMap<quint8, QPixmap>::ConstIterator i = this->tilePixmap.constBegin(); i != this->tilePixmap.constEnd(); i++){
		if(!i.value().isNull()){
			QString filename = "0x" % QString::number(i.key(), 16) % ".png";
			if(!i.value().save(savingDir.filePath(filename))){
				qDebug() << CodePath << "Saving" << filename << "to" << savingDir.path() << "failed.";
			}else{
				count++;
			}
		}
	}
	qDebug() << CodePath << "Successfully dumped" << count << "/ 47 block tile image(s) at:" << savingDir.path();
}
