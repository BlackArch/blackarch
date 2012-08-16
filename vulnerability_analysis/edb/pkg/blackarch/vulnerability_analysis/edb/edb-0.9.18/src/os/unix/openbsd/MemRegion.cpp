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

#include "MemRegion.h"
#include "Types.h"
#include "Util.h"
#include "State.h"
#include "DebuggerCoreInterface.h"
#include "DebugEventHandlerInterface.h"
#include "MemoryRegions.h"
#include "Debugger.h"

#include <QtGlobal>
#include <QMessageBox>
#include <QApplication>
#include <cstring>

#include <sys/syscall.h>
#include <sys/mman.h>

//------------------------------------------------------------------------------
// Name: swap(MemRegion &other)
// Desc:
//------------------------------------------------------------------------------
void MemRegion::swap(MemRegion &other) {
	using std::swap;
	swap(start, other.start);
	swap(end, other.end);
	swap(base, other.base);
	swap(name, other.name);
	swap(permissions_, other.permissions_);
}

//------------------------------------------------------------------------------
// Name: accessible() const
// Desc:
//------------------------------------------------------------------------------
bool MemRegion::accessible() const {
	return readable() || writable() || executable();
}

//------------------------------------------------------------------------------
// Name: readable() const
// Desc:
//------------------------------------------------------------------------------
bool MemRegion::readable() const {
	return (permissions_ & PROT_READ) != 0;
}

//------------------------------------------------------------------------------
// Name: writable() const
// Desc:
//------------------------------------------------------------------------------
bool MemRegion::writable() const {
	return (permissions_ & PROT_WRITE) != 0;
}

//------------------------------------------------------------------------------
// Name: executable() const
// Desc:
//------------------------------------------------------------------------------
bool MemRegion::executable() const {
	return (permissions_ & PROT_EXEC) != 0;
}

//------------------------------------------------------------------------------
// Name: size() const
// Desc:
//------------------------------------------------------------------------------
int MemRegion::size() const {
	return end - start;
}

//------------------------------------------------------------------------------
// Name: ~MemRegion()
// Desc:
//------------------------------------------------------------------------------
MemRegion::~MemRegion() {
}

//------------------------------------------------------------------------------
// Name: set_permissions(bool read, bool write, bool execute)
// Desc: wrapper around normal set permissions
//------------------------------------------------------------------------------
void MemRegion::set_permissions(bool read, bool write, bool execute) {
	// TODO: implement this
}
