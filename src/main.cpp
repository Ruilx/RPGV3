#include "MainWindow.h"
#include <Rpg/exception/RpgException.h>
#include <QUnhandledException>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qDebug() << CodePath << "Build time:" <<  __DATE__ << " " << __TIME__;

	a.setApplicationDisplayName("RPGV3");
	a.setApplicationVersion("0.0.1");
	a.setDoubleClickInterval(1000);
	a.setKeyboardInputInterval(1000);

	qDebug() << CodePath << "Library paths:" << a.libraryPaths();

	a.setFont(QApplication::font());

	try{
		MainWindow w;
		w.show();

		return a.exec();
	}catch(const RpgException &e){
		qDebug() << "RPG_EXCEPTION:" << e.toString();
		return 1;
	}catch(const QUnhandledException &e){
		qDebug() << "QTU_EXCEPTION:" << e.what();
		return 1;
	}catch(const std::exception &e){
		qDebug() << "STD_EXCEPTION:" << e.what();
		return 1;
	}catch(...){
		qDebug() << "FTL_EXCEPTION: FATAL! Unknown error occurred, aborting...";
		return 2;
	}
}
