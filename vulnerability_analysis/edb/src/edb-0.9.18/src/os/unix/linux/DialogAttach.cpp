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

#include "DialogAttach.h"
#include "Types.h"

#include <QtDebug>
#include <QMap>
#include <QDir>
#include <QTextStream>

#include <pwd.h>

//------------------------------------------------------------------------------
// Name: DialogAttach(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
DialogAttach::DialogAttach(QWidget *parent) : DialogAttachUNIX(parent) {
}

//------------------------------------------------------------------------------
// Name: find_processes()
// Desc:
//------------------------------------------------------------------------------
QMap<edb::pid_t, DialogAttachUNIX::ProcessInfo> DialogAttach::find_processes() {
	QMap<edb::pid_t, ProcessInfo> procs;

	QDir proc_directory("/proc/");
	QFileInfoList entries = proc_directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	Q_FOREACH(const QFileInfo &info, entries) {
		if(is_numeric(info.fileName())) {

			const QDir proc(info.absoluteFilePath());
			const QString status = proc.absolutePath() + "/stat";

			ProcessInfo process_info;
			process_info.pid = info.fileName().toUInt();
			process_info.uid = info.ownerId();

			if(struct passwd *const pwd = ::getpwuid(process_info.uid)) {
				process_info.user = pwd->pw_name;
			}

			QFile file(status);
	        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream in(&file);

				int pid;
				QString name;
				// we already know the pid, so this'll just get read and ignored
				in >> pid;
				// get the name
				in >> name;
				// chop off the silly ( and )
				process_info.name = name.mid(1);
				process_info.name.chop(1);

			}
			procs.insert(process_info.pid, process_info);
		}
	}
	return procs;
}
