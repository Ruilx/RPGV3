#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Rpg/com/RpgWidget.h>

#include <Rpg/core/RpgFileManager.h>
#include <Rpg/core/RpgPreload.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	RpgWidget *widget = nullptr;

	void closeEvent(QCloseEvent *event){
		if(!widget->canClose()){
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

public:
	MainWindow(QWidget *parent = nullptr): QMainWindow(parent){
		this->widget = new RpgWidget(this);
		this->setCentralWidget(this->widget);

//			this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
		this->setMinimumSize(ScreenWidth + 2, ScreenHeight + 2);

		//RpgFileManager::instance()->addFile(RpgFileManager::MusicFile, "title", QUrl::fromLocalFile("data/music/title.ogg"));
		//RpgMusic::instance()->playMusic("title");

		QString initJsonFile = "initialize.json";
		RpgPreload preload(initJsonFile);
		RpgFileManager::instance()->dumpFiles();

	}
	~MainWindow();
};

#endif // MAINWINDOW_H
