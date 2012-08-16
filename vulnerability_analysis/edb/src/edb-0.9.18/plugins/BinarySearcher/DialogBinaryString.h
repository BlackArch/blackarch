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

#ifndef DIALOGBINARYSTRING_20061101_H_
#define DIALOGBINARYSTRING_20061101_H_

#include <QDialog>

class QListWidgetItem;

namespace Ui { class DialogBinaryString; }

class DialogBinaryString : public QDialog {
	Q_OBJECT

public:
	DialogBinaryString(QWidget *parent = 0);
	virtual ~DialogBinaryString();

public Q_SLOTS:
	void on_btnFind_clicked();
	void on_listWidget_itemDoubleClicked(QListWidgetItem *);

private:
	void do_find();

private:
	 Ui::DialogBinaryString *const ui;
};

#endif
