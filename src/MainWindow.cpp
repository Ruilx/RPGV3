#include "MainWindow.h"

#include <Rpg/Rpg.h>
#include <Rpg/com/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialogItem.h>
#include <Rpg/com/RpgMusic.h>
#include <Rpg/core/RpgDialogMessage.h>

void MainWindow::testModel()
{
	//rpgMusic->playMusic("title");

	RpgAxisItem *item = new RpgAxisItem(QRectF(0, 0, ScreenWidth, ScreenHeight), Rpg::MapTileSize, Rpg::MapTileSize);

	RpgScene *scene = new RpgScene();
	RpgView::instance()->setScene(scene);

	scene->addItem(item);

	RpgSpecLocationChopingDialog base("skin");

	RpgDialogItem dialog(&base);

	dialog.setDialogAlign(Rpg::AlignBottom);

	dialog.appendMessage("This is a text");
	dialog.appendMessage("This is another text");
	RpgDialogMessage msg1 = RpgDialogMessage("测试5秒消息");
	msg1.setWaitTime(5000);
	dialog.appendMessage(msg1);
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
	RpgDialogMessage msg2 = RpgDialogMessage("测试最后5秒消息");
	msg2.setWaitTime(5000);
	dialog.appendMessage(msg2);

	dialog.run();
	dialog.waitForComplete();

}

MainWindow::~MainWindow()
{

}
