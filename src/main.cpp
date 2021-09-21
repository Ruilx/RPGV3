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

	qDebug() << CodePath << "Build time:" << parseBuildDatetime().toString("yyyy-MM-dd HH:mm:ss");

	initRandomSeed();

	a.setApplicationDisplayName("RPGV3");
	a.setApplicationVersion("0.0.1");
	//a.setDoubleClickInterval(1000);
	a.setKeyboardInputInterval(1000);


	qDebug() << CodePath << "Current working dir:" << QDir::currentPath();
	qDebug() << CodePath << "Library paths:" << a.libraryPaths();

	a.setFont(qApp->font());
	qDebug() << CodePath << "Application font:" << a.font();

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
