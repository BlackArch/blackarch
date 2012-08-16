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

#ifndef DIALOGINPUTVALUE_20061101_H_
#define DIALOGINPUTVALUE_20061101_H_

#include <QDialog>
#include "Types.h"

namespace Ui { class DialogInputValue; }

class DialogInputValue : public QDialog {
	Q_OBJECT

public:
	DialogInputValue(QWidget *parent = 0);
	virtual ~DialogInputValue();

public Q_SLOTS:
	void on_hexInput_textEdited(const QString &);
	void on_signedInput_textEdited(const QString &);
	void on_unsignedInput_textEdited(const QString &);

public:
	edb::reg_t value() const;
	void set_value(edb::reg_t value);

private:
	Ui::DialogInputValue *const ui;
};

#endif
