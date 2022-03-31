#ifndef RPGWIDGET_H
#define RPGWIDGET_H

#include <QGraphicsView>
#include <QVBoxLayout>
#include <Rpg/Global.h>
#include <QMouseEvent>

#include <Rpg/core/RpgState.h>
#include <Rpg/core/RpgUtils.h>
#include <Rpg/com/RpgView.h>
#include <Rpg/com/RpgScene.h>

#include <Rpg/com/RpgDialog.h>

#include <Rpg/core/RpgSpecLocationChopingDialog.h>

#include <Rpg/com/RpgMusic.h>
#include <Rpg/com/RpgLyric.h>

#include <Rpg/core/RpgAutoTileRendererSpecLocation.h>
#include <Rpg/core/RpgVar.h>

/**
 * @brief The RpgWidget class
 * RpgWidget组件
 * //是RpgScene展示和输入输出的一个窗口, 用来解决映画展示和交互的组件
 * RpgWidget是展示Rpg组件的基类
 */

class RpgWidget : public QWidget
{
	Q_OBJECT

	int timerId = -1;

	bool _canClose = false;
protected:

//	void mouseDoubleClickEvent(QMouseEvent *event){
//		qDebug() << CodePath << "Mouse pos:" << event->pos();
//	}

//	void mouseMoveEvent(QMouseEvent *event){
//		qDebug() << (event->button());
//	}

//	void keyPressEvent(QKeyEvent *event){
//		int key = event->key();
//		Qt::KeyboardModifiers mod = event->modifiers();
//		qDebug() << CodePath << tr("Receive Key Press ▼: ") + RpgUtils::keysToString((Qt::Key)key, mod);
//		emit this->receiveKeyPressSignal(key, mod);
//		if(RpgView::instance()->scene() != nullptr){
//			RpgState::instance()->receiveKeyPress(key, mod, RpgView::instance()->scene());
//		}
//		event->accept();
//	}

//	void keyReleaseEvent(QKeyEvent *event){
//		int key = event->key();
//		Qt::KeyboardModifiers mod = event->modifiers();
//		qDebug() << CodePath << tr("Receive Key Release ▲: ") + RpgUtils::keysToString((Qt::Key)key, mod);
//		emit this->receiveKeyReleaseSignal(key, mod);
//		if(RpgView::instance()->scene() != nullptr){
//			RpgState::instance()->receiveKeyRelease(key, mod, RpgView::instance()->scene());
//		}
//		event->accept();
//	}

//	void closeEvent(QCloseEvent *){}

public:
	RpgWidget(QWidget *parent = nullptr): QWidget(parent){
		RpgView::instance(this);

		QVBoxLayout *mainLay = new QVBoxLayout;
		this->setLayout(mainLay);
		mainLay->addWidget(RpgView::instance());
		mainLay->setMargin(0);

		//RpgView::instance()->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

		QTimer::singleShot(1000, this, &RpgWidget::ready);
	}

	inline bool canClose() const{
		return this->_canClose;
	}

	inline void setCanClose(bool e){
		this->_canClose = e;
	}

signals:
	//void receiveKeyPressSignal(int key, Qt::KeyboardModifiers mod);
	//void receiveKeyReleaseSignal(int key, Qt::KeyboardModifiers mod);
	void readyToClose();
private slots:
	void ready();
public slots:
	void doReadyToClose(){
		emit this->readyToClose();
	}
};

#endif // RPGWIDGET_H
