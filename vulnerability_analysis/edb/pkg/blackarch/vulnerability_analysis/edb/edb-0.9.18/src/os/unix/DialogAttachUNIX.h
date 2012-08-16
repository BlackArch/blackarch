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

#ifndef DIALOGATTACHUNIX_20091218_H_
#define DIALOGATTACHUNIX_20091218_H_

#include "Types.h"
#include <QString>
#include <QDialog>

namespace Ui { class DialogAttach; }

class DialogAttachUNIX : public QDialog {
	Q_OBJECT

protected:
	struct ProcessInfo {
		edb::pid_t pid;
		edb::uid_t uid;
		QString    user;
		QString    name;
	};

public:
	DialogAttachUNIX(QWidget *parent = 0);
	virtual ~DialogAttachUNIX();

private:
	virtual void showEvent(QShowEvent *event);

private:
	void update_list(const QString &filter);

protected:
	virtual QMap<edb::pid_t, ProcessInfo> find_processes() = 0;

public Q_SLOTS:
	void on_filter_textChanged(const QString &text);
	void on_filter_uid_clicked(bool checked);

public:
	edb::pid_t selected_pid(bool &ok) const;

protected:
	static bool is_numeric(const QString &s);

private:
	 Ui::DialogAttach *const ui;
};

#endif

