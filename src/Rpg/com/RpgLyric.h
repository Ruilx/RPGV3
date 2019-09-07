#ifndef RPGLYRIC_H
#define RPGLYRIC_H

#include <QGraphicsObject>
#include <Rpg/core/RpgObject.h>


class RpgLyric : public RpgObject{
public:
	explicit RpgLyric(QGraphicsObject *parentObject = nullptr): RpgObject(parentObject){

	}
};

#endif // RPGLYRIC_H
