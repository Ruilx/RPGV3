#ifndef RPGMUSICINVALIDMEDIAEXCEPTION_H
#define RPGMUSICINVALIDMEDIAEXCEPTION_H

#include <Rpg/exception/RpgException.h>

class RpgMusicInvalidMediaException : public RpgException
{
	QString filename;
public:
	RpgMusicInvalidMediaException(const QString &filename): RpgException("Invalid media"){
		this->filename = filename;
	}

	QString toString() const Q_DECL_NOTHROW{
		if(this->filename.isEmpty()){
			return QString("%1: %2").arg(this->info).arg(this->filename);
		}else{
			return QString("[No more info]: %1").arg(this->filename);
		}

	}
};

#endif // RPGMUSICINVALIDMEDIAEXCEPTION_H
