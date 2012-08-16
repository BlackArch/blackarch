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

#ifndef DIALOGASCIISTRING_20082201_H_
#define DIALOGASCIISTRING_20082201_H_

#include <QDialog>

class QListWidgetItem;

namespace Ui { class DialogASCIIString; }

class DialogASCIIString : public QDialog {
	Q_OBJECT

public:
	DialogASCIIString(QWidget *parent = 0);
	virtual ~DialogASCIIString();

public Q_SLOTS:
	void on_btnFind_clicked();
	void on_listWidget_itemDoubleClicked(QListWidgetItem *);

private:
	void do_find();

private:
	 Ui::DialogASCIIString *const ui;
};

#endif
