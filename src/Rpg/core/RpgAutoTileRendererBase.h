#ifndef RPGAUTOTILERENDERERBASE_H
#define RPGAUTOTILERENDERERBASE_H

#include <QtCore>
#include <QDebug>
#include <QPixmap>

#include <Rpg/Global.h>
#include <Rpg/Rpg.h>
#include <Rpg/core/RpgFileManager.h>

/**
 * @brief The RpgAutoTileRendererBase class
 *
 * 从RpgAutoTileBase转移而来, 作为autotile的渲染基础类
 * 有其他渲染方式可继承此类的renderBlock函数
 */
class RpgAutoTileRendererBase
{

protected:
	static const int ArrayListInitCapacity = 8;
	typedef QVarLengthArray<QPixmap, RpgAutoTileRendererBase::ArrayListInitCapacity> RpgSeqTilesArray;
	/**
	 * @brief tilePixmap
	 * 使用QMap存储autotile的47张拼接图片, 使用qint8标记每张tile.
	 * 键: quint8的标记方法:
	 * 使用其中的每个位记录对应方向位置上的tile, 如四周没有相同tile, 则会调用相应的边界tile图片.
	 * 相应位置标记:
	 * 0b x x x x x x x x
	 *    │ │ │ │ │ │ │ └─ 左     左上  →  上  →  右上
	 *    │ │ │ │ │ │ └─── 左下       ┌──────────┐  ↓
	 *    │ │ │ │ │ └───── 下         │          │
	 *    │ │ │ │ └─────── 右下    左 │   Tile   │ 右
	 *    │ │ │ └───────── 右      ↑  │          │  ↓
	 *    │ │ └─────────── 右上       └──────────┘
	 *    │ └───────────── 上     左下  ←  下  ←  右下
	 *    └─────────────── 左上
	 *  如果设置了'上', '下', '左'或者'右'方向上的标志, 则对应方向上两角的位置也自动置1
	 *        上   左   下   右
	 *  左上   1    1    x    x
	 *  右上   1    x    x    1
	 *  左下   x    1    1    x
	 *  右下   x    x    1    1
	 *  [ x = 无关, 1 = 自动置1 ]
	 *  每个tile还预置了8个显示帧图片(一般是4个)
	 */
	QMap<quint8, RpgSeqTilesArray* > tilesMap;

	QString name;

	QPixmap originTilePixmap;

	QPixmap emptyPixmap;

	/**
	 * @brief RpgAutoTileRendererBase
	 * @param name
	 * 渲染父类构造函数, 从名称中获取图片并保存
	 */
	RpgAutoTileRendererBase(const QString &name);

	~RpgAutoTileRendererBase(){
		for(QMap<quint8, RpgSeqTilesArray*>::const_iterator i = this->tilesMap.constBegin(); i != this->tilesMap.constEnd(); i++){
			if(i.value() != nullptr){
				i.value()->clear();
				delete i.value();
			}
		}
		this->tilesMap.clear();
	}

	/**
	 * @brief renderTiles
	 * 虚函数, 渲染工作函数, 需要继承实现
	 */
	virtual void renderTiles() NoThrow Pure;

public:
	/**
	 * @brief insertTile
	 * @param type
	 * @param pixmap
	 * 插入tile函数
	 */
	void insertTile(Rpg::AutoTileBlockType type, const QPixmap &pixmap);

	/**
	 * @brief getTile
	 * @param location
	 * @return
	 * 拿取tile函数
	 */
	const QPixmap &getTile(Rpg::AutoTileBlockLocation location, int sequence) const;

	const RpgAutoTileRendererBase::RpgSeqTilesArray *getTiles(Rpg::AutoTileBlockLocation location) const;

	const QPixmap &getTileByType(Rpg::AutoTileBlockType type, int sequence) const;

	const RpgAutoTileRendererBase::RpgSeqTilesArray *getTilesByType(Rpg::AutoTileBlockType type) const;

	/**
	 * @brief dumpTiles
	 * @param savingDir
	 * 测试函数, 将每个tile图片存储导出
	 */
	void dumpTiles(const QDir &savingDir);
};

#endif // RPGAUTOTILERENDERERBASE_H
