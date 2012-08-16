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

#ifndef DIALOGSPECIFIEDFUNCTIONS_20070705_H_
#define DIALOGSPECIFIEDFUNCTIONS_20070705_H_


#include "Types.h"
#include <QDialog>

class QStringListModel;
class QSortFilterProxyModel;
class QModelIndex;

namespace Ui { class DialogSpecified; }

class DialogSpecifiedFunctions : public QDialog {
	Q_OBJECT

public:
	DialogSpecifiedFunctions(QWidget *parent = 0);
	virtual ~DialogSpecifiedFunctions();

public Q_SLOTS:
	void on_function_list_doubleClicked(const QModelIndex &index);
	void on_refresh_button_clicked();

private:
	virtual void showEvent(QShowEvent *event);

private:
	void do_find();

private:
	 Ui::DialogSpecified *const ui;
	 QStringListModel *         model_;
	 QSortFilterProxyModel *    filter_model_;
};

#endif
