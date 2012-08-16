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

#ifndef DEBUGGERCOREINTERFACE_20061101_H_
#define DEBUGGERCOREINTERFACE_20061101_H_

#include <QtPlugin>
#include <QByteArray>
#include <QHash>
#include <QStringList>
#include "Breakpoint.h"
#include "State.h"

class QString;
class DebugEvent;

class DebuggerCoreInterface {
public:
	virtual ~DebuggerCoreInterface() {}

public:
	// system information
	virtual edb::address_t page_size() const = 0;
	virtual int pointer_size() const = 0;

public:
	typedef QHash<edb::address_t, Breakpoint::pointer > BreakpointState;

public:
	virtual bool has_extension(quint64 ext) const = 0;

public:
	// returns true on success, false on failure, all bytes must be successfully
	// read/written in order for a success. The debugged application should be stopped
	// or this will return false immediately.
	virtual bool write_bytes(edb::address_t address, const void *buf, std::size_t len) = 0;
	virtual bool read_bytes(edb::address_t address, void *buf, std::size_t len) = 0;
	virtual bool read_pages(edb::address_t address, void *buf, std::size_t count) = 0;

public:
	// thread support stuff (optional)
	virtual QList<edb::tid_t> thread_ids() const { return QList<edb::tid_t>(); }
	virtual edb::tid_t active_thread() const     { return static_cast<edb::tid_t>(-1); }
	virtual void set_active_thread(edb::tid_t)   {}

public:
	virtual bool attach(edb::pid_t pid) = 0;
	virtual bool open(const QString &path, const QString &cwd, const QStringList &args) = 0;
	virtual bool open(const QString &path, const QString &cwd, const QStringList &args, const QString &tty) = 0;
	virtual bool wait_debug_event(DebugEvent &event, int msecs) = 0;
	virtual void detach() = 0;
	virtual void get_state(State &state) = 0;
	virtual void kill() = 0;
	virtual void pause() = 0;
	virtual void resume(edb::EVENT_STATUS status) = 0;
	virtual void set_state(const State &state) = 0;
	virtual void step(edb::EVENT_STATUS status) = 0;

public:
	// basic breakpoint managment
	virtual BreakpointState backup_breakpoints() const = 0;
	virtual Breakpoint::pointer add_breakpoint(edb::address_t address) = 0;
	virtual Breakpoint::pointer find_breakpoint(edb::address_t address) = 0;
	virtual int breakpoint_size() const = 0;
	virtual void clear_breakpoints() = 0;
	virtual void remove_breakpoint(edb::address_t address) = 0;

public:
	virtual StateInterface *create_state() const = 0;

public:
	// what is the PID of the process we are currently debugging
	// equal to "(edb::pid_t)0" if we are not attached
	virtual edb::pid_t pid() const = 0;
};

Q_DECLARE_INTERFACE(DebuggerCoreInterface, "EDB.DebuggerCoreInterface/1.0")

#endif
