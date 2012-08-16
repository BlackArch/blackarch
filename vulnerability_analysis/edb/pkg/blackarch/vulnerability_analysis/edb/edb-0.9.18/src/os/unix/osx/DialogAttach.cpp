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
#include <QMessageBox>

#include <sys/types.h>
#include <unistd.h>
#include <paths.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/proc.h>

//------------------------------------------------------------------------------
// Name: DialogAttachUNIX(QWidget *parent)
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

	static const int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
	size_t length = 0;

	sysctl(const_cast<int*>(name), (sizeof(name) / sizeof(*name)) - 1, 0, &length, 0, 0);
	struct kinfo_proc *proc_info = static_cast<struct kinfo_proc*>(malloc(length));
	sysctl(const_cast<int*>(name), (sizeof(name) / sizeof(*name)) - 1, proc_info, &length, 0, 0);

	size_t count = length / sizeof(struct kinfo_proc);
	for(size_t i = 0; i < count; ++i) {
		ProcessInfo procInfo;
		procInfo.pid  = proc_info[i].kp_proc.p_pid;
		procInfo.uid  = proc_info[i].kp_eproc.e_ucred.cr_uid;
		procInfo.name = proc_info[i].kp_proc.p_comm;

		procs.insert(procInfo.pid, procInfo);
	}

	free(proc_info);

	return procs;
}
