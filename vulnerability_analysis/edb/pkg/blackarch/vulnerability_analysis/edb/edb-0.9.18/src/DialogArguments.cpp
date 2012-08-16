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

#include "DialogArguments.h"
#include <QListWidgetItem>

#include "ui_dialog_arguments.h"

//------------------------------------------------------------------------------
// Name: DialogArguments(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogArguments::DialogArguments(QWidget *parent) : QDialog(parent), ui(new Ui::DialogArguments) {
	ui->setupUi(this);
}

//------------------------------------------------------------------------------
// Name: ~DialogArguments()
// Desc:
//------------------------------------------------------------------------------
DialogArguments::~DialogArguments() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_btnAdd_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogArguments::on_btnAdd_clicked() {
	QListWidgetItem *p = new QListWidgetItem(tr("New Argument"), ui->listWidget);
	p->setFlags(p->flags() | Qt::ItemIsEditable);
}

//------------------------------------------------------------------------------
// Name: on_btnDel_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogArguments::on_btnDel_clicked() {
	qDeleteAll(ui->listWidget->selectedItems());
}

//------------------------------------------------------------------------------
// Name: arguments() const
// Desc:
//------------------------------------------------------------------------------
QStringList DialogArguments::arguments() const {
	QStringList ret;
	for(int i = 0; i < ui->listWidget->count(); ++i) {
		ret << ui->listWidget->item(i)->text();
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: set_arguments(const QStringList &args)
// Desc:
//------------------------------------------------------------------------------
void DialogArguments::set_arguments(const QStringList &args) {
	ui->listWidget->clear();
	ui->listWidget->addItems(args);
}

//------------------------------------------------------------------------------
// Name: on_btnUp_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogArguments::on_btnUp_clicked() {
	const int x = ui->listWidget->currentRow();
	if(x > 0 && x != -1) {
		ui->listWidget->insertItem(x - 1, ui->listWidget->takeItem(x));
		ui->listWidget->setCurrentRow(x - 1);
	}
}

//------------------------------------------------------------------------------
// Name: on_btnDown_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogArguments::on_btnDown_clicked() {
	const int x = ui->listWidget->currentRow();
	if(x < (ui->listWidget->count() - 1) && x != -1) {
		ui->listWidget->insertItem(x + 1, ui->listWidget->takeItem(x));
		ui->listWidget->setCurrentRow(x + 1);
	}
}
