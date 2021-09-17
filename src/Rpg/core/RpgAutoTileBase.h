#ifndef RPGAUTOTILEBASE_H
#define RPGAUTOTILEBASE_H

#include <QtCore>
#include <QDebug>
#include <Rpg/Global.h>
#include <Rpg/core/RpgAutoTileBlock.h>

#define AutoTileImageBlockWidth 96
#define AutoTileImageBlockHeight 128

/**
 * @brief The RpgAutoTileBase class
 *
 * RpgAutoTileBase记录着自动边框平铺的图片切割的算法以及提供切割后拼接的各个位置的图片
 *
 */
class RpgAutoTileBase{

	QString name;
	QImage originImage;

	QList<RpgAutoTileBlock> blockImageList;

	const QRect singleBlockRect = QRect(0, 0, 32, 32);				// ClosedFrame
	const QRect backgroundRect = QRect(32, 0, 32, 32);				// Unknown
	const QRect singleHubRect = QRect(64, 0, 32, 32);				// SingleHub
	const QRect rightBottomInnerCornerRect = QRect(64, 0, 16, 16);	// RightBottomMultiInnerCorner
	const QRect leftBottomInnerCornerRect = QRect(80, 0, 16, 16);		// LeftBottomMultiInnerCorner
	const QRect rightTopInnerCornerRect = QRect(64, 16, 16, 16);		// RightTopMultiInnerCorner
	const QRect leftTopInnerCornerRect = QRect(80, 16, 16, 16);		// LeftTopMultiInnerCorner

	const QPoint rightBottomInnerCornerLocalOffset = QPoint(0, 0);
	const QPoint leftBottomInnerCornerLocalOffset = QPoint(16, 0);
	const QPoint rightTopInnerCornerLocalOffset = QPoint(0, 16);
	const QPoint leftTopInnerCornerLocalOffset = QPoint(16, 16);

	const QRect leftTopRect = QRect(0, 32, 32, 32);					// LeftTopMultiCorner
	const QRect topRect = QRect(32, 32, 32, 32);					// TopMulti
	const QRect rightTopRect = QRect(64, 32, 32, 32);				// RightTopMultiCorner
	const QRect leftRect = QRect(0, 64, 32, 32);					// LeftMulti
	const QRect centerRect = QRect(32, 64, 32, 32);					// None
	const QRect rightRect = QRect(64, 64, 32, 32);					// RightMulti
	const QRect leftBottomRect = QRect(0, 96, 32, 32);				// LeftBottomMultiCorner
	const QRect bottomRect = QRect(32, 96, 32, 32);					// BottomMulti
	const QRect rightBottomRect = QRect(64, 96, 32, 32);			// RightBottomMultiCorner

	const QRect topHHalfRect = QRect(32, 32, 32, 16);				// top32 Top
	const QRect bottomHHalfRect = QRect(32, 112, 32, 16);			// bottom32 Bottom
	const QRect leftVHalfRect = QRect(0, 64, 16, 32);				// left32 Left
	const QRect rightVHalfRect = QRect(80, 64, 16, 32);				// right32 Right

	const QPoint leftTopPos = QPoint(0, 0);
	const QPoint leftBottomPos = QPoint(0, 16);
	const QPoint rightTopPos = QPoint(16, 0);
	const QPoint rightBottomPos = QPoint(16, 16);
public:
	/**
	 * @brief The SplitBlock enum
	 * 在组合图像中九宫格图片中指定图片表示位置的顺序
	 */
	enum SplitBlock{
		LeftTop = 0,		// 左上角 ↖
		Top = 1,			// 上边  ↑
		RightTop = 2,		// 右上角 ↗
		Left = 3,			// 左边  ←
		Center = 4,			// 中间  ·
		Right = 5,			// 右边  →
		LeftBottom = 6,		// 左下角 ↙
		Bottom = 7,			// 下边  ↓
		RightBottom = 8		// 右下角 ↘
	};

	enum SplitCorner{
		RightBottomCorner = 0,
		LeftBottomCorner = 1,
		RightTopCorner = 2,
		LeftTopCorner = 3
	};

private:
	inline QRect adjustRect(QRect original, int offset = 0) const {
		return QRect(original.left() * (offset + 1), original.top(), original.width(), original.height());
	}

	void renderBlock(const QString &autoTileFileName);

public:
	RpgAutoTileBase(const QString &name, const QString &autoTileName = QString()){
		if(name.isEmpty()){
			// Todo: 名称不能传空, 如果传空可能需要抛出异常ValueError
			return;
		}
		this->name = name;
		if(!autoTileName.isEmpty()){
			this->renderBlock(autoTileName);
			qDebug() << CodePath << "Name" << autoTileName << "Loaded" << this->blockImageList.at(0).getCount() << "Tiles";
		}
	}

	void setName(const QString &name){
		this->name = name;
	}

	QString getName() const{
		return this->name;
	}

	void setAutoTileFileName(const QString &filename){
		this->blockImageList.clear();
		this->renderBlock(filename);
	}



	void _dumpImage(int index = 0){
		if((this->blockImageList.length() > index)){
			this->blockImageList.at(index)._dumpAllImages("temp");
		}else{
			qDebug() << CodePath << "Index is out of range.";
		}

	}
};

#endif // RPGAUTOTILEBASE_H
