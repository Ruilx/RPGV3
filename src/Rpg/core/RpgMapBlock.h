#ifndef RPGMAPBLOCK_H
#define RPGMAPBLOCK_H

#include <QGraphicsPixmapItem>
#include <QTimeLine>
#include <QtCore>
#include <Rpg/Global.h>
#include <Rpg/core/RpgTileSet1.h>
#include <Rpg/exception/RpgValueOutOfRangeException.h>

/**
 * @brief The RpgMapBlock class
 * RpgMapBlock类, 单独形成一块背景, 一般是作为背景的物件, 窗口, 柜子, 桌子等物品
 * 如果物体大于1x1, 将会被分解成1x1的块, 由于游戏中每一行都需要设置不同的ZValue, 保证上一层的玩家精灵
 * 会被下一层的物件挡住.
 * RpgMapBlock是一个地图块类, 输入一个Location, 内容, 通过方向设置
 * 可以使用addPixmap添加帧
 *
 * ZValue的定义在Global中预先定义
 *
 * @example 假设有一个三层高的物件, 最后一层未占满地板, 而且ZValue和地面值范围相同
 * 最高一层也能通过, ZValue和物品值范围相同, 中间一层ZValue高于地板, 但不能通过
 *
 *       ***      <- 能通过, 会被下面的块挡住脚
 *  +====...====+
 *  |    ...    | <- 能通过, ZValue值比较高
 *  +-----------+
 *  |           | <- 不能通过, ZValue值比较高
 *  +-----------+
 *  |    ***    | <- 能通过, ZValue值比较低
 *  +====***====+
 *       ***      <- 精灵, ZValue值等于同层行ZValue + 0.4
 *
 * 同层优先级定义:
 * 0.0f - 0.39f ZValue值比较低的显示在玩家后面
 * 0.4f - 0.49f ZValue值表示精灵位置, 其中主角位置为0.49f, 其他角色均小于这个值
 * 0.5f - 0.99f ZValue值比较高的显示在玩家前面
 */
class RpgMapBlock : public QGraphicsPixmapItem, public QObject
{
public:
	enum Direction{
		DirUp    = 0b01000000,
		DirRight = 0b00010000,
		DirDown  = 0b00000100,
		DirLeft  = 0b00000001,
	};
	Q_DECLARE_FLAGS(Directions, Direction)
private:
	/**
	 * @brief mapPixmapList
	 * 图像存储序列
	 */
	QList<QPixmap> mapPixmapList;
	int currentIndex = 0;

	/**
	 * @brief pass
	 * 能够按照相应方式通过设置
	 * Directions Flag
	 */
	//QHash<Direction, bool> pass = {{DirUp, false}, {DirDown, false}, {DirLeft, false}, {DirRight, false}};
	Directions pass;

	/**
	 * @brief timeLine
	 * 时间轴
	 */
	QTimeLine *timeLine = new QTimeLine(1000, this);

	/**
	 * @brief level
	 * 内部ZLevel
	 */
	qreal level = 0.0f;

	/**
	 * @brief setPixmapPrivate
	 * @param content
	 * @param append
	 * 后台设置增加图片的方法
	 */
	void setPixmapPrivate(const QPixmap &content, int index = -1, bool append = false){
		if(!content.isNull()){
			if(content.width() != MapTileWidth || content.height() != MapTileHeight){
				qDebug() << "[WARNI]" << CodePath << QString("Content is not fit at (%1, %2), source (%1, %2)")
							.arg(MapTileWidth).arg(MapTileHeight).arg(content.width()).arg(content.height());
			}
			if(append == false){
				this->clearPixmap();
			}
			if(index == -1){
				this->mapPixmapList.append(content);
			}else{
				this->mapPixmapList.insert(index, content);
			}
			this->showPixmapIndex(0);
		}else{
			qDebug() << "[WARNI]" << CodePath << "Pixmap is null.";
		}
	}

public:
	RpgMapBlock(const QPixmap &content, QGraphicsItem *parentItem = nullptr, QObject *parent = nullptr): QGraphicsPixmapItem(parentItem), QObject(parent){
		this->setPixmapPrivate(content);

		this->timeLine->setLoopCount(0);
		this->connect(this->timeLine, &QTimeLine::frameChanged, [this](int frame){
			this->showPixmapIndex(frame);
		});
	}

	void addPixmap(const QPixmap &content, int index = -1){
		this->setPixmapPrivate(content, index, true);
	}

	void clearPixmap(){
		this->mapPixmapList.clear();
	}

	void showPixmapIndex(int index){
		if(index < 0 || index >= this->mapPixmapList.length()){
			throw RpgValueOutOfRangeException("mapPixmapList", index, 0, this->mapPixmapList.length());
		}
		this->currentIndex = index;
		this->setPixmap(this->mapPixmapList.at(index));
	}

	void setPass(Direction dir, bool enable){
		this->pass.setFlag(dir, enable);
	}

	bool getPass(Direction dir) const {
		return this->pass.testFlag(dir);
	}

	QFlags<Direction>::Int getPass() const{
		return int(this->pass);
	}

	void showPixmapNext(){
		if(this->mapPixmapList.length() == 1){
			return;
		}
		this->currentIndex++;
		if(this->currentIndex < 0 || this->currentIndex >= this->mapPixmapList.length()){
			this->currentIndex = 0;
		}
		this->showPixmapIndex(this->currentIndex);
	}

	int getLength() const{
		return this->mapPixmapList.length();
	}

	void setDuringTime(int ms){
		if(ms <= 0){
			ms = 1000;
		}
		this->timeLine->setDuration(ms);
	}

	void setLevel(qreal level){
		this->level = level;
	}

	qreal getLevel() const{
		return this->level;
	}


};

#endif // RPGMAPBLOCK_H
