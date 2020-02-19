#include "RpgAutoTileBase.h"

#include <QPainter>
#include <Rpg/core/RpgFileManager.h>

void RpgAutoTileBase::renderBlock(const QString &autoTileFileName)
{
	QPixmap blockImageOrigin;
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::ImageFile, autoTileFileName);
	if(filename.isEmpty()){
		qDebug() << CodePath << "Name" << autoTileFileName << "File not found.";
		return;
	}

	blockImageOrigin.load(filename);
	if(blockImageOrigin.isNull()){
		qDebug() << CodePath << "load block texture failed.";
		return;
	}
	if(blockImageOrigin.height() < AutoTileImageBlockHeight || blockImageOrigin.width() < AutoTileImageBlockWidth){
		qDebug() << CodePath << "load block texture is not fit.";
		return;
	}
	int count = blockImageOrigin.width() / AutoTileImageBlockWidth;
	for(int i = 0; i < count; i++){
		RpgAutoTileBlock block;

		QPixmap blockImage = blockImageOrigin.copy(adjustRect(QRect(0, 0, AutoTileImageBlockWidth, AutoTileImageBlockHeight), count));

		// 背景草地
		QPixmap backgroundBase = blockImage.copy(this->centerRect); // 32x32
		block.insertImage(RpgAutoTileBlock::None, backgroundBase);		// 保存背景块

		// 1x1单独的块
		block.insertImage(RpgAutoTileBlock::ClosedFrame, blockImage.copy(this->adjustRect(this->singleBlockRect, i)));	// 保存封闭块
		QPixmap leftTopBase = blockImage.copy(this->leftTopRect);	// 32x32
		block.insertImage(RpgAutoTileBlock::LeftTopMultiCorner, leftTopBase);		// 保存左上角内拐块E3(checked)
		QPixmap rightTopBase = blockImage.copy(this->rightTopRect);	// 32x32
		block.insertImage(RpgAutoTileBlock::RightTopMultiCorner, rightTopBase);	// 保存右上角内拐块F8(checked)
		QPixmap leftBottomBase = blockImage.copy(this->leftBottomRect);	// 32x32
		block.insertImage(RpgAutoTileBlock::LeftBottomMultiCorner, leftBottomBase); // 保存左下角内拐块8F(checked)
		QPixmap rightBottomBase = blockImage.copy(this->rightBottomRect);	// 32x32
		block.insertImage(RpgAutoTileBlock::RightBottomMultiCorner, rightBottomBase);	// 保存右下角内拐块3E(checked)
		QPixmap singleHub = blockImage.copy(this->singleHubRect);	// 32x32
		block.insertImage(RpgAutoTileBlock::SingleHub, singleHub);						// 保存四角外拐块AA(checked)
		{
			QPixmap leftTopBaseLoad = leftTopBase;
			QPainter p(&leftTopBaseLoad); // 左上角内拐块
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(leftBottomPos, leftBottomBase, QRect(leftBottomPos, QSize(32, 16)));
			block.insertImage(RpgAutoTileBlock::RightPassedSingleLine, leftTopBaseLoad);	// 保存右缺口块EF(checked)
			p.end();
		}
		{
			QPixmap leftTopBaseLoad = leftTopBase;
			QPainter p(&leftTopBaseLoad); // 左上角内拐块
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(rightTopPos, rightTopBase, QRect(rightTopPos, QSize(16, 32)));
			block.insertImage(RpgAutoTileBlock::BottomPassedSingleLine, leftTopBaseLoad);	// 保存下缺口块FB(checked)
			p.end();
		}
		{
			QPixmap rightTopBaseLoad = rightTopBase;
			QPainter p(&rightTopBaseLoad);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(leftBottomPos, rightBottomBase, QRect(leftBottomPos, QSize(32, 16)));
			block.insertImage(RpgAutoTileBlock::LeftPassedSingleLine, rightTopBaseLoad);	// 保存左缺口块FE(checked)
			p.end();
		}
		{
			QPixmap rightBottomBaseLoad = rightBottomBase;
			QPainter p(&rightBottomBaseLoad);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawPixmap(leftTopPos, leftBottomBase, QRect(leftTopPos, QSize(16, 32)));
			block.insertImage(RpgAutoTileBlock::TopPassedSingleLine, rightBottomBaseLoad);	// 保存上缺口块BF(checked)
			p.end();
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
			block.insertImage(RpgAutoTileBlock::LeftTopSingleCorner, leftTopBase);	// 保存左上角单行块EB(checked)
			p.end();
		}
		{
			QPainter p(&rightTopBase);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::RightTopSingleCorner, rightTopBase);	// 保存右上角单行块FA(checked)
			p.end();
		}
		{
			QPainter p(&leftBottomBase);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, leftBottomInnerBase);
			block.insertImage(RpgAutoTileBlock::LeftBottomSingleCorner, leftBottomBase);	// 保存左下角单行块AF(checked)
			p.end();
		}
		{
			QPainter p(&rightBottomBase);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, rightBottomInnerBase);
			block.insertImage(RpgAutoTileBlock::RightBottomSingleCorner, rightBottomBase);	// 保存右下角单行块BE(checked)
			p.end();
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
			block.insertImage(RpgAutoTileBlock::LeftTopMultiInnerCorner, backgroundLoad);	// 保存左上角外拐块80(checked)
			leftTopInnerBg = backgroundLoad;
			p.drawPixmap(rightTopPos, leftBottomInnerBase); // 右上角外拐块的样式是左下样式
			block.insertImage(RpgAutoTileBlock::MultiAreaTop, backgroundLoad);				// 保存左上角和右上角的外拐块A0(checked)
			multiAreaTopBg = backgroundLoad;
			p.drawPixmap(rightBottomPos, leftTopInnerBase); // 右下角外拐块的样式是左上样式
			block.insertImage(RpgAutoTileBlock::MultiHubRightTop, backgroundLoad);			// 保存左上角和右上角和右下角的外拐块A8(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightBottomPos, leftTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiAreaBackSlash, backgroundLoad);		// 保存左上角和右下角外拐块88(checked)
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiHubLeftBottom, backgroundLoad);		// 保存左上角,左下角和右下角的外拐块8A(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiAreaLeft, backgroundLoad);			// 保存左上角和左下角的外拐块82(checked)
			multiAreaLeftBg = backgroundLoad;
			p.end();
		}
		{
			QPixmap backgroundLoad = multiAreaTopBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiHubLeftTop, backgroundLoad);			// 保存左上角, 右上角和左下角的外拐块A2(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, leftBottomInnerBase);
			block.insertImage(RpgAutoTileBlock::RightTopMultiInnerCorner, backgroundLoad); // 保存右上角外拐块20(checked)
			rightTopInnerBg = backgroundLoad;
			p.drawPixmap(rightBottomPos, leftTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiAreaRight, backgroundLoad);			// 保存右上角和右下角外拐块28(checked)
			multiAreaRightBg = backgroundLoad;
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiHubRightBottom, backgroundLoad);		// 保存右上角, 右下角和左下角的外拐块2A(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = rightTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiAreaSlash, backgroundLoad);			// 保存右上角和左下角的外拐块22(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightBottomPos, leftTopInnerBase);
			block.insertImage(RpgAutoTileBlock::RightBottomMultiInnerCorner, backgroundLoad);	// 保存右下角外拐块08(checked)
			rightBottomInnerBg = backgroundLoad;
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::MultiAreaBottom, backgroundLoad);				// 保存左下角和右下角外拐块0A(checked)
			multiAreaBottomBg = backgroundLoad;
			p.end();
		}
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, rightTopInnerBase);
			block.insertImage(RpgAutoTileBlock::LeftBottomMultiInnerCorner, backgroundLoad);	// 保存左下角外拐块02(checked)
			leftBottomInnerBg = backgroundLoad;
			p.end();
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
			block.insertImage(RpgAutoTileBlock::RightSingleTee, backgroundLoad);				// 保存左上, 左下外拐块和右边BA(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = multiAreaRightBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			block.insertImage(RpgAutoTileBlock::LeftSingleTee, backgroundLoad);				// 保存右上, 右下外拐块和左边AB(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = multiAreaBottomBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			block.insertImage(RpgAutoTileBlock::TopSingleTee, backgroundLoad);					// 保存左下, 右下外拐块和上边EA(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = multiAreaTopBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			block.insertImage(RpgAutoTileBlock::BottomSingleTee, backgroundLoad);				// 保存左上, 右上外拐块和下边AE(checked)
			p.end();
		}

		// 1x外拐块和边, 两边
		{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, rightBase);
			block.insertImage(RpgAutoTileBlock::RightMultiTeeTop, backgroundLoad);				// 保存左上外拐块和右边B8(checked)
			p.drawPixmap(leftTopPos, leftBase);
			block.insertImage(RpgAutoTileBlock::VerticalPassedSingleLine, backgroundLoad);		// 保存左边和右边BB(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = leftTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			block.insertImage(RpgAutoTileBlock::BottomMultiTeeLeft, backgroundLoad);			// 保存左上外拐块和下边8E(checked)
			p.drawPixmap(leftTopPos, topBase);
			block.insertImage(RpgAutoTileBlock::HorizonalPassedSingleLine, backgroundLoad);	// 保存上边和下边EE(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = rightTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			block.insertImage(RpgAutoTileBlock::LeftMultiTeeTop, backgroundLoad);				// 保存左边和右上外拐块A3(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = rightTopInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			block.insertImage(RpgAutoTileBlock::BottomMultiTeeRight, backgroundLoad);			// 保存下边和右上角外拐块2E(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = leftBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			block.insertImage(RpgAutoTileBlock::TopMultiTeeLeft, backgroundLoad);				// 保存上边和左下角外拐块E2(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = leftBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, rightBase);
			block.insertImage(RpgAutoTileBlock::RightMultiTeeBottom, backgroundLoad);			// 保存左下角外拐块和右边3A(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = rightBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			block.insertImage(RpgAutoTileBlock::LeftMultiTeeBottom, backgroundLoad);			// 保存左边和右下角外拐块8B(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = rightBottomInnerBg;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			block.insertImage(RpgAutoTileBlock::TopMultiTeeRight, backgroundLoad);				// 保存上边和右下角外拐块E8(checked)
			p.end();
		}

		// 单边
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, leftBase);
			block.insertImage(RpgAutoTileBlock::LeftMulti, backgroundLoad);					// 保存左边的边83(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftTopPos, topBase);
			block.insertImage(RpgAutoTileBlock::TopMulti, backgroundLoad);						// 保存上边的边E0(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(rightTopPos, rightBase);
			block.insertImage(RpgAutoTileBlock::RightMulti, backgroundLoad);					// 保存右边的边38(checked)
			p.end();
		}
		{
			QPixmap backgroundLoad = backgroundBase;
			QPainter p(&backgroundLoad);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			p.drawPixmap(leftBottomPos, bottomBase);
			block.insertImage(RpgAutoTileBlock::BottomMulti, backgroundLoad);					// 保存下边的边0E(checked)
			p.end();
		}
		this->blockImageList.append(block);
	}
}
