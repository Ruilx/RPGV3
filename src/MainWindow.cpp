#include "MainWindow.h"

#include <Rpg/Rpg.h>
#include <Rpg/com/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialogItem.h>
#include <Rpg/com/RpgChoiceItem.h>
#include <Rpg/com/RpgMusic.h>
#include <Rpg/core/RpgDialogMessage.h>

#include <Rpg/com/RpgSound.h>

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

	RpgDialogItem msg(&base);
	msg.setDialogAlign(Rpg::AlignBottom);
	msg.appendMessage("Choice 1<br>Choice 2<br>Choice 3");
	msg.run();
	msg.waitForComplete();

	RpgChoiceItem dialog(&base);
	dialog.setDialogAlign(Rpg::AlignBottom);
	//dialog.setDialogSize(QSize(150, 180));
	dialog.appendChoice("Choice 1");
	dialog.appendChoice("Choice 2", false);
	dialog.appendChoice("Choice 3");
	dialog.appendChoice("Choice 4", false);
	dialog.appendChoice("Choice 5");
	dialog.appendChoice("Choice 6", false);
	dialog.appendChoice("Choice 7");
	dialog.appendChoice("Choice 8", false);
	dialog.appendChoice("Choice 9");
	dialog.setDefaultChoice(0);
	//dialog.setTimeout(5000);

	dialog.run();
	int res = dialog.waitForComplete();
	rDebug() << "RES:" << res;


	this->canClose = true;
}

MainWindow::~MainWindow()
{

}
