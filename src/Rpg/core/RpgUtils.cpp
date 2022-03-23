#include "RpgUtils.h"

#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QGraphicsItem>

#include <Rpg/exception/RpgFileCannotOpenException.h>

void RpgUtils::msleep(int msec){
	QEventLoop eventLoop;
	QTimer::singleShot(msec, &eventLoop, SLOT(quit()));
	eventLoop.exec();
}

const QByteArray RpgUtils::readFile(const QString &filename, int length){
	QFile f(filename);
	if(!f.open(QIODevice::ReadOnly)){
		throw RpgFileCannotOpenException(f);
	}
	QByteArray data = f.read(length);
	f.close();
	return data;
}

const QString RpgUtils::detectJsonValue(const QJsonValue &value){
	if(value.isNull()){
		return "Null";
	}else if(value.isArray()){
		return "Array";
	}else if(value.isObject()){
		return "Object";
	}else if(value.isBool()){
		return "Bool";
	}else if(value.isDouble()){
		return "Double";
	}else if(value.isString()){
		return "String";
	}else if(value.isUndefined()){
		return "Undefined";
	}else{
		return "Unknown";
	}
}

const QString RpgUtils::detectJsonValue(const QJsonDocument &value){
	if(value.isNull()){
		return "Null";
	}else if(value.isEmpty()){
		return "Empty";
	}else if(value.isArray()){
		return "Array";
	}else if(value.isObject()){
		return "Object";
	}else{
		return "Unknown";
	}
}

bool RpgUtils::iskeyModifierSameToKey(Qt::KeyboardModifiers mod, Qt::Key key){
	switch(key){
		case Qt::Key_Control: return bool(mod & Qt::ControlModifier);
		case Qt::Key_Shift: return bool(mod & Qt::ShiftModifier);
		case Qt::Key_Meta: return bool(mod & Qt::MetaModifier);
		case Qt::Key_Alt: return bool(mod & Qt::AltModifier);
		default: return false;
	}
}

const QString RpgUtils::keyModifierToString(Qt::KeyboardModifiers mod){
	QStringList modList;
	if(mod & Qt::ControlModifier){
#ifdef Q_OS_MACOS
		modlist.append("Command");
#else
		modList.append("Ctrl");
#endif
	}
	if(mod & Qt::MetaModifier){
#ifdef Q_OS_MACOS
		modlist.append("Control");
#else
		modList.append("Meta");
#endif
	}
	if(mod & Qt::AltModifier){
		modList.append("Alt");
	}
	if(mod & Qt::ShiftModifier){
		modList.append("Shift");
	}
	return modList.join(" + ");
}

const QString RpgUtils::keyToString(Qt::Key key){
	switch(key){
		case Qt::Key_Escape: return "Esc";
		case Qt::Key_Tab: return "Tab";
		case Qt::Key_Backtab: return "Backtab";
		case Qt::Key_Backspace: return "Backspace";
		case Qt::Key_Return: return "Enter";
		case Qt::Key_Enter: return "Enter(KP)";
		case Qt::Key_Insert: return "Insert";
		case Qt::Key_Delete: return "Delete";
		case Qt::Key_Pause: return "Pause";
		case Qt::Key_Print: return "Print";
		case Qt::Key_SysReq: return "SysReq";
		case Qt::Key_Clear: return "Clear";
		case Qt::Key_Home: return "Home";
		case Qt::Key_End: return "End";
		case Qt::Key_Left: return "←";
		case Qt::Key_Right: return "→";
		case Qt::Key_Up: return "↑";
		case Qt::Key_Down: return "↓";
		case Qt::Key_PageUp: return "Page Up";
		case Qt::Key_PageDown: return "Page Down";
		case Qt::Key_Shift: return "Shift";
		case Qt::Key_Control: return "Ctrl";
		case Qt::Key_Meta: return "Meta";
		case Qt::Key_Alt: return "Alt";
		case Qt::Key_AltGr: return "AltGr";
		case Qt::Key_CapsLock: return "Caps Lock";
		case Qt::Key_NumLock: return "Num Lock";
		case Qt::Key_ScrollLock: return "Scroll Lock";
		case Qt::Key_F1: return "F1";
		case Qt::Key_F2: return "F2";
		case Qt::Key_F3: return "F3";
		case Qt::Key_F4: return "F4";
		case Qt::Key_F5: return "F5";
		case Qt::Key_F6: return "F6";
		case Qt::Key_F7: return "F7";
		case Qt::Key_F8: return "F8";
		case Qt::Key_F9: return "F9";
		case Qt::Key_F10: return "F10";
		case Qt::Key_F11: return "F11";
		case Qt::Key_F12: return "F12";
		case Qt::Key_F13: return "F13";
		case Qt::Key_F14: return "F14";
		case Qt::Key_F15: return "F15";
		case Qt::Key_F16: return "F16";
		case Qt::Key_F17: return "F17";
		case Qt::Key_F18: return "F18";
		case Qt::Key_F19: return "F19";
		case Qt::Key_F20: return "F20";
		case Qt::Key_F21: return "F21";
		case Qt::Key_F22: return "F22";
		case Qt::Key_F23: return "F23";
		case Qt::Key_F24: return "F24";
		case Qt::Key_F25: return "F25";
		case Qt::Key_F26: return "F26";
		case Qt::Key_F27: return "F27";
		case Qt::Key_F28: return "F28";
		case Qt::Key_F29: return "F29";
		case Qt::Key_F30: return "F30";
		case Qt::Key_F31: return "F31";
		case Qt::Key_F32: return "F32";
		case Qt::Key_F33: return "F33";
		case Qt::Key_F34: return "F34";
		case Qt::Key_F35: return "F35";
		case Qt::Key_Super_L: return "Super L";
		case Qt::Key_Super_R: return "Super R";
		case Qt::Key_Menu: return "Menu";
		case Qt::Key_Hyper_L: return "Hyper L";
		case Qt::Key_Hyper_R: return "Hyper R";
		case Qt::Key_Help: return "Help";
		case Qt::Key_Direction_L: return "Key Direction L";
		case Qt::Key_Direction_R: return "Key Direction R";
		case Qt::Key_Space: return "<space>";
		case Qt::Key_Exclam ... Qt::Key_AsciiTilde: return QString(QChar(key));
		case Qt::Key_nobreakspace: return "<nbsp>";
		case Qt::Key_exclamdown: return "¡";
		case Qt::Key_cent: return "￠";
		case Qt::Key_sterling: return "£";
		case Qt::Key_currency: return "¤";
		case Qt::Key_yen: return "¥";
			/*_*/
		case Qt::Key_Back: return "Back";
		case Qt::Key_Forward: return "Forward";
		case Qt::Key_Stop: return "Stop";
		case Qt::Key_Refresh: return "Refresh";
		case Qt::Key_VolumeDown: return "Volume Down";
		case Qt::Key_VolumeMute: return "Volume Mute";
		case Qt::Key_VolumeUp: return "Volume Up";
		case Qt::Key_BassBoost: return "Bass Boost";
		case Qt::Key_BassUp: return "Bass Up";
		case Qt::Key_BassDown: return "Bass Down";
		case Qt::Key_TrebleUp: return "Treble Up";
		case Qt::Key_TrebleDown: return "Treble Down";
		case Qt::Key_MediaPlay: return "Media Play";
		case Qt::Key_MediaStop: return "Media Stop";
		case Qt::Key_MediaPrevious: return "Media Previous";
		case Qt::Key_MediaNext: return "Media Next";
		case Qt::Key_MediaRecord: return "Media Record";
		case Qt::Key_MediaPause: return "Media Pause";
		case Qt::Key_MediaTogglePlayPause: return "Media Toggle Play Pause";
		case Qt::Key_HomePage: return "Home Page";
		case Qt::Key_Favorites: return "Favorites";
		case Qt::Key_Search: return "Search";
		case Qt::Key_Standby: return "Stand By";
		case Qt::Key_OpenUrl: return "Open Url";
		case Qt::Key_LaunchMail: return "Launch Mail";
		case Qt::Key_LaunchMedia: return "Launch Media";
		case Qt::Key_Launch0: return "Launch0";
		case Qt::Key_Launch1: return "Launch1";
		case Qt::Key_Launch2: return "Launch2";
		case Qt::Key_Launch3: return "Launch3";
		case Qt::Key_Launch4: return "Launch4";
		case Qt::Key_Launch5: return "Launch5";
		case Qt::Key_Launch6: return "Launch6";
		case Qt::Key_Launch7: return "Launch7";
		case Qt::Key_Launch8: return "Launch8";
		case Qt::Key_Launch9: return "Launch9";
		case Qt::Key_LaunchA: return "LaunchA";
		case Qt::Key_LaunchB: return "LaunchB";
		case Qt::Key_LaunchC: return "LaunchC";
		case Qt::Key_LaunchD: return "LaunchD";
		case Qt::Key_LaunchE: return "LaunchE";
		case Qt::Key_LaunchF: return "LaunchF";
		case Qt::Key_LaunchG: return "LaunchG";
		case Qt::Key_LaunchH: return "LaunchH";
		default: return EnumKeyToString(key);
	}
}

const QString RpgUtils::keysToString(Qt::Key key, Qt::KeyboardModifiers mod){
	if(mod != Qt::NoModifier){
		if(RpgUtils::iskeyModifierSameToKey(mod, key)){
			return RpgUtils::keyModifierToString(mod);
		}
		return RpgUtils::keyModifierToString(mod) + " + " + RpgUtils::keyToString(key);
	}else{
		return RpgUtils::keyToString(key);
	}
}

//QPointF RpgUtils::getDialogPos(Rpg::BlockAlign align, const QSize &dialogSize, int marginH, int marginV){
//	switch(align){
//		case Rpg::AlignTopLeft:
//			return QPointF(marginH, marginV);
//		case Rpg::AlignTop:
//			return QPointF((ScreenWidth - dialogSize.width()) / 2.0f, marginV);
//		case Rpg::AlignTopRight:
//			return QPointF(ScreenWidth - dialogSize.width() - marginH, marginV);
//		case Rpg::AlignLeft:
//			return QPointF(marginH, (ScreenHeight - dialogSize.height()) / 2.0f);
//		case Rpg::AlignCenter:
//			return QPointF((ScreenWidth - dialogSize.width()) / 2.0f, (ScreenHeight - dialogSize.height()) / 2.0f);
//		case Rpg::AlignRight:
//			return QPointF(ScreenWidth - dialogSize.width() - marginH, (ScreenHeight - dialogSize.height()) / 2.0f);
//		case Rpg::AlignBottomLeft:
//			return QPointF(marginH, ScreenHeight - dialogSize.height() - marginV);
//		case Rpg::AlignBottom:
//			return QPointF((ScreenWidth - dialogSize.width()) / 2.0f, ScreenHeight - dialogSize.height() - marginV);
//		case Rpg::AlignBottomRight:
//			return QPointF(ScreenWidth - dialogSize.width() - marginH, ScreenHeight - dialogSize.height() - marginV);
//		default:
//			return QPointF(0.0f, 0.0f);
//	}
//}

QPointF RpgUtils::getBlockPos(Rpg::BlockAlign align, const QSizeF &blockSize, const QSizeF &margin, const QSizeF &parentSize){
	switch(align){
		case Rpg::AlignTopLeft:
			return QPointF(margin.width(), margin.height());
		case Rpg::AlignTop:
			return QPointF((parentSize.width() - blockSize.width()) / 2.0f, margin.height());
		case Rpg::AlignTopRight:
			return QPointF(parentSize.width() - blockSize.width() - margin.width(), margin.height());
		case Rpg::AlignLeft:
			return QPointF(margin.width(), (parentSize.height() - blockSize.height()) / 2.0f);
		case Rpg::AlignCenter:
			return QPointF((parentSize.width() - blockSize.width()) / 2.0f, (parentSize.height() - blockSize.height()) / 2.0f);
		case Rpg::AlignRight:
			return QPointF(parentSize.width() - blockSize.width() - margin.width(), (parentSize.height() - blockSize.height()) / 2.0f);
		case Rpg::AlignBottomLeft:
			return QPointF(margin.width(), parentSize.height() - blockSize.height() - margin.height());
		case Rpg::AlignBottom:
			return QPointF((parentSize.width() - blockSize.width()) / 2.0f, parentSize.height() - blockSize.height() - margin.height());
		case Rpg::AlignBottomRight:
			return QPointF(parentSize.width() - blockSize.width() - margin.width(), parentSize.height() - blockSize.height() - margin.height());
		default:
			return QPointF(0.0f, 0.0f);
	}
}

QPointF RpgUtils::getDialogPos(Rpg::BlockAlign align, const QSize &dialogSize, int marginW, int marginH){
	return RpgUtils::getBlockPos(align, dialogSize, QSizeF(marginW, marginH), QSizeF(ScreenWidth, ScreenHeight));
}






















