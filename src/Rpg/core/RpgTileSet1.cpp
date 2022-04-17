#include "RpgTileSet1.h"

void RpgTileSetBase1::readHandle(const QString &filename){
	qDebug() << "[INFOR]" << CodePath << "Reading image path:" << filename;
	QImage fileImage(filename);
	if(fileImage.isNull()){
		return;
	}
	int width = fileImage.width();
	int height = fileImage.height();
	if(width % int(MapTileWidth) != 0){
		qDebug() << "[WARNI]" << CodePath << "Source image's width not grid by " << MapTileWidth << "px, may cause mapblocks dislocated.";
	}
	if(height % int(MapTileHeight) != 0){
		qDebug() << "[WARNI]" << CodePath << "Source image's height not grid by " << MapTileHeight << "px, may cause mapblocks dislocated.";
	}
	this->rows = width / MapTileWidth;
	this->cols = width / MapTileHeight;

	for(int i = 0; i < this->rows; i++){
		for(int j = 0; j < this->cols; j++){
			QImage *p = new QImage(fileImage.copy(j * MapTileHeight, i * MapTileWidth, MapTileWidth, MapTileHeight));
			this->imageList.insert((quint64)(j) << 32 | (quint64)(i), p);
		}
	}
}

RpgTileSetBase1::RpgTileSetBase1(const QString &filename){
	if(!filename.isEmpty()){
		this->readHandle(filename);
	}
}

QImage *RpgTileSetBase1::getRpgTile(const QPoint &loc) const{
	quint64 index = this->loc2Index(loc);
	if(!this->imageList.contains(index)){
		throw RpgMapKeyNotFoundException(QString::number(index));
	}
	QImage *image = this->imageList.value(index, new QImage());
	if(image == nullptr){
		throw RpgNullPointerException(QObject::tr("this->imageList.value(%1)").arg(index));
	}
	return image;
}

QPixmap RpgTileSetBase1::getRpgTilePixmap(int x, int y) const{
	QImage *image = getRpgTile(x, y);
	if(image == nullptr){
		throw RpgNullPointerException("image");
	}
	return QPixmap::fromImage(*image);
}

QPixmap RpgTileSetBase1::getRpgTilePixmap(const QPoint &loc) const{
	QImage *image = getRpgTile(loc);
	if(image == nullptr){
		throw RpgNullPointerException("image");
	}
	return QPixmap::fromImage(*image);
}
