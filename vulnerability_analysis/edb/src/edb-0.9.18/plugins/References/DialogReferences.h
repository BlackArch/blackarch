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

#ifndef DIALOGREFERENCES_20061101_H_
#define DIALOGREFERENCES_20061101_H_

#include <QDialog>
#include "Types.h"

class MemRegion;
class QListWidgetItem;

namespace Ui { class DialogReferences; }

class DialogReferences : public QDialog {
	Q_OBJECT

public:
	DialogReferences(QWidget *parent = 0);
	virtual ~DialogReferences();

public Q_SLOTS:
	void on_btnFind_clicked();
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

Q_SIGNALS:
	void updateProgress(int);

private:
	virtual void showEvent(QShowEvent *event);

private:
	void do_find();

private:
	 Ui::DialogReferences *const ui;
};

#endif
