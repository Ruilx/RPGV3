#include "MainWindow.h"

#include <Rpg/Rpg.h>
#include <Rpg/com/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

void MainWindow::testModel(){
	RpgAxisItem *item = new RpgAxisItem(QRectF(0, 0, ScreenWidth, ScreenHeight), Rpg::MapTileSize, Rpg::MapTileSize);

	RpgScene *scene = new RpgScene();
	RpgView::instance()->setScene(scene);

	scene->addItem(item);

}

MainWindow::~MainWindow()
{

}
