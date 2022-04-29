#include "RpgInputItem.h"

#include <QTextDocument>

#include <Rpg/core/RpgFileManager.h>
#include <Rpg/core/RpgFont.h>
#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgHtmlSplit.h>

void RpgInputItem::setFont(const QString &name, int pointSize, int weight, bool italic){
	this->setFont(rpgFont->getFont(name, pointSize, weight, italic));
}

RpgInputItem::RpgInputItem(RpgDialogBase *skin, RpgObject *parent): RpgObject(parent){
	this->setTextColor(Qt::white);

	if(skin == nullptr){
		throw RpgNullPointerException("RpgDialogBase not set.");
	}
	this->skin = static_cast<RpgDialogBase*>(skin);

	this->input->setStyleSheet(Rpg::getDefaultCss());
	QPalette pal = this->input->palette();{
		pal.setColor(QPalette::Background, Qt::transparent);
		pal.setColor(QPalette::Foreground, Qt::white);
		this->input->setPalette(pal);
	}

	this->setFont(FontName, 16);
	rDebug() << "Input using font:" << this->getFont();

	// MessageBox
	this->messageBox->document()->setDefaultStyleSheet(Rpg::getDefaultCss());
	this->messageBox->document()->setUndoRedoEnabled(false);
	QTextOption messageTextOption = this->messageBox->document()->defaultTextOption();{
		messageTextOption.setWrapMode(QTextOption::WrapAnywhere);
		this->messageBox->document()->setDefaultTextOption(messageTextOption);
	}

	// 设置位置和大小
	this->messageBox->setTextWidth(this->dialogSize.width() - (2 * MessageMarginH));
	this->messageBox->setPos(MessageMarginH, MessageMarginV);

	// 阴影设置
	this->textShadowEffect->setColor(Qt::black);
	this->textShadowEffect->setBlurRadius(5.0f);
	this->textShadowEffect->setOffset(2, 2);
	this->messageBox->setGraphicsEffect(this->textShadowEffect);

	this->inputItem->setWidget(this->input);

	this->hide();

	rpgState->registerRpgObject(this, RpgState::DialogMode);
}

RpgInputItem::~RpgInputItem(){
	rpgState->unregisterRpgObject(this, RpgState::DialogMode);
}

void RpgInputItem::run(){

}

int RpgInputItem::waitingForComplete()
{

}

void RpgInputItem::end()
{

}

const QString RpgInputItem::getValue()
{

}

void RpgInputItem::showDialog()
{

}

void RpgInputItem::hideDialog()
{

}

void RpgInputItem::showMessage(){
	if(this->message.isEmpty()){
		return;
	}
	this->showTextInProgressFlag = true;
	this->messageBox->document()->clear();
	// 设定textalign
	QTextOption textOption = this->messageBox->document()->defaultTextOption();{
		if(textOption.alignment() != (Qt::Alignment)this->messageAlign){
			textOption.setAlignment((Qt::Alignment)this->messageAlign);
			this->messageBox->document()->setDefaultTextOption(textOption);
		}
	}

	if(this->speed > 0){
		this->quickShowFlag = false;
		RpgHtmlSplit htmlSplit(this->message);
		while(htmlSplit.hasNext()){
			QString wordLeft = htmlSplit.chopLeft();
			this->messageBox->setHtml(wordLeft);
			// this->setLineHeight();
			RpgUtils::msleep(this->speed);
			if(this->quickShowFlag == true){
				this->messageBox->setHtml(this->message);
				//this->setLineHeight();
				this->quickShowFlag = false;
				break;
			}
		}
	}else{
		this->messageBox->setHtml(this->message);
	}
	this->showTextInProgressFlag = false;
}


