#include "RpgBanner.h"

#include <Rpg/exception/RpgNullPointerException.h>

RpgBanner::RpgBanner(){
	this->items.clear();

	this->enterAnimation->setTargetObject(this);
	this->exitAnimation->setTargetObject(this);
}

void RpgBanner::removeItem(const QString &name){
	if(!this->items.contains(name)){
		rDebug() << "Given name: '" << name << "' is not in items.";
		return;
	}
	QGraphicsItem *item = this->items.value(name);
	if(item == nullptr){
		rDebug() << "item name: '" << name << "' in items is nullptr already.";
		this->items.remove(name);
	}else if(item->scene() == nullptr){
		// 这个item没有绑定到任何一个scene
		delete item;
		rDebug() << "item: '" << item << "' deleted.";
		this->items.remove(name);
	}else{
		this->items.remove(name);
	}
}

void RpgBanner::addItem(const QString &name, QGraphicsItem *item){
	if(item == nullptr){
		rDebug() << "Giving item obj is nullptr.";
		throw RpgNullPointerException("item", item);
	}
	if(this->items.contains(name)){
		QGraphicsItem *itemInHash = this->items.value(name);
		if(itemInHash == item){
			rDebug() << "Item already in items: '" << name << "' with value: '" << itemInHash << "'.";
			return;
		}else{
			rDebug() << "Items will update '" << name << "' new item '" << item << "' to replace old item: '" << itemInHash << "'.";
			this->removeItem(name);
			item->setParentItem(this);
			this->items.insert(name, item);
		}
	}else{
		this->setParentItem(this);
		this->items.insert(name, item);
	}
}
