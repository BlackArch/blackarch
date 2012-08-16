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

#ifndef DIALOGHEAP_20061101_H_
#define DIALOGHEAP_20061101_H_

#include "Types.h"
#include "ResultViewModel.h"

#include <QDialog>

class QSortFilterProxyModel;

namespace Ui { class DialogHeap; }

class DialogHeap : public QDialog {
	Q_OBJECT

public:
	DialogHeap(QWidget *parent = 0);
	virtual ~DialogHeap();

public Q_SLOTS:
	void on_btnFind_clicked();
	void on_btnGraph_clicked();
	void on_tableView_doubleClicked(const QModelIndex & index);

private:
	virtual void showEvent(QShowEvent *event);

private:
	void get_library_names(QString &libcName, QString &ldName) const;
	void collect_blocks(edb::address_t start_address, edb::address_t end_address);
	void detect_pointers();
	void do_find();
	void process_potential_pointer(const QHash<edb::address_t, edb::address_t> &targets, Result &result);

	edb::address_t find_heap_start_heuristic(edb::address_t end_address, size_t offset) const;

private:
	 Ui::DialogHeap *const ui;
	 ResultViewModel *     model_;
};

#endif
