#ifndef RPGNULLPOINTEREXCEPTION_H
#define RPGNULLPOINTEREXCEPTION_H

#include <Rpg/exception/RpgException.h>

class RpgNullPointerException : public RpgException
{
	QString name;
	const void *pointer = nullptr;
public:
	RpgNullPointerException(const QString &name, const void *pointer = 0x00): RpgException("Null pointer found"){
		this->name = name;
		this->pointer = pointer;
	}

	QString toString() const Q_DECL_NOTHROW{
		return QString("%1: %2 = 0x%3").arg(this->info).arg(this->name).arg(long(this->pointer));
	}
};

#endif // RPGNULLPOINTEREXCEPTION_H
