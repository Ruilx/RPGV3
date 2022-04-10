#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>

#define ScreenWidth 640.0
#define ScreenHeight 480.0

#define MapTileWidth 32.0
#define MapTileHeight 32.0

#ifndef QT_DEBUG
#	define QT_DEBUG 1
#endif

#define BackgroundZValue 0
#define MapZValue 1
#define MapFrontZValue 2

#define LyricZValue 10
#define DialogZValue 11
#define FrontZValue 12

#ifndef CODEPATH_SIMPLIFY_FILE_PATH
#	define __CUR_FILE__ __FILE__
#else
#	include <QDir>
#	define __CUR_FILE__ QDir(__FILE__).dirName()
#endif

#ifdef QT_DEBUG
#	ifdef QT_USE_QSTRINGBUILDER
#		define CodePath "[" __FILE__ ":" % QString::number(__LINE__) % "] " % Q_FUNC_INFO % ":"
#	else
#		define CodePath "[" __FILE__ ":" + QString::number(__LINE__) + "] " + Q_FUNC_INFO + ":"
#	endif
#else
#	define CodePath ""
#endif

#ifndef rDebug
#	define rDebug() (qDebug().noquote() << CodePath)
#endif

// 打印Debug信息, 信息之间不加空格分隔, 常用在拼写格式紧凑的打印中
#ifndef rInlineDebug
#	define rInlineDebug() (qDebug().noquote().nospace() << CodePath)
#endif

#ifndef rInfo
#	define rInfo() (qInfo().noquote() << CodePath)
#endif

#ifndef rInlineInfo
#	define rInlineInfo() (qDebug().noquote().nospace() << CodePath)
#endif

#ifndef rWarning
#	define rWarning() (qWarning().noquote() << CodePath)
#endif

#ifndef rInlineWarning
#	define rInlineWarning() (qWarning().noquote().nospace() << CodePath)
#endif

#ifndef rCritical
#	define rCritical() (qCritical().noquote() << CodePath)
#endif

#ifndef rInlineCritical
#	define rInlineCritical() (qCritical().noquote().nospace() << CodePath)
#endif

#ifndef rError
#	define rError() rCritical()
#endif

#ifndef rInlineError
#	define rInlineError() rInlineCritical()
#endif

#ifndef rFatal
#	define rFatal(...) qFatal(...)
#endif

#ifndef Pure
#	define Pure =0
#endif

#ifndef Final
#	ifdef Q_DECL_FINAL
#		define Final Q_DECL_FINAL
#	else
#		define Final final
#	endif
#endif

#ifndef Override
#	ifdef Q_DECL_OVERRIDE
#		define Override Q_DECL_OVERRIDE
#	else
#		define Override override
#	endif
#endif

#ifndef Invokable
#	ifdef Q_INVOKABLE
#		define Invokable Q_INVOKABLE
#	else
#		define Invokable
#	endif
#endif

#ifndef NoThrow
#	ifdef Q_DECL_NOTHROW
#		define NoThrow Q_DECL_NOTHROW
#	else
#		define NoThrow noexcept
#	endif
#endif

#ifndef Unused
#	ifdef Q_UNUSED
#		define Unused(x) Q_UNUSED(x)
#	else
#		define Unused(x) ((void)x);
#	endif
#endif

//#ifndef DebugMessageFormat
//#define DebugMessageFormat
//void debugMessageFormat(QtMsgType type, const QMessageLogContext &context, const QString &msg){

//}
//#endif

//#ifndef RpgInit
//#define RpgInit
//void rpgInit() NoThrow{

//}
//#endif

#endif // GLOBAL_H
