#include "MainWindow.h"

#include <Rpg/core/RpgItem/RpgAxisItem.h>
#include <Rpg/com/RpgScene.h>

void MainWindow::testModel(){
	RpgAxisItem *item = new RpgAxisItem(QRectF(0, 0, ScreenWidth, ScreenHeight));

	RpgScene *scene = new RpgScene();
	RpgView::instance()->setScene(scene);

	scene->addItem(item);

}

MainWindow::~MainWindow()
{

}
