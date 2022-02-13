#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Rpg/com/RpgWidget.h>

#include <Rpg/core/RpgFileManager.h>
#include <Rpg/core/RpgPreload.h>

/**
 * @brief The MainWindow class
 * 主界面窗口的建立, 布置页面样式以及启动scene脚本等工作
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

	bool canClose = false;

	void closeEvent(QCloseEvent *event){
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

	void testModel();

public:
	MainWindow(QWidget *parent = nullptr): QMainWindow(parent){
		//this->widget = new RpgWidget(this);
		//this->setCentralWidget(this->widget);
		RpgView *view = RpgView::instance(this);
		this->setCentralWidget(view);

//			this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
		this->setMinimumSize(ScreenWidth + 2, ScreenHeight + 2);

		//this->setWindowOpacity(0.5);
		//setMouseTracking(false); // 设置是否响应鼠标移动时未按下按钮的事件发生
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
	~MainWindow();
};

#endif // MAINWINDOW_H
