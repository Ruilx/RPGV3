#include "RpgTileSetBase.h"

#include <Rpg/core/RpgFileManager.h>
#include <Rpg/core/RpgUtils.h>

void RpgTileSetBase::readOriTile(){
	QString filename = rpgFileManager->getFileString(RpgFileManager::TileSetFile, this->name);
	if(filename.isEmpty()){
		rError() << "Tileset name:" << name << "not found in rpgFileManager.";
		return;
	}
	if(!this->oriTile.load(filename)){
		rError() << "Cannot load pixmap: '" << filename << "' as oriTile";
		return;
	}
}

RpgTileSetBase::RpgTileSetBase(const QString &name){
	if(name.isEmpty()){
		rError() << "Given name:" << name << "is empty, cannot readOriTile.";
	}
	this->name = name;
	this->tiles.clear();
	this->readOriTile();
}

const QPixmap &RpgTileSetBase::getTilePixmap(const QPoint &loc, int index){
	if(loc.x() < 0 || loc.x() >= this->tileSize.width()){
		rError() << "Given loc.x() == '" << loc.x() << "' out of oriTile width: [0," << this->tileSize.width() << ").";
		return this->nullPixmap;
	}
	if(loc.y() < 0 || loc.y() >= this->tileSize.height()){
		rError() << "Given loc.y() == '" << loc.y() << "' out of oriTile height: [0," << this->tileSize.height() << ").";
		return this->nullPixmap;
	}
	if(!this->tiles.contains(loc)){
		rDebug() << "Given loc not in tiles yet. will rander loc: " << loc << " position.";
		this->renderTile(loc);
	}
	if(!this->tiles.contains(loc)){
		rError() << "Given loc still not in tiles even rendered. Please check randerTile() registered correct pixmap at specified location. loc:" << loc << ", will throw not found error.";
		throw RpgMapKeyNotFoundException(RpgUtils::toString(loc));
	}
	QList<QPixmap> tile = this->tiles.value(loc);
	if(tile.length() <= 0){
		rError() << "Given tile loc:" << loc << "has no valid pixmaps.";
		return this->nullPixmap;
	}
	if(index >= tile.length()){
		rError() << "Given tile loc:" << loc << "has not longer index:" << index << "at range [0," << tile.length() << ").";
		return this->nullPixmap;
	}
	return tile.at(index);
}
