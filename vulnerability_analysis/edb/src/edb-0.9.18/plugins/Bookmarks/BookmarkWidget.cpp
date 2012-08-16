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

#include "BookmarkWidget.h"
#include "ui_bookmarks.h"
#include "Expression.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QMenu>
#include "Debugger.h"

//------------------------------------------------------------------------------
// Name: BookmarkWidget(QWidget *parent, Qt::WindowFlags f)
// Desc:
//------------------------------------------------------------------------------
BookmarkWidget::BookmarkWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), ui(new Ui::Bookmarks) {
	ui->setupUi(this);
}

//------------------------------------------------------------------------------
// Name: ~BookmarkWidget()
// Desc:
//------------------------------------------------------------------------------
BookmarkWidget::~BookmarkWidget() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_tableWidget_cellDoubleClicked(int row, int col)
// Desc:
//------------------------------------------------------------------------------
void BookmarkWidget::on_tableWidget_cellDoubleClicked(int row, int col) {
	switch(col) {
		case 0: //address
		{
			if(QTableWidgetItem *const address_item = ui->tableWidget->item(row, 0)) {
				bool ok;
				const edb::address_t addr = edb::v1::string_to_address(address_item->text(), ok);
				if(ok) {
					edb::v1::jump_to_address(addr);
				}
			}
			break;
		}
		case 1: //comment
		{
			QString old_comment;
			if(QTableWidgetItem *const comment_item = ui->tableWidget->item(row, 1)) {
				 old_comment = comment_item->text();
			}
			bool ok;
			const QString new_comment = QInputDialog::getText(ui->tableWidget, tr("Comment"), tr("Set Comment:"), QLineEdit::Normal, old_comment, &ok);
			if(ok) {
				ui->tableWidget->setItem(row, 1, new QTableWidgetItem(new_comment));
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: on_btnAdd_clicked()
// Desc:
//------------------------------------------------------------------------------
void BookmarkWidget::on_btnAdd_clicked() {

	edb::address_t address;
	if(edb::v1::get_expression_from_user(tr("Bookmark Address"), tr("Address:"), address)) {
		add_address(address);
	}
}

//------------------------------------------------------------------------------
// Name: on_btnDel_clicked()
// Desc:
//------------------------------------------------------------------------------
void BookmarkWidget::on_btnDel_clicked() {
	std::auto_ptr<QTableWidgetItem> item(ui->tableWidget->takeItem(ui->tableWidget->currentRow(), 0));
	ui->tableWidget->removeRow(ui->tableWidget->currentRow());
	if(item.get()) {
		bool ok;
		const QString s = item->text();
		const edb::address_t address = edb::v1::string_to_address(s, ok);
		entries_.remove(address);
	}
}

//------------------------------------------------------------------------------
// Name: on_btnClear_clicked()
// Desc:
//------------------------------------------------------------------------------
void BookmarkWidget::on_btnClear_clicked() {
	ui->tableWidget->clearContents();
	ui->tableWidget->setRowCount(0);
	entries_.clear();
}

//------------------------------------------------------------------------------
// Name: add_address(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
void BookmarkWidget::add_address(edb::address_t address) {
	if(!entries_.contains(address)) {
		QTableWidgetItem *const new_item = new QTableWidgetItem(edb::v1::format_pointer(address));
		const int row = ui->tableWidget->rowCount();
		ui->tableWidget->setRowCount(row + 1);
		ui->tableWidget->setItem(row, 0, new_item);
		ui->tableWidget->resizeColumnToContents(0);
		entries_.insert(address);
	}
}

//------------------------------------------------------------------------------
// Name: shortcut(int index)
// Desc:
//------------------------------------------------------------------------------
void BookmarkWidget::shortcut(int index) {
	if(QTableWidgetItem *const item = ui->tableWidget->item(index, 0)) {
		emit on_tableWidget_cellDoubleClicked(index, 0);
	}
}

//------------------------------------------------------------------------------
// Name: on_tableWidget_customContextMenuRequested(const QPoint &pos)
// Desc:
//------------------------------------------------------------------------------
void BookmarkWidget::on_tableWidget_customContextMenuRequested(const QPoint &pos) {

	QMenu menu;
	QAction *const actionAdd     = menu.addAction(tr("&Add Address"));
	QAction *const actionDel     = menu.addAction(tr("&Delete Address"));
	QAction *const actionClear   = menu.addAction(tr("&Clear"));
	menu.addSeparator();
	QAction *const actionComment = menu.addAction(tr("&Set Comment"));
	QAction *const chosen = menu.exec(ui->tableWidget->mapToGlobal(pos));

	if(chosen == actionAdd) {
		on_btnAdd_clicked();
	} else if(chosen == actionDel) {
		on_btnDel_clicked();
	} else if(chosen == actionClear) {
		on_btnClear_clicked();
	} else if(chosen == actionComment) {
		bool ok;

		QString current_comment;
		if(QTableWidgetItem *const item = ui->tableWidget->item(ui->tableWidget->currentRow(), 1)) {
			current_comment = item->text();
		}

	    const QString text = QInputDialog::getText(ui->tableWidget, tr("Comment"), tr("Set Comment:"), QLineEdit::Normal, current_comment, &ok);
		if(ok) {
			ui->tableWidget->setItem(ui->tableWidget->currentRow(), 1, new QTableWidgetItem(text));
		}
	}
}


