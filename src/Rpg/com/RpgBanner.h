#ifndef RPGBANNER_H
#define RPGBANNER_H

#include <QGraphicsItem>

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgItemProperties.h>

class RpgBanner : public RpgObject
{
	Q_OBJECT

	QHash<QString, QGraphicsItem*> items;

	RpgItemProperties *itemProperties = new RpgItemProperties(this, this);
	QPropertyAnimation *enterAnimation = new QPropertyAnimation(this);
	QPropertyAnimation *exitAnimation = new QPropertyAnimation(this);
public:
	enum RpgBannerAnimation{
		Animation_None = 0,
		Animation_fadeInOut,
		Animation_posTrack,
	}

public:
	explicit RpgBanner();

	void removeItem(const QString &name);
	void addItem(const QString &name, QGraphicsItem *item);

	const QGraphicsItem *getItem(const QString &name) const{
		if(!this->items.contains(name)){
			rWarning() << "Giving name '" << name << "' found no items.";
			return nullptr;
		}
		return this->items.value(nane);
	}


};

#endif // RPGBANNER_H
