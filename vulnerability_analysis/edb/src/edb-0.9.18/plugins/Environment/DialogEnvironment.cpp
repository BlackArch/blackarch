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

#include "DialogEnvironment.h"
#include "DebuggerCoreInterface.h"
#include "SymbolManagerInterface.h"
#include "Debugger.h"
#include "Configuration.h"

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QFileInfo>
#include <QtDebug>

#include "ui_dialogenvironment.h"

//------------------------------------------------------------------------------
// Name: DialogEnvironment(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogEnvironment::DialogEnvironment(QWidget *parent) : QDialog(parent), ui(new Ui::DialogEnvironment) {
	ui->setupUi(this);

	model_        = new QStringListModel(this);
	filter_model_ = new QSortFilterProxyModel(this);

	filter_model_->setFilterKeyColumn(0);
	filter_model_->setSourceModel(model_);
	ui->listView->setModel(filter_model_);

	connect(ui->txtSearch, SIGNAL(textChanged(const QString &) ), filter_model_, SLOT(setFilterFixedString(const QString &)));
}

//------------------------------------------------------------------------------
// Name: ~DialogEnvironment()
// Desc:
//------------------------------------------------------------------------------
DialogEnvironment::~DialogEnvironment() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_listView_doubleClicked(const QModelIndex &index)
// Desc: follows the found item in the data view
//------------------------------------------------------------------------------
void DialogEnvironment::on_listView_doubleClicked(const QModelIndex &index) {

	bool ok;
	const QString s = index.data().toString();
	const edb::address_t addr = edb::v1::string_to_address(s, ok);
	if(ok) {
		edb::v1::dump_data(addr, false);
	}
}

//------------------------------------------------------------------------------
// Name: libc_name()
// Desc:
//------------------------------------------------------------------------------
QString DialogEnvironment::libc_name() const {

	QString libc;

	const QStringList libs = edb::v1::loaded_libraries();

	Q_FOREACH(const QString &s, libs) {
		if(!libc.isEmpty()) {
			break;
		}

		const QFileInfo fileinfo(s);

		// this tries its best to cover all possible libc library versioning possibilities
		// we need to find out if this is 100% accurate, so far seems correct based on my system

		if(fileinfo.completeBaseName().startsWith("libc-")) {
			libc = fileinfo.completeBaseName() + "." + fileinfo.suffix();
			qDebug() << "[Environment] libc library appears to be:" << libc;
			continue;
		}

		if(fileinfo.completeBaseName().startsWith("libc.so")) {
			libc = fileinfo.completeBaseName() + "." + fileinfo.suffix();
			qDebug() << "[Environment] libc library appears to be:" << libc;
			continue;
		}
	}

	return libc;
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogEnvironment::do_find() {
	QStringList results;

	const int min_string_length = edb::v1::config().min_string_length;

	const QString libc = libc_name();
	if(const Symbol::pointer s = edb::v1::symbol_manager().find(libc + "::__environ")) {
		edb::address_t evironment_address = s->address;

		qDebug() << "[Environment] pointer to environment array is at: " << edb::v1::format_pointer(evironment_address);
		edb::v1::debugger_core->read_bytes(evironment_address, &evironment_address, sizeof(evironment_address));
		qDebug() << "[Environment] environment array is at: " << edb::v1::format_pointer(evironment_address);

		edb::address_t address;
		edb::v1::debugger_core->read_bytes(evironment_address, &address, sizeof(address));
		QString temp;
		while(address != 0) {

			QString str;
			int stringLen;
			if(edb::v1::get_ascii_string_at_address(address, str, min_string_length, 256, stringLen)) {
				results << QString("%1:%2").arg(edb::v1::format_pointer(address)).arg(str);
			}

			evironment_address += sizeof(edb::address_t);
			edb::v1::debugger_core->read_bytes(evironment_address, &address, sizeof(address));
		}
	} else {
#ifdef Q_OS_LINUX
		QFile proc_environ(QString("/proc/%1/environ").arg(edb::v1::debugger_core->pid()));
		if(proc_environ.open(QIODevice::ReadOnly)) {
			QByteArray env = proc_environ.readAll();
			char *p = env.data();
			char *ptr = p;
			while(ptr != p + env.size()) {
				results << QString("%1:%2").arg(edb::v1::format_pointer(0)).arg(ptr);
				ptr += qstrlen(ptr) + 1;
			}

		}
#else
		qDebug("[Environment] __environ symbol not found in libc");
#endif
	}

	model_->setStringList(results);
}

//------------------------------------------------------------------------------
// Name: on_btnRefresh_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogEnvironment::on_btnRefresh_clicked() {
	ui->btnRefresh->setEnabled(false);
	do_find();
	ui->btnRefresh->setEnabled(true);
}


//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogEnvironment::showEvent(QShowEvent *) {
	on_btnRefresh_clicked();
}
