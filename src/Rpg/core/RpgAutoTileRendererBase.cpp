#include "RpgAutoTileRendererBase.h"


RpgAutoTileRendererBase::RpgAutoTileRendererBase(const QString &name){
	if(name.isEmpty()){
		qDebug() << CodePath << "Given name is empty.";
		return;
	}
	this->name = name;
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::AutoTileFile, name);
	if(filename.isEmpty()){
		qDebug() << CodePath << "RpgFileManager has no registation name:" << name;
		return;
	}
	this->originTilePixmap = QPixmap(filename);
	if(this->originTilePixmap.isNull()){
		qDebug() << CodePath << "tilePixmap not load successful, empty content:" << this->originTilePixmap;
		return;
	}
}

RpgAutoTileRendererBase::~RpgAutoTileRendererBase(){
	for(QMap<quint8, RpgSeqTilesArray*>::const_iterator i = this->tilesMap.constBegin(); i != this->tilesMap.constEnd(); i++){
		if(i.value() != nullptr){
			i.value()->clear();
			delete i.value();
		}
	}
	this->tilesMap.clear();
}

void RpgAutoTileRendererBase::insertTile(Rpg::AutoTileBlockType type, const QPixmap &pixmap){
	if(pixmap.isNull()){
		qDebug() << CodePath << "Given pixmap is null.";
		return;
	}
	if(!this->tilesMap.contains(type)){
		RpgSeqTilesArray *tileMap = new RpgSeqTilesArray();
		tileMap->append(pixmap);
		this->tilesMap.insert(type, tileMap);
	}else{
		RpgSeqTilesArray *tileMap = this->tilesMap.value(type);
		if(tileMap != nullptr){
			tileMap->append(pixmap);
		}else{
			qDebug() << CodePath << "Given type in tilesMap array is nullptr";
		}
	}
}

const QPixmap &RpgAutoTileRendererBase::getTile(Rpg::AutoTileBlockLocation location, int sequence) const{
	if(this->tilesMap.contains(location)){
		RpgSeqTilesArray *arr = this->tilesMap.value(location);
		if(arr != nullptr){
			if(sequence < arr->length()){
				return arr->at(sequence);
			}else{
				qDebug() << CodePath << "TileSet not found" << location << "location with sequence:" << sequence;
				return this->emptyPixmap;
			}
		}else{
			qDebug() << CodePath << "Location:" << location << "in tilesmap is null!";
			return this->emptyPixmap;
		}
	}else{
		qDebug() << CodePath << "TileSet has no type named:" << location;
		return this->emptyPixmap;
	}
}

const RpgAutoTileRendererBase::RpgSeqTilesArray *RpgAutoTileRendererBase::getTiles(Rpg::AutoTileBlockLocation location) const{
	if(this->tilesMap.contains(location)){
		return this->tilesMap.value(location);
	}else{
		qDebug() << CodePath << "TileSet has no location named:" << location;
		return nullptr;
	}
}

const QPixmap &RpgAutoTileRendererBase::getTileByType(Rpg::AutoTileBlockType type, int sequence) const{
	if(this->tilesMap.contains(type)){
		RpgSeqTilesArray *arr = this->tilesMap.value(type);
		if(arr != nullptr){
			if(sequence < arr->length()){
				return arr->at(sequence);
			}else{
				qDebug() << CodePath << "TileSet not found" << type << "location with sequence:" << sequence;
				return this->emptyPixmap;
			}
		}else{
			qDebug() << CodePath << "Type:" << type << "in tilesmap is null!";
			return this->emptyPixmap;
		}
	}else{
		qDebug() << CodePath << "TileSet has no type named:" << type;
		return this->emptyPixmap;
	}
}

const RpgAutoTileRendererBase::RpgSeqTilesArray *RpgAutoTileRendererBase::getTilesByType(Rpg::AutoTileBlockType type) const{
	if(this->tilesMap.contains(type)){
		return this->tilesMap.value(type);
	}else{
		qDebug() << CodePath << "TileSet has no type named:" << type;
		return nullptr;
	}
}

void RpgAutoTileRendererBase::dumpTiles(const QDir &savingDir){
	if(!savingDir.isEmpty()){
		qDebug() << CodePath << "saving path has files in it. please delete those first. path:" << savingDir.absolutePath();
		return;
	}
	int count = 0;
	int successCount = 0;
	for(QMap<quint8, RpgSeqTilesArray*>::const_iterator i = this->tilesMap.constBegin(); i != this->tilesMap.constEnd(); i++){
		RpgSeqTilesArray *arr = i.value();
		if(arr != nullptr){
			int seq = 0;
			for(const QPixmap &pixmap: *arr){
				QString filename = QString::asprintf("0x%2X_%d.png", i.key(), seq);
				successCount++;
				if(!pixmap.save(savingDir.filePath(filename))){
					qDebug() << CodePath << "Saving" << filename << "to" << savingDir.path() << "failed.";
				}else{
					count++;
				}
				seq++;
			}
		}
	}
	qDebug() << CodePath << "Successfully dumped" << count << "/" << successCount << "block tile image(s) at:" << savingDir.path();
}
