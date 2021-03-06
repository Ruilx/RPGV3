#ifndef RPGSTATE_H
#define RPGSTATE_H

#include <QtCore>
#include <Rpg/Global.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgUtils.h>

#include <Rpg/exception/RpgNullPointerException.h>

class RpgState : public QObject
{
	Q_OBJECT
	static RpgState *_instance;
public:

	static RpgState *instance(){
		if(_instance == nullptr){
			_instance = new RpgState(nullptr);
		}
		return _instance;
	}

	enum Mode{
		UnknowMode = -1,
		AutoMode = 0,
		NormalMode,
		SystemMenuMode,
		DialogMode,
		ItemMode,
	};

private:
//	QHash<Mode, QString> modeStrHash = {
//		{UnknowMode, "Unknown mode"},
//		{AutoMode, "Auto mode"},
//		{NormalMode, "Normal mode"},
//		{SystemMenuMode, "System menu mode"},
//		{DialogMode, "Dialog mode"},
//		{ItemMode, "Item mode"},
//	};

	QStack<Mode> modeStack;
	QHash<Mode, QVector<RpgObject*> > modeObjects;

public:
	explicit RpgState(QObject *parent = nullptr): QObject(parent){
		if(this->modeStack.isEmpty()){
			this->modeStack.push(AutoMode);
		}
	}

//	inline const QString getModeStr(Mode mode){
//		return this->modeStrHash.value(mode, "");
//	}

	inline Mode getTop() const{
		return this->modeStack.top();
	}

	void pushMode(Mode mode){
		this->modeStack.push(mode);
	}

	Mode popMode(){
		if(this->modeStack.isEmpty()){
			qDebug() << CodePath << "Mode Stack is empty. cannot pop.";
			return UnknowMode;
		}
		return this->modeStack.pop();
	}

	void registerRpgObject(RpgObject *obj, Mode mode){
		if(obj == nullptr){
			qDebug() << CodePath << "RpgObject is null";
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

	void unregisterRpgObject(RpgObject *obj, Mode mode){
		if(obj == nullptr){
			qDebug() << CodePath << "RpgObject is null";
			throw RpgNullPointerException("RpgState::unregisterRpgObject=>obj", obj);
		}
		if(this->modeObjects.contains(mode)){
			QVector<RpgObject*> objs = this->modeObjects.value(mode);
			int index = -1;
			if((index = objs.indexOf(obj)) == -1){
				qDebug() << CodePath << "The mode " << mode << " does not register this RpgObject: " << obj;
				return;
			}
			objs.removeAt(index);
			this->modeObjects.insert(mode, objs);
		}else{
			qDebug() << CodePath << "The mode " << mode << " does not have any object called: " << obj;
			return;
		}
	}

signals:

public slots:
	void receiveKeyPress(int key, Qt::KeyboardModifiers mod, const QGraphicsScene *scene){
//		qDebug() << CodePath << QString("[%1] Receive key Press: %2 + %3").arg(this->modeStrHash.value(this->modeStack.top(), "UNKNOWN")).arg(RpgUtils::keyModifierToString(mod)).arg(RpgUtils::keyToString((Qt::Key)key));
		if(this->modeStack.isEmpty()){
			qDebug() << CodePath << "Mode stack is empty, cannot getting Mode.";
			return;
		}
		if(scene == nullptr){
			qDebug() << CodePath << "Must specified a valid scene to pass the key.";
			throw RpgNullPointerException("RpgState::receiveKeyPress=>scene", scene);
		}
		Mode topMode = this->modeStack.top();
		QVector<RpgObject*> objects = this->modeObjects.value(topMode);
		for(RpgObject *obj: objects){
			if(obj == nullptr){
				qDebug() << "RpgObject is null, cannot pass key to the object";
				continue;
			}
			if(obj->scene() == scene){
				if(obj->getProcessing()){
					obj->receiveKeyPress(key, mod);
				}
			}
		}
	}

	void receiveKeyRelease(int key, Qt::KeyboardModifiers mod, const QGraphicsScene *scene){
//		qDebug() << CodePath << QString("[%1]Receive key Release: %2 + %3").arg(this->modeStrHash.value(this->modeStack.top(), "UNKNOWN")).arg(RpgUtils::keyModifierToString(mod)).arg(RpgUtils::keyToString((Qt::Key)key));
		if(this->modeStack.isEmpty()){
			qDebug() << CodePath << "Mode stack is empty, cannot getting Mode.";
			return;
		}
		if(scene == nullptr){
			qDebug() << CodePath << "Must specified a valid scene to pass the key.";
			throw RpgNullPointerException("RpgState::receiveKeyRelease=>scene", scene);
		}
		Mode topMode = this->modeStack.top();
		QVector<RpgObject*> objects = this->modeObjects.value(topMode);
		for(RpgObject *obj: objects){
			if(obj == nullptr){
				qDebug() << "RpgObject is null, cannot pass key to the object";
				continue;
			}
			if(obj->scene() == scene){
				if(obj->getProcessing()){
					obj->receiveKeyRelease(key, mod);
				}
			}
		}
	}
};

#endif // RPGSTATE_H
