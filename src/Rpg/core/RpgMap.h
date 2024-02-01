#ifndef RPGMAP_H
#define RPGMAP_H

#include <Rpg/Rpg.h>

#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgTileSetBase.h>

#define RpgMapWidthPrealloc 64
#define RpgMapHeightPrealloc 64

class RpgMap : public RpgObject
{
	Q_OBJECT

	QVarLengthArray<QVarLengthArray<RpgTileSetBase*, RpgMapWidthPrealloc> , RpgMapHeightPrealloc> map;

public:
	RpgMap(const QSize &size, RpgObject *parent = nullptr): RpgObject(parent){
		this->setSize(size);
	}

    void setSize(const QSize &size);

	const QSize getSize() const{
		return QSize(this->map.length() > 0 ? this->map.at(0).length() : 0, this->map.length());
	}

    const RpgTileSetBase *getMapTile(const QPoint &pos) const;

	quint8 getMapTileNeghbor(){ return 0; }

};

#endif // RPGMAP_H
