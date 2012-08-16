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

#include "DialogFunctions.h"
#include "Debugger.h"
#include "MemoryRegions.h"
#include "AnalyzerInterface.h"

#include <QDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include "ui_dialogfunctions.h"

#define MIN_REFCOUNT 2

//------------------------------------------------------------------------------
// Name: DialogFunctions(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogFunctions::DialogFunctions(QWidget *parent) : QDialog(parent), ui(new Ui::DialogFunctions) {
	ui->setupUi(this);
	ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	filter_model_ = new QSortFilterProxyModel(this);
	connect(ui->txtSearch, SIGNAL(textChanged(const QString &)), filter_model_, SLOT(setFilterFixedString(const QString &)));
}

//------------------------------------------------------------------------------
// Name: ~DialogFunctions()
// Desc:
//------------------------------------------------------------------------------
DialogFunctions::~DialogFunctions() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_tableWidget_cellDoubleClicked(int row, int column)
// Desc: follows the found item in the data view
//------------------------------------------------------------------------------
void DialogFunctions::on_tableWidget_cellDoubleClicked(int row, int column) {
	Q_UNUSED(column);

	QTableWidgetItem *const item = ui->tableWidget->item(row, 0);
	const edb::address_t addr = item->data(Qt::UserRole).toULongLong();
	edb::v1::jump_to_address(addr);
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogFunctions::showEvent(QShowEvent *) {
	filter_model_->setFilterKeyColumn(3);
	filter_model_->setSourceModel(&edb::v1::memory_regions());
	ui->tableView->setModel(filter_model_);

	ui->progressBar->setValue(0);
	ui->tableWidget->setRowCount(0);
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogFunctions::do_find() {

	if(AnalyzerInterface *const analyzer = edb::v1::analyzer()) {
		const QItemSelectionModel *const selModel = ui->tableView->selectionModel();
		const QModelIndexList sel = selModel->selectedRows();

		if(sel.size() == 0) {
			QMessageBox::information(this, tr("No Region Selected"), tr("You must select a region which is to be scanned for functions."));
			return;
		}

		QObject *const analyzer_object = dynamic_cast<QObject *>(analyzer);

		if(analyzer_object != 0) {
			connect(analyzer_object, SIGNAL(update_progress(int)), ui->progressBar, SLOT(setValue(int)));
		}

		ui->tableWidget->setRowCount(0);
		ui->tableWidget->setSortingEnabled(false);

		Q_FOREACH(const QModelIndex &selected_item, sel) {

			const QModelIndex index = filter_model_->mapToSource(selected_item);

			// do the search for this region!
			if(const MemRegion *const region_ptr = reinterpret_cast<const MemRegion *>(index.internalPointer())) {

				// NOTE: see Analyzer::analyze for an explanation for this copy...
				MemRegion region(*region_ptr);

				analyzer->analyze(region);

				const AnalyzerInterface::FunctionMap &results = analyzer->functions(region);

				Q_FOREACH(const AnalyzerInterface::Function &info, results) {

					const int row = ui->tableWidget->rowCount();
					ui->tableWidget->insertRow(row);

					// entry point
					QTableWidgetItem *const p = new QTableWidgetItem(edb::v1::format_pointer(info.entry_address));
					p->setData(Qt::UserRole, info.entry_address);
					ui->tableWidget->setItem(row, 0, p);

					// upper bound of the function
					if(info.reference_count >= MIN_REFCOUNT) {
						ui->tableWidget->setItem(row, 1, new QTableWidgetItem(edb::v1::format_pointer(info.end_address)));

						QTableWidgetItem *const size_item = new QTableWidgetItem;
						size_item->setData(Qt::DisplayRole, info.end_address - info.entry_address + 1);

						ui->tableWidget->setItem(row, 2, size_item);
					}

					// reference count
					QTableWidgetItem *const itemCount = new QTableWidgetItem;
					itemCount->setData(Qt::DisplayRole, info.reference_count);
					ui->tableWidget->setItem(row, 3, itemCount);

					// type
					if(info.type == AnalyzerInterface::Function::FUNCTION_THUNK) {
						ui->tableWidget->setItem(row, 4, new QTableWidgetItem(tr("Thunk")));
					} else {
						ui->tableWidget->setItem(row, 4, new QTableWidgetItem(tr("Standard Function")));
					}
				}
			}
		}
		ui->tableWidget->setSortingEnabled(true);

		if(analyzer_object != 0) {
			disconnect(analyzer_object, SIGNAL(update_progress(int)), ui->progressBar, SLOT(setValue(int)));
		}
	}
}

//------------------------------------------------------------------------------
// Name: on_btnFind_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogFunctions::on_btnFind_clicked() {
	ui->btnFind->setEnabled(false);
	ui->progressBar->setValue(0);
	do_find();
	ui->progressBar->setValue(100);
	ui->btnFind->setEnabled(true);
}
