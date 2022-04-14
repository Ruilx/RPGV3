#include "RpgSprite.h"

void RpgSprite::timerEvent(QTimerEvent *event){
	if(event->timerId() == this->timerId){
		if(this->motionLocker.tryLock(10)){
			rError() << "Event try lock failed. give up 1 frame.";
			return;
		}
		if(!this->updateSpriteMotion()){
			this->motionLocker.unlock();
			// TODO: throw...
			return;
		}
		this->motionLocker.unlock();
	}
}

bool RpgSprite::updateSpriteMotion(){
	MotionPixmapList *motionSeq = this->motions.value(this->motion);
	if(motionSeq == nullptr){
		rError() << "Motion:" << this->motion << "cannot be find in motions.";
		return false;
	}
	if(this->motionIndex >= motionSeq->length()){
		this->motionIndex = 0;
	}
	this->spritePixmap->setPixmap(motionSeq->at(this->motionIndex));
	this->motionIndex++;
	return true;
}

void RpgSprite::setMotion(const QString &motion){
	this->motionLocker.lock();
	this->motion = motion;
	this->motionIndex = 0;
	this->motionLocker.unlock();
}

void RpgSprite::addMotionPixmap(const QString &motion, const QPixmap &frame){
	if(this->motions.contains(motion)){
		MotionPixmapList* motionSeq = this->motions.value(motion);
		if(motionSeq == nullptr){
			rError() << "Motion:" << motion << "has a nullptr sequence container. will create one.";
			motionSeq = new MotionPixmapList();
			this->motions.insert(motion, motionSeq); // Replace it
		}
		motionSeq->append(frame);
	}else{
		// 未定义
		MotionPixmapList* motionSeq = new MotionPixmapList();
		this->motions.insert(motion, motionSeq);
		motionSeq->append(frame);
	}
}

const RpgSprite::MotionPixmapList *RpgSprite::getMotionPixmaps(const QString &motion) const{
	if(this->motions.contains(motion)){
		return this->motions.value(motion);
	}
	return nullptr;
}

int RpgSprite::getMotionPixmapCount(const QString &motion) const{
	if(this->motions.contains(motion)){
		MotionPixmapList* motionSeq = this->motions.value(motion);
		if(motionSeq == nullptr){
			rError() << "Motion:" << motion << "has a nullptr sequence container, count set to 0.";
			return 0;
		}
		return motionSeq->length();
	}else{
		rError() << "Motion:" << motion << "not exist.";
		return 0;
	}
}

void RpgSprite::run(){
	if(this->timerId > 0){
		rWarning() << "Timer is running when run called.";
		return;
	}
	RpgObject::run();
	this->timerId = this->startTimer(this->timerInterval);
}

void RpgSprite::end(){
	if(this->timerId <= 0){
		rWarning() << "Sprite end when timerId not enabled... timerId =" << timerId;
	}else{
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
	RpgObject::end();
}

RpgSprite::RpgSprite(const QString &name, QGraphicsObject *parent): RpgObject(parent){
	this->setName(name);
	this->motions.insert("idle", new MotionPixmapList());
}
