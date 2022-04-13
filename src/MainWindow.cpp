#include "MainWindow.h"

#include <QMenu>
#include <QAction>

#include <Rpg/Rpg.h>
#include <Rpg/com/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialogItem.h>
#include <Rpg/com/RpgChoiceItem.h>
#include <Rpg/com/RpgMusic.h>
#include <Rpg/core/RpgDialogMessage.h>
#include <Rpg/com/RpgBanner.h>

#include <Rpg/com/RpgSound.h>

#include <QTextEdit>

void MainWindow::closeEvent(QCloseEvent *event){
	if(!this->canClose){
		int result = QMessageBox::question(this, qApp->applicationDisplayName(), tr("Do you really want to quit the game?\nThe progress not saved will be lost."), QMessageBox::Yes | QMessageBox::Cancel);
		if(result != QMessageBox::Yes){
			event->ignore();
			return;
		}
	}
	event->accept();
	qApp->quit();
	return;
}

void MainWindow::testModel()
{
	//rpgMusic->playMusic("title");

	RpgAxisItem *item = new RpgAxisItem(QRectF(0, 0, ScreenWidth, ScreenHeight), Rpg::MapTileSize, Rpg::MapTileSize);

	RpgScene *scene = new RpgScene();
	RpgView::instance()->setScene(scene);

	scene->addItem(item);

	RpgSpecLocationChopingDialog base("skin");

	//	RpgDialogItem dialog(&base);

	//	dialog.setDialogAlign(Rpg::AlignBottom);

//	dialog.appendMessage("This is a text");
//	dialog.appendMessage("This is another text");
//	RpgDialogMessage msg1 = RpgDialogMessage("测试5秒消息");
//	msg1.setWaitTime(5000);
//	dialog.appendMessage(msg1);
//	dialog.appendMessage("中文显示");
//	dialog.appendMessage(RpgDialogMessage("RPGDialogMessage", "NAME"));
//	dialog.appendMessage(RpgDialogMessage({
//											  {"name", "MY"},
//											  {"text", "MY test text 2333333333333333333333333333333333333333333333"},
//											  {"characterName", "002"},
//											  {"avatarMode", Rpg::AvatarHalfBodyBehind},
//											  {"avatarAround", Rpg::AvatarAroundRight},
//											  {"pointSize", 20},
//											  {"speed", 100},
//											  {"waitTime", 5000}
//										  }));
//	RpgDialogMessage msg2 = RpgDialogMessage("测试最后5秒消息");
//	msg2.setWaitTime(5000);
//	dialog.appendMessage(msg2);

//	dialog.run();
//	dialog.waitForComplete();

//	for(int i = 0; i < 11; i++){
//		rpgSound->play("banned");
//		RpgUtils::msleep(1);
//	}

//	RpgUtils::msleep(2000);
//	for(int i = 0; i < 10; i++){
//		rpgSound->play("select");
//		RpgUtils::msleep(1);
//	}

//	RpgBanner banner(10000);
//	QGraphicsTextItem *text = new QGraphicsTextItem("Hello, world!");
//	text->setFont(rpgFont->getFont("dialog", 36));
//	text->setDefaultTextColor(Qt::white);
//	text->setPos(50, 50);

//	banner.addItem("text", text);
//	banner.setTimeLineTimestampCb(100, [this](){
//		rDebug() << "100MS!";
//	});
//	banner.setTimeLineTimestampCb(500, [this](){
//		rDebug() << "500MS!";
//	});
//	banner.setTimeLineFrameCb(150, [this](){
//		rDebug() << "150 FRAMES!";
//	});

//	banner.run();
//	banner.waitForComplete();

	RpgDialogItem msg(&base);
	msg.setDialogAlign(Rpg::AlignBottom);
	msg.appendMessage("Choice 1<br>Choice 2<br>Choice 3");
	msg.run();
	msg.waitForComplete();

	RpgChoiceItem dialog(&base);
	dialog.setDialogAlign(Rpg::AlignBottom);
	dialog.setDialogSize(QSize(180, 160));
	dialog.appendChoice("はじめから");
	dialog.appendChoice("つづきから", false);
	dialog.appendChoice("オプション");
	dialog.appendChoice("お わ り");
	dialog.setDefaultChoice(0);
	//dialog.setTimeout(5000);

	dialog.run();
	int res = dialog.waitForComplete();
	rDebug() << "RES:" << res;

	if(res == 3){
		this->canClose = true;
		this->close();
	}

//	QTextEdit *edit = new QTextEdit(nullptr);
//	edit->resize(200, 150);
//	QGraphicsProxyWidget *editProxy = scene->addWidget(edit);
//	editProxy->setPos(50, 50);
//	editProxy->setZValue(Rpg::ZValueFront);

	this->canClose = true;
}

void MainWindow::setupMenus(){

}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
	//this->widget = new RpgWidget(this);
	//this->setCentralWidget(this->widget);
	RpgView *view = RpgView::instance(this);
	this->setCentralWidget(view);

	//			this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
	this->setMinimumSize(ScreenWidth + 2, ScreenHeight + 2);

	//this->setWindowOpacity(0.5);
	setMouseTracking(false); // 设置是否响应鼠标移动时未按下按钮的事件发生
	QSizePolicy policy = this->sizePolicy();{
		policy.setHorizontalPolicy(QSizePolicy::Preferred);
		policy.setVerticalPolicy(QSizePolicy::Preferred);
		policy.setHeightForWidth(true);
	};
	this->setSizePolicy(policy);

	//		RpgFileManager::instance()->addFile(RpgFileManager::MusicFile, "title", QUrl::fromLocalFile("data/music/title.ogg"));
	//		RpgMusic::instance()->playMusic("title");

	QString initJsonFile = "initialize.json";
	RpgPreload preload(initJsonFile);
	RpgFileManager::instance()->dumpFiles();


	QTimer::singleShot(1000, this, &MainWindow::testModel);
}

MainWindow::~MainWindow()
{

}
