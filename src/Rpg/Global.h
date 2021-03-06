#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>

#define ScreenWidth 640.0
#define ScreenHeight 480.0

#define MapBlockWidth 32.0
#define MapBlockHeight 32.0

#ifndef QT_DEBUG
#	define QT_DEBUG 1
#endif

#define BackgroundZValue 0
#define MapZValue 1
#define MapFrontZValue 2

#define LyricZValue 10
#define DialogZValue 11
#define FrontZValue 12

#ifdef QT_DEBUG
#	ifdef QT_USE_QSTRINGBUILDER
#		define CodePath "[" __FILE__ ":" % QString::number(__LINE__) % "] " % Q_FUNC_INFO % ":"
#	else
#		define CodePath "[" __FILE__ ":" + QString::number(__LINE__) + "] " + Q_FUNC_INFO + ":"
#	endif
#else
#	define CodePath ""
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

#endif // GLOBAL_H
