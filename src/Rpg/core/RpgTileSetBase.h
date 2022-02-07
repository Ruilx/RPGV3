#ifndef RPGTILESETBASE_H
#define RPGTILESETBASE_H

#include <QtCore>
#include <QImage>
#include <QPixmap>
#include <Rpg/Global.h>

#include <Rpg/exception/RpgNullPointerException.h>
#include <Rpg/exception/RpgMapKeyNotFoundException.h>

class RpgTileSetBase
{
	int rows = 0;
	int cols = 0;
protected:
	QMap<quint64, QImage*> imageList;

	void readHandle(const QString &filename){
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

	quint64 loc2Index(const QPoint &loc) const{
		return (quint64)(loc.x()) << 32 | (quint64)(loc.y());
	}
public:
	RpgTileSetBase(const QString &filename = QString()){
		if(!filename.isEmpty()){
			this->readHandle(filename);
		}
	}

	QImage *getRpgTile(int x, int y) const{
		return this->getRpgTile(QPoint(x, y));
	}

	QImage *getRpgTile(const QPoint &loc) const{
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

	QPixmap getRpgTilePixmap(int x, int y) const{
		QImage *image = getRpgTile(x, y);
		if(image == nullptr){
			throw RpgNullPointerException("image");
		}
		return QPixmap::fromImage(*image);
	}

	QPixmap getRpgTilePixmap(const QPoint &loc) const{
		QImage *image = getRpgTile(loc);
		if(image == nullptr){
			throw RpgNullPointerException("image");
		}
		return QPixmap::fromImage(*image);
	}

	inline int getRows() const{ return this->rows; }
	inline int getCols() const{ return this->cols; }
};

#endif // RPGTILESETBASE_H
