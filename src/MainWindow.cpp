#include "MainWindow.h"

#include <Rpg/Rpg.h>
#include <Rpg/com/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialog.h>
#include <Rpg/com/RpgMusic.h>

void MainWindow::testModel()
{
	rpgMusic->playMusic("title");

	RpgAxisItem *item = new RpgAxisItem(QRectF(0, 0, ScreenWidth, ScreenHeight), Rpg::MapTileSize, Rpg::MapTileSize);

	RpgScene *scene = new RpgScene();
	RpgView::instance()->setScene(scene);

	scene->addItem(item);

	RpgSpecLocationChopingDialog base("skin");

	RpgDialog dialog(&base);

	dialog.setDialogAlign(Rpg::AlignBottom);

	dialog.appendMessage("This is a text");
	dialog.appendMessage("This is another text");
	dialog.appendMessage("中文显示");
	dialog.appendMessage(RpgDialogMessage("RPGDialogMessage", "NAME"));

	dialog.run();
	dialog.waitForComplete();

}

MainWindow::~MainWindow()
{

}
