#-------------------------------------------------
#
# Project created by QtCreator 2018-12-29T15:04:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets qml concurrent multimedia

DEFINES *= QT_USE_QSTRINGBUILDER
TARGET = RPGV3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

unix{
	DESTDIR = $$PWD/bin
	OBJECTS_DIR = $$PWD/tmp
	MOC_DIR = $$PWD/tmp
	RCC_DIR = $$PWD/tmp
	UI_DIR = $$PWD/tmp
}

SOURCES += \
	src/Rpg/com/RpgDialogItem.cpp \
    src/Rpg/core/RpgAutoTileRendererBase.cpp \
	src/Rpg/core/RpgAutoTileRendererSpecLocation.cpp \
	src/Rpg/com/RpgItem/RpgAxisItem.cpp \
	src/Rpg/core/RpgVar.cpp \
    src/main.cpp \
    src/MainWindow.cpp \
    src/Rpg/exception/RpgException.cpp \
    src/Rpg/exception/RpgFileCannotOpenException.cpp \
    src/Rpg/exception/RpgResourceNotFoundException.cpp \
    src/Rpg/exception/RpgNullPointerException.cpp \
    src/Rpg/com/RpgWidget.cpp \
    src/Rpg/core/RpgUtils.cpp \
    src/Rpg/com/RpgScene.cpp \
    src/Rpg/com/RpgScript.cpp \
    src/Rpg/core/RpgFileManager.cpp \
    src/Rpg/exception/RpgMapKeyNotFoundException.cpp \
    src/Rpg/core/RpgMapBlock.cpp \
    src/Rpg/com/RpgMusic.cpp \
    src/Rpg/exception/RpgValueOutOfRangeException.cpp \
    src/Rpg/exception/RpgMusicInvalidMediaException.cpp \
    src/Rpg/com/RpgSound.cpp \
    src/Rpg/core/RpgTileSetBase.cpp \
    src/Rpg/com/RpgLyric.cpp \
    src/Rpg/core/RpgObject.cpp \
    src/Rpg/core/RpgDialogBase.cpp \
    src/Rpg/core/RpgState.cpp \
    src/Rpg/core/RpgSpecLocationChopingDialog.cpp \
    src/Rpg/exception/RpgRuntimeException.cpp \
    src/Rpg/core/RpgPreload.cpp \
    src/Rpg/com/RpgView.cpp \
    src/Rpg/Rpg.cpp \
    src/Rpg/com/RpgDialog.cpp \
    src/Rpg/core/RpgAvatar.cpp \
    src/Rpg/core/RpgFont.cpp \
    src/Rpg/core/RpgDialogMessage.cpp \
    src/Rpg/core/RpgHtmlSplit.cpp \
    src/Rpg/core/RpgDialogAnimation.cpp \
    src/Rpg/core/RpgItemProperties.cpp \
    src/Rpg/com/RpgChoice.cpp \
    src/Rpg/core/RpgChoiceMessage.cpp \
    src/Rpg/core/RpgChoiceAnimation.cpp


HEADERS += \
    src/MainWindow.h \
	src/Rpg/com/RpgDialogItem.h \
	src/Rpg/core/RpgAutoTileRendererBase.h \
	src/Rpg/core/RpgAutoTileRendererSpecLocation.h \
	src/Rpg/com/RpgItem/RpgAxisItem.h \
	src/Rpg/core/RpgVar.h \
    src/Rpg/exception/RpgException.h \
    src/Rpg/exception/RpgFileCannotOpenException.h \
    src/Rpg/exception/RpgResourceNotFoundException.h \
    src/Rpg/exception/RpgNullPointerException.h \
    src/Rpg/com/RpgWidget.h \
    src/Rpg/core/RpgUtils.h \
    src/Rpg/com/RpgScene.h \
    src/Rpg/com/RpgScript.h \
    src/Rpg/core/RpgFileManager.h \
    src/Rpg/exception/RpgMapKeyNotFoundException.h \
    src/Rpg/Global.h \
    src/Rpg/core/RpgMapBlock.h \
    src/Rpg/com/RpgMusic.h \
    src/Rpg/exception/RpgValueOutOfRangeException.h \
    src/Rpg/exception/RpgMusicInvalidMediaException.h \
    src/Rpg/com/RpgSound.h \
    src/Rpg/core/RpgTileSetBase.h \
    src/Rpg/com/RpgLyric.h \
    src/Rpg/core/RpgObject.h \
    src/Rpg/core/RpgDialogBase.h \
    src/Rpg/core/RpgState.h \
    src/Rpg/core/RpgSpecLocationChopingDialog.h \
    src/Rpg/exception/RpgRuntimeException.h \
    src/Rpg/core/RpgPreload.h \
    src/Rpg/com/RpgView.h \
    src/Rpg/Rpg.h \
    src/Rpg/com/RpgDialog.h \
    src/Rpg/core/RpgAvatar.h \
    src/Rpg/core/RpgFont.h \
    src/Rpg/core/RpgDialogMessage.h \
    src/Rpg/core/RpgHtmlSplit.h \
    src/Rpg/core/RpgDialogAnimation.h \
    src/Rpg/core/RpgItemProperties.h \
    src/Rpg/com/RpgChoice.h \
    src/Rpg/core/RpgChoiceMessage.h \
    src/Rpg/core/RpgChoiceAnimation.h


DISTFILES += \
	README.md \
	data/font/A-OTF-FolkPro-Bold.otf \
	data/font/A-OTF-FolkPro-Medium.otf \
	data/font/JF-Dot-MPlus12.ttf \
	data/font/TsangerYunHei-W05.ttf \
	data/font/cinecaption227.ttf \
	data/font/msyh.ttc \
	data/font/msyhbd.ttc \
	data/font/msyhl.ttc \
	data/font/simsun.ttc \
	data/font/unifont-12.1.03.ttf \
	data/font/unifont_jp-12.1.03.ttf \
	data/font/wuxin.ttf \
	data/git/rpgv3_logo.png \
	data/git/rpgv3_logo_x1.png \
	data/images/autotiles/autotileDemo.png \
	data/images/autotiles/test.png \
	data/images/avatar/002.png \
	data/images/avatar/002_head.png \
	data/images/skin/blackAlpha.png \
	data/images/skin/blue.png \
	data/images/skin/blueAlpha.png \
	data/images/skin/redAlpha.png \
	data/images/skin/whiteAlpha.png \
	data/lyrics/TOKYO BON.lrc \
	data/lyrics/TOKYO BON.lrc.bak \
	data/lyrics/Turnin.lrc \
	data/lyrics/Turnin.lrc.bak \
	data/lyrics/p1.lrc \
	data/lyrics/p2.lrc \
	data/lyrics/カンタレラ~grace edition~.lrc \
	data/lyrics/カンタレラ~grace edition~.lrc.bak \
	data/music/TOKYO BON.mp3 \
	data/music/Turnin.mp3 \
	data/music/title.mp3 \
	data/music/title.ogg \
	data/music/ネコ登場.ogg \
	data/music/月光（オルゴール）.mp3 \
	data/music/月光（オルゴール）.ogg \
	data/se/banned.wav \
	data/se/select.wav \
	initialize.json \
	.gitignore \
	rpgvar.ini

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lQtAV1 -lQtAVWidgets1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lQtAVd1 -lQtAVWidgetsd1

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
