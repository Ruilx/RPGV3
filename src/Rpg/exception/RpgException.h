#ifndef RPGEXCEPTION_H
#define RPGEXCEPTION_H

#include <QException>

class RpgException : public QException{
protected:
	QString info;
public:
	RpgException(const QString &info): QException (){
		this->setInfo(info);
	}

	void setInfo(const QString &info) Q_DECL_NOTHROW{
		this->info = info;
	}

	QString getInfo() const Q_DECL_NOTHROW{
		return this->info;
	}

	virtual QString toString() const Q_DECL_NOTHROW{
		return this->info;
	}

	void raise() const{
		throw *this;
	}
};

#endif // RPGEXCEPTION_H
