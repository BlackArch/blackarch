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

#include "DialogInputValue.h"
#include "QULongValidator.h"
#include "QLongValidator.h"
#include "Debugger.h"

#include <QRegExpValidator>

#include "ui_dialog_inputvalue.h"

#if defined(EDB_X86)
	#define SNUM_FMT "%d"
	#define UNUM_FMT "%u"
#elif defined(EDB_X86_64)
	#define SNUM_FMT "%lld"
	#define UNUM_FMT "%llu"
#endif

//------------------------------------------------------------------------------
// Name: DialogInputValue(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogInputValue::DialogInputValue(QWidget *parent) : QDialog(parent), ui(new Ui::DialogInputValue) {
	ui->setupUi(this);

	const QString regex = QString("[A-Fa-f0-9]{0,%1}").arg(edb::v1::pointer_size() * 2);

	ui->hexInput->setValidator(new QRegExpValidator(QRegExp(regex), this));
	ui->signedInput->setValidator(new QLongValidator(LONG_MIN, LONG_MAX, this));
	ui->unsignedInput->setValidator(new QULongValidator(0, ULONG_MAX, this));
}

//------------------------------------------------------------------------------
// Name: ~DialogInputValue()
// Desc:
//------------------------------------------------------------------------------
DialogInputValue::~DialogInputValue() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: value() const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t DialogInputValue::value() const {
	bool ok;
	return ui->hexInput->text().toULongLong(&ok, 16);
}

//------------------------------------------------------------------------------
// Name: set_value(edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void DialogInputValue::set_value(edb::reg_t value) {
	QString temp;
	ui->hexInput->setText(edb::v1::format_pointer(value));
	ui->signedInput->setText(temp.sprintf(SNUM_FMT, value));
	ui->unsignedInput->setText(temp.sprintf(UNUM_FMT, value));
}

//------------------------------------------------------------------------------
// Name: on_hexInput_textEdited(const QString &s)
// Desc:
//------------------------------------------------------------------------------
void DialogInputValue::on_hexInput_textEdited(const QString &s) {
	bool ok;
	edb::reg_t value = s.toULongLong(&ok, 16);

	if(!ok) {
		value = 0;
	}

	QString temp;
	ui->signedInput->setText(temp.sprintf(SNUM_FMT, value));
	ui->unsignedInput->setText(temp.sprintf(UNUM_FMT, value));

}

//------------------------------------------------------------------------------
// Name: on_signedInput_textEdited(const QString &s)
// Desc:
//------------------------------------------------------------------------------
void DialogInputValue::on_signedInput_textEdited(const QString &s) {
	bool ok;
	edb::reg_t value = s.toLongLong(&ok, 10);

	if(!ok) {
		value = 0;
	}

	QString temp;
	ui->hexInput->setText(edb::v1::format_pointer(value));
	ui->unsignedInput->setText(temp.sprintf(UNUM_FMT, value));
}

//------------------------------------------------------------------------------
// Name: on_unsignedInput_textEdited(const QString &s)
// Desc:
//------------------------------------------------------------------------------
void DialogInputValue::on_unsignedInput_textEdited(const QString &s) {
	bool ok;
	edb::reg_t value = s.toULongLong(&ok, 10);

	if(!ok) {
		value = 0;
	}
	QString temp;
	ui->hexInput->setText(edb::v1::format_pointer(value));
	ui->signedInput->setText(temp.sprintf(SNUM_FMT, value));
}
