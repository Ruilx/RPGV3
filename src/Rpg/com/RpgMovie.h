#ifndef RPGMOVIE_H
#define RPGMOVIE_H

#include <Rpg/Rpg.h>
#include <Rpg/core/RpgObject.h>
#include <Rpg/core/RpgItemProperties.h>

#include <Rpg/com/RpgMusic.h>

#include <Rmod/QtAVWidgets/RGraphicsItemRenderer.h>

/**
 * @brief The RpgMovie class
 */
class RpgMovie : public RpgObject
{
	Q_OBJECT

	RpgMusic *musicInstance = rpgMusic;

	RGraphicsItemRenderer *movie = new RGraphicsItemRenderer(this);

	QPropertyAnimation *enterAnimation = new QPropertyAnimation(this);
	QPropertyAnimation *exitAnimation = new QPropertyAnimation(this);

	QString movieName;

	bool canSkip = false;
	bool willWaitKeyPress = false;

	int loop = 1;

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
		rDebug() << "SET MUSIC CHECK MUSICINSTANCE";
		if(this->musicInstance != nullptr){
			this->musicInstance->setRenderer(nullptr);
			this->musicInstance->disconnect();
		}
		rDebug() << "SET NEW MUSIC";
		if(music != nullptr){
			this->musicInstance = music;
		}else{
			this->musicInstance = rpgMusic;
		}
		rDebug() << "CONNECT";
		this->connect(this->musicInstance, &RpgMusic::stopped, [this](){
			if(!this->isRunning()){
				rDebug() << "RpgMovie not running!";
				return;
			}
			// TODO: music stopped
			this->hideMovie();
		});
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

	inline void setLoop(int loop){ this->loop = loop; }
	inline int getLoop() const { return this->loop; }

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
