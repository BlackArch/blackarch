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

#include "X86Breakpoint.h"
#include "DebuggerCoreInterface.h"
#include "Debugger.h"
namespace {
const quint8 BreakpointInstruction[X86Breakpoint::size] = {0xcc};
}

//------------------------------------------------------------------------------
// Name: Breakpoint(edb::address_t address, bool onetime)
// Desc: constructor
//------------------------------------------------------------------------------
X86Breakpoint::X86Breakpoint(edb::address_t address) : address_(address), hit_count_(0), enabled_(false), one_time_(false), internal_(false) {
	enable();
}

//------------------------------------------------------------------------------
// Name: ~X86Breakpoint()
// Desc:
//------------------------------------------------------------------------------
X86Breakpoint::~X86Breakpoint() {
	disable();
}

//------------------------------------------------------------------------------
// Name: enable()
// Desc:
//------------------------------------------------------------------------------
bool X86Breakpoint::enable() {
	if(!enabled()) {
		char prev[size];
		if(edb::v1::debugger_core->read_bytes(address(), prev, size)) {
			if(edb::v1::debugger_core->write_bytes(address(), BreakpointInstruction, size)) {
				original_bytes_ = QByteArray(prev, size);
				enabled_ = true;
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: disable()
// Desc:
//------------------------------------------------------------------------------
bool X86Breakpoint::disable() {
	if(enabled()) {
		if(edb::v1::debugger_core->write_bytes(address(), original_bytes_, size)) {
			enabled_ = false;
			return true;
		}
	}
	return false;
}
