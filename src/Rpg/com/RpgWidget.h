#ifndef RPGWIDGET_H
#define RPGWIDGET_H

#include <QGraphicsView>
#include <QVBoxLayout>
#include <Rpg/Global.h>
#include <QMouseEvent>

#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/com/RpgView.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialog.h>

#include <Rpg/core/RpgSpecLocationChopingDialog.h>

#include <Rpg/com/RpgMusic.h>
#include <Rpg/com/RpgLyric.h>

class RpgWidget : public QWidget
{
	Q_OBJECT

	int timerId = -1;

	bool _canClose = false;
protected:

//	void mouseDoubleClickEvent(QMouseEvent *event){
//		qDebug() << CodePath << "Mouse pos:" << event->pos();
//	}

//	void mouseMoveEvent(QMouseEvent *event){
//		qDebug() << (event->button());
//	}

	void keyPressEvent(QKeyEvent *event){
		int key = event->key();
		Qt::KeyboardModifiers mod = event->modifiers();
		qDebug() << CodePath << tr("Receive Key Press ↓: ") + RpgUtils::keysToString((Qt::Key)key, mod);
		emit this->receiveKeyPressSignal(key, mod);
		if(RpgView::instance()->scene() != nullptr){
			RpgState::instance()->receiveKeyPress(key, mod, RpgView::instance()->scene());
		}
		event->accept();
	}

	void keyReleaseEvent(QKeyEvent *event){
		int key = event->key();
		Qt::KeyboardModifiers mod = event->modifiers();
		qDebug() << CodePath << tr("Receive Key Release ↑: ") + RpgUtils::keysToString((Qt::Key)key, mod);
		emit this->receiveKeyReleaseSignal(key, mod);
		if(RpgView::instance()->scene() != nullptr){
			RpgState::instance()->receiveKeyRelease(key, mod, RpgView::instance()->scene());
		}
		event->accept();
	}

	void closeEvent(QCloseEvent *){}

public:
	RpgWidget(QWidget *parent = nullptr): QWidget(parent){
		RpgView::instance(this);

		QVBoxLayout *mainLay = new QVBoxLayout;
		this->setLayout(mainLay);
		mainLay->addWidget(RpgView::instance());
		mainLay->setMargin(0);

		//RpgView::instance()->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
		RpgView::instance()->setRenderHint(QPainter::TextAntialiasing);

		QTimer::singleShot(1000, this, &RpgWidget::ready);
	}

	inline bool canClose() const{
		return this->_canClose;
	}

	inline void setCanClose(bool e){
		this->_canClose = e;
	}

signals:
	void receiveKeyPressSignal(int key, Qt::KeyboardModifiers mod);
	void receiveKeyReleaseSignal(int key, Qt::KeyboardModifiers mod);
	void readyToClose();
private slots:
	void ready(){
		RpgFileManager::instance()->addFile(RpgFileManager::ImageFile, "blackSkin", "data/images/skin/blackAlpha.png");
		RpgFileManager::instance()->addFile(RpgFileManager::ImageFile, "blueSkin", "data/images/skin/blueAlpha.png");
		RpgFileManager::instance()->addFile(RpgFileManager::ImageFile, "whiteSkin", "data/images/skin/whiteAlpha.png");
		RpgFileManager::instance()->addFile(RpgFileManager::FontFile, "fallback", "data/font/msyhl.ttc");
		RpgFileManager::instance()->addFile(RpgFileManager::FontFile, "dialog", "data/font/TsangerYunHei-W05.ttf");
		RpgFileManager::instance()->addFile(RpgFileManager::FontFile, "lyric", "data/font/TsangerYunHei-W05.ttf");
		RpgFileManager::instance()->addFile(RpgFileManager::AvatarFile, "002", "data/images/avatar/002.png");
		RpgFileManager::instance()->addFile(RpgFileManager::AvatarFile, "002_head", "data/images/avatar/002_head.png");
		RpgFileManager::instance()->addFile(RpgFileManager::MusicFile, "Turnin", "data/music/Turnin.mp3");
		RpgFileManager::instance()->addFile(RpgFileManager::LyricFile, "Turnin", "data/lyrics/Turnin.lrc");
		RpgFileManager::instance()->addFile(RpgFileManager::MusicFile, "TOKYO BON", "data/music/TOKYO BON.mp3");
		RpgFileManager::instance()->addFile(RpgFileManager::LyricFile, "TOKYO BON", "data/lyrics/TOKYO BON.lrc");

		RpgFont::setFallbackFont("dialog", "fallback");

		RpgScene *scene = new RpgScene();

		RpgView::instance()->setScene(scene);

		scene->addRect(0, 0, ScreenWidth, ScreenHeight, QPen(Qt::white));

		for(int i = -1000; i <= 1000; i += 50){
			scene->addEllipse(QRectF(i-2, -2, 4, 4), QPen(Qt::white));
			QGraphicsTextItem *label = new QGraphicsTextItem(QString::number(i));
			label->setFont(QFont("RuilxFixedSys2", 6));
			label->setPos(i + 2, 2);
			label->setDefaultTextColor(QColor(Qt::white));
			scene->addItem(label);
			if(i != 0){
				scene->addEllipse(QRectF(-2, i-2, 4, 4), QPen(Qt::white));
				QGraphicsTextItem *label = new QGraphicsTextItem(QString::number(i));
				label->setFont(QFont("RuilxFixedSys2", 6));
				label->setPos(2, i - 2);
				label->setDefaultTextColor(QColor(Qt::white));
				scene->addItem(label);
			}
		}

		RpgSpecLocationChopingDialog *base = new RpgSpecLocationChopingDialog("blueSkin");

		RpgDialog *rpgDialog = new RpgDialog(base);
		rpgDialog->setDialogAlign(Rpg::AlignLeft);
		rpgDialog->setDialogSize(QSize(500, rpgDialog->getDialogSize().height()));
		rpgDialog->appendMessage(RpgDialogMessage("普通文字 <x>黑色文字</x> <r>红色文字</r> <g>绿色文字</g> <b>蓝色文字</b> <y>黄色文字</y> <m>品红文字</m> <c>青色文字</c> <w>白色文字</w> <xl>亮黑色文字</xl> <rl>亮红色文字</rl> <gl>亮绿色文字</gl> <bl>亮蓝色文字</bl> <yl>亮黄色文字</yl> <ml>亮品红色文字</ml> <cl>亮青色文字</cl> <wl>亮白色文字</wl>"));
		rpgDialog->appendMessage(RpgDialogMessage("<big>测试Big文本</big>"));
		rpgDialog->appendMessage(RpgDialogMessage("测试一段没有立绘的文字"));
		RpgDialogMessage aa("头像在左侧, 使用<r>Rpg::AvatarFrame</r>和<r>Rpg::AvatarAroundLeft</r>");
		aa.setName("Rpg制作者");
		aa.setCharacterName("002_head");
		aa.setAvatarAround(Rpg::AvatarAroundLeft);
		aa.setAvatarMode(Rpg::AvatarFrame);
		rpgDialog->appendMessage(aa);
		RpgDialogMessage bb("头像在右侧, 使用<r>Rpg::AvatarFrame</r>和<r>Rpg::AvatarAroundRight</r>");
		bb.setName("Rpg制作者");
		bb.setCharacterName("002_head");
		bb.setAvatarAround(Rpg::AvatarAroundRight);
		bb.setAvatarMode(Rpg::AvatarFrame);
		rpgDialog->appendMessage(bb);
		RpgDialogMessage cc("立绘在左侧, 位于对话框之后, 使用<r>Rpg::AvatarHalfBodyBehind</r>和<r>Rpg::AvatarAroundLeft</r>");
		cc.setName("Rpg制作者");
		cc.setCharacterName("002");
		cc.setAvatarAround(Rpg::AvatarAroundLeft);
		cc.setAvatarMode(Rpg::AvatarHalfBodyBehind);
		rpgDialog->appendMessage(cc);
		RpgDialogMessage dd("立绘在右侧, 位于对话框之后, 使用<r>Rpg::AvatarHalfBodyBehind</r>和<r>Rpg::AvatarAroundRight</r>");
		dd.setName("Rpg制作者");
		dd.setCharacterName("002");
		dd.setAvatarAround(Rpg::AvatarAroundRight);
		dd.setAvatarMode(Rpg::AvatarHalfBodyBehind);
		rpgDialog->appendMessage(dd);
		RpgDialogMessage ee("立绘在左侧, 位于对话框之前, 使用<r>Rpg::AvatarHalfBodyFront</r>和<r>Rpg::AvatarAroundLeft</r>");
		ee.setName("Rpg制作者");
		ee.setCharacterName("002");
		ee.setAvatarAround(Rpg::AvatarAroundLeft);
		ee.setAvatarMode(Rpg::AvatarHalfBodyFront);
		ee.setPointSize(12);
		ee.setLineHeight(20);
		rpgDialog->appendMessage(ee);
		RpgDialogMessage ff("立绘在右侧, 位于对话框之前, 使用<r>Rpg::AvatarHalfBodyFront</r>和<r>Rpg::AvatarAroundRight</r>");
		ff.setName("Rpg制作者");
		ff.setCharacterName("002");
		ff.setAvatarAround(Rpg::AvatarAroundRight);
		ff.setAvatarMode(Rpg::AvatarHalfBodyFront);
		ff.setPointSize(12);
		ff.setLineHeight(20);
		rpgDialog->appendMessage(ff);
		rpgDialog->appendMessage(aa);
		rpgDialog->appendMessage(cc);
		rpgDialog->appendMessage(ee);
		rpgDialog->appendMessage(bb);
		rpgDialog->appendMessage(dd);
		rpgDialog->appendMessage(ff);
		rpgDialog->appendMessage(RpgDialogMessage("测试一段没有立绘的文字", "一个没有立绘的Rpg制作者"));
		rpgDialog->appendMessage(RpgDialogMessage("<r>あいうえお</r> <g>かきくけこ</g> <b>さしすせそ</b> <y>たちつてと</y> <m>なにぬねの</m> <c>はひふへほ</c> <x>まみむめも</x> やゆよ <w>らりるれろ</w> わを ん"));
		rpgDialog->run();
		rpgDialog->waitForComplete();

//		RpgLyric *lrc = new RpgLyric();
//		lrc->loadLyric("TOKYO BON");

//		RpgMusic::instance()->setLoop(1);
//		RpgMusic::instance()->playMusic("TOKYO BON");
//		lrc->run();



		qDebug() << "Finished";
	}
public slots:
	void doReadyToClose(){
		emit this->readyToClose();
	}
};

#endif // RPGWIDGET_H
