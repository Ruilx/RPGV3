#include "RpgSound.h"
#include <QtConcurrent>

#include <Rpg/exception/RpgNullPointerException.h>

RpgSound *RpgSound::_instance = nullptr;

void RpgSound::play(const QString &soundName, qreal volume, int times){
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::SoundEffectFile, soundName);
	QUrl url = QUrl::fromLocalFile(filename);
	if(this->threadPool == nullptr){
		throw RpgNullPointerException("threadPool", this->threadPool);
	}
	QtConcurrent::run(this->threadPool, [this, volume, times, &soundName, url](){
		if(this->threadPool->activeThreadCount() >= this->threadPool->maxThreadCount()){
			qWarning() << CodePath << QString("Threadpool is full. %1/%2").arg(this->threadPool->activeThreadCount()).arg(this->threadPool->maxThreadCount());
			return;
		}
		QSoundEffect *sound = new QSoundEffect(nullptr);
		sound->setLoopCount(times);
		sound->setVolume(volume);
		sound->setSource(url);
		if(sound->status() == QSoundEffect::Error){
			qWarning() << CodePath << "Sound status error.";
			sound->deleteLater();
			return;
		}
		sound->play();
		emit this->started(soundName);
		qDebug() << CodePath << QString("Playing Sound Effect: '%1'").arg(sound->source().url(QUrl::PreferLocalFile));
		QEventLoop loop;
		this->connect(sound, &QSoundEffect::playingChanged, [this, &loop, &sound](){
			if(sound->isPlaying() == false){
				loop.quit();
			}
		});
		loop.exec();
		qDebug() << CodePath << QString("Stop Sound Effect: '%1'").arg(sound->source().url(QUrl::PreferLocalFile));
		sound->deleteLater();
		return;
	});
}

void RpgSound::stop(const QString &soundName){
	if(!this->threadPool->waitForDone(500)){
		this->threadPool->clear();
	}
	Q_UNUSED(soundName);
	qDebug() << CodePath << "Sound changed cannot be stopped manmully.";
}
