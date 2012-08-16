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

#ifndef DIALOGFUNCTIONS_20061101_H_
#define DIALOGFUNCTIONS_20061101_H_

#include "Types.h"
#include <QDialog>

class QSortFilterProxyModel;
class AnalyzerInterface;

namespace Ui { class DialogFunctions; }

class DialogFunctions : public QDialog {
	Q_OBJECT

public:
	DialogFunctions(QWidget *parent = 0);
	virtual ~DialogFunctions();

public Q_SLOTS:
	void on_btnFind_clicked();
	void on_tableWidget_cellDoubleClicked (int row, int column);

private:
	virtual void showEvent(QShowEvent *event);

private:
	void do_find();

private:
	Ui::DialogFunctions *const ui;
	QSortFilterProxyModel *    filter_model_;
};

#endif
