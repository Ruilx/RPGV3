#ifndef RPGSCENE_H
#define RPGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <Rpg/Global.h>

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
