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

#ifndef DEBUGGERUI_20090811_H_
#define DEBUGGERUI_20090811_H_

#include "DataViewInfo.h"
#include "Debugger.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QVector>

#include <cstring>

namespace Ui { class DebuggerUI; }

class QToolButton;

class DebuggerUI : public QMainWindow {
	Q_OBJECT

public:
	DebuggerUI(QWidget *parent = 0);
	virtual ~DebuggerUI();

protected:
	enum GUI_STATE {
		PAUSED,
		RUNNING,
		TERMINATED
	};

public:
	int current_tab() const;
	DataViewInfo::pointer current_data_view_info() const;
	void finish_plugin_setup(const QHash<QString, QObject *> &plugins);

protected:
	void update_menu_state(GUI_STATE state);
	QString create_tty();
	void set_debugger_caption(const QString &appname);
	void delete_data_tab();
	void create_data_tab();
	edb::reg_t get_follow_register(bool &ok) const;
	edb::address_t get_goto_expression(bool &ok);

private Q_SLOTS:
	void tty_proc_finished(int exit_code, QProcess::ExitStatus exit_status);

public:
	Ui::DebuggerUI *const          ui;

protected:
	QToolButton *                  add_tab_;
	QToolButton *                  del_tab_;
	QProcess *                     tty_proc_;
	GUI_STATE                      gui_state_;
	QString                        tty_file_;
	QVector<DataViewInfo::pointer> data_regions_;
	DataViewInfo                   stack_view_info_;
};

#endif

