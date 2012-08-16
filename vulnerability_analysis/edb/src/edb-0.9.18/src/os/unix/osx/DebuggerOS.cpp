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

#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "MemoryRegions.h"

#include <QtDebug>
#include <QTextStream>
#include <QFile>

//------------------------------------------------------------------------------
// Name: primary_code_region()
// Desc:
//------------------------------------------------------------------------------
EDB_EXPORT MemRegion edb::v1::primary_code_region() {

	const QString process_executable = get_process_exe();

	memory_regions().sync();

	const QList<MemRegion> r = memory_regions().regions();
	Q_FOREACH(const MemRegion &region, r) {
		if(region.executable() && region.name == process_executable) {
			return region;
		}
	}
	return MemRegion();
}

//------------------------------------------------------------------------------
// Name: loaded_libraries()
// Desc:
//------------------------------------------------------------------------------
QStringList edb::v1::loaded_libraries() {
	QStringList ret;

	memory_regions().sync();

	const QList<MemRegion> r = memory_regions().regions();
	Q_FOREACH(const MemRegion &region, r) {
		Q_UNUSED(region);
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: get_process_exe()
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::get_process_exe() {
	QString ret;

	if(debugger_core != 0) {
		// TODO: assert attached!
		const edb::pid_t pid = debugger_core->pid();
		Q_UNUSED(pid);
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: get_process_cwd()
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::get_process_cwd() {
	QString ret;
	if(debugger_core != 0) {
		// TODO: assert attached!
		const edb::pid_t pid = debugger_core->pid();
		Q_UNUSED(pid);
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: get_process_args()
// Desc:
//------------------------------------------------------------------------------
QStringList edb::v1::get_process_args() {
	QStringList ret;
	if(debugger_core != 0) {
		// TODO: assert attached!
		const edb::pid_t pid = debugger_core->pid();
		Q_UNUSED(pid);
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: get_parent_pid(edb::pid_t pid)
// Desc:
//------------------------------------------------------------------------------
edb::pid_t edb::v1::get_parent_pid(edb::pid_t pid) {
    Q_UNUSED(pid);
    return 0;
}
