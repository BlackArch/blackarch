/*
Copyright (C) 2006 - 2011 Evan Teran
                          eteran@alum.rit.edu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "BinaryString.h"
#include "Util.h"

#include <QValidator>
#include <QStringList>
#include <QString>

#include <cctype> // for std::isxdigit

#include "ui_binarystring.h"

class HexStringValidator : public QValidator {
public:
	HexStringValidator(QObject * parent) : QValidator(parent) {}

public:
	virtual void fixup(QString &input) const {
		QString temp;
		int index = 0;

		Q_FOREACH(QChar ch, input) {
			const int c = ch.toAscii();
			if(c < 0x80 && std::isxdigit(c)) {

				if(index != 0 && (index & 1) == 0) {
					temp += ' ';
				}

				temp += ch.toUpper();
				++index;
			}
		}

		input = temp;
	}

	virtual State validate(QString &input, int &pos) const {
		if(!input.isEmpty()) {
			// TODO: can we detect if the char which was JUST deleted
			// (if any was deleted) was a space? and special case this?
			// as to not have the minor bug in this case?

			const int char_pos = pos - input.left(pos).count(' ');
			int chars          = 0;
			fixup(input);

			pos = 0;

			while(chars != char_pos) {
				if(input[pos] != ' ') {
					++chars;
				}
				++pos;
			}

			// favor the right side of a space
			if(input[pos] == ' ') {
				++pos;
			}
		}
		return QValidator::Acceptable;
	}
};


//------------------------------------------------------------------------------
// Name: setMaxLength(int n)
// Desc:
//------------------------------------------------------------------------------
void BinaryString::setMaxLength(int n) {
	ui->txtAscii->setMaxLength(n);
	ui->txtUTF16->setMaxLength(n / 2);
	ui->txtHex->setMaxLength(n * 3);
}

//------------------------------------------------------------------------------
// Name: BinaryString(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
BinaryString::BinaryString(QWidget *parent) : QWidget(parent), ui(new Ui::BinaryStringWidget) {
	ui->setupUi(this);
	ui->txtHex->setValidator(new HexStringValidator(this));
}

//------------------------------------------------------------------------------
// Name: ~BinaryString()
// Desc: destructor
//------------------------------------------------------------------------------
BinaryString::~BinaryString() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_txtAscii_textEdited(const QString &text)
// Desc:
//------------------------------------------------------------------------------
void BinaryString::on_txtAscii_textEdited(const QString &text) {

	const QByteArray p = text.toAscii();
	QString textHex;
	QString textUTF16;
	QString temp;
	quint16 utf16Char = 0;

	int counter = 0;

	Q_FOREACH(quint8 ch, p) {

		textHex += temp.sprintf("%02x ", ch & 0xff);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		utf16Char = (utf16Char >> 8) | (ch << 8);
#else
		utf16Char = (utf16Char << 8) | ch;
#endif
		if(counter++ & 1) {
			textUTF16 += QChar(utf16Char);
		}
	}

	ui->txtHex->setText(textHex.simplified());
	ui->txtUTF16->setText(textUTF16);
}

//------------------------------------------------------------------------------
// Name: on_txtUTF16_textEdited(const QString &text)
// Desc:
//------------------------------------------------------------------------------
void BinaryString::on_txtUTF16_textEdited(const QString &text) {

	QString textAscii;
	QString textHex;
	QString temp;

	Q_FOREACH(QChar i, text) {
		const quint16 ch = i.unicode();

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		textAscii += ch & 0xff;
		textAscii += (ch >> 8) & 0xff;
		textHex += temp.sprintf("%02x %02x ", ch & 0xff, (ch >> 8) & 0xff);
#else
		textAscii += (ch >> 8) & 0xff;
		textAscii += ch & 0xff;
		textHex += temp.sprintf("%02x %02x ", (ch >> 8) & 0xff, ch & 0xff);
#endif
	}

	ui->txtHex->setText(textHex.simplified());
	ui->txtAscii->setText(textAscii);
}

//------------------------------------------------------------------------------
// Name: on_txtHex_textEdited(const QString &text)
// Desc:
//------------------------------------------------------------------------------
void BinaryString::on_txtHex_textEdited(const QString &text) {

	quint16 utf16Char = 0;
	int counter = 0;

	QString textAscii;
	QString textUTF16;

	const QStringList list1 = text.split(" ", QString::SkipEmptyParts);

	Q_FOREACH(const QString &s, list1) {

		const quint8 ch = s.toUInt(0, 16);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		utf16Char = (utf16Char >> 8) | (ch << 8);
#else
		utf16Char = (utf16Char << 8) | ch;
#endif

		textAscii += ch;

		if(counter++ & 1) {
			textUTF16 += QChar(utf16Char);
		}
	}

	ui->txtUTF16->setText(textUTF16);
	ui->txtAscii->setText(textAscii);
}

//------------------------------------------------------------------------------
// Name: value() const
// Desc:
//------------------------------------------------------------------------------
QByteArray BinaryString::value() const {

	QByteArray ret;
	const QStringList list1 = ui->txtHex->text().split(" ", QString::SkipEmptyParts);

	Q_FOREACH(const QString &i, list1) {
		ret += static_cast<quint8>(i.toUInt(0, 16));
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: setValue(const QByteArray &value)
// Desc:
//------------------------------------------------------------------------------
void BinaryString::setValue(const QByteArray &data) {

	const QString temp = QString::fromAscii(data.data(), data.size());

	ui->txtAscii->setText(temp);
	on_txtAscii_textEdited(temp);
}
