#ifndef RPGFILECANNOTOPENEXCEPTION_H
#define RPGFILECANNOTOPENEXCEPTION_H

#include <Rpg/exception/RpgException.h>
#include <QFile>

class RpgFileCannotOpenException : public RpgException
{
	QString fileName;
	QString fileErrorString;
	int fileError = 0;
public:
	RpgFileCannotOpenException(const QFile &file): RpgException("File cannot open"){
		this->fileName = file.fileName();
		this->fileErrorString = file.errorString();
		this->fileError = int(file.error());
	}

	QString toString() const Q_DECL_NOTHROW{
		return QString("%1: %2: (%3)%4").arg(this->info).arg(this->fileName).arg(this->fileError).arg(this->fileErrorString);
	}
};

#endif // RPGFILECANNOTOPENEXCEPTION_H
