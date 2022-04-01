#include "RpgAutoTileRendererSpecLocation.h"

#include <QPainter>

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
		QPixmap blockImage = this->originTilePixmap.copy(this->adjustRect(QRect(0, 0, width, height), count));

		// 背景草地
		QPixmap backgroundBase = blockImage.copy(RpgAutoTileRendererSpecLocation::centerRect);
		this->insertTile(Rpg::None, backgroundBase);

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
			QPixmap leftTopBaseLoad = leftTopBase;
			QPainter p(&leftTopBaseLoad); // 左上角内拐块
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(leftBottomPos, leftBottomBase, QRect(leftBottomPos, QSize(32, 16)));
			p.end();
			this->insertTile(Rpg::RightPassedSingleLine, leftTopBaseLoad);	// 保存右缺口块 EF
		}{
			QPixmap leftTopBaseLoad = leftTopBase;
			QPainter p(&leftTopBaseLoad); // 左上角内拐块
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(rightTopPos, rightTopBase, QRect(rightTopPos, QSize(16, 32)));
			p.end();
			this->insertTile(Rpg::BottomPassedSingleLine, leftTopBaseLoad);	// 保存下缺口块 FB
		}{
			QPixmap rightTopBaseLoad = rightTopBase;
			QPainter p(&rightTopBaseLoad);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(leftBottomPos, rightBottomBase, QRect(leftBottomPos, QSize(32, 16)));
			p.end();
			this->insertTile(Rpg::LeftPassedSingleLine, rightTopBaseLoad);	// 保存左缺口块 FE
		}{
			QPixmap rightBottomBaseLoad = rightBottomBase;
			QPainter p(&rightBottomBaseLoad);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(leftTopPos, leftBottomBase, QRect(leftTopPos, QSize(16, 32)));
			p.end();
			this->insertTile(Rpg::TopPassedSingleLine, rightBottomBaseLoad);	// 保存上缺口块 BF
		}

		// 内拐的块, 四个方向, 半成品
		QPixmap leftTopInnerBase = blockImage.copy(this->leftTopInnerCornerRect);	// 16x16
		QPixmap leftBottomInnerBase = blockImage.copy(this->leftBottomInnerCornerRect); // 16x16
		QPixmap rightTopInnerBase = blockImage.copy(this->rightTopInnerCornerRect); // 16x16
		QPixmap rightBottomInnerBase = blockImage.copy(this->rightBottomInnerCornerRect); // 16x16

		// 外拐块和内拐块合并
		{
			QPainter p(&leftTopBase);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightBottomPos, leftTopInnerBase);
			p.end();
			this->insertTile(Rpg::LeftTopSingleCorner, leftTopBase);	// 保存左上角单行块 EB
		}{
			QPainter p(&rightTopBase);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			p.end();
			this->insertTile(Rpg::RightTopSingleCorner, rightTopBase);	// 保存右上角单行块 FA
		}{
			QPainter p(&leftBottomBase);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, leftBottomInnerBase);
			p.end();
			this->insertTile(Rpg::LeftBottomSingleCorner, leftBottomBase);	// 保存左下角单行块 AF
		}{
			QPainter p(&rightBottomBase);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, rightBottomInnerBase);
			p.end();
			this->insertTile(Rpg::RightBottomSingleCorner, rightBottomBase);	// 保存右下角单行块 BE
		}

		// 4个内拐的块和背景合成
		QPixmap multiAreaLeftBg;
		QPixmap multiAreaRightBg;
		QPixmap multiAreaTopBg;
		QPixmap multiAreaBottomBg;

		QPixmap leftTopInnerBg;
		QPixmap rightTopInnerBg;
		QPixmap leftBottomInnerBg;
		QPixmap rightBottomInnerBg;
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, rightBottomInnerBase); // 左上角外拐块样式是右下样式
			this->insertTile(Rpg::LeftTopMultiInnerCorner, backgroundLoad);	// 保存左上角外拐块 80
			leftTopInnerBg = backgroundLoad;
			p.drawPixmap(rightTopPos, leftBottomInnerBase); // 右上角外拐块的样式是左下样式
			this->insertTile(Rpg::MultiAreaTop, backgroundLoad);				// 保存左上角和右上角的外拐块 A0
			multiAreaTopBg = backgroundLoad;
			p.drawPixmap(rightBottomPos, leftTopInnerBase); // 右下角外拐块的样式是左上样式
			this->insertTile(Rpg::MultiHubRightTop, backgroundLoad);			// 保存左上角和右上角和右下角的外拐块 A8
			p.end();
		}{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightBottomPos, leftTopInnerBase);
			this->insertTile(Rpg::MultiAreaBackSlash, backgroundLoad);		// 保存左上角和右下角外拐块 88
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			this->insertTile(Rpg::MultiHubLeftBottom, backgroundLoad);		// 保存左上角,左下角和右下角的外拐块 8A
			p.end();
		}{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			p.end();
			this->insertTile(Rpg::MultiAreaLeft, backgroundLoad);			// 保存左上角和左下角的外拐块 82
			multiAreaLeftBg = backgroundLoad;
		}{
			QPixmap backgroundLoad = multiAreaTopBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			p.end();
			this->insertTile(Rpg::MultiHubLeftTop, backgroundLoad);			// 保存左上角, 右上角和左下角的外拐块 A2
		}{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, leftBottomInnerBase);
			this->insertTile(Rpg::RightTopMultiInnerCorner, backgroundLoad); // 保存右上角外拐块 20
			rightTopInnerBg = backgroundLoad;
			p.drawPixmap(rightBottomPos, leftTopInnerBase);
			this->insertTile(Rpg::MultiAreaRight, backgroundLoad);			// 保存右上角和右下角外拐块 28
			multiAreaRightBg = backgroundLoad;
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			this->insertTile(Rpg::MultiHubRightBottom, backgroundLoad);		// 保存右上角, 右下角和左下角的外拐块 2A
			p.end();
		}{
			QPixmap backgroundLoad = rightTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			p.end();
			this->insertTile(Rpg::MultiAreaSlash, backgroundLoad);			// 保存右上角和左下角的外拐块 22
		}{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightBottomPos, leftTopInnerBase);
			this->insertTile(Rpg::RightBottomMultiInnerCorner, backgroundLoad);	// 保存右下角外拐块 08
			rightBottomInnerBg = backgroundLoad;
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			p.end();
			this->insertTile(Rpg::MultiAreaBottom, backgroundLoad);				// 保存左下角和右下角外拐块 0A
			multiAreaBottomBg = backgroundLoad;

		}{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			p.end();
			this->insertTile(Rpg::LeftBottomMultiInnerCorner, backgroundLoad);	// 保存左下角外拐块 02
			leftBottomInnerBg = backgroundLoad;
		}

		// 竖直的块(16x32竖, 32x16横)
		QPixmap leftBase = blockImage.copy(leftVHalfRect);
		QPixmap rightBase = blockImage.copy(rightVHalfRect);
		QPixmap topBase = blockImage.copy(topHHalfRect);
		QPixmap bottomBase = blockImage.copy(bottomHHalfRect);

		// 2x外拐块和边
		{
			QPixmap backgroundLoad = multiAreaLeftBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, rightBase);
			p.end();
			this->insertTile(Rpg::RightSingleTee, backgroundLoad);				// 保存左上, 左下外拐块和右边 BA
		}{
			QPixmap backgroundLoad = multiAreaRightBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			p.end();
			this->insertTile(Rpg::LeftSingleTee, backgroundLoad);				// 保存右上, 右下外拐块和左边 AB
		}{
			QPixmap backgroundLoad = multiAreaBottomBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			p.end();
			this->insertTile(Rpg::TopSingleTee, backgroundLoad);					// 保存左下, 右下外拐块和上边 EA
		}{
			QPixmap backgroundLoad = multiAreaTopBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			p.end();
			this->insertTile(Rpg::BottomSingleTee, backgroundLoad);				// 保存左上, 右上外拐块和下边 AE
		}

		// 1x外拐块和边, 两边
		{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, rightBase);
			this->insertTile(Rpg::RightMultiTeeTop, backgroundLoad);				// 保存左上外拐块和右边 B8
			p.drawPixmap(leftTopPos, leftBase);
			this->insertTile(Rpg::VerticalPassedSingleLine, backgroundLoad);		// 保存左边和右边 BB
			p.end();
		}{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			this->insertTile(Rpg::AutoTileBlockType::BottomMultiTeeLeft, backgroundLoad);			// 保存左上外拐块和下边 8E
			p.drawPixmap(leftTopPos, topBase);
			this->insertTile(Rpg::AutoTileBlockType::HorizonalPassedSingleLine, backgroundLoad);	// 保存上边和下边 EE
			p.end();
		}{
			QPixmap backgroundLoad = rightTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			p.end();
			this->insertTile(Rpg::LeftMultiTeeTop, backgroundLoad);				// 保存左边和右上外拐块 A3
		}{
			QPixmap backgroundLoad = rightTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			p.end();
			this->insertTile(Rpg::BottomMultiTeeRight, backgroundLoad);			// 保存下边和右上角外拐块 2E
		}{
			QPixmap backgroundLoad = leftBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			p.end();
			this->insertTile(Rpg::TopMultiTeeLeft, backgroundLoad);				// 保存上边和左下角外拐块 E2
		}{
			QPixmap backgroundLoad = leftBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, rightBase);
			p.end();
			this->insertTile(Rpg::RightMultiTeeBottom, backgroundLoad);			// 保存左下角外拐块和右边 3A
		}{
			QPixmap backgroundLoad = rightBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			p.end();
			this->insertTile(Rpg::LeftMultiTeeBottom, backgroundLoad);			// 保存左边和右下角外拐块 8B
		}{
			QPixmap backgroundLoad = rightBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			p.end();
			this->insertTile(Rpg::TopMultiTeeRight, backgroundLoad);				// 保存上边和右下角外拐块 E8
		}

		// 单边
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			p.end();
			this->insertTile(Rpg::LeftMulti, backgroundLoad);					// 保存左边的边 83
		}{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			p.end();
			this->insertTile(Rpg::TopMulti, backgroundLoad);						// 保存上边的边 E0
		}{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, rightBase);
			p.end();
			this->insertTile(Rpg::RightMulti, backgroundLoad);					// 保存右边的边 38
		}{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			p.end();
			this->insertTile(Rpg::BottomMulti, backgroundLoad);					// 保存下边的边 0E
		}
	}
}
