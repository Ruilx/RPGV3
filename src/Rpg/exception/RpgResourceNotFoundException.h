#ifndef RPGRESOURCENOTFOUNDEXCEPTION_H
#define RPGRESOURCENOTFOUNDEXCEPTION_H

#include <Rpg/exception/RpgException.h>

class RpgResourceNotFoundException : public RpgException
{
	QString key;
public:
	RpgResourceNotFoundException(const QString &resourceName): RpgException("Resource not found"){
		this->key = resourceName;
	}

	QString toString() const Q_DECL_NOTHROW{
		return QString("%1: %2").arg(this->info).arg(this->key);
	}
};

#endif // RPGRESOURCENOTFOUNDEXCEPTION_H
