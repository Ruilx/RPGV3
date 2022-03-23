#ifndef RPGMUSIC_H
#define RPGMUSIC_H

#include <Rpg/Global.h>
#include <QObject>
#include <QPropertyAnimation>
#include <QEventLoop>
#include <Rpg/core/RpgFileManager.h>
#include <QtAV/QtAV>

#include <Rpg/exception/RpgValueOutOfRangeException.h>
#include <Rpg/exception/RpgMusicInvalidMediaException.h>
#include <Rpg/exception/RpgResourceNotFoundException.h>

/**
 * @brief The RpgMusic class
 * RpgMusic类, 是进行RPG游戏的时候输出游戏声音的组件(BGM)
 * 是一个持续的声音, 所以只能被复用一次, 所有场景都可以使用这一个声音, 所以作为/单例模式/出现
 * 进行另一个声音的播放时, 前一个声音将会停止, 默认声音会一直循环播放
 *
 * 首先需要载入声音文件, 并给其取一个名字, 播放的时候只需要提供名字即可.
 *
 * 2019/01/31
 * 最新版RpgMusic使用的是QtAV插件进行实现, QtAV插件克服了原生不能进行解码将数据移交至DirectShow进行处理
 * 因为DirectShow的Decoder解码功能太弱, 导致多数音频文件格式不支持, 产生错误代码0x80040266(MS: VFW_E_NO_TRANSPORT)
 * QtAV使用FFmpeg进行解码并使用OpenAL播放音频
 * 使用的是QtAV版本 1.12.0, 我改过源码, 主要删除了一些冗余的调试文本
 *
 * 另外将循环给QtAV托管了
 *
 * @see https://www.qtav.org
 * @see https://github.com/wang-bin/QtAV
 *
 * 2022/03/24
 * 使用编译的1.13版本的QtAV组件, 升级至64位, 使用Qt 5.12.11 MinGW x86_64
 */
class RpgMusic : public QObject
{
	Q_OBJECT
	static RpgMusic *_instance;

	QtAV::AVPlayer *music = nullptr;
	qreal volume = 1.0f;

	QPropertyAnimation *volumeAnimation = nullptr;

	void volumeTransition(bool upward, int duration = 500){
		if(upward){
			this->volumeAnimation->setStartValue(0);
			this->volumeAnimation->setEndValue(this->volume);
		}else{
			this->volumeAnimation->setStartValue(this->volume);
			this->volumeAnimation->setEndValue(0);
		}
		this->volumeAnimation->setDuration(duration);
		this->volumeAnimation->start();
		QEventLoop loop;
		this->connect(this->volumeAnimation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
		loop.exec();
	}
public:
	static RpgMusic *instance(){
		if(_instance == nullptr){
			_instance = new RpgMusic(nullptr);
		}
		return _instance;
	}

	explicit RpgMusic(QObject *parent = nullptr): QObject(parent){
		QtAV::setLogLevel(QtAV::LogWarning);
		QtAV::setFFmpegLogLevel("warn");

		rDebug() << "====================";
		rDebug() << QtAV::aboutQtAV_PlainText();
		rDebug() << "====================";

		if(this->music == nullptr){
			this->music = new QtAV::AVPlayer(this);
		}

		if(this->volumeAnimation == nullptr){
			this->volumeAnimation = new QPropertyAnimation(this->music->audio(), "volume", this);
		}

		this->music->audio()->setVolume(this->volume);

		connect(this->music, &QtAV::AVPlayer::stateChanged, [this](QtAV::AVPlayer::State state){
			if(state == QtAV::AVPlayer::PlayingState){
				if(this->music->currentRepeat() == 0){
					qDebug() << CodePath << "Now playing:" << this->music->file() << "with volume:" << this->music->audio()->volume();
					emit this->started();
				}else{
					qDebug() << CodePath << "Continue playing:" << this->music->file();
				}
			}else if(state == QtAV::AVPlayer::StoppedState){
				emit this->stopped();
			}
		});

		connect(this->music, &QtAV::AVPlayer::mediaStatusChanged, [this](QtAV::MediaStatus status){
			if(status == QtAV::EndOfMedia){

			}else if(status == QtAV::InvalidMedia){
				qDebug() << CodePath << "INVALID MEDIA";
				this->music->stop();
				emit this->invaildMediaSignal();
			}
		});

		this->music->setNotifyInterval(50);
		connect(this->music, &QtAV::AVPlayer::positionChanged, [this](qint64 position){
			emit this->seeked(position);
		});

		connect(this->music, &QtAV::AVPlayer::error, [this](QtAV::AVError error){
			qDebug() << "Error:" << error.string();
		});

		this->music->setRepeat(-1);
	}

	~RpgMusic(){
		if(this->music->isPlaying()){
			this->stopMusic();
//			this->music->audio()->close();
//			this->music->deleteLater();
			// BUG: 这里回收之后QtAV会报"Can not copy codec properties when it's open", 只是qWarning, 之后代码会执行close()
		}
	}

	bool isRunning() const{
		return this->music->state() == QtAV::AVPlayer::PlayingState;
	}

	QString getMusic(const QString &musicName){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::MusicFile, musicName);
		if(filename.isEmpty()){
			throw RpgResourceNotFoundException(musicName);
		}
		return filename;
	}

	inline void setVolume(int value){
		if(value <= 100 && value >= 0){
			this->music->audio()->setVolume(value / 100);
			this->volume = value;
		}else{
			throw RpgValueOutOfRangeException("value", value, 0, 100);
		}
	}

	inline int getVolume() const{
		return int(this->music->audio()->volume() * 100);
	}

	inline void setLoop(int loop = -1){
		//this->loop = loop == 0 ? 1 : loop;
		//这个loop和music的repeat是有区别的
		// loop的值 | repeat的值 | 歌会重复几遍
		//       -1 |         -1 | 无数次
		//        0 |         -1 | 无数次
		//        1 |          0 | 不重复
		//        2 |          1 | 重复一次
		// 等等
		switch(loop){
			case -1: case 0: this->music->setRepeat(-1); break;
			default: this->music->setRepeat(loop -1); break;
		}
		return;
	}

	inline int getLoop() const{
		int repeat = this->music->repeat();
		if(repeat < 0){
			repeat = -1;
		}
		switch(repeat){
			case -1: return -1;
			default: return repeat + 1;
		}
	}

	inline void setNotifyInterval(int ms){
		this->music->setNotifyInterval(ms);
	}

	inline int getNotifyInterval() const{
		return this->music->notifyInterval();
	}

signals:
	void started();
	void stopped();
	void seeked(qint64 position);

	void invaildMediaSignal();
public slots:

	void playMusic(const QString &musicName){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::MusicFile, musicName);
		if(filename.isEmpty()){
			throw RpgResourceNotFoundException(musicName);
		}
		if(this->music->state() != QtAV::AVPlayer::PlayingState){
			this->stopMusic();
		}
		this->music->setFile(filename);
		this->music->play();
		this->volumeTransition(true);
		//this->currentLoop++;
	}

	void stopMusic(){
		this->volumeTransition(false);
		this->music->stop();
		//this->currentLoop = 0;
	}
private slots:
	void invaildMediaSlot(){
		throw RpgMusicInvalidMediaException(this->music->file());
	}
};

#ifndef rpgMusic
#define rpgMusic (RpgMusic::instance())
#endif

#endif // RPGMUSIC_H
