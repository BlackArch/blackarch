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

#ifndef DIALOGSYMBOLVIEWER_20080812_H_
#define DIALOGSYMBOLVIEWER_20080812_H_

#include <QDialog>
#include "Types.h"

class QModelIndex;
class QPoint;
class QSortFilterProxyModel;
class QStringListModel;

namespace Ui { class DialogSymbolViewer; }

class DialogSymbolViewer : public QDialog {
	Q_OBJECT

public:
	DialogSymbolViewer(QWidget *parent = 0);
	virtual ~DialogSymbolViewer();

public Q_SLOTS:
	void on_listView_doubleClicked(const QModelIndex &index);
	void on_listView_customContextMenuRequested(const QPoint &pos);
	void on_btnRefresh_clicked();

private Q_SLOTS:
	void mnuFollowInDump();
	void mnuFollowInDumpNewTab();
	void mnuFollowInStack();
	void mnuFollowInCPU();

private:
	virtual void showEvent(QShowEvent *event);

private:
	void do_find();

private:
	 Ui::DialogSymbolViewer *const ui;
	 QStringListModel *            model_;
	 QSortFilterProxyModel *       filter_model_;
};

#endif
