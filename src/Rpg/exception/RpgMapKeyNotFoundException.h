#ifndef RPGMAPKEYNOTFOUNDEXCEPTION_H
#define RPGMAPKEYNOTFOUNDEXCEPTION_H

#include <Rpg/exception/RpgException.h>
#include <QString>

class RpgMapKeyNotFoundException : public RpgException
{
	QString key;
public:
	RpgMapKeyNotFoundException(const QString &keyName): RpgException("Key not found"){
		this->key = keyName;
	}

	QString toString() const Q_DECL_NOTHROW{
		if(this->key.isEmpty()){
			return this->info;
		}
		return QString("%1: %2").arg(this->info).arg(this->key);
	}
};

#endif // RPGMAPKEYNOTFOUNDEXCEPTION_H
