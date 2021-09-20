#ifndef RPGAUTOTILERENDERERSPECLOCATION_H
#define RPGAUTOTILERENDERERSPECLOCATION_H

#include <QtCore>
#include <QDebug>

#include <Rpg/core/RpgAutoTileRendererBase.h>

/**
 * @brief The RpgAutoTileRendererSpecLocation class
 * 继承RpgAutoTileRendererBase, RPGツクール模式, 图像拼接剪裁的自动瓦片渲染器
 */
class RpgAutoTileRendererSpecLocation : public RpgAutoTileRendererBase
{
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

	inline QRect adjustRect(QRect original, int offset = 0) const {
		return QRect(original.left() * (offset + 1), original.top(), original.width(), original.height());
	}

public:
	/**
	 * @brief AutoTilePixelSize
	 * 设置图片如图所示(1制表符 = 2空格宽)
	 * ┃←  32  →┃←  32  →┃←  32  →┃    根据功能可分为12块
	 * ┏━━━━┳━━━━┳━━━━┓━  ┏━━━━┳━━━━┳━━━━┓
	 * ┃┏━━┓┃        ┃┛    ┗┃↑   ┃ Closed ┃  Empty ┃    Four┃
	 * ┃┃    ┃┃        ┃        ┃32  ┃  Frame ┃  Frame ┃ Corners┃
	 * ┃┃    ┃┃        ┃        ┃    ┃   关闭 ┃   空的 ┃  四角的┃
	 * ┃┗━━┛┃        ┃┓    ┏┃↓   ┃ 的样式 ┃   样式 ┃    样式┃
	 * ┣━━━━╋━━━━╋━━━━┫━  ┣━━━━╋━━━━╋━━━━┫
	 * ┃┏━━━┃━━━━┃━━━┓┃↑   ┃ LeftTop┃     Top┃RightTop┃
	 * ┃┃      ┃        ┃      ┃┃32  ┃  Corner┃    Side┃  Corner┃
	 * ┃┃      ┃        ┃      ┃┃    ┃  左上角┃    顶边┃  右上角┃
	 * ┃┃      ┃        ┃      ┃┃↓   ┃基础背景┃基础背景┃基础背景┃
	 * ┣━━━━╋━━━━╋━━━━┫━  ┣━━━━╋━━━━╋━━━━┫
	 * ┃┃      ┃        ┃      ┃┃↑   ┃    Left┃        ┃   Right┃
	 * ┃┃      ┃        ┃      ┃┃32  ┃    Side┃        ┃    Side┃
	 * ┃┃      ┃        ┃      ┃┃    ┃    左边┃        ┃    右边┃
	 * ┃┃      ┃        ┃      ┃┃↓   ┃基础背景┃        ┃基础背景┃
	 * ┣━━━━╋━━━━╋━━━━┫━  ┣━━━━╋━━━━╋━━━━┫
	 * ┃┃      ┃        ┃      ┃┃↑   ┃ LeftBot┃  Bottom┃RightBot┃
	 * ┃┃      ┃        ┃      ┃┃32  ┃  Corner┃    Side┃  Corner┃
	 * ┃┃      ┃        ┃      ┃┃    ┃  左下角┃    下边┃  右下角┃
	 * ┃┗━━━┃━━━━┃━━━┛┃↓   ┃基础背景┃基础背景┃基础背景┃
	 * ┗━━━━┻━━━━┻━━━━┛━  ┗━━━━┻━━━━┻━━━━┛
	 * 每一张auto tile大小都是96x128像素, 根据上图描述可分为4行3列的32x32像素的基础背景
	 * 各个基础背景和空样式或四角样式合并成为47 tile种样式
	 * 如本auto tile有时间轴(动态效果), 会向右平铺其他帧的图像, 渲染方式相同.
	 * ┃←      96      →┃┃←      96      →┃┃←      96      →┃┃←      96      →┃
	 * ┏━━┳━━┳━━┓┏━━┳━━┳━━┓┏━━┳━━┳━━┓┏━━┳━━┳━━┓
	 * ┃    ┃    ┃    ┃┃    ┃    ┃    ┃┃    ┃    ┃    ┃┃    ┃    ┃    ┃
	 * ┣━━╋━━╋━━┫┣━━╋━━╋━━┫┣━━╋━━╋━━┫┣━━╋━━╋━━┫
	 * ┃    ┃ 01 ┃    ┃┃    ┃ 02 ┃    ┃┃    ┃ 03 ┃    ┃┃    ┃ 04 ┃    ┃
	 * ┣━━╋━━╋━━┫┣━━╋━━╋━━┫┣━━╋━━╋━━┫┣━━╋━━╋━━┫
	 * ┃    ┃    ┃    ┃┃    ┃    ┃    ┃┃    ┃    ┃    ┃┃    ┃    ┃    ┃
	 * ┣━━╋━━╋━━┫┣━━╋━━╋━━┫┣━━╋━━╋━━┫┣━━╋━━╋━━┫
	 * ┃    ┃    ┃    ┃┃    ┃    ┃    ┃┃    ┃    ┃    ┃┃    ┃    ┃    ┃
	 * ┗━━┻━━┻━━┛┗━━┻━━┻━━┛┗━━┻━━┻━━┛┗━━┻━━┻━━┛
	 */
	static const QSize AutoTilePixelSize = QSize(96, 128);

	RpgAutoTileRendererSpecLocation(const QString &name): RpgAutoTileRendererBase(name){}

	void renderTiles() NoThrow Final;
};

#endif // RPGAUTOTILERENDERERSPECLOCATION_H
