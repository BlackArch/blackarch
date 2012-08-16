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

#include "DumpStateOptionsPage.h"
#include <QSettings>
#include "ui_dumpstate_options_page.h"

//------------------------------------------------------------------------------
// Name: DumpStateOptionsPage(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DumpStateOptionsPage::DumpStateOptionsPage(QWidget *parent) : QWidget(parent), ui(new Ui::DumpStateOptionsPage) {
	ui->setupUi(this);
}

//------------------------------------------------------------------------------
// Name: ~DumpStateOptionsPage()
// Desc:
//------------------------------------------------------------------------------
DumpStateOptionsPage::~DumpStateOptionsPage() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *event)
// Desc:
//------------------------------------------------------------------------------
void DumpStateOptionsPage::showEvent(QShowEvent *event) {
	Q_UNUSED(event);

	QSettings settings;
	ui->instructionsBeforeIP->setValue(settings.value("DumpState/instructions_before_ip", 0).toInt());
	ui->instructionsAfterIP->setValue(settings.value("DumpState/instructions_after_ip", 5).toInt());
}

//------------------------------------------------------------------------------
// Name: on_instructionsBeforeIP_valueChanged(int i)
// Desc:
//------------------------------------------------------------------------------
void DumpStateOptionsPage::on_instructionsBeforeIP_valueChanged(int i) {
	QSettings settings;
	settings.setValue("DumpState/instructions_before_ip", i);
}

//------------------------------------------------------------------------------
// Name: on_instructionsAfterIP_valueChanged(int i)
// Desc:
//------------------------------------------------------------------------------
void DumpStateOptionsPage::on_instructionsAfterIP_valueChanged(int i) {
	QSettings settings;
	settings.setValue("DumpState/instructions_after_ip", i);
}

