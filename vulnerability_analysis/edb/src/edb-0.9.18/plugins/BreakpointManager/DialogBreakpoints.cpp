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

#include "DialogBreakpoints.h"
#include "DebuggerCoreInterface.h"
#include "Debugger.h"
#include "Expression.h"
#include "Debugger.h"

#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>

#include "ui_dialogbreakpoints.h"

//------------------------------------------------------------------------------
// Name: DialogBreakpoints(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogBreakpoints::DialogBreakpoints(QWidget *parent) : QDialog(parent), ui(new Ui::DialogBreakpoints) {
	ui->setupUi(this);
	ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

//------------------------------------------------------------------------------
// Name: ~DialogBreakpoints()
// Desc:
//------------------------------------------------------------------------------
DialogBreakpoints::~DialogBreakpoints() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogBreakpoints::showEvent(QShowEvent *) {
	updateList();
}

//------------------------------------------------------------------------------
// Name: updateList()
// Desc:
//------------------------------------------------------------------------------
void DialogBreakpoints::updateList() {


	ui->tableWidget->setSortingEnabled(false);
	ui->tableWidget->setRowCount(0);

	const DebuggerCoreInterface::BreakpointState breakpoint_state = edb::v1::debugger_core->backup_breakpoints();

	Q_FOREACH(const Breakpoint::pointer &bp, breakpoint_state) {
		const int row = ui->tableWidget->rowCount();
		ui->tableWidget->insertRow(row);

		if(!bp->internal()) {

			const edb::address_t address = bp->address();
			const QString condition      = bp->condition;
			const QByteArray orig_bytes  = bp->original_bytes();
			const bool onetime           = bp->one_time();
			const QString symname        = edb::v1::find_function_symbol(address, QString(), 0);
			const QString bytes          = edb::v1::format_bytes(orig_bytes);

			ui->tableWidget->setItem(row, 0, new QTableWidgetItem(edb::v1::format_pointer(address)));
			ui->tableWidget->setItem(row, 1, new QTableWidgetItem(condition));
			ui->tableWidget->setItem(row, 2, new QTableWidgetItem(bytes));
			ui->tableWidget->setItem(row, 3, new QTableWidgetItem(onetime ? tr("One Time") : tr("Standard")));
			ui->tableWidget->setItem(row, 4, new QTableWidgetItem(symname));
		}
	}

	ui->tableWidget->setSortingEnabled(true);
}

//------------------------------------------------------------------------------
// Name: on_btnAdd_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogBreakpoints::on_btnAdd_clicked() {

	bool ok;
    QString text = QInputDialog::getText(this, tr("Add Breakpoint"), tr("Address:"), QLineEdit::Normal, QString(), &ok);

	if(ok && !text.isEmpty()) {
		Expression<edb::address_t> expr(text, edb::v1::get_variable, edb::v1::get_value);
		ExpressionError err;
		const edb::address_t address = expr.evaluate_expression(ok, err);
		if(ok) {
			edb::v1::create_breakpoint(address);
			updateList();

		} else {
			QMessageBox::information(this, tr("Error In Address Expression!"), err.what());
		}
	}
}

//------------------------------------------------------------------------------
// Name: on_btnCondition_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogBreakpoints::on_btnCondition_clicked() {
	QList<QTableWidgetItem *> sel = ui->tableWidget->selectedItems();
	if(sel.size() != 0) {
		bool ok;
		const edb::address_t address = edb::v1::string_to_address(sel.begin()[0]->text(), ok);
		if(ok) {
			const QString condition = edb::v1::get_breakpoint_condition(address);
			const QString text = QInputDialog::getText(this, tr("Set Breakpoint Condition"), tr("Expression:"), QLineEdit::Normal, condition, &ok);
		    if(ok) {
				edb::v1::set_breakpoint_condition(address, text);
				updateList();
			}
		}
	}
}

#if 0
//------------------------------------------------------------------------------
// Name: on_btnAddFunction_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogBreakpoints::on_btnAddFunction_clicked() {
    bool ok;
    const QString text = QInputDialog::getText(this, tr("Add Breakpoint On Library Function"), tr("Function Name:"), QLineEdit::Normal, QString(), &ok);
	if(ok && !text.isEmpty()) {
		const QList<Symbol::pointer> syms = edb::v1::symbol_manager().symbols();
		Q_FOREACH(const Symbol::pointer &current, syms) {
			if(current.name_no_prefix == text) {
				edb::v1::create_breakpoint(current.address);
			}
		}
		updateList();
	}
}
#endif

//------------------------------------------------------------------------------
// Name: on_btnRemove_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogBreakpoints::on_btnRemove_clicked() {
	QList<QTableWidgetItem *> sel = ui->tableWidget->selectedItems();
	Q_FOREACH(QTableWidgetItem *it, sel) {
		if(it->column() == 0) { // address column
			bool ok;
			const edb::address_t address = edb::v1::string_to_address(it->text(), ok);
			if(ok) {
				edb::v1::remove_breakpoint(address);
			}
		}
	}
	updateList();
}

//------------------------------------------------------------------------------
// Name: on_tableWidget_cellDoubleClicked(int row, int col)
// Desc:
//------------------------------------------------------------------------------
void DialogBreakpoints::on_tableWidget_cellDoubleClicked(int row, int col) {
	switch(col) {
		case 0: // address
		{
			if(QTableWidgetItem *const address_item = ui->tableWidget->item(row, 0)) {
				bool ok;
				const edb::address_t address = edb::v1::string_to_address(address_item->text(), ok);
				if(ok) {
					edb::v1::jump_to_address(address);
				}
			}
			break;
		}
		case 1: // condition
		{
			if(QTableWidgetItem *const address_item = ui->tableWidget->item(row, 0)) {
				bool ok;
				const edb::address_t address = edb::v1::string_to_address(address_item->text(), ok);
				if(ok) {
					const QString condition = edb::v1::get_breakpoint_condition(address);
					const QString text = QInputDialog::getText(this, tr("Set Breakpoint Condition"), tr("Expression:"), QLineEdit::Normal, condition, &ok);
					if(ok) {
						edb::v1::set_breakpoint_condition(address, text);
						updateList();
					}
				}
			}
			break;
		}
	}
}
