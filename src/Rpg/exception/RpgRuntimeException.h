#ifndef RPGRUNTIMEEXCEPTION_H
#define RPGRUNTIMEEXCEPTION_H

#include <Rpg/exception/RpgException.h>
#include <QString>

class RpgRuntimeException : public RpgException
{
	QString message;
public:
	RpgRuntimeException(const QString &message): RpgException("Runtime error"){
		this->message = message;
	}

	QString toString() const Q_DECL_NOTHROW{
		if(this->message.isEmpty()){
			return this->info;
		}
		return this->info % ": " % this->message;
	}
};

#endif // RPGRUNTIMEEXCEPTION_H
