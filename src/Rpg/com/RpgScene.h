#ifndef RPGSCENE_H
#define RPGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <Rpg/Global.h>

/**
 * @brief The RpgScene class
 * RpgScene预示着游戏中的每一个地图, 包括标题和各种过场
 * 每一个场景对应一个场景脚本, 用来定义场景的地图, 布置, 和触发选项
 * 启动脚本中必须指定一个入口场景, 载入场景后, 会执行相应的地图(如果有的话), 以及启动脚本
 *
 * 如果启动加载需要一定时间, 可以载入预置的loading scene, 执行后再转换回地图
 */
class RpgScene : public QGraphicsScene
{
	Q_OBJECT

	QString mapFile;
public:
	RpgScene(QObject *parent = nullptr): QGraphicsScene(parent){
		this->setSceneRect(0, 0, ScreenWidth, ScreenHeight);
	}
};

#endif // RPGSCENE_H
