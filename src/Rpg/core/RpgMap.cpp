#include "RpgMap.h"



void RpgMap::setSize(const QSize &size){
    if(size.width() <= 0 || size.height() <= 0){
        rError() << "Map size cannot be ZERO or NEGATIVE size, given size is:" << size;
        return;
    }
    if(map.length() != 0){
        QVarLengthArray<RpgTileSetBase*, RpgMapWidthPrealloc> mapRow = map.at(0);
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
        for(QVarLengthArray<RpgTileSetBase*, RpgMapWidthPrealloc> &i: this->map){
            i.resize(width);
        }
    }
}

const RpgTileSetBase *RpgMap::getMapTile(const QPoint &pos) const{
    QSize mapSize = this->getSize();
    if(mapSize.width() <= pos.x()){
        rError() << "Given pos:" << pos << "out of map width. Map size:" << mapSize;
        return nullptr;
    }
    if(mapSize.height() <= pos.y()){
        rError() << "Given pos:" << pos << "out of map height. Map size:" << mapSize;
        return nullptr;
    }
    return this->map.at(pos.y()).at(pos.x());
}
