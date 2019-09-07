#ifndef RPGSOUND_H
#define RPGSOUND_H

#include <QObject>
#include <Rpg/Global.h>
#include <Rpg/core/RpgFileManager.h>

#include <QSoundEffect>

/**
 * @brief The RpgSound class
 * RpgSound类, 是进行RPG游戏输出的低延时音效组件
 */
class RpgSound : public QObject
{
	Q_OBJECT
	static RpgSound *_instance;

	QThreadPool *threadPool = nullptr;
public:
	static RpgSound *instance(){
		if(_instance == nullptr){
			_instance = new RpgSound(nullptr);
		}
		return _instance;
	}

	explicit RpgSound(QObject *parent = nullptr): QObject(parent){
		if(this->threadPool == nullptr){
			this->threadPool = new QThreadPool(this);
			this->threadPool->setMaxThreadCount(10);
		}
	}

	~RpgSound(){
		if(this->threadPool != nullptr){
			this->threadPool->waitForDone();
			this->threadPool->clear();
		}
	}

signals:
	void started(const QString &soundName);
	void stopped(const QString &soundName);
public slots:
	void play(const QString &soundName, qreal volume = 0.7f, int times = 1);
	void stop(const QString &soundName);
};

#endif // RPGSOUND_H
