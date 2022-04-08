#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Rpg/com/RpgWidget.h>

#include <Rpg/core/RpgFileManager.h>
#include <Rpg/core/RpgPreload.h>

/**
 * @brief The MainWindow class
 * 主界面窗口的建立, 布置页面样式以及启动scene脚本等工作
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

	bool canClose = false;

	void closeEvent(QCloseEvent *event);

	void testModel();

	void setupMenus();

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
};

#endif // MAINWINDOW_H
