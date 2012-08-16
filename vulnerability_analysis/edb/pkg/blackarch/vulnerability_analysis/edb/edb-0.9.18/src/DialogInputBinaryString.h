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

#ifndef DIALOGINPUTBINARYSTRING_20061127_H_
#define DIALOGINPUTBINARYSTRING_20061127_H_

#include <QDialog>

class QLineEdit;
class BinaryString;

namespace Ui { class DialogInputBinaryString; }

class DialogInputBinaryString : public QDialog {
	Q_OBJECT

public:
	DialogInputBinaryString(QWidget *parent = 0);
	virtual ~DialogInputBinaryString();

public:
	BinaryString *binary_string() const;

private:
	Ui::DialogInputBinaryString *const ui;
};

#endif
