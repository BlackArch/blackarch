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

#ifndef DIALOGOPTIONS_20061101_H_
#define DIALOGOPTIONS_20061101_H_

#include <QDialog>

namespace Ui { class DialogOptions; }

class QToolBox;

class DialogOptions : public QDialog {
	Q_OBJECT
public:
	DialogOptions(QWidget *parent = 0);
	virtual ~DialogOptions();

public Q_SLOTS:
	void on_btnStackFont_clicked();
	void on_btnMemoryFont_clicked();
	void on_btnDisFont_clicked();
	void on_btnRegisterFont_clicked();
	void on_btnSymbolDir_clicked();
	void on_btnPluginDir_clicked();
	void on_btnTTY_clicked();
	void on_btnSessionDir_clicked();
	void closeEvent(QCloseEvent *event);
	void accept();

public:
	virtual void showEvent(QShowEvent *event);
	void addOptionsPage(QWidget *page);

private:
	QString font_from_dialog(const QString &default_font);
	QString directory_from_dialog();

private:
	Ui::DialogOptions *const ui;
	QToolBox *               toolbox_;
};

#endif
