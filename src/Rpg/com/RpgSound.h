#ifndef RPGSOUND_H
#define RPGSOUND_H

#include <QtCore>

#include <QSoundEffect>
#include <QMutex>

/**
 * @brief The RpgSound class
 * RpgSound类, 是进行RPG游戏输出的低延时音效组件
 */
class RpgSound : public QObject
{
	Q_OBJECT
	static RpgSound *_instance;
	int maxPlayingCount = 10;

	QHash<qint64, QSoundEffect *> playingSounds;
public:
	static RpgSound *instance(){
		if(_instance == nullptr){
			_instance = new RpgSound(nullptr);
		}
		return _instance;
	}

	explicit RpgSound(QObject *parent = nullptr): QObject(parent){
		this->playingSounds.clear();
	}

	~RpgSound(){
		for(auto i: this->playingSounds){
			if(i != nullptr){
				i->stop();
			}
		}
	}

signals:
	void started(const QString &soundName);
	void stopped(const QString &soundName);
public slots:
	qint64 play(const QString &soundName, qreal volume = 1.0, int times = 1);
	void stop(qint64 index);
};

#ifndef rpgSound
#	define rpgSound (RpgSound::instance())
#endif

#endif // RPGSOUND_H
