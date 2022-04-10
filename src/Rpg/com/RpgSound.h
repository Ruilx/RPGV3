#ifndef RPGSOUND_H
#define RPGSOUND_H

#include <QtCore>

#include <QSoundEffect>
#include <QMutex>

/**
 * @brief The RpgSound class
 * RpgSound类, 是进行RPG游戏输出的低延时音效组件
 *
 * 2022/04/06 RpgSound调整播放机制, 去除Concurrent中间态执行
 * 2022/04/08 RpgSound将正在运行中的音效登记至playingSounds, 实现了可停止一个确定的实例
 * 2022/04/09 RpgSound增加Cache, 至少保证1条并行声音保存读取后的信息
 *
 * @note QSoundEffect只能打开WAV格式音频, 保证音效时长要短
 * @note QSoundEffect不允许复制构造函数, 所以每次setSource时可能需要重读, 为保证时效,
 * 采用了cache
 * @note 运行完毕的SoundEffect如果未登记在cache中则会登记入cache, 如果cache已存在, 则
 * 会在stop时删除掉自己
 */
class RpgSound : public QObject
{
	Q_OBJECT
	static RpgSound *_instance;
	int maxPlayingCount = 10;

	QHash<QString, QSoundEffect *> cachedSounds;
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

	~RpgSound();

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
