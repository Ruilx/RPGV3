#include "RpgLyric.h"

void RpgLyric::lyricAnimationShow(){
	if(this->scene() == nullptr){
		rDebug() << "This object not in any scene";
		return;
	}
	if(this->lyricOpacityAnimation->state() != QPropertyAnimation::Stopped){
		this->lyricOpacityAnimation->stop();
	}
	this->lyricOpacityAnimation->setDirection(QPropertyAnimation::Forward);
	this->lyricOpacityAnimation->setCurrentTime(0);
	this->lyricOpacityAnimation->start();
}

void RpgLyric::lyricAnimationHide(){
	if(this->scene() == nullptr){
		rDebug() << "This object not in any scene";
		return;
	}
	if(this->lyricOpacityAnimation->state() != QPropertyAnimation::Stopped){
		this->lyricOpacityAnimation->stop();
	}
	this->lyricOpacityAnimation->setDirection(QPropertyAnimation::Backward);
	this->lyricOpacityAnimation->setCurrentTime(this->lyricOpacityAnimation->totalDuration());
	this->lyricOpacityAnimation->start();
	RpgUtils::msleep(100);
}

RpgLyric::RpgLyric(QGraphicsObject *parentObject): RpgObject(parentObject){
	this->lyric->setDefaultTextColor(QColor(Qt::white));
	this->lyric->document()->setDefaultStyleSheet(Rpg::getDefaultCss());

	this->lyric->setFont(RpgFont::instance()->getFont("lyric", 12));
	this->lyric->setPos(this->defaultPos);

	this->setZValue(LyricZValue);

	this->lyricShadowEffect->setColor(QColor("#202020"));
	this->lyricShadowEffect->setBlurRadius(5.0f);
	this->lyricShadowEffect->setOffset(1.0, 1.0);
	this->lyric->setGraphicsEffect(this->lyricShadowEffect);

	this->lyricOpacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
	this->lyricOpacityAnimation->setStartValue(0.0);
	this->lyricOpacityAnimation->setEndValue(1.0);
	this->lyricOpacityAnimation->setDuration(200);
	this->lyricOpacityAnimation->setLoopCount(1);

	this->lyric->setOpacity(0);
}

void RpgLyric::setRpgMusic(RpgMusic *music){
	if(this->musicObj != nullptr){
		this->musicObj->disconnect();
	}
	if(music != nullptr){
		this->musicObj = music;
		// TODO: 这里应该connect吧 (showLyric搬过来?)
	}else{
		rDebug() << "Given RpgMusic point is not a pointer. using default RpgMusic instead.";
		this->musicObj = RpgMusic::instance();
	}
}

void RpgLyric::loadLyricFromFile(const QString &filename){
	if(this->getRunning()){
		rDebug() << "Cannot load the lyric while running!";
		return;
	}
	if(filename.isEmpty() || !QFile::exists(filename)){
		rDebug() << QString("filename \"%1\" is not exist.").arg(filename);
		// Todo: throw a exception file not exist.
		return;
	}
	QFile f(filename);
	if(!f.open(QIODevice::ReadOnly)){
		rDebug() << QString("filename \"%1\" cannot open: %2").arg(filename, f.errorString());
		return;
	}
	this->lyricMap.clear();
	QTextStream ts(&f);
	ts.setCodec("utf-8");
	while(!ts.atEnd()){
		QString line = ts.readLine().trimmed();
		if(line.isEmpty()){
			// Empty line
			continue;
		}
		if(!line.startsWith("[")){
			rDebug() << QString("Invalid line data: not start with '[': \"%1\". Ignored.").arg(line);
			continue;
		}
		int rightBraPos = line.indexOf("]");
		if(rightBraPos == -1){
			rDebug() << QString("Invalid line data: Syntax error: cannot find timestamp tip: \"%1\". Ignored.").arg(line);
			continue;
		}
		QString timestamp = line.mid(1, rightBraPos -1);
		QString lyricText;
		if(line.length() >= (rightBraPos +1)){
			lyricText = line.mid(rightBraPos +1);
		}

		bool ok = false;
		int intTimestamp = timestamp.toInt(&ok);
		if(!ok){
			double doubleTimestamp = timestamp.toDouble(&ok);
			if(!ok){
				QTime timeTimestamp = QTime::fromString(timestamp, "hh:mm:ss.zzz");
				if(timeTimestamp.isNull() || !timeTimestamp.isValid()){
					timeTimestamp = QTime::fromString(timestamp, "mm:ss.zzz");
					if(timeTimestamp.isNull() || !timeTimestamp.isValid()){
						rDebug() << QString("Invalid line data: timestamp cannot resolve to [int], [double] [hh:mm:ss.zzz] or [mm:ss:zzz]: \"%1\". Ignored.").arg(line);
						continue;
					}else{
						this->lyricMap.insert(timeTimestamp.msecsSinceStartOfDay(), lyricText);
						continue;
					}
				}else{
					this->lyricMap.insert(timeTimestamp.msecsSinceStartOfDay(), lyricText);
					continue;
				}
			}else{
				this->lyricMap.insert(int(doubleTimestamp * 1000), lyricText);
				continue;
			}
		}else{
			this->lyricMap.insert(intTimestamp, lyricText);
			continue;
		}
	}
	this->lyricI = this->lyricMap.constBegin();
	if(f.isOpen()){
		f.close();
	}
}

void RpgLyric::loadLyric(const QString &name){
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::LyricFile, name);
	if(filename.isEmpty()){
		rDebug() << QString("Cannot open resource: \"%1\"").arg(name);
		// Todo: throw Resource not found exception;
		return;
	}
	this->loadLyricFromFile(filename);
}
