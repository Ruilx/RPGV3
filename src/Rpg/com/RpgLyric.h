#ifndef RPGLYRIC_H
#define RPGLYRIC_H

#include <QtWidgets>
#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgFileManager.h>
#include <Rpg/com/RpgMusic.h>
#include <Rpg/core/RpgFont.h>

#include <Rpg/com/RpgView.h>

#include <QtConcurrent>
/**
 * @brief The RpgLyric class
 * RpgLyric类
 * 是歌词在屏幕上输出类, 在屏幕上运行歌词
 * 使用RpgMusic进行音乐播放, 并监控播放位置并过渡输出歌词
 * 继承RpgObject
 *
 * 歌词格式 UTF-8格式, 内容格式可以是以下一种
 * [毫秒整数时间] 一行歌词
 * [小数秒时间] 一行歌词
 * [小时:分钟:秒.毫秒] 一行歌词
 * [分钟:秒.毫秒] 一行歌词
 * 其中 小时/分钟/秒 必须是两位数, 毫秒必须是三位数
 */
class RpgLyric : public RpgObject{
	Q_OBJECT
private:
	QGraphicsTextItem *lyric = new QGraphicsTextItem(this);	// 显示歌词
	QGraphicsDropShadowEffect *lyricShadowEffect = new QGraphicsDropShadowEffect(this); // 歌词下的阴影
	QPropertyAnimation *lyricOpacityAnimation = new QPropertyAnimation(this->lyric, "opacity", this);	// 歌词淡入淡出接口

	QMap<int, QString> lyricMap; // 顺序保存歌词的map列表
	QMap<int, QString>::ConstIterator lyricI;	// 顺序保存歌词的迭代器

	RpgMusic *musicObj = RpgMusic::instance();	// 音乐控件

	const QPoint defaultPos = QPoint(50, ScreenHeight - 65); // 默认位置

	/**
	 * @brief lyricAnimationShow
	 * 显示歌词的歌词过渡动画
	 */
	void lyricAnimationShow(){
		if(this->scene() == nullptr){
			qDebug() << CodePath << "This object not in any scene";
			return;
		}
		if(this->lyricOpacityAnimation->state() != QPropertyAnimation::Stopped){
			this->lyricOpacityAnimation->stop();
		}
		this->lyricOpacityAnimation->setDirection(QPropertyAnimation::Forward);
		this->lyricOpacityAnimation->setCurrentTime(0);
		this->lyricOpacityAnimation->start();
	}

	/**
	 * @brief lyricAnimationHide
	 * 隐藏歌词的歌词过渡动画
	 */
	void lyricAnimationHide(){
		if(this->scene() == nullptr){
			qDebug() << CodePath << "This object not in any scene";
			return;
		}
		if(this->lyricOpacityAnimation->state() != QPropertyAnimation::Stopped){
			this->lyricOpacityAnimation->stop();
		}
		this->lyricOpacityAnimation->setDirection(QPropertyAnimation::Backward);
		this->lyricOpacityAnimation->setCurrentTime(this->lyricOpacityAnimation->totalDuration());
		this->lyricOpacityAnimation->start();
		RpgUtils::msleep(150);
	}

public:
	explicit RpgLyric(QGraphicsObject *parentObject = nullptr): RpgObject(parentObject){
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
		this->lyricOpacityAnimation->setDuration(300);
		this->lyricOpacityAnimation->setLoopCount(1);

		this->lyric->setOpacity(0);
	}

	~RpgLyric(){}

	/**
	 * @brief setRpgMusic
	 * @param music
	 * 设置RpgMusic组件
	 */
	void setRpgMusic(RpgMusic *music){
		if(this->musicObj != nullptr){
			this->musicObj->disconnect();
		}
		if(music != nullptr){
			this->musicObj = music;
		}else{
			qDebug() << CodePath << "Given RpgMusic point is not a pointer. using default RpgMusic instead.";
			this->musicObj = RpgMusic::instance();
		}
	}

	/**
	 * @brief loadLyricFromFile
	 * @param filename
	 * 从文件读取歌词(不推荐)
	 */
	void loadLyricFromFile(const QString &filename){
		if(this->getProcessing()){
			qDebug() << CodePath << "Cannot load the lyric while running!";
			return;
		}
		if(filename.isEmpty() || !QFile::exists(filename)){
			qDebug() << CodePath << QString("filename \"%1\" is not exist.").arg(filename);
			// Todo: throw a exception file not exist.
			return;
		}
		QFile f(filename);
		if(!f.open(QIODevice::ReadOnly)){
			qDebug() << CodePath << QString("filename \"%1\" cannot open: %2").arg(filename, f.errorString());
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
				qDebug() << CodePath << QString("Invalid line data: not start with '[': \"%1\". Ignored.").arg(line);
				continue;
			}
			int rightBraPos = line.indexOf("]");
			if(rightBraPos == -1){
				qDebug() << CodePath << QString("Invalid line data: Syntax error: cannot find timestamp tip: \"%1\". Ignored.").arg(line);
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
							qDebug() << CodePath << QString("Invalid line data: timestamp cannot resolve to [int], [double] [hh:mm:ss.zzz] or [mm:ss:zzz]: \"%1\". Ignored.").arg(line);
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

	/**
	 * @brief loadLyric
	 * @param name
	 * 从RpgFileManager读取歌词
	 */
	void loadLyric(const QString &name){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::LyricFile, name);
		if(filename.isEmpty()){
			qDebug() << CodePath << QString("Cannot open resource: \"%1\"").arg(name);
			// Todo: throw Resource not found exception;
			return;
		}
		this->loadLyricFromFile(filename);
	}

	/**
	 * @brief run
	 * 开始追显歌词
	 */
	void run() Override {
		RpgObject::run();
		// 重新设定其Scene
		if(RpgView::instance()->scene() == nullptr){
			qDebug() << CodePath << "RpgView not loaded scene yet.";
			this->end();
			throw RpgNullPointerException("RpgView::instance()->scene()");
		}else{
			// 如果该对象已装载至其他scene的时候, 这个函数能自动将这个对象在其他scene删除后添加至该scene
			RpgView::instance()->scene()->addItem(this);
		}

		if(this->lyricMap.isEmpty()){
			qDebug() << CodePath << "RpgLyric::lyricMap is empty, please load lyric first";
			this->end();
			return;
		}

		if(this->musicObj != nullptr && this->musicObj->getLoop() != 1){
			qDebug() << CodePath << "The loop BGM cannot display the lyric...";
			this->end();
			return;
		}

		this->lyric->setOpacity(0);
		this->showLyric();
	}

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认文字颜色
	 */
	inline void setTextColor(const QColor &color){ this->lyric->setDefaultTextColor(color); }

	/**
	 * @brief setFont
	 * @param font
	 * 设置字体
	 */
	inline void setFont(const QFont &font){ this->lyric->setFont(font); }

	inline void setPos(const QPointF &pos){ this->lyric->setPos(pos); }
	inline void setPos(qreal x, qreal y){ this->setPos(QPointF(x, y)); }

	/**
	 * @brief _dumpLyric
	 * 在debug显示全部歌词
	 */
	void _dumpLyric(){
		qDebug() << CodePath << "lyric =================================================";
		for(QMap<int, QString>::ConstIterator i = this->lyricMap.constBegin(); i != this->lyricMap.constEnd(); i++){
			qDebug() << CodePath << QString("[%1]%2").arg(i.key()).arg(i.value());
		}
		qDebug() << CodePath << "=======================================================";
	}

protected:
	/**
	 * @brief showLyric
	 * 开始执行歌词(由exec调用)
	 */
	void showLyric(){
		this->connect(this->musicObj, &RpgMusic::seeked, [this](int ms){
			if(this->lyricI == this->lyricMap.constEnd() || !this->getProcessing()){
				return;
			}
			while(this->lyricI != this->lyricMap.constEnd() && int(this->lyricI.key()) < ms){
				this->lyricI++;
			}
			if(int(this->lyricI.key()) <= ms + 300){
//				QtConcurrent::run([this, ms](){
				// bug: 这里如果执行等待的话, 有多个lyric的时候, 会按照顺序执行导致后面lyric执行延迟
				// 但加上Concurrent之后, setHtml会发出信号, 但这不是Qt信号线程传递的方式, 报错
				// Cannot send events to objects owned by a different thread.
				// 想办法将setHtml发送到lyric线程进行执行
				this->lyricAnimationHide();
				RpgUtils::msleep(int(this->lyricI.key()) - ms);
				this->lyric->setHtml(this->lyricI.value());
				this->lyricAnimationShow();
				this->lyricI++;
//				});
			}
		});

		this->connect(this->musicObj, &RpgMusic::stopped, [this](){
			if(!this->getProcessing()){
				return;
			}
			this->lyricAnimationHide();
			RpgUtils::msleep(300);
			this->hideLyric();
			this->lyricI = this->lyricMap.constBegin();
			this->setProcessing(false);
		});

		this->show();
	}

	/**
	 * @brief hideLyric
	 * 结束, 隐藏lyric控件(由Music的停止调用)
	 */
	void hideLyric(){
		this->musicObj->disconnect();
	}
};

#endif // RPGLYRIC_H
