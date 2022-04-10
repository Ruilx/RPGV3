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
	void lyricAnimationShow();

	/**
	 * @brief lyricAnimationHide
	 * 隐藏歌词的歌词过渡动画
	 */
	void lyricAnimationHide();

public:
	explicit RpgLyric(QGraphicsObject *parentObject = nullptr);

	~RpgLyric(){}

	/**
	 * @brief setRpgMusic
	 * @param music
	 * 设置RpgMusic组件
	 */
	void setRpgMusic(RpgMusic *music);

	/**
	 * @brief loadLyricFromFile
	 * @param filename
	 * 从文件读取歌词(不推荐)
	 */
	void loadLyricFromFile(const QString &filename);

	/**
	 * @brief loadLyric
	 * @param name
	 * 从RpgFileManager读取歌词
	 */
	void loadLyric(const QString &name);

	/**
	 * @brief run
	 * 开始追显歌词
	 */
	void run() Override {
		RpgObject::run();
		// 重新设定其Scene
		if(rpgView->scene() == nullptr){
			rDebug() << "RpgView not loaded scene yet.";
			this->end();
			throw RpgNullPointerException("RpgView::instance()->scene()");
		}else{
			// 如果该对象已装载至其他scene的时候, 这个函数能自动将这个对象在其他scene删除后添加至该scene
			rpgView->scene()->addItem(this);
		}

		if(this->lyricMap.isEmpty()){
			rDebug() << "RpgLyric::lyricMap is empty, please load lyric first";
			this->end();
			return;
		}

		if(this->musicObj != nullptr && this->musicObj->getLoop() != 1){
			rWarning() << "The loop BGM cannot display the lyric...";
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
		rDebug() << "lyric =================================================";
		for(QMap<int, QString>::ConstIterator i = this->lyricMap.constBegin(); i != this->lyricMap.constEnd(); i++){
			rDebug() << QString("[%1]%2").arg(i.key()).arg(i.value());
		}
		rDebug() << "=======================================================";
	}

protected:
	/**
	 * @brief showLyric
	 * 开始执行歌词(由exec调用)
	 */
	void showLyric(){
		this->connect(this->musicObj, &RpgMusic::seeked, [this](int ms){
			if(this->lyricI == this->lyricMap.constEnd() || !this->getRunning()){
				return;
			}
			while(this->lyricI != this->lyricMap.constEnd() && int(this->lyricI.key()) < ms){
				this->lyricI++;
			}
			if(int(this->lyricI.key()) <= ms + 200){
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
			if(!this->getRunning()){
				return;
			}
			this->lyricAnimationHide();
			RpgUtils::msleep(300);
			this->hideLyric();
			this->lyricI = this->lyricMap.constBegin();
			this->setRunning(false);
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
