#include "RpgHtmlSplit.h"

RpgHtmlSplit::RpgHtmlSplit(const QString &text){
	this->text = text;
	this->iter = text.constBegin();
}

bool RpgHtmlSplit::hasNext() const {
	//		qDebug() << this->iter << ":" << *this->iter << ":" << QChar(*this->iter).unicode();
	return this->iter != this->text.constEnd();
}

const QString RpgHtmlSplit::getOne(){
	QString one;
	bool findTagRear = false;
	if(!this->hasNext()){
		rDebug() << "Touched getOne no next.";
		return QString();
	}
	one.append(*this->iter);
	if(*this->iter == '<'){
		findTagRear = true;
	}
	this->iter++;
	while(this->hasNext() && findTagRear){
		one.append(*this->iter);
		if(*this->iter == '>' && findTagRear){
			findTagRear = false;
		}
		iter++;
	}
	return one;
}

const QString RpgHtmlSplit::chopLeft(){
	if(!this->hasNext()){
		rDebug() << "Touched chopLeft no next.";
		return this->left;
	}else{
		const QString one = this->getOne();
		this->left.append(one);
		if(one.startsWith("<") && one.endsWith(">")){
			if(one.startsWith("</")){
				QString currentCloseTag = QString(this->tags.top()).insert(1, '/');
				if(currentCloseTag == one){
					this->tags.pop();
				}else{
					rDebug() << CodePath << "Code tag is not legal:" << one;
					int fixTagsLoc = this->tags.lastIndexOf(one);
					if(fixTagsLoc != -1){
						rDebug() << CodePath << "Auto fix the tag:" << one;
						this->tags.removeAt(fixTagsLoc);
					}
				}
			}else{
				this->tags.push(one);
			}
		}
		if(!this->tags.isEmpty()){
			const QString currentCloseTag = QString(this->tags.top()).insert(1, '/');
			return this->left + currentCloseTag;
		}else{
			return this->left;
		}
	}
}
