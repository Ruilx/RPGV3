#include "MainWindow.h"

#include <Rpg/Rpg.h>
#include <Rpg/com/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialog.h>
#include <Rpg/com/RpgMusic.h>

void MainWindow::testModel()
{
	rpgMusic->playMusic("年中混乱中");

	RpgAxisItem *item = new RpgAxisItem(QRectF(0, 0, ScreenWidth, ScreenHeight), Rpg::MapTileSize, Rpg::MapTileSize);

	RpgScene *scene = new RpgScene();
	RpgView::instance()->setScene(scene);

	scene->addItem(item);

	RpgSpecLocationChopingDialog base("blueSkin");

	RpgDialog dialog(&base);

	dialog.setDialogAlign(Rpg::AlignBottom);

	dialog.appendMessage("This is a text");

	dialog.run();
	dialog.waitForComplete();

}

MainWindow::~MainWindow()
{

}
