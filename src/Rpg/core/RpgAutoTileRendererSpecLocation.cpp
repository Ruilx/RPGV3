#include "RpgAutoTileRendererSpecLocation.h"

// TODO: 注意: autotile也有图片队列

void RpgAutoTileRendererSpecLocation::renderTiles() NoThrow{
	if(this->originTilePixmap.isNull()){
		qDebug() << CodePath << "origin pixmap is null.";
		return;
	}
	const int width = RpgAutoTileRendererSpecLocation::AutoTilePixelSize.width();
	const int height = RpgAutoTileRendererSpecLocation::AutoTilePixelSize.height();

	if(this->originTilePixmap.height() < height || this->originTilePixmap.width() < width){
		qDebug() << CodePath << "loaded block texture size" << this->originTilePixmap.size() << "is not fit:" << RpgAutoTileRendererSpecLocation::AutoTilePixelSize;
		return;
	}
	int count = this->originTilePixmap.width() / width;
	for(int i = 0; i < count; i++){
		QPixmap blockImage = this->originTilePixmap.copy(adjustRect(QRect(0, 0, width, height), count));

		// 背景草地
		QPixmap backgroundBase = blockImage.copy(RpgAutoTileRendererSpecLocation::centerRect);
		this->tilesMap.insert(Rpg::None, backgroundBase);

		// 1x1单独的块
		QPixmap closedFrame = blockImage.copy(RpgAutoTileRendererSpecLocation::singleBlockRect);
		this->insertTile(Rpg::ClosedFrame, closedFrame);	// 保存封闭块
		QPixmap leftTopBase = blockImage.copy(this->leftTopRect);
		this->insertTile(Rpg::LeftTopMultiCorner, leftTopBase);	// 保存左上角内拐块 E3
		QPixmap rightTopBase = blockImage.copy(this->rightTopRect);
		this->insertTile(Rpg::RightTopMultiCorner, rightTopBase);	// 保存右上角内拐块 F8
		QPixmap leftBottomBase = blockImage.copy(this->leftBottomRect);
		this->insertTile(Rpg::LeftBottomMultiCorner, leftBottomBase); // 保存左下角内拐块 8F
		QPixmap rightBottomBase = blockImage.copy(this->rightBottomRect);
		this->insertTile(Rpg::RightBottomMultiCorner, rightBottomBase);	// 保存右下角内拐块 3E
		QPixmap singleHub = blockImage.copy(this->singleHubRect);
		this->insertTile(Rpg::SingleHub, singleHub);	// 保存四角外拐块 AA
		{
			QPainter p(&leftTopBase); // 左上角内拐块
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(leftBottomPos, leftBottomBase, QRect(leftBottomPos, QSize(32, 16)));
			p.end();
			this->insertTile(Rpg::RightPassedSingleLine, leftTopBase);	// 保存右缺口块 EF
		}{
			QPainter p(&leftTopBase); // 左上角内拐块
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(rightTopPos, rightTopBase, QRect(rightTopPos, QSize(16, 32)));
			p.end();
			this->insertTile(Rpg::BottomPassedSingleLine, leftTopBase);	// 保存下缺口块 FB
		}{

		}





	}
}
