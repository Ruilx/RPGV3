#include "RpgState.h"

RpgState *RpgState::_instance = nullptr;

RpgState *RpgState::instance(){
	if(_instance == nullptr){
		_instance = new RpgState(nullptr);
	}
	return _instance;
}

RpgState::RpgState(QObject *parent): QObject(parent){
	if(this->modeStack.isEmpty()){
		this->modeStack.push(AutoMode);
	}
}

void RpgState::pushState(RpgState::Mode mode){
	if(this->modeStack.length() > MaxStates){
		rWarning() << "Mode Stack is full, it won't be pushed.";
		return;
	}
	this->modeStack.push(mode);
}

RpgState::Mode RpgState::popState(){
	if(this->modeStack.isEmpty()){
		rWarning() << "Mode Stack is empty. cannot pop.";
		return UnknowMode;
	}
	return this->modeStack.pop();
}

void RpgState::registerRpgObject(RpgObject *obj, RpgState::Mode mode){
	if(obj == nullptr){
		rDebug() << "RpgObject is null";
		throw RpgNullPointerException("RpgState::registerRpgObject=>obj", obj);
	}
	if(this->modeObjects.contains(mode)){
		QVector<RpgObject*> objs = this->modeObjects.value(mode);
		objs.append(obj);
		this->modeObjects.insert(mode, objs);
	}else{
		QVector<RpgObject*> objs;
		objs.append(obj);
		this->modeObjects.insert(mode, objs);
	}
}

void RpgState::unregisterRpgObject(RpgObject *obj, RpgState::Mode mode){
	if(obj == nullptr){
		rDebug() << "RpgObject is null";
		throw RpgNullPointerException("RpgState::unregisterRpgObject=>obj", obj);
	}
	if(this->modeObjects.contains(mode)){
		QVector<RpgObject*> objs = this->modeObjects.value(mode);
		int index = -1;
		if((index = objs.indexOf(obj)) == -1){
			rDebug() << "The mode " << mode << " does not register this RpgObject: " << obj;
			return;
		}
		objs.removeAt(index);
		this->modeObjects.insert(mode, objs);
	}else{
		rDebug() << "The mode " << mode << " does not have any object called: " << obj;
		return;
	}
}

void RpgState::keyPressEvent(QKeyEvent *event, const QGraphicsScene *scene){
	if(event == nullptr){
		rDebug() << "Receive key event is nullptr.";
		return;
	}
	int key = event->key();
	Qt::KeyboardModifiers mod = event->modifiers();
	if(this->modeStack.isEmpty()){
		rDebug() << "Mode stack is empty, cannot getting Mode.";
		throw RpgNullPointerException("RpgState::keyPressEvent => event", event);
	}
	rDebug() << "[" << RpgState::stateModeString.value(this->modeStack.top()) << "] Received key PRESS [▼]:" << RpgUtils::keysToString((Qt::Key)key, mod);
	if(scene == nullptr){
		rDebug() << "Must specified a valid scene to pass the key.";
		throw RpgNullPointerException("RpgState::keyPressEvent => scene", scene);
	}
	Mode topMode = this->modeStack.top();
	QVector<RpgObject*> objects = this->modeObjects.value(topMode);
	for(RpgObject *obj: objects){
		if(obj == nullptr){
			rDebug() << "RpgObject is null, cannot pass key to the object";
			continue;
		}
		if(obj->scene() == scene){
			if(obj->getRunning()){
				/* The event must be allocated on the heap since the post event queue will take ownership of the event
				 * and delete it once it has been posted.
				 */
				//QKeyEvent *keyPressEvent = new QKeyEvent(QEvent::KeyPress, key, mod, RpgUtils::keysToString((Qt::Key)key, mod), false, 1);
				// @date 2022/05/04: 自己搭的QKeyEvent在系统的控件(如QLineEdit)的输入的位置可能会发生转义(比如按Ctrl, 会插入"Ctrl"四个字母), 更改为直接复制QKeyEvent构造
				QKeyEvent *keyPressEvent = new QKeyEvent(*event);
				rDebug() << "Send Event to obj:" << obj;
				qApp->postEvent(obj, keyPressEvent);
			}
		}
	}
}

void RpgState::keyReleaseEvent(QKeyEvent *event, const QGraphicsScene *scene){
	if(event == nullptr){
		rDebug() << "Receive key event is nullptr.";
		throw RpgNullPointerException("RpgState::keyReleaseEvent => event", event);
	}
	int key = event->key();
	Qt::KeyboardModifiers mod = event->modifiers();
	if(this->modeStack.isEmpty()){
		rDebug() << "Mode stack is empty, cannot getting Mode.";
		return;
	}
	rDebug() << "[" << RpgState::stateModeString.value(this->modeStack.top()) << "] Received key RELEASE [▲]:" << RpgUtils::keysToString((Qt::Key)key, mod);
	if(scene == nullptr){
		rDebug() << "Must specified a valid scene to pass the key.";
		throw RpgNullPointerException("RpgState::keyReleaseEvent => scene", scene);
	}
	Mode topMode = this->modeStack.top();
	QVector<RpgObject*> objects = this->modeObjects.value(topMode);
	for(RpgObject *obj: objects){
		if(obj == nullptr){
			rDebug() << "RpgObject is null, cannot pass key to the object";
			continue;
		}
		if(obj->scene() == scene){
			if(obj->getRunning()){
				/* The event must be allocated on the heap since the post event queue will take ownership of the event
				 * and delete it once it has been posted.
				 */
				//QKeyEvent *keyReleaseEvent = new QKeyEvent(QEvent::KeyRelease, key, mod, RpgUtils::keysToString((Qt::Key)key, mod), false, 1);
				// @date 2022/05/04: 自己搭的QKeyEvent在系统的控件(如QLineEdit)的输入的位置可能会发生转义(比如按Ctrl, 会插入"Ctrl"四个字母), 更改为直接复制QKeyEvent构造
				QKeyEvent *keyReleaseEvent = new QKeyEvent(*event);
				rDebug() << "Send Event to obj:" << obj;
				qApp->postEvent(obj, keyReleaseEvent);
			}
		}
	}
}
