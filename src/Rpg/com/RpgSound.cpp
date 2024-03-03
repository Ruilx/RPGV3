#include "RpgSound.h"

#include <Rpg/Global.h>
#include <Rpg/core/RpgFileManager.h>

#include <Rpg/exception/RpgNullPointerException.h>
#include <Rpg/exception/RpgRuntimeException.h>

RpgSound *RpgSound::_instance = nullptr;

//void RpgSound::play(const QString &soundName, qreal volume, int times){
//	//QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::SoundEffectFile, soundName);
//	QUrl soundUrl = rpgFileManager->getFile(RpgFileManager::SoundEffectFile, soundName);
//	if(!soundUrl.isValid() || soundUrl.isEmpty()){
//		throw RpgRuntimeException("sound name: " % soundName % " cannot find a valid url.");
//	}
//	//QUrl url = QUrl::fromLocalFile(filename);
//	if(this->threadPool == nullptr){
//		throw RpgNullPointerException("threadPool", this->threadPool);
//	}
//	QtConcurrent::run(this->threadPool, [this, volume, times, &soundName, soundUrl](){
//		if(this->threadPool->activeThreadCount() >= this->threadPool->maxThreadCount()){
//			qWarning() << CodePath << QString("Threadpool is full. %1/%2").arg(this->threadPool->activeThreadCount()).arg(this->threadPool->maxThreadCount());
//			return;
//		}
//		QSoundEffect *sound = nullptr;
//		if(this->sounds.contains(soundName)){
//			sound = this->sounds.value(soundName);
//		}
//		if(sound == nullptr){
//			if(this->soundRegisterLock.tryLock(1000)){
//				sound = new QSoundEffect(nullptr);
//				sound->setSource(soundUrl);
//				if(!this->sounds.contains(soundName)){
//					this->sounds.insert(soundName, sound); // Write operation
//				}
//				this->soundRegisterLock.unlock(); // Locked before
//			}else{
//				rError() << "Sound effect register try lock timeout, cannot play sound.";
//				return;
//			}
//		}

//		sound->setLoopCount(times);
//		sound->setVolume(volume);

//		if(sound->status() == QSoundEffect::Error){
//			rWarning() << "Sound status error.";
//			if(!this->sounds.contains(soundName)){
//				// 可能总有忘了注册的时候, 如果真忘了, 这个判断会回收一次内存。
//				sound->deleteLater();
//			}
//			return;
//		}
//		sound->play();
//		emit this->started(soundName);
//		rDebug() << QString("Playing Sound Effect: '%1'").arg(sound->source().url(QUrl::PreferLocalFile));
//		// 有了sound在hash(this->sounds)中存储后, 这里就不用loop卡住了
////		QEventLoop loop;
////		this->connect(sound, &QSoundEffect::playingChanged, [this, &loop, &sound](){
////			if(sound->isPlaying() == false){
////				loop.quit();
////			}
////		});
////		loop.exec();
//		//qDebug() << CodePath << QString("Stop Sound Effect: '%1'").arg(sound->source().url(QUrl::PreferLocalFile));
//		if(!this->sounds.contains(soundName)){
//			// 可能总有忘了注册的时候, 如果真忘了, 这个判断会回收一次内存。
//			// 但这里可能就会出现声音刚一播放就回收了, 但这里一般不会达到, 因为前面写入了hash(this->sounds),所以不会因为没有contains而无法播放.
//			sound->deleteLater();
//		}
//		return;
//	});
//}

//void RpgSound::stop(const QString &soundName){
//	if(!this->threadPool->waitForDone(500)){
//		this->threadPool->clear();
//	}
//	Q_UNUSED(soundName);
//	qDebug() << CodePath << "Sound changed cannot be stopped manmully.";
//}

RpgSound::~RpgSound(){
	for(auto i: this->playingSounds){
		if(i != nullptr){
			i->stop();
			i->deleteLater();
		}
	}
	for(auto i: this->cachedSounds){
		if(i != nullptr){
			i->deleteLater();
		}
	}
}

#if 0
qint64 RpgSound::play(const QString &soundName, qreal volume, int times){
	static qint64 autoIncrementIndex = 0;

	if(this->playingSounds.count() >= this->maxPlayingCount){
		rError() << "Sound effect playing count too many: max slot:" << this->maxPlayingCount;
		return -1;
	}
	QSoundEffect *sound = nullptr;
	if(this->cachedSounds.contains(soundName)){
		QSoundEffect *cached = this->cachedSounds.value(soundName);
		if(cached == nullptr){
			rDebug() << "cached soundEffect object is nullptr, will remove from cache.";
			this->cachedSounds.remove(soundName);
		}else if(!cached->isPlaying()){
			sound = cached;
		}
	}
	if(sound == nullptr){
		sound = new QSoundEffect(this);
		//rDebug() << "Sound:" << sound << "CREATED.";

		QUrl url = rpgFileManager->getFile(RpgFileManager::SoundEffectFile, soundName);
		if(!url.isValid() || url.isEmpty()){
			throw RpgRuntimeException("sound name: " % soundName % " cannot get a valid url.");
		}

		sound->setSource(url);
		sound->setProperty("name", soundName);
		this->connect(sound, &QSoundEffect::playingChanged, [this, sound, soundName](){
			if(!sound->isPlaying()){
				bool ok = false;
				quint64 index = sound->property("index").toLongLong(&ok);
				if(!ok){
					rWarning() << "get a sound has no 'index' property and cannot figure out which playing sound it is.";
					return;
				}
				if(this->playingSounds.contains(index)){
					QSoundEffect *soundInHash = this->playingSounds.take(index); // & removed
					if(soundInHash != sound){
						rDebug() << "Not same! Index:" << index << "Hash:" << soundInHash << "Sound:" << sound;
					}
					if(this->cachedSounds.contains(soundName)){
						QSoundEffect *soundInCache = this->cachedSounds.value(soundName);
						if(soundInCache != sound){
							sound->deleteLater();
						}
					}else{
						sound->deleteLater();
					}
				}
			}
			//rDebug() << "Cached:" << this->cachedSounds;
		});

//		this->connect(sound, &QSoundEffect::destroyed, [](QObject *obj){
//			rDebug() << "Sound:" << obj << "DESTROYED.";
//		});
	}
	sound->setLoopCount(times);
	sound->setVolume(volume);

	if(sound->status() == QSoundEffect::Error){
		rInlineWarning() << "Sound '" << soundName << "' aka file: '" << url << "' status error.";
		return -1;
	}
//	if(!this->sounds.contains(soundName)){
//		this->sounds.insert(soundName, sound); // Write operation
//	}
	this->playingSounds.insert(autoIncrementIndex, sound);
	sound->setProperty("index", autoIncrementIndex);

	if(!this->cachedSounds.contains(soundName)){
		this->cachedSounds.insert(soundName, sound);
	}

	sound->play();
	emit this->started(soundName);
	rInlineDebug() << "Playing sound effect: '" << soundName << "' with entity: '" << autoIncrementIndex << "' aka file: '" << sound->source().url(QUrl::PreferLocalFile) << "'";

	return autoIncrementIndex++;
}
#endif

qint64 RpgSound::play(const QString &soundName, qreal volume, int times){
	static qint64 autoIncrementIndex = 0;
	QSoundEffect *sound = nullptr;
	if(this->cachedSounds.contains(soundName)){
		sound = this->cachedSounds.value(soundName);
		if(sound == nullptr){
			rWarning() << "Sound: '" << soundName << "' cached but have been deleted. Reload from file manager.";
		}
	}
	if(sound == nullptr){
		QUrl url = rpgFileManager->getFile(RpgFileManager::SoundEffectFile, soundName);
		if(!url.isValid() || url.isEmpty()){
			throw RpgRuntimeException("sound name: " % soundName % " cannot get a valid url.");
		}
		sound = new QSoundEffect(this);
		sound->setSource(url);
		sound->setProperty("name", soundName);
	}

	if(sound->isPlaying()){
		sound->stop();
	}
	sound->setVolume(volume);
	sound->setLoopCount(times);

	switch(sound->status()){
		case QSoundEffect::Null:
			rError() << "Sound '" << soundName << "' has no source.";
			break;
		case QSoundEffect::Error:
			rError() << "Sound '" << soundName << "' status Error.";
			break;
		case QSoundEffect::Loading:
			rInfo() << "Sound '" << soundName << "' is loading...";
		case QSoundEffect::Ready:
			sound->play();
			break;
	}
	return ++autoIncrementIndex;
}

#if 0
void RpgSound::stop(qint64 index){
	if(this->playingSounds.contains(index)){
		QSoundEffect *sound = this->playingSounds.value(index);
		if(sound == nullptr){
			rError() << "Sound index: '" << index << "' not valid (nullptr), will delete from this->playingSounds.";
			this->playingSounds.remove(index);
			return; // sound index with nullptr, returned.
		}
		if(sound->isPlaying()){
			sound->stop();
			rInlineInfo() << "Sound index: '" << index << "' name: '" << sound->property("name") << "' aka '" << sound->source() << "' stopped.";
		}
	}else{
		rError() << "Sound index: '" << index << "' not found in playing sounds.";
		return;
	}
}
#endif

void RpgSound::stop(const QString &soundName){
	if(!this->cachedSounds.contains(soundName)){
		rError() << "Sound '" << soundName << "' not found in cached sounds";
		return;
	}
	QSoundEffect *sound = this->cachedSounds.value(soundName);
	if(sound == nullptr){
		rError() << "Sound '" << soundName << "' is nullptr in cache sounds.";
		this->cachedSounds.remove(soundName);
		return;
	}
	if(sound->isPlaying()){
		sound->stop();
		rInfo() << "Sound '" << soundName << "' is manually stopped.";
	}else{
		rInfo() << "Sound '" << soundName << "' already stopped.";
	}
}

//void RpgSound::stop(const QString &soundName){
//	if(this->sounds.contains(soundName)){
//		QSoundEffect *sound = this->sounds.value(soundName);
//		if(sound == nullptr){
//			rError() << "Sound name: '" << soundName << "' not valid (nullptr), will delete from this->sounds.";
//			this->sounds.remove(soundName);
//			return; //sound in this->sounds is nullptr, return
//		}
//		if(sound->isPlaying()){
//			sound->stop();
//			rInfo() << "Sound name: '" << soundName << "' stopped.";
//			emit this->stopped(soundName);
//		}
//	}else{
//		rError() << "Sound name: '" << soundName << "' not found in list.";
//		return;
//	}
//}
