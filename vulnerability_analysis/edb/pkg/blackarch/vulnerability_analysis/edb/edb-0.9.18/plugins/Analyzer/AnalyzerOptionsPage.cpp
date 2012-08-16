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

#include "AnalyzerOptionsPage.h"
#include <QSettings>

#include "ui_analyzer_options_page.h"

//------------------------------------------------------------------------------
// Name: AnalyzerOptionsPage(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
AnalyzerOptionsPage::AnalyzerOptionsPage(QWidget *parent) : QWidget(parent), ui(new Ui::AnalyzerOptionsPage) {
	ui->setupUi(this);
}

//------------------------------------------------------------------------------
// Name: ~AnalyzerOptionsPage()
// Desc:
//------------------------------------------------------------------------------
AnalyzerOptionsPage::~AnalyzerOptionsPage() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *event)
// Desc:
//------------------------------------------------------------------------------
void AnalyzerOptionsPage::showEvent(QShowEvent *event) {
	Q_UNUSED(event);

	QSettings settings;
	ui->checkBox->setChecked(settings.value("Analyzer/fuzzy_logic_functions.enabled", true).toBool());
}

//------------------------------------------------------------------------------
// Name: on_checkBox_toggled(bool checked)
// Desc:
//------------------------------------------------------------------------------
void AnalyzerOptionsPage::on_checkBox_toggled(bool checked) {
	Q_UNUSED(checked);

	QSettings settings;
	settings.setValue("Analyzer/fuzzy_logic_functions.enabled", ui->checkBox->isChecked());
}
