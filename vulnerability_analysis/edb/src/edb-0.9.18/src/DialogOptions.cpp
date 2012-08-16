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

#include "DialogOptions.h"
#include "Debugger.h"
#include "Configuration.h"

#include <QFontDialog>
#include <QFileDialog>
#include <QFont>
#include <QCloseEvent>
#include <QToolBox>

#include "ui_dialog_options.h"

namespace {
//------------------------------------------------------------------------------
// Name: width_to_index(int n)
// Desc:
//------------------------------------------------------------------------------
int width_to_index(int n) {
	switch(n) {
	case 16: return 4;
	case 8:  return 3;
	case 4:  return 2;
	case 2:  return 1;
	default:
		return 0;
	}
}
}

//------------------------------------------------------------------------------
// Name: DialogOptions(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogOptions::DialogOptions(QWidget *parent) : QDialog(parent), ui(new Ui::DialogOptions), toolbox_(0) {
	ui->setupUi(this);
}

//------------------------------------------------------------------------------
// Name: ~DialogOptions()
// Desc:
//------------------------------------------------------------------------------
DialogOptions::~DialogOptions() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: font_from_dialog(const QString &default_font)
// Desc:
//------------------------------------------------------------------------------
QString DialogOptions::font_from_dialog(const QString &default_font) {
	QFont old_font;
	old_font.fromString(default_font);
	return QFontDialog::getFont(0, old_font, this).toString();
}

//------------------------------------------------------------------------------
// Name: addOptionsPage(QWidget *page)
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::addOptionsPage(QWidget *page) {

	if(toolbox_ == 0) {
		delete ui->tabWidget->findChild<QLabel *>("label_plugins");
		QWidget *const tab        = ui->tabWidget->findChild<QLabel *>("tab_plugins");
		QGridLayout *const layout = ui->tabWidget->findChild<QGridLayout *>("tab_plugins_layout");
		toolbox_ = new QToolBox(tab);
		layout->addWidget(toolbox_, 0, 0, 1, 1);
	}

	toolbox_->addItem(page, QIcon(":/debugger/images/edb32-preferences-plugin.png"), page->windowTitle());
}

//------------------------------------------------------------------------------
// Name: directory_from_dialog()
// Desc:
//------------------------------------------------------------------------------
QString DialogOptions::directory_from_dialog() {
	return QFileDialog::getExistingDirectory(
		this,
		tr("Choose a directory"),
		QString(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

//------------------------------------------------------------------------------
// Name: on_btnTTY_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnTTY_clicked() {
	const QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Choose Your Terminal Program"));

	ui->txtTTY->setText(filename);
}

//------------------------------------------------------------------------------
// Name: on_btnStackFont_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnStackFont_clicked() {
	ui->txtStackFont->setText(font_from_dialog(ui->txtStackFont->text()));
}

//------------------------------------------------------------------------------
// Name: on_btnMemoryFont_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnMemoryFont_clicked() {
	ui->txtMemoryFont->setText(font_from_dialog(ui->txtMemoryFont->text()));
}

//------------------------------------------------------------------------------
// Name: on_btnRegisterFont_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnRegisterFont_clicked() {
	ui->txtRegisterFont->setText(font_from_dialog(ui->txtRegisterFont->text()));
}

//------------------------------------------------------------------------------
// Name: on_btnDisFont_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnDisFont_clicked() {
	ui->txtDisFont->setText(font_from_dialog(ui->txtDisFont->text()));
}

//------------------------------------------------------------------------------
// Name: on_btnSymbolDir_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnSymbolDir_clicked() {
	const QString s = directory_from_dialog();

	if(!s.isEmpty()) {
		ui->txtSymbolDir->setText(s);
	}
}

//------------------------------------------------------------------------------
// Name: on_btnSessionDir_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnSessionDir_clicked() {
	const QString s = directory_from_dialog();

	if(!s.isEmpty()) {
		ui->txtSessionDir->setText(s);
	}
}

//------------------------------------------------------------------------------
// Name: on_btnPluginDir_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::on_btnPluginDir_clicked() {
	const QString s = directory_from_dialog();

	if(!s.isEmpty()) {
		ui->txtPluginDir->setText(s);
	}
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *event)
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::showEvent(QShowEvent *event) {
	QDialog::showEvent(event);
	
	const Configuration &config = edb::v1::config();

	ui->rdoSytntaxATT->setChecked(config.syntax == Configuration::ATT);
	ui->rdoSytntaxIntel->setChecked(config.syntax != Configuration::ATT);

	ui->rdoDetach->setChecked(config.close_behavior == Configuration::Detach);
	ui->rdoKill->setChecked(config.close_behavior != Configuration::Detach);

	ui->rdoBPEntry->setChecked(config.initial_breakpoint == Configuration::EntryPoint);
	ui->rdoBPMain->setChecked(config.initial_breakpoint != Configuration::EntryPoint);

	ui->chkTTY->setChecked(config.tty_enabled);
	ui->txtTTY->setText(config.tty_command);

	ui->chkZerosAreFilling->setChecked(config.zeros_are_filling);
	ui->chkUppercase->setChecked(config.uppercase_disassembly);

	ui->chkFindMain->setChecked(config.find_main);
	ui->chkWarnDataBreakpoint->setChecked(config.warn_on_no_exec_bp);

	ui->spnMinString->setValue(config.min_string_length);

	ui->txtStackFont->setText(config.stack_font);
	ui->txtMemoryFont->setText(config.data_font);
	ui->txtRegisterFont->setText(config.registers_font);
	ui->txtDisFont->setText(config.disassembly_font);

	ui->txtSymbolDir->setText(config.symbol_path);
	ui->txtPluginDir->setText(config.plugin_path);
	ui->txtSessionDir->setText(config.session_path);

	ui->chkDataShowAddress->setChecked(config.data_show_address);
	ui->chkDataShowHex->setChecked(config.data_show_hex);
	ui->chkDataShowAscii->setChecked(config.data_show_ascii);
	ui->chkDataShowComments->setChecked(config.data_show_comments);
	ui->cmbDataWordWidth->setCurrentIndex(width_to_index(config.data_word_width));
	ui->cmbDataRowWidth->setCurrentIndex(width_to_index(config.data_row_width));

	ui->chkAddressSemicolon->setChecked(config.show_address_separator);
}

//------------------------------------------------------------------------------
// Name: closeEvent(QCloseEvent *event)
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::closeEvent(QCloseEvent *event) {

	Configuration &config = edb::v1::config();

	if(ui->rdoSytntaxIntel->isChecked()) {
		config.syntax = Configuration::Intel;
	} else if(ui->rdoSytntaxATT->isChecked()) {
		config.syntax = Configuration::ATT;
	}

	if(ui->rdoDetach->isChecked()) {
		config.close_behavior = Configuration::Detach;
	} else if(ui->rdoKill->isChecked()) {
		config.close_behavior = Configuration::Terminate;
	}

	config.stack_font            = ui->txtStackFont->text();
	config.data_font             = ui->txtMemoryFont->text();
	config.registers_font        = ui->txtRegisterFont->text();
	config.disassembly_font      = ui->txtDisFont->text();
	config.tty_command           = ui->txtTTY->text();
	config.tty_enabled           = ui->chkTTY->isChecked();
	config.zeros_are_filling     = ui->chkZerosAreFilling->isChecked();
	config.uppercase_disassembly = ui->chkUppercase->isChecked();

	config.symbol_path           = ui->txtSymbolDir->text();
	config.plugin_path           = ui->txtPluginDir->text();
	config.session_path          = ui->txtSessionDir->text();

	if(ui->rdoBPMain->isChecked()) {
		config.initial_breakpoint = Configuration::MainSymbol;
	} else if(ui->rdoBPEntry->isChecked()) {
		config.initial_breakpoint = Configuration::EntryPoint;
	}

	config.warn_on_no_exec_bp     = ui->chkWarnDataBreakpoint->isChecked();
	config.find_main              = ui->chkFindMain->isChecked();

	config.show_address_separator = ui->chkAddressSemicolon->isChecked();

	config.min_string_length      = ui->spnMinString->value();

	config.data_show_address  = ui->chkDataShowAddress->isChecked();
	config.data_show_hex      = ui->chkDataShowHex->isChecked();
	config.data_show_ascii    = ui->chkDataShowAscii->isChecked();
	config.data_show_comments = ui->chkDataShowComments->isChecked();
	config.data_word_width    = 1 << ui->cmbDataWordWidth->currentIndex();
	config.data_row_width     = 1 << ui->cmbDataRowWidth->currentIndex();

	event->accept();
}

//------------------------------------------------------------------------------
// Name: accept()
// Desc:
//------------------------------------------------------------------------------
void DialogOptions::accept() {
	close();
}
