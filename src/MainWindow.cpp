#include "MainWindow.h"

#include <QMenu>
#include <QAction>

#include <Rpg/Rpg.h>
#include <Rpg/com/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialogItem.h>
#include <Rpg/com/RpgChoiceItem.h>
#include <Rpg/com/RpgMusic.h>
#include <Rpg/com/RpgMovie.h>
#include <Rpg/core/RpgDialogMessage.h>
#include <Rpg/com/RpgBanner.h>
#include <Rpg/com/RpgItem/RpgSpinItem.h>
#include <Rpg/com/RpgItem/RpgInputItem.h>

#include <Rpg/com/RpgSound.h>

#include <QtAVWidgets/global.h>

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

	QList<QPair<int, int>> test;
	test.append(QPair<int, int>(1, 1));
	test.append(QPair<int, int>(5, 20));
	test.append(QPair<int, int>(3, 15));
	test.append(QPair<int, int>(4, 7));
	test.append(QPair<int, int>(2, 5));
	test.append(QPair<int, int>(6, 18));
	test.append(QPair<int, int>(1, 15));
	test.append(QPair<int, int>(3, 18));

	QList<QPair<int, int>> rres = RpgUtils::optmize2dCoordGaps(test);
	for(auto &i: rres){
		qDebug() << "(" << i.first << ", " << i.second << ")";
	}

	RpgAxisItem *item = new RpgAxisItem(QRectF(0, 0, ScreenWidth, ScreenHeight), Rpg::MapTileSize, Rpg::MapTileSize);

	RpgScene *scene = new RpgScene();
	RpgView::instance()->setScene(scene);

	scene->addItem(item);

	RpgSpecLocationChopingDialog base("skin");
	RpgSpecLocationChopingDialog clear("clearSkin");

//	RpgMovie movie("test");
//	movie.setZValue(Rpg::ZValueMapFront);
//	movie.run();

	RpgDialogItem d(&clear);
	RpgDialogMessage msg1("测试文本:RPGV3是一个使用<r>Qt图像处理</r>, <g>QtAV组件多媒体</g>, <b>JavaScript作为脚本执行</b>的2D RPGMaker样式的C++ RPG<sup>故事执行器</sup>");
	msg1.setSpeed(Rpg::SingleWordSpeedFast);
	d.appendMessage(msg1);
	d.run();
	d.waitForComplete();

	RpgInputItem inputItem(&base);
	inputItem.setMinLength(3);
	inputItem.setMessage("请输入一个长长的名字, 看下面能否显示:");
	//inputItem.setInputMask("1");
	inputItem.run();
	inputItem.waitingForComplete();

	const QString text = inputItem.getValue();
	rDebug() << "Input text:" << text;

	RpgDialogItem dialog(&base);
	dialog.setDialogAlign(Rpg::AlignBottom);
	dialog.appendMessage("This is a text");
	dialog.appendMessage("This is another text");
	RpgDialogMessage msg2 = RpgDialogMessage("测试5秒消息");
	msg2.setWaitTime(5000);
	dialog.appendMessage(msg2);
	dialog.appendMessage("中文显示");
	dialog.appendMessage(RpgDialogMessage("RPGDialogMessage", "NAME"));
	dialog.appendMessage(RpgDialogMessage({
											  {"name", "MY"},
											  {"text", "MY test text 2333333333333333333333333333333333333333333333"},
											  {"characterName", "002"},
											  {"avatarMode", Rpg::AvatarHalfBodyBehind},
											  {"avatarAround", Rpg::AvatarAroundRight},
											  {"pointSize", 20},
											  {"speed", 100},
											  {"waitTime", 5000}
										  }));
	RpgDialogMessage msg3 = RpgDialogMessage("测试最后5秒消息");
	msg3.setWaitTime(5000);
	dialog.appendMessage(msg3);

	dialog.run();
	dialog.waitForComplete();

//	for(int i = 0; i < 11; i++){
//		rpgSound->play("banned");
//		RpgUtils::msleep(1);
//	}

//	RpgUtils::msleep(2000);
//	for(int i = 0; i < 10; i++){
//		rpgSound->play("select");
//		RpgUtils::msleep(1);
//	}

	RpgBanner banner(5000);
	QGraphicsTextItem *bitem = new QGraphicsTextItem("Hello, world!");
	bitem->setFont(rpgFont->getFont("dialog", 36));
	bitem->setDefaultTextColor(Qt::white);
	bitem->setPos(RpgUtils::getDialogPos(Rpg::AlignCenter, bitem->boundingRect().size()));

	banner.addItem("bitem", bitem);
	banner.setTimeLineTimestampCb(100, [this](){
		rDebug() << "100MS!";
	});
	banner.setTimeLineTimestampCb(500, [this](){
		rDebug() << "500MS!";
	});
	banner.setTimeLineFrameCb(150, [this](){
		rDebug() << "150 FRAMES!";
	});

	banner.run();
	banner.waitForComplete();

	RpgChoiceItem cdialog(&base);
	cdialog.setDialogAlign(Rpg::AlignBottom);
	cdialog.setDialogSize(QSize(180, 160));
	cdialog.appendChoice(RpgChoiceMessage({
		{"text", "はじめから"},
		{"align", Rpg::AlignCenter},
	}));
	cdialog.appendChoice(RpgChoiceMessage({
		{"text", "つづきから"},
		{"enabled", false},
		{"align", Rpg::AlignCenter},
	}));
	cdialog.appendChoice(RpgChoiceMessage({
		{"text", "オプション"},
		{"align", Rpg::AlignCenter},
	}));
	cdialog.appendChoice(RpgChoiceMessage({
		{"text", "お わ り"},
		{"align", Rpg::AlignCenter},
	}));
	cdialog.setDefaultChoice(0);
	//dialog.setTimeout(5000);

	cdialog.run();
	int res = cdialog.waitForComplete();
	rDebug() << "RES:" << res;

	RpgDialogItem msg(&base);
	msg.setDialogAlign(Rpg::AlignBottom);
	msg.appendMessage(QString("你选择了第%1项, 值为:%2").arg(res).arg(cdialog.getValue()));
	msg.run();
	msg.waitForComplete();

//	if(res == 3){
//		this->canClose = true;
//		this->close();
//	}

	RpgSpinItem spinItem(&base);
	spinItem.setMessage("请输入密码:");
	spinItem.appendSpinValue(RpgSpinValue("0;1;2;3;4;5;6;7;8;9;A;B;C;D;E;F"));
	spinItem.appendSpinValue(RpgSpinValue("0;1;2;3;4;5;6;7;8;9;A;B;C;D;E;F"));
	spinItem.appendSpinValue(RpgSpinValue("0;1;2;3;4;5;6;7;8;9;A;B;C;D;E;F"));
	spinItem.appendSpinValue(RpgSpinValue("0;1;2;3;4;5;6;7;8;9;A;B;C;D;E;F"));
	spinItem.appendSpinValue(RpgSpinValue("0;1;2;3;4;5;6;7;8;9;A;B;C;D;E;F"));
	spinItem.appendSpinValue(RpgSpinValue("0;1;2;3;4;5;6;7;8;9;A;B;C;D;E;F"));
	spinItem.appendSpinValue(RpgSpinValue("洛天依;乐正绫;言和;初音ミク！"));
	spinItem.run();
	spinItem.waitForComplete();
	QStringList result = spinItem.getValue();
	rDebug() << "Result:" << result;

//	RpgDialogItem dialogItem(&base);
//	dialogItem.appendMessage("您输入了: " % result.join(""));
//	dialogItem.run();
//	dialogItem.waitForComplete();

//	QTextEdit *edit = new QTextEdit(nullptr);
//	edit->resize(200, 150);
//	QGraphicsProxyWidget *editProxy = scene->addWidget(edit);
//	editProxy->setPos(50, 50);
//	editProxy->setZValue(Rpg::ZValueFront);


	this->canClose = true;

//	movie.waitForComplete();
}

void MainWindow::setupMenus(){
	QMenu *fileMenu = new QMenu(this->tr("&File"), this);{
		this->menuBar()->addMenu(fileMenu);
	}

	QMenu *editMenu = new QMenu(this->tr("&Edit"), this);{
		this->menuBar()->addMenu(editMenu);
	}

	QMenu *toolMenu = new QMenu(this->tr("&Tool"), this);{
		this->menuBar()->addMenu(toolMenu);
	}

	QMenu *helpMenu = new QMenu(this->tr("&Help"), this);{
		this->menuBar()->addMenu(helpMenu);

		QAction *aboutAct = new QAction("&About", this);
		connect(aboutAct, &QAction::triggered, [this](){
			QtAV::about();
		});
		helpMenu->addAction(aboutAct);

		QAction *aboutQtAct = new QAction("About &Qt", this);
		connect(aboutQtAct, &QAction::triggered, [this](){
			qApp->aboutQt();
		});
		helpMenu->addAction(aboutQtAct);

		QAction *aboutQtAVAct = new QAction("About QtA&V", this);
		connect(aboutQtAVAct, &QAction::triggered, [this](){
			QtAV::aboutQtAV();
		});
		helpMenu->addAction(aboutQtAVAct);

		QAction *aboutFFmpegAct = new QAction("About &FFmpeg", this);
		connect(aboutFFmpegAct, &QAction::triggered, [this](){
			QtAV::aboutFFmpeg();
		});
		helpMenu->addAction(aboutFFmpegAct);

	}

}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
	//this->widget = new RpgWidget(this);
	//this->setCentralWidget(this->widget);
	RpgView *view = RpgView::instance(this);
	this->setCentralWidget(view);

	this->setupMenus();

	//			this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
	//this->setMinimumSize(ScreenWidth + 2, ScreenHeight + 2);



	//this->adjustSize();

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
