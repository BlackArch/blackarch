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

#ifndef DIALOGENVIRONMENT_20070705_H_
#define DIALOGENVIRONMENT_20070705_H_

#include "Types.h"
#include <QString>
#include <QDialog>

class QStringListModel;
class QSortFilterProxyModel;
class QModelIndex;

namespace Ui { class DialogEnvironment; }

class DialogEnvironment : public QDialog {
	Q_OBJECT

public:
	DialogEnvironment(QWidget *parent = 0);
	virtual ~DialogEnvironment();

public Q_SLOTS:
	void on_listView_doubleClicked(const QModelIndex &index);
	void on_btnRefresh_clicked();

private:
	virtual void showEvent(QShowEvent *event);

private:
	QString libc_name() const;
	void do_find();

private:
	 Ui::DialogEnvironment *const ui;
	 QStringListModel *           model_;
	 QSortFilterProxyModel *      filter_model_;
};

#endif
