#ifndef RPG_H
#define RPG_H

#include <QtCore>

#include <Rpg/Global.h>

class Rpg
{
public:
	static const int Infinity = 0;
	static const int AnimationLoopInfinity = -1;

	static constexpr QSizeF ScreenSize = QSizeF(ScreenWidth, ScreenHeight);
	static constexpr QSizeF MapTileSize = QSizeF(MapTileWidth, MapTileHeight);

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

	enum AvatarMirroringMode{
		AvatarNormal = 0,
		AvatarMirror = 1,
	};

	enum ZValue{
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

	enum BlockAlign{
		AlignTopLeft     = Qt::AlignTop | Qt::AlignLeft,
		AlignTop         = Qt::AlignTop | Qt::AlignHCenter,
		AlignTopRight    = Qt::AlignTop | Qt::AlignRight,
		AlignLeft        = Qt::AlignLeft | Qt::AlignVCenter,
		AlignCenter      = Qt::AlignCenter,
		AlignRight       = Qt::AlignRight | Qt::AlignVCenter,
		AlignBottomLeft  = Qt::AlignBottom | Qt::AlignLeft,
		AlignBottom      = Qt::AlignBottom | Qt::AlignHCenter,
		AlignBottomRight = Qt::AlignBottom | Qt::AlignRight,
		AlignAbsolute    = Qt::AlignAbsolute,
	};

	// TextAlign沿用BlockAlign中的AlignLeft, AlignCenter, AlignRight
	// 因为命名可能会一样, 所以索性不新增了
	typedef BlockAlign TextAlign;

	enum AutoTileBlockType{
		None						= 0x00,
		LeftBottomMultiInnerCorner	= 0x02,
		RightBottomMultiInnerCorner	= 0x08,
		MultiAreaBottom				= 0x0A,
		BottomMulti					= 0x0E,
		RightTopMultiInnerCorner	= 0x20,
		MultiAreaSlash				= 0x22,
		MultiAreaRight				= 0x28,
		MultiHubRightBottom			= 0x2A,
		BottomMultiTeeRight			= 0x2E,
		RightMulti					= 0x38,
		RightMultiTeeBottom			= 0x3A,
		RightBottomMultiCorner		= 0x3E,
		LeftTopMultiInnerCorner		= 0x80,
		MultiAreaLeft				= 0x82,
		LeftMulti					= 0x83,
		MultiAreaBackSlash			= 0x88,
		MultiHubLeftBottom			= 0x8A,
		LeftMultiTeeBottom			= 0x8B,
		BottomMultiTeeLeft			= 0x8E,
		LeftBottomMultiCorner		= 0x8F,
		MultiAreaTop				= 0xA0,
		MultiHubLeftTop				= 0xA2,
		LeftMultiTeeTop				= 0xA3,
		MultiHubRightTop			= 0xA8,
		SingleHub					= 0xAA,
		LeftSingleTee				= 0xAB,
		BottomSingleTee				= 0xAE,
		LeftBottomSingleCorner		= 0xAF,
		RightMultiTeeTop			= 0xB8,
		RightSingleTee				= 0xBA,
		VerticalPassedSingleLine	= 0xBB,
		RightBottomSingleCorner		= 0xBE,
		TopPassedSingleLine			= 0xBF,
		TopMulti					= 0xE0,
		TopMultiTeeLeft				= 0xE2,
		LeftTopMultiCorner			= 0xE3,
		TopMultiTeeRight			= 0xE8,
		TopSingleTee				= 0xEA,
		LeftTopSingleCorner			= 0xEB,
		HorizonalPassedSingleLine	= 0xEE,
		RightPassedSingleLine		= 0xEF,
		RightTopMultiCorner			= 0xF8,
		RightTopSingleCorner		= 0xFA,
		BottomPassedSingleLine		= 0xFB,
		LeftPassedSingleLine		= 0xFE,
		ClosedFrame					= 0xFF
	};

	enum AutoTileBlockLocation{
		Block_None        = 0b00000000,
		Block_Center      = Block_None,
		Block_LeftTop     = 0b10000000,
		Block_RightTop    = 0b00100000,
		Block_RightBottom = 0b00001000,
		Block_LeftBottom  = 0b00000010,
		Block_Top         = 0b01000000 | Block_LeftTop  | Block_RightTop,
		Block_Right       = 0b00010000 | Block_RightTop | Block_RightBottom,
		Block_Bottom      = 0b00000100 | Block_LeftBottom | Block_RightBottom,
		Block_Left        = 0b00000001 | Block_LeftTop  | Block_LeftBottom,

		Block_LT = Block_LeftTop,
		Block_T  = Block_Top,
		Block_RT = Block_RightTop,
		Block_R  = Block_Right,
		Block_RB = Block_RightBottom,
		Block_B  = Block_Bottom,
		Block_LB = Block_LeftBottom,
		Block_L  = Block_Left,
	};

	enum SoundEffect{
		SoundEffect_Select = 1,
		SoundEffect_Accept,
		SoundEffect_Banned,
		SoundEffect_Close,
		SoundEffect_Continuous,
	};

};

#endif // RPG_H
