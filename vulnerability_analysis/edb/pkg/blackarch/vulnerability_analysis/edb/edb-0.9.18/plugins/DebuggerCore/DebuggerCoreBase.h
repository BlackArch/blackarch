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

#ifndef DEBUGGERCOREBASE_20090529_H_
#define DEBUGGERCOREBASE_20090529_H_

#include "DebuggerCoreInterface.h"

class DebuggerCoreBase : public QObject, public DebuggerCoreInterface {
public:
	DebuggerCoreBase();
	virtual ~DebuggerCoreBase();

public:
	// thread support stuff (optional)
	virtual QList<edb::tid_t> thread_ids() const;
	virtual edb::tid_t active_thread() const;
	virtual void set_active_thread(edb::tid_t);

public:
	virtual bool open(const QString &path, const QString &cwd, const QStringList &args);
	virtual bool open(const QString &path, const QString &cwd, const QStringList &args, const QString &tty) = 0;

public:
	virtual BreakpointState backup_breakpoints() const;
	virtual Breakpoint::pointer add_breakpoint(edb::address_t address);
	virtual Breakpoint::pointer find_breakpoint(edb::address_t address);
	virtual int breakpoint_size() const;
	virtual void clear_breakpoints();
	virtual void remove_breakpoint(edb::address_t address);

public:
	virtual edb::pid_t pid() const;

protected:
	bool attached() const;

protected:
	edb::tid_t      active_thread_;
	edb::pid_t      pid_;
	BreakpointState breakpoints_;
};

#endif
