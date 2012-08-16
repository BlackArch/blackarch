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

#include "DialogThreads.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"

#include <QHeaderView>

#include "ui_dialog_threads.h"

//------------------------------------------------------------------------------
// Name: DialogThreads(QWidget *parent, Qt::WindowFlags f)
// Desc:
//------------------------------------------------------------------------------
DialogThreads::DialogThreads(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f), ui(new Ui::DialogThreads) {
	ui->setupUi(this);
	ui->thread_table->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

//------------------------------------------------------------------------------
// Name: ~DialogThreads()
// Desc:
//------------------------------------------------------------------------------
DialogThreads::~DialogThreads() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogThreads::showEvent(QShowEvent *) {

	ui->thread_table->setSortingEnabled(false);
	ui->thread_table->setRowCount(0);

	QList<edb::tid_t> threads       = edb::v1::debugger_core->thread_ids();
	const edb::tid_t current_thread = edb::v1::debugger_core->active_thread();

	Q_FOREACH(edb::tid_t thread, threads) {
		const int row = ui->thread_table->rowCount();
		ui->thread_table->insertRow(row);

		QTableWidgetItem *item;

		if(thread == current_thread) {
			item = new QTableWidgetItem(QString("*%1").arg(thread));
		} else {
			item = new QTableWidgetItem(QString("%1").arg(thread));
		}

		item->setData(Qt::UserRole, static_cast<qulonglong>(thread));

		ui->thread_table->setItem(row, 0, item);
	}

	ui->thread_table->resizeRowsToContents();
	ui->thread_table->resizeColumnsToContents();
	ui->thread_table->setSortingEnabled(true);
}

//------------------------------------------------------------------------------
// Name: selected_thread()
// Desc:
//------------------------------------------------------------------------------
edb::tid_t DialogThreads::selected_thread() {
	QList<QTableWidgetItem *> selected = ui->thread_table->selectedItems();
	if(!selected.isEmpty()) {
		return selected[0]->data(Qt::UserRole).toUInt();
	}
	return 0;
}
