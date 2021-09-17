#ifndef RPGAUTOTILEBLOCK_H
#define RPGAUTOTILEBLOCK_H

#include <Rpg/Rpg.h>
#include <QMap>
#include <QPixmap>
#include <QDebug>

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

public:
	enum BlockType{
		None						= 0x00,
		LeftBottomMultiInnerCorner	= 0x02,
		RightBottomMultiInnerCorner	= 0x08,
		MultiAreaBottom				= 0x0A,
		BottomMulti					= 0x0E,
		RightTopMultiInnerCorner	= 0x20,
		MultiAreaSlash				= 0x22,
		MultiAreaRight				= 0x28,
		MultiHubRightBottom			= 0x2A,
		BottomMultiTeeRight			= 0x2E,
		RightMulti					= 0x38,
		RightMultiTeeBottom			= 0x3A,
		RightBottomMultiCorner		= 0x3E,
		LeftTopMultiInnerCorner		= 0x80,
		MultiAreaLeft				= 0x82,
		LeftMulti					= 0x83,
		MultiAreaBackSlash			= 0x88,
		MultiHubLeftBottom			= 0x8A,
		LeftMultiTeeBottom			= 0x8B,
		BottomMultiTeeLeft			= 0x8E,
		LeftBottomMultiCorner		= 0x8F,
		MultiAreaTop				= 0xA0,
		MultiHubLeftTop				= 0xA2,
		LeftMultiTeeTop				= 0xA3,
		MultiHubRightTop			= 0xA8,
		SingleHub					= 0xAA,
		LeftSingleTee				= 0xAB,
		BottomSingleTee				= 0xAE,
		LeftBottomSingleCorner		= 0xAF,
		RightMultiTeeTop			= 0xB8,
		RightSingleTee				= 0xBA,
		VerticalPassedSingleLine	= 0xBB,
		RightBottomSingleCorner		= 0xBE,
		TopPassedSingleLine			= 0xBF,
		TopMulti					= 0xE0,
		TopMultiTeeLeft				= 0xE2,
		LeftTopMultiCorner			= 0xE3,
		TopMultiTeeRight			= 0xE8,
		TopSingleTee				= 0xEA,
		LeftTopSingleCorner			= 0xEB,
		HorizonalPassedSingleLine	= 0xEE,
		RightPassedSingleLine		= 0xEF,
		RightTopMultiCorner			= 0xF8,
		RightTopSingleCorner		= 0xFA,
		BottomPassedSingleLine		= 0xFB,
		LeftPassedSingleLine		= 0xFE,
		ClosedFrame					= 0xFF
	};

	enum BlockLocation{
		Block_None        = 0b00000000,
		Block_Center      = Block_None,
		Block_LeftTop     = 0b10000000,
		Block_RightTop    = 0b00100000,
		Block_RightBottom = 0b00001000,
		Block_LeftBottom  = 0b00000010,
		Block_Top         = 0b01000000 | Block_LeftTop  | Block_RightTop,
		Block_Right       = 0b00010000 | Block_RightTop | Block_RightBottom,
		Block_Bottom      = 0b00000100 | Block_LeftBottom | Block_RightBottom,
		Block_Left        = 0b00000001 | Block_LeftTop  | Block_LeftBottom,

		Block_LT = Block_LeftTop,
		Block_T  = Block_Top,
		Block_RT = Block_RightTop,
		Block_R  = Block_Right,
		Block_RB = Block_RightBottom,
		Block_B  = Block_Bottom,
		Block_LB = Block_LeftBottom,
		Block_L  = Block_Left,
	};

	~RpgAutoTileBlock(){
		this->imageMap.clear();
	}

	RpgAutoTileBlock(){
		this->imageMap.clear();
	}

	void insertImage(BlockType type, const QImage &image){
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

	QPixmap getImage(BlockLocation location){
		return this->imageMap.value(location, QPixmap());
	}

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
