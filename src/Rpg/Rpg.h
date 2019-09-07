#ifndef RPG_H
#define RPG_H

#include <QtCore>

#include <Rpg/Global.h>

class Rpg
{
public:
	static const int Infinity = 0;

	enum Speed{
		SpeedSlow = 500,
		SpeedNormal = 250,
		SpeedFast = 150,
		SpeedInfinity = 0,
	};

	enum SingleWordSpeed{
		SingleWordSpeedSlow = 55,
		SingleWordSpeedNormal = 30,
		SingleWordSpeedFast = 17,
		SingleWordSpeedInfinity = 0,
	};

	enum ZValues{
		ZValueBackground = BackgroundZValue,
		ZValueMap = MapZValue,
		ZValueMapFront = MapFrontZValue,
		ZValueLyric = LyricZValue,
		ZValueDialog = DialogZValue,
		ZValueFront = FrontZValue,
	};

	static inline const QString getDefaultCss(){
		//static const QString defaultCss = "r{color:#FF3333;}g{color:#33FF33;}b{color:#9999FF; font-weight: normal;}y{color:#EEEE33;}m{color: #BB00BB;}c{color: #00CDCD}";
		return "x{color:#000000;}r{color:#BB0000;}g{color:#006400;}b{color:#1E90F5; font-weight: normal;}y{color:#C8AF00;}m{color:#BB00BB;}c{color:#00CDCD;}w{color:#EBEBEB;}"
			   "xl{color:#555555;}rl{color:#FF5555;}gl{color:#55FF55;}bl{color:#50BEFF;}yl{color:#FFF555;}ml{color:#FF55FF;}cl{color:#55FFFF;}wl{color:#FFFFFF;}";
	}

	enum AvatarAround{
		AvatarAroundUnknown = 0,
		AvatarAroundLeft = 1,
		AvatarAroundRight = 2,
	};

	enum AvatarMode{
		AvatarModeUnknown = 0,
		AvatarHalfBodyFront = 1, // 主角半身立绘在框后面
		AvatarHalfBodyBehind = 2, // 主角半身立绘在框前面
		AvatarFrame = 3 // 主角头像(框前面)
	};

	enum DialogAlign{
		AlignTopLeft = 0,
		AlignTop = 1,
		AlignTopRight = 2,
		AlignLeft = 3,
		AlignCenter = 4,
		AlignRight = 5,
		AlignBottomLeft = 6,
		AlignBottom = 7,
		AlignBottomRight = 8,
	};

};

#endif // RPG_H
