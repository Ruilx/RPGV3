#ifndef RPGAUTOTILEBLOCK_H
#define RPGAUTOTILEBLOCK_H

#include <QMap>
#include <QPixmap>
#include <QDebug>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgAutoTileBase.h>

/**
 * @brief The RpgAutoTileBlock class
 * RpgAutoTileBlock存储着切割后的每组自动平铺图片的结果, 存储47种平铺可能需要的图片
 * 可以按照摆放的location选择合适的平铺边框图像
 */
class RpgAutoTileBlock{
	/**
	 * @brief imageMap
	 * QMap Image from unsigned int8 to QPixmap
	 * Key: quint8 Note:
	 *
	 * 0b x x x x x x x x
	 *    | | | | | | | +- LEFT         LEFT TOP ---> TOP ---> RIGHT TOP
	 *    | | | | | | +--- LEFT BOTTOM           ┌───────────┐    |
	 *    | | | | | +----- BOTTOM                │           │    V
	 *    | | | | +------- RIGHT BOTTOM     LEFT │           │ RIGHT
	 *    | | | +--------- RIGHT            ^    │           │    |
	 *    | | +----------- RIGHT TOP        |    │           │    V
	 *    | +------------- TOP                   └───────────┘
	 *    +--------------- LEFT TOP  LEFT BOTTOM <- BOTTOM <- RIGHT BOTTOM
	 *
	 * Note that if 'top' 'left' 'right' 'bottom' blocked (set to 1), the rest
	 * area will force to set with table below:
	 *              top left right bottom
	 *  topLeft       1    1     x      x
	 *  topRight      1    x     1      x
	 *  bottomLeft    x    1     x      1
	 *  bottomRight   x    x     1      1
	 *  [ x = no effect, 1 = force to set 1 ]
	 */
	QMap<quint8, QPixmap> imageMap;

	RpgAutoTileBase *base = nullptr;

public:
	RpgAutoTileBlock(RpgAutoTileBase *base){
		this->imageMap.clear();
		if(base == nullptr){
			qDebug() << CodePath << "RpgAutoTileBase is nullptr";
		}
		this->base = base;
	}

	~RpgAutoTileBlock(){
		this->imageMap.clear();
	}


	void insertImage(Rpg::AutoTileBlockType type, const QImage &image){
		QPixmap i = QPixmap::fromImage(image);
		if(i.isNull()){
			qDebug() << CodePath << "Image is null.";
			return;
		}
		this->imageMap.insert(type, i);
		qDebug() << CodePath << "Image information:" << image;
	}

	void insertImage(BlockType type, const QPixmap &pixmap){
		this->imageMap.insert(type, pixmap);
	}

	const QPixmap &getImage(Rpg::AutoTileBlockLocation location) const{
		return this->imageMap.value(location, QPixmap());
	}

	/**
	 * @brief getCount
	 * @return
	 * 这个函数用来干啥的?
	 */
	int getCount() const{
		return this->imageMap.count();
	}

	void _dumpAllImages(const QString &dirpath = QString())const {
		QString _dirPath = dirpath;
		if(dirpath.isEmpty()){
			_dirPath = QDir::currentPath();
		}
		QDir dir(_dirPath);
		if(!dir.exists()){
			QDir::current().mkpath(_dirPath);
		}
		int count = 0;
		for(QMap<quint8, QPixmap>::ConstIterator i = this->imageMap.constBegin(); i != this->imageMap.constEnd(); i++){
			if(!i.value().isNull()){
				i.value().save(dir.path() % "/0x" % QString::number(i.key(), 16) % ".png");
				count ++;
			}
		}
		qDebug() << CodePath << "Successfully dumped" << count << "/ 47 block image(s) at:" << dir.path();
	}
};

#endif // RPGAUTOTILEBLOCK_H
