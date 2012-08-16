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

#include "DialogPlugins.h"
#include "Debugger.h"
#include "DebuggerPluginInterface.h"

#include <QHeaderView>
#include <QMetaClassInfo>

#include "ui_dialog_plugins.h"

//------------------------------------------------------------------------------
// Name: DialogPlugins(QWidget *parent, Qt::WindowFlags f)
// Desc:
//------------------------------------------------------------------------------
DialogPlugins::DialogPlugins(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f), ui(new Ui::DialogPlugins) {
	ui->setupUi(this);
}

//------------------------------------------------------------------------------
// Name: ~DialogPlugins()
// Desc:
//------------------------------------------------------------------------------
DialogPlugins::~DialogPlugins() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogPlugins::showEvent(QShowEvent *) {

	QHash<QString, QObject *> plugins = edb::v1::plugin_list();

	ui->plugins_table->setSortingEnabled(false);
	ui->plugins_table->setRowCount(0);

	for(QHash<QString, QObject *>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
		const int row = ui->plugins_table->rowCount();
		ui->plugins_table->insertRow(row);

		const QString filename = it.key();
		QString plugin_name;
		QString author;
		QString url;

		// get a QObject from the plugin
		if(QObject *const p = it.value()) {
			const QMetaObject *const meta = p->metaObject();
			plugin_name = meta->className();
			const int author_index = meta->indexOfClassInfo("author");
			if(author_index != -1) {
				author = meta->classInfo(author_index).value();
			}

			const int url_index = meta->indexOfClassInfo("url");
			if(url_index != -1) {
				url = meta->classInfo(url_index).value();
			}
		}

		ui->plugins_table->setItem(row, 0, new QTableWidgetItem(filename));
		ui->plugins_table->setItem(row, 1, new QTableWidgetItem(plugin_name));
		ui->plugins_table->setItem(row, 2, new QTableWidgetItem(author));
		ui->plugins_table->setItem(row, 3, new QTableWidgetItem(url));
	}

	ui->plugins_table->resizeRowsToContents();
	ui->plugins_table->resizeColumnsToContents();
	ui->plugins_table->setSortingEnabled(true);
}
