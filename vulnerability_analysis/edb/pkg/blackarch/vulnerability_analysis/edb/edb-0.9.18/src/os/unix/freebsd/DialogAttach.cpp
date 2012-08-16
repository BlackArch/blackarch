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

#include <fcntl.h>
#include <kvm.h>

#include <paths.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>

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

	char ebuffer[_POSIX2_LINE_MAX];
	int numprocs;
	if(kvm_t *const kaccess = kvm_openfiles(_PATH_DEVNULL, _PATH_DEVNULL, 0, O_RDONLY, ebuffer)) {
		if(struct kinfo_proc *const kprocaccess = kvm_getprocs(kaccess, KERN_PROC_ALL, 0, &numprocs)) {
			for(int i = 0; i < numprocs; ++i) {
				ProcessInfo procInfo;

				procInfo.pid   = kprocaccess[i].ki_pid;
				procInfo.uid   = kprocaccess[i].ki_uid;
				procInfo.name  = kprocaccess[i].ki_comm;
				procs.insert(procInfo.pid, procInfo);
			}
		}
		kvm_close(kaccess);
	} else {
		QMessageBox::warning(this, "Error Listing Processes", ebuffer);
	}

	return procs;
}
