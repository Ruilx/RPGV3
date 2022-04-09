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

#include <QGraphicsWidget>
/**
 * @brief The RpgWidget class
 * RpgWidget组件
 * //是RpgScene展示和输入输出的一个窗口, 用来解决映画展示和交互的组件
 * RpgWidget是展示Rpg组件的基类
 *
 * @deprecated 已废弃, 之后widget将会作为QGraphicsWidget的基类
 */

class RpgWidget : public QWidget
{
	Q_OBJECT

public:

signals:

private slots:
public slots:
};

#endif // RPGWIDGET_H
