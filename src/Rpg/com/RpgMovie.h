#ifndef RPGMOVIE_H
#define RPGMOVIE_H

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgItemProperties.h>

#include <Rpg/com/RpgMusic.h>

#include <QtAVWidgets/GraphicsItemRenderer.h>

/**
 * @brief The RpgMovie class
 */
class RpgMovie : public RpgObject
{
	Q_OBJECT

	RpgMusic *musicInstance = rpgMusic;

	QtAV::GraphicsItemRenderer *movie = new QtAV::GraphicsItemRenderer(this);

    QPropertyAnimation *enterAnimation = new QPropertyAnimation(this);
    QPropertyAnimation *exitAnimation = new QPropertyAnimation(this);

	QString movieName;

	bool canSkip = false;
	bool willWaitKeyPress = false;

    void keyReleaseEvent(QKeyEvent *event) override;

	void setup();
public:
	RpgMovie(const QString &movieName, RpgMusic *music = nullptr, QGraphicsObject *parentObject = nullptr): RpgObject(parentObject){
		setRpgMusic(music);
		this->movieName = movieName;
		this->setup();
	}
	~RpgMovie();

	void setRpgMusic(RpgMusic *music){
		if(this->musicInstance != nullptr){
			this->musicInstance->setRenderer(nullptr);
			this->musicInstance->disconnect();
		}
		if(music != nullptr){
			this->musicInstance = music;
			this->musicInstance->setRenderer(this->movie);
			this->connect(this->musicInstance, &RpgMusic::stopped, [this](){
				if(!this->isRunning()){
					rDebug() << "RpgMovie not running!";
					return;
				}
				// TODO: music stopped
				this->hideMovie();
			});

		}else{
			if(rpgMusic != nullptr){
				rDebug() << "Given RpgMusic point is not a pointer. using default RpgMusic instead.";
				this->setRpgMusic(rpgMusic);
			}
		}
	}

	const RpgMusic* getRpgMusic() const{
		return this->musicInstance;
	}

	void setMovieName(const QString &movieName){
		this->movieName = movieName;
	}

	const QString getMoveName() const{
		return this->movieName;
	}

	inline void setCanSkip(bool canSkip){ this->canSkip = canSkip; }
    inline bool getCanSkip() const { return this->canSkip; }

	inline void setWillWaitKeyPress(bool willWaitKey);
    inline bool getWillWaitKeyPress() const { return this->willWaitKeyPress; }

	void run() override;
	int waitForComplete();
	void end() override;
	void showMovie();
	void hideMovie();

signals:
	void enterAutoMode();
    void exitAutoMode();
};

#endif // RPGMOVIE_H
