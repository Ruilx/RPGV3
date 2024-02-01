#include "MainWindow.h"
#include <Rpg/exception/RpgException.h>
#include <QUnhandledException>
#include <QDebug>
#include <QApplication>

#include <QLocale>
#include <QDateTime>

QDateTime parseBuildDatetime(){
	QStringList dateSplit = QString(__DATE__).split(" ", QString::SkipEmptyParts);
	if(dateSplit.length() < 3){
		return QDateTime(); // Invalid
	}
	if(dateSplit.at(1).length() <= 1){
		dateSplit[1] = "0" + dateSplit[1];
	}
	QString date = dateSplit.join(" ");
	return QLocale::c().toDateTime(date % " " __TIME__, "MMM dd yyyy HH:mm:ss");
}

void initRandomSeed(){
	qsrand(QDateTime::currentSecsSinceEpoch() & UINT_MAX);
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	rDebug() << "Build time:" << parseBuildDatetime().toString("yyyy-MM-dd HH:mm:ss");

	initRandomSeed();

	a.setApplicationDisplayName("RPGV3");
	a.setApplicationVersion("0.0.1");
	//a.setDoubleClickInterval(1000);
	a.setKeyboardInputInterval(1000);


	rDebug() << "Current working dir:" << QDir::currentPath();
	rDebug() << "Library paths:" << a.libraryPaths();

	a.setFont(qApp->font());
	rDebug() << "Application font:" << a.font();

	try{
        MainWindow w;
        w.show();

		return a.exec();
	}catch(const RpgException &e){
		rDebug() << "RPG_EXCEPTION:" << e.toString();
		return 1;
	}catch(const QUnhandledException &e){
		rDebug() << "QTU_EXCEPTION:" << e.what();
		return 1;
	}catch(const std::exception &e){
		rDebug() << "STD_EXCEPTION:" << e.what();
		return 1;
	}catch(...){
		rDebug() << "FTL_EXCEPTION: FATAL! Unknown error occurred, aborting...";
		return 2;
	}
}
