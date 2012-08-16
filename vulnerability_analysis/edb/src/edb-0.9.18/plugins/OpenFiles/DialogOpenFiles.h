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

#ifndef DIALOGOPENFILES_20070423_H_
#define DIALOGOPENFILES_20070423_H_

#include "Types.h"

#include <QDialog>
#include <QString>
#include <QFile>
#include <QTextStream>

namespace Ui { class DialogOpenFiles; }

class DialogOpenFiles : public QDialog {
	Q_OBJECT

public:
	DialogOpenFiles(QWidget *parent = 0);
	virtual ~DialogOpenFiles();

public Q_SLOTS:
	void on_btnRefresh_clicked();

private:
	virtual void showEvent(QShowEvent *event);

private:
	void do_find();

private:
	static QString file_type(const QString &filename);
	static QString process_socket_tcp(QString &symlink);
	static QString process_socket_udp(QString &symlink);
	static QString process_socket_unix(QString &symlink);

private:
	Ui::DialogOpenFiles *const ui;
};

#endif
