#include "RpgDialogBase.h"



const QPixmap &RpgDialogBase::getDialogImage(const QSize &dialogSize){
	QPair<qreal, qreal> dialogSizePair = RpgDialogBase::generateQSizePair(dialogSize);
	if(!this->dialogBackground.contains(dialogSizePair)){
		this->renderDialog(dialogSize);
	}
	return this->dialogBackground.value(dialogSizePair);
}

const QPixmap &RpgDialogBase::getSelectBarImage(const QSize &selectBarSize){
	QPair<qreal, qreal> selectBarSizePair = RpgDialogBase::generateQSizePair(selectBarSize);
	if(!this->selectBarBackground.contains(selectBarSizePair)){
		this->renderSelectBar(selectBarSize);
	}
	return this->selectBarBackground.value(selectBarSizePair);
}

const QPixmap &RpgDialogBase::getContinueSymbolImage(int index) const{
	if(index < 0 || index >= this->continueSymbolList.length()){
		rWarning() << "Index out of range:" << index;
		return this->invalidPixmap;
	}
	return this->continueSymbolList.at(index);
}

const QPixmap &RpgDialogBase::getUpArrowSymbolImage(int index) const{
	if(index < 0 || index >= this->upArrowSymbolList.length()){
		rWarning() << "Index out of range:" << index;
		return this->invalidPixmap;
	}
	return this->upArrowSymbolList.at(index);
}

const QPixmap &RpgDialogBase::getDownArrowSymbolImage(int index) const{
	if(index < 0 || index >= this->downArrowSymbolList.length()){
		rWarning() << "Index out of range:" << index;
		return this->invalidPixmap;
	}
	return this->downArrowSymbolList.at(index);
}

RpgDialogBase::RpgDialogBase(const QString &dialogSkinName){
	this->setDialogSkinName(dialogSkinName);
}

void RpgDialogBase::setDialogSkinName(const QString &dialogSkinName){
	this->dialogSkinName = dialogSkinName;
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::ImageFile, dialogSkinName);
	if(filename.isEmpty()){
		throw RpgResourceNotFoundException(filename);
	}
	this->base = QPixmap(filename);
	if(this->base.isNull()){
		rError() << "Cannot load skin file:" << filename;
		throw RpgFileCannotOpenException(filename);
	}
}
