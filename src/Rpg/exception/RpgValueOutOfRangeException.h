#ifndef RPGVALUEOUTOFRANGEEXCEPTION_H
#define RPGVALUEOUTOFRANGEEXCEPTION_H

#include <Rpg/exception/RpgException.h>
#include <QString>
#include <QVariant>

class RpgValueOutOfRangeException : public RpgException
{
	QString name;
	QVariant rangeA;
	QVariant rangeB;
	QVariant value;
public:
//	RpgValueOutOfRangeException(const QString &name, const QVariant &value = nullptr, const QVariant &rangeA = nullptr, const QVariant &rangeB = nullptr):  RpgException("Value Out Of Range"){
//		this->name = &name;
//		this->rangeA = &rangeA;
//		this->rangeB = &rangeB;
//		this->value = &value;
//	}

	RpgValueOutOfRangeException(const QString &name, int value = 0, int rangeA = 0, int rangeB = 0): RpgException("Value out of range"){
		this->name = name;
		this->value = QVariant(value);
		this->rangeA = QVariant(rangeA);
		this->rangeB = QVariant(rangeB);
	}

	RpgValueOutOfRangeException(const QString &name, double value = 0, double rangeA = 0, double rangeB = 0): RpgException("Value out of range"){
		this->name = name;
		this->value = QVariant(value);
		this->rangeA = QVariant(rangeA);
		this->rangeB = QVariant(rangeB);
	}

	RpgValueOutOfRangeException(const QString &name, const QString &value, const QString &rangeA, const QString &rangeB): RpgException("Value out of range"){
		this->name = name;
		this->value = QVariant(value);
		this->rangeA = QVariant(rangeA);
		this->rangeB = QVariant(rangeB);
	}

	QString toString() const Q_DECL_NOTHROW{
		QString info = this->info + ": " + this->name + " ";
		if(this->value.isValid()){
			info.append(this->value.toString());
		}
		if(this->rangeA.isValid() && this->rangeB.isValid()){
			info.append(QString(" but not in range: (%1, %2)").arg(this->rangeA.toString()).arg(this->rangeB.toString()));
		}else if(this->rangeA.isValid()){
			info.append(QString(" but not smaller than: %1").arg(this->rangeA.toString()));
		}else if(this->rangeB.isValid()){
			info.append(QString(" but not bigger than: %1").arg(this->rangeB.toString()));
		}else{
			info.append(" out of range.");
		}
		return info;
	}
};

#endif // RPGVALUEOUTOFRANGEEXCEPTION_H
