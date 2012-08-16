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

#include "DialogAttachUNIX.h"
#include "Types.h"

#include <QtDebug>
#include <QMap>
#include <QHeaderView>
#include <QDir>
#include <QTextStream>

#include <sys/types.h>
#include <unistd.h>

#include "ui_dialog_attach.h"

//------------------------------------------------------------------------------
// Name: DialogAttachUNIX(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
DialogAttachUNIX::DialogAttachUNIX(QWidget *parent) : QDialog(parent), ui(new Ui::DialogAttach) {
	ui->setupUi(this);
}

//------------------------------------------------------------------------------
// Name: ~DialogAttachUNIX()
// Desc:
//------------------------------------------------------------------------------
DialogAttachUNIX::~DialogAttachUNIX() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: is_numeric(const QString &s)
// Desc: returns true if the string only contains decimal digits
//------------------------------------------------------------------------------
bool DialogAttachUNIX::is_numeric(const QString &s) {
	Q_FOREACH(QChar ch, s) {
		if(!ch.isDigit()) {
			return false;
		}
	}

	return true;
}

//------------------------------------------------------------------------------
// Name: on_filter_textChanged(const QString &text)
// Desc:
//------------------------------------------------------------------------------
void DialogAttachUNIX::on_filter_textChanged(const QString &text) {
	update_list(text);
}

//------------------------------------------------------------------------------
// Name: update_list(const QString &filter)
// Desc:
//------------------------------------------------------------------------------
void DialogAttachUNIX::update_list(const QString &filter) {

	QMap<edb::pid_t, ProcessInfo> procs = find_processes();

	const edb::uid_t user_id = getuid();
	const bool filterUID = ui->filter_uid->isChecked();
	const QString lowerFilter = filter.toLower();
	ui->processes_table->setSortingEnabled(false);
	ui->processes_table->setRowCount(0);

	Q_FOREACH(const ProcessInfo &process_info, procs) {

		const QString process_name = process_info.name;

		if(filter.isEmpty() || process_name.toLower().contains(lowerFilter)) {
			if(!filterUID || process_info.uid == user_id) {
				const int row = ui->processes_table->rowCount();
				ui->processes_table->insertRow(row);

				QTableWidgetItem *const item_pid = new QTableWidgetItem;
				item_pid->setData(Qt::DisplayRole, process_info.pid);

				QTableWidgetItem *item_uid;
				if(!process_info.user.isEmpty()) {
					item_uid = new QTableWidgetItem(process_info.user);
				} else {
					item_uid = new QTableWidgetItem;
					item_uid->setData(Qt::DisplayRole, process_info.uid);
				}

				ui->processes_table->setItem(row, 0, item_pid);
				ui->processes_table->setItem(row, 1, item_uid);
		        ui->processes_table->setItem(row, 2, new QTableWidgetItem(process_info.name));
			}
		}
	}
	ui->processes_table->setSortingEnabled(true);
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *event)
// Desc:
//------------------------------------------------------------------------------
void DialogAttachUNIX::showEvent(QShowEvent *event) {
	Q_UNUSED(event);
	update_list(ui->filter->text());
}

//------------------------------------------------------------------------------
// Name: on_filter_uid_clicked(bool checked)
// Desc:
//------------------------------------------------------------------------------
void DialogAttachUNIX::on_filter_uid_clicked(bool checked) {
	Q_UNUSED(checked);
	update_list(ui->filter->text());
}

//------------------------------------------------------------------------------
// Name: selected_pid(bool &ok) const
// Desc:
//------------------------------------------------------------------------------
edb::pid_t DialogAttachUNIX::selected_pid(bool &ok) const {
	const QList<QTableWidgetItem *> sel = ui->processes_table->selectedItems();
	if(sel.size() != 0) {
		ok = true;
		return sel.first()->text().toUInt();
	} else {
		ok = false;
		return edb::pid_t();
	}
}
