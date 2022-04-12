#ifndef RPGHTMLSPLIT_H
#define RPGHTMLSPLIT_H

#include <QString>
#include <QStack>

#include <Rpg/Global.h>

/**
 * @brief The RpgHtmlSplit class
 * RpgHtmlSplit 对HTML文本左至右每个文字切分, 并返回每次切分的结果, 用来达到字符缓出的效果
 * 对于HTML标签, RpgHtmlSplit会将HTML标签在一个阶段一起输出
 * @example
 *     HTML: This is a <b>BOLD</b> text
 *        0: T
 *        1: Th
 *      ...
 *        9: This is a
 *       10: This is a <b>
 *       11: This is a <b>B
 *      ...
 *       14: This is a <b>BOLD
 *       15: This is a <b>BOLD</b>
 *      ...
 *       20: This is a <b>BOLD</b> text
 *
 *    RpgHtmlSplit rpgHtmlSplit(html);
 *    while(rpgHtmlSplit.hasNext()){
 *        QString left = rpgHtmlSplit.chopLeft();
 *        qDebug() << left;
 *    }
 */
class RpgHtmlSplit
{
	QString text;
	QString::ConstIterator iter;

	QStack<QString> tags;
	QString left;
public:
	explicit RpgHtmlSplit(const QString &text);

	bool hasNext() const;

	const QString getOne();
	const QString chopLeft();
};

#endif // RPGHTMLSPLIT_H
