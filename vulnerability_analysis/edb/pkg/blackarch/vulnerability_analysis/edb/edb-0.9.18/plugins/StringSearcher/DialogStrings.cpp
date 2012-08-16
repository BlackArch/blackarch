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

#include "DialogStrings.h"
#include "Debugger.h"
#include "MemoryRegions.h"
#include "Util.h"
#include "Configuration.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include "ui_dialogstrings.h"

//------------------------------------------------------------------------------
// Name: DialogStrings(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogStrings::DialogStrings(QWidget *parent) : QDialog(parent), ui(new Ui::DialogStrings) {
	ui->setupUi(this);
	ui->tableView->verticalHeader()->hide();
	ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	filter_model_ = new QSortFilterProxyModel(this);
	connect(ui->txtSearch, SIGNAL(textChanged(const QString &)), filter_model_, SLOT(setFilterFixedString(const QString &)));
}

//------------------------------------------------------------------------------
// Name: ~DialogStrings()
// Desc:
//------------------------------------------------------------------------------
DialogStrings::~DialogStrings() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_listWidget_itemDoubleClicked(QListWidgetItem *item)
// Desc: follows the found item in the data view
//------------------------------------------------------------------------------
void DialogStrings::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
	bool ok;
	const edb::address_t addr = item->data(Qt::UserRole).toULongLong(&ok);
	if(ok) {
		edb::v1::dump_data(addr, false);
	}
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogStrings::showEvent(QShowEvent *) {
	filter_model_->setFilterKeyColumn(3);
	filter_model_->setSourceModel(&edb::v1::memory_regions());
	ui->tableView->setModel(filter_model_);

	ui->progressBar->setValue(0);
	ui->listWidget->clear();
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogStrings::do_find() {

	const int min_string_length = edb::v1::config().min_string_length;

	const QItemSelectionModel *const selection_model = ui->tableView->selectionModel();
	const QModelIndexList sel = selection_model->selectedRows();

	QString str;

	if(sel.size() == 0) {
		QMessageBox::information(
			this,
			tr("No Region Selected"),
			tr("You must select a region which is to be scanned for strings."));
	}

	Q_FOREACH(const QModelIndex &selected_item, sel) {

		const QModelIndex index = filter_model_->mapToSource(selected_item);

		if(const MemRegion *const region = reinterpret_cast<const MemRegion *>(index.internalPointer())) {

			edb::address_t start_address     = region->start;
			const edb::address_t end_address = region->end;
			const edb::address_t orig_start  = start_address;

			// do the search for this region!
			while(start_address < end_address) {

				int string_length = 0;
				const bool ok = edb::v1::get_ascii_string_at_address(start_address, str, min_string_length, 256, string_length);
				if(ok) {
					QListWidgetItem *const item = new QListWidgetItem(QString("%1: %2").arg(edb::v1::format_pointer(start_address)).arg(str));
					item->setData(Qt::UserRole, start_address);
					ui->listWidget->addItem(item);
				}

				ui->progressBar->setValue(util::percentage((start_address - orig_start), region->size()));

				if(ok) {
					start_address += string_length;
				} else {
					++start_address;
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: on_btnFind_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogStrings::on_btnFind_clicked() {
	ui->btnFind->setEnabled(false);
	ui->listWidget->clear();
	ui->progressBar->setValue(0);
	do_find();
	ui->progressBar->setValue(100);
	ui->btnFind->setEnabled(true);
}
