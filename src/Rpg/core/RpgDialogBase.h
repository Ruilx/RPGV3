#ifndef RPGDIALOGBASE_H
#define RPGDIALOGBASE_H

#include <QtCore>
#include <QPixmap>
#include <QPair>

#include <Rpg/Global.h>
#include <Rpg/core/RpgFileManager.h>
#include <Rpg/exception/RpgFileCannotOpenException.h>
#include <Rpg/exception/RpgResourceNotFoundException.h>
#include <Rpg/exception/RpgValueOutOfRangeException.h>

/**
 * @brief The RpgDialogBase class
 * 加载对话框背景类
 *
 * 提供对话框图片
 * 缓存指定图片大小 处理图片的基类
 */
class RpgDialogBase
{
public:
	enum DialogSkinChopingMode{
		SpecLocationChopingMode = 0, // RPGツクール模式: 图像拼接剪裁
		RpgMakerMode = SpecLocationChopingMode,
		SimplePixmapMode = 1, // Wolf模式: 直接显示图像
		WolfSimpleMode = SimplePixmapMode,
	};

public:
	static const int MarginH = 10;
	static const int MarginV = 10;
//	static constexpr QSize dialogSize = QSize(ScreenWidth - MarginH - MarginH, 125);
//	static constexpr QSize maxDialogSize = QSize(ScreenWidth - MarginH - MarginH, ScreenHeight - MarginV - MarginV);

	static inline const QSize dialogSize(){ return QSize(ScreenWidth - (MarginH << 1), 125); }
	static inline const QSize maxDialogSize(){ return QSize(ScreenWidth - (MarginH << 1), ScreenHeight - (MarginV << 1)); }

	static const int PaddingH = 10;
	static const int PaddingV = 5;
//	static constexpr QSize selectBarSize = QSize(ScreenWidth - MarginH - MarginH - PaddingH - PaddingH, 32);

	static inline const QSize selectBarSize(){ return QSize(ScreenWidth - (MarginH << 1) - (PaddingH << 1), 32); }

	static const int MinDialogWidth = 125;
	static const int MinDialogHeight = 125;

protected:
	QString dialogSkinName;

	/**
	 * @brief base
	 * 从名字中读出的图像原图
	 */
	QPixmap base;

	DialogSkinChopingMode chopingMode;

	/**
	 * @brief dialogBackground
	 * 所缓存的背景内容
	 */
	QHash<QPair<int, int>, QPixmap> dialogBackground;

	/**
	 * @brief selectBarBackground
	 * 所缓存的选择框背景
	 */
	QHash<QPair<int, int>, QPixmap> selectBarBackground;

	/**
	 * @brief continueSymbolList
	 * 所记录的继续按钮列表
	 */
	QList<QPixmap> continueSymbolList;

	/**
	 * @brief downArrowSymbolList
	 * 所记录的下箭头动画列表
	 */
	QList<QPixmap> downArrowSymbolList;

	/**
	 * @brief upArrowSymbolList
	 * 所记录的上箭头动画列表
	 */
	QList<QPixmap> upArrowSymbolList;

//	virtual void renderDialog(const QSize &dialogSize) NoThrow{ Unused(dialogSize) }
//	virtual void renderSelectBar(const QSize &selectBarSize) NoThrow{ Unused(selectBarSize) }
//	virtual void renderContinueSymbols() NoThrow{}

	virtual void renderDialog(const QSize &dialogSize) NoThrow Pure;
	virtual void renderSelectBar(const QSize &selectBarSize) NoThrow Pure;
	virtual void renderContinueSymbols() NoThrow Pure;
	virtual void renderUpArrowSymbols() NoThrow Pure;
	virtual void renderDownArrowSymbols() NoThrow Pure;

	static const QPair<int, int> generateQSizePair(const QSize &size){
		return QPair<int, int>(size.width(), size.height());
	}

private:
	QPixmap invalidPixmap;

public:

	/**
	 * @brief getDialogImage
	 * @param dialogSize
	 * @return
	 * 获得窗口背景(缓存)
	 */
	const QPixmap &getDialogImage(const QSize &dialogSize);

	/**
	 * @brief getSelectBarImage
	 * @param selectBarSize
	 * @return
	 * 获得选择框(缓存)
	 */
	const QPixmap &getSelectBarImage(const QSize &selectBarSize);

	/**
	 * @brief getContinueSymbolImage
	 * @param index
	 * @return
	 * 获得继续图标
	 */
	const QPixmap &getContinueSymbolImage(int index) const;

	Q_DECL_DEPRECATED inline int getContinueSymbolImageLength() const{ return this->getContinueSymbolImageCount(); }
	inline int getContinueSymbolImageCount() const{ return this->continueSymbolList.length(); }

	const QPixmap &getUpArrowSymbolImage(int index) const;
	inline int getUpArrowSymbolImageCount() const{ return this->upArrowSymbolList.length(); }

	const QPixmap &getDownArrowSymbolImage(int index) const;
	inline int getDownArrowSymbolImageCount() const{ return this->downArrowSymbolList.length(); }

	explicit RpgDialogBase(const QString &dialogSkinName);

	~RpgDialogBase(){}

	void setDialogSkinName(const QString &dialogSkinName);
	inline QString getDialogSkinName() const{ return this->dialogSkinName; }
	inline DialogSkinChopingMode getChopingMode() const{ return this->chopingMode; }
};

#endif // RPGDIALOGBASE_H
