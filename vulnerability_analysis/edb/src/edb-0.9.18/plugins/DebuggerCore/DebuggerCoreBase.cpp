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

#include "DebuggerCoreBase.h"
#include "X86Breakpoint.h"

//------------------------------------------------------------------------------
// Name: DebuggerCoreBase()
// Desc: constructor
//------------------------------------------------------------------------------
DebuggerCoreBase::DebuggerCoreBase() : active_thread_(0), pid_(0) {
}

//------------------------------------------------------------------------------
// Name: ~DebuggerCoreBase()
// Desc: destructor
//------------------------------------------------------------------------------
DebuggerCoreBase::~DebuggerCoreBase() {
}

//------------------------------------------------------------------------------
// Name: clear_breakpoints()
// Desc: removes all breakpoints
//------------------------------------------------------------------------------
void DebuggerCoreBase::clear_breakpoints() {
	if(attached()) {
		breakpoints_.clear();
	}
}

//------------------------------------------------------------------------------
// Name: add_breakpoint(edb::address_t address)
// Desc: creates a new breakpoint
//       (only if there isn't already one at the given address)
//------------------------------------------------------------------------------
Breakpoint::pointer DebuggerCoreBase::add_breakpoint(edb::address_t address) {

	if(attached()) {
		if(!find_breakpoint(address)) {
			Breakpoint::pointer bp(new X86Breakpoint(address));
			breakpoints_[address] = bp;
			return bp;
		}
	}

	return Breakpoint::pointer();
}

//------------------------------------------------------------------------------
// Name: find_breakpoint(edb::address_t address) const
// Desc: returns the breakpoint at the given address or Breakpoint::pointer()
//------------------------------------------------------------------------------
Breakpoint::pointer DebuggerCoreBase::find_breakpoint(edb::address_t address) {
	if(attached()) {
		const BreakpointState::const_iterator it = breakpoints_.find(address);
		if(it != breakpoints_.end()) {
			return it.value();
		}
	}
	return Breakpoint::pointer();
}


//------------------------------------------------------------------------------
// Name: remove_breakpoint(edb::address_t address)
// Desc: removes the breakpoint at the given address, this is a no-op if there
//       is no breakpoint present.
// Note: if another part of the code has a reference to the BP, it will not
//       actually be removed until it releases the reference.
//------------------------------------------------------------------------------
void DebuggerCoreBase::remove_breakpoint(edb::address_t address) {

	// TODO: assert paused
	if(attached()) {
		const BreakpointState::iterator it = breakpoints_.find(address);
		if(it != breakpoints_.end()) {
			breakpoints_.erase(it);
		}
	}
}

//------------------------------------------------------------------------------
// Name: backup_breakpoints() const
// Desc: returns a copy of the BP list, these count as references to the BPs
//       preventing full removal until this list is destructed.
//------------------------------------------------------------------------------
DebuggerCoreBase::BreakpointState DebuggerCoreBase::backup_breakpoints() const {
	return breakpoints_;
}

//------------------------------------------------------------------------------
// Name: open(const QString &path, const QString &cwd, const QStringList &args)
// Desc: executes the given program
//------------------------------------------------------------------------------
bool DebuggerCoreBase::open(const QString &path, const QString &cwd, const QStringList &args) {
	return open(path, cwd, args, QString());
}

//------------------------------------------------------------------------------
// Name: pid() const
// Desc: returns the pid of the currently debugged process (0 if not attached)
//------------------------------------------------------------------------------
edb::pid_t DebuggerCoreBase::pid() const {
	return pid_;
}

//------------------------------------------------------------------------------
// Name: attached() const
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCoreBase::attached() const {
	return pid() != 0;
}

//------------------------------------------------------------------------------
// Name: thread_ids() const
// Desc:
//------------------------------------------------------------------------------
QList<edb::tid_t> DebuggerCoreBase::thread_ids() const {
	return QList<edb::tid_t>();
}

//------------------------------------------------------------------------------
// Name: active_thread() const
// Desc:
//------------------------------------------------------------------------------
edb::tid_t DebuggerCoreBase::active_thread() const {
	return active_thread_;
}

//------------------------------------------------------------------------------
// Name: set_active_thread(edb::tid_t)
// Desc:
//------------------------------------------------------------------------------
void DebuggerCoreBase::set_active_thread(edb::tid_t) {
}

//------------------------------------------------------------------------------
// Name: breakpoint_size() const
// Desc:
//------------------------------------------------------------------------------
int DebuggerCoreBase::breakpoint_size() const {
	return X86Breakpoint::size;
}
