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

#include "DialogSymbolViewer.h"
#include "DebuggerCoreInterface.h"
#include "Debugger.h"
#include "SymbolManagerInterface.h"
#include "Configuration.h"
#include "Util.h"
#include "Debugger.h"

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QMenu>

#include "ui_dialogsymbols.h"

//------------------------------------------------------------------------------
// Name: DialogSymbolViewer(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogSymbolViewer::DialogSymbolViewer(QWidget *parent) : QDialog(parent), ui(new Ui::DialogSymbolViewer) {
	ui->setupUi(this);

	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

	model_        = new QStringListModel(this);
	filter_model_ = new QSortFilterProxyModel(this);

	filter_model_->setFilterKeyColumn(0);
	filter_model_->setSourceModel(model_);
	ui->listView->setModel(filter_model_);

	connect(ui->txtSearch, SIGNAL(textChanged(const QString &)), filter_model_, SLOT(setFilterFixedString(const QString &)));
}

//------------------------------------------------------------------------------
// Name: ~DialogSymbolViewer()
// Desc:
//------------------------------------------------------------------------------
DialogSymbolViewer::~DialogSymbolViewer() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_listView_doubleClicked(const QModelIndex &index)
// Desc: follows the found item in the data view
//------------------------------------------------------------------------------
void DialogSymbolViewer::on_listView_doubleClicked(const QModelIndex &index) {

	bool ok;
	const QString s = index.data().toString();
	const edb::address_t addr = edb::v1::string_to_address(s, ok);

	if(ok) {

		const Symbol::pointer sym = edb::v1::symbol_manager().find(addr);

		if(sym != 0 && sym->is_code()) {
			edb::v1::jump_to_address(addr);
		} else {
			edb::v1::dump_data(addr, false);
		}
	}
}

//------------------------------------------------------------------------------
// Name: on_listView_customContextMenuRequested(const QPoint &pos)
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::on_listView_customContextMenuRequested(const QPoint &pos) {

	const QModelIndex index = ui->listView->indexAt(pos);
	if(index.isValid()) {

		bool ok;
		const QString s = index.data().toString();
		const edb::address_t addr = edb::v1::string_to_address(s, ok);

		QMenu menu;
		QAction *const action1 = menu.addAction(tr("&Follow In Disassembly"), this, SLOT(mnuFollowInCPU()));
		QAction *const action2 = menu.addAction(tr("&Follow In Dump"), this, SLOT(mnuFollowInDump()));
		QAction *const action3 = menu.addAction(tr("&Follow In Dump (New Tab)"), this, SLOT(mnuFollowInDumpNewTab()));
		QAction *const action4 = menu.addAction(tr("&Follow In Stack"), this, SLOT(mnuFollowInStack()));

		action1->setData(addr);
		action2->setData(addr);
		action3->setData(addr);
		action4->setData(addr);

		menu.exec(ui->listView->mapToGlobal(pos));
	}
}

//------------------------------------------------------------------------------
// Name: mnuFollowInDump()
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::mnuFollowInDump() {
	if(QAction *const action = qobject_cast<QAction *>(sender())) {
		const edb::address_t address = action->data().toULongLong();
		edb::v1::dump_data(address, false);
	}
}

//------------------------------------------------------------------------------
// Name: mnuFollowInDumpNewTab()
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::mnuFollowInDumpNewTab() {
	if(QAction *const action = qobject_cast<QAction *>(sender())) {
		const edb::address_t address = action->data().toULongLong();
		edb::v1::dump_data(address, true);
	}
}

//------------------------------------------------------------------------------
// Name: mnuFollowInStack()
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::mnuFollowInStack() {
	if(QAction *const action = qobject_cast<QAction *>(sender())) {
		const edb::address_t address = action->data().toULongLong();
		edb::v1::dump_stack(address, false);
	}
}

//------------------------------------------------------------------------------
// Name: mnuFollowInCPU()
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::mnuFollowInCPU() {
	if(QAction *const action = qobject_cast<QAction *>(sender())) {
		const edb::address_t address = action->data().toULongLong();
		edb::v1::jump_to_address(address);
	}
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::do_find() {
	QStringList results;
	QString temp;

	const QList<Symbol::pointer> symbols = edb::v1::symbol_manager().symbols();
	Q_FOREACH(const Symbol::pointer &sym, symbols) {
		results << QString("%1: %2").arg(edb::v1::format_pointer(sym->address)).arg(sym->name);
	}

	model_->setStringList(results);
}

//------------------------------------------------------------------------------
// Name: on_btnRefresh_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::on_btnRefresh_clicked() {
	ui->btnRefresh->setEnabled(false);
	do_find();
	ui->btnRefresh->setEnabled(true);
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogSymbolViewer::showEvent(QShowEvent *) {
	on_btnRefresh_clicked();
}
