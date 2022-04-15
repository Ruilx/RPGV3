#ifndef RPGMAP_H
#define RPGMAP_H

#include <Rpg/Rpg.h>

#include <Rpg/core/RpgObject.h>

#define RpgMapWidthPrealloc 64
#define RpgMapHeightPrealloc 64

class RpgMap : public RpgObject
{
	Q_OBJECT

	QVarLengthArray<QVarLengthArray<QGraphicsPixmapItem, RpgMapWidthPrealloc> , RpgMapHeightPrealloc> map;

	QGraphicsPixmapItem nullItem;
public:
	RpgMap(const QSize &size, RpgObject *parent = nullptr): RpgObject(parent){
		this->setSize(size);
	}

	void setSize(const QSize &size){
		if(size.width() <= 0 || size.height() <= 0){
			rError() << "Map size cannot be ZERO or NEGATIVE size, given size is:" << size;
			return;
		}
		if(map.length() != 0){
			QVarLengthArray<QGraphicsPixmapItem> mapRow = map.at(0);
			QSize currentSize = QSize(this->map.length(), mapRow.length());
			if(currentSize == size){
				rWarning() << "New map size == current map size, ignored.";
				return;
			}
		}
		int height = size.height();
		if(height != this->map.length()){
			this->map.resize(height);
		}
		int width = size.width();
		if(width != this->map.at(0).length()){
			for(QVarLengthArray<QGraphicsPixmapItem, RpgMapWidthPrealloc> &i: this->map){
				i.resize(width);
			}
		}
	}

	const QSize getSize() const{
		return QSize(this->map.length() > 0 ? this->map.at(0).length() : 0, this->map.length());
	}

	const QGraphicsPixmapItem &getMapTile(const QPoint &pos) const{
		QSize mapSize = this->getSize();
		if(mapSize.width() <= pos.x()){
			rError() << "Given pos:" << pos << "out of map width. Map size:" << mapSize;
			return this->nullItem;
		}
		if(mapSize.height() <= pos.y()){
			rError() << "Given pos:" << pos << "out of map height. Map size:" << mapSize;
			return this->nullItem;
		}
		map.a;
		return this->map.at(pos.y()).at(pos.x());
	}

	quint8 getMapTileNeghbor

};

#endif // RPGMAP_H
