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
#include "MemoryRegions.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"

//------------------------------------------------------------------------------
// Name: ~MemRegion()
// Desc:
//------------------------------------------------------------------------------
MemRegion::~MemRegion() {
}

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
// Name: set_permissions(bool read, bool write, bool execute)
// Desc: wrapper around normal set permissions
//------------------------------------------------------------------------------
void MemRegion::set_permissions(bool read, bool write, bool execute) {

	HANDLE ph = OpenProcess(PROCESS_VM_OPERATION, FALSE, edb::v1::debugger_core->pid());
	if(ph != 0) {
		DWORD prot = PAGE_NOACCESS;

		switch((static_cast<int>(read) << 2) | (static_cast<int>(write) << 1) | static_cast<int>(execute)) {
		case 0x0: prot = PAGE_NOACCESS;          break;
		case 0x1: prot = PAGE_EXECUTE;           break;
		case 0x2: prot = PAGE_WRITECOPY;         break;
		case 0x3: prot = PAGE_EXECUTE_WRITECOPY; break;
		case 0x4: prot = PAGE_READONLY;          break;
		case 0x5: prot = PAGE_EXECUTE_READ;      break;
		case 0x6: prot = PAGE_READWRITE;         break;
		case 0x7: prot = PAGE_EXECUTE_READWRITE; break;
		}

		prot |= permissions_ & ~KNOWN_PERMISSIONS; // keep modifiers

		DWORD prev_prot;
		if(VirtualProtectEx(ph, reinterpret_cast<LPVOID>(start), size(), prot, &prev_prot)) {
			permissions_ = prot;
		}
		CloseHandle(ph);
	}
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
	switch(permissions_ & KNOWN_PERMISSIONS) { // ignore modifiers
		case PAGE_EXECUTE_READ:
		case PAGE_EXECUTE_READWRITE:
		case PAGE_READONLY:
		case PAGE_READWRITE:
			return true;
		default:
			return false;
	}
}

//------------------------------------------------------------------------------
// Name: writable() const
// Desc:
//------------------------------------------------------------------------------
bool MemRegion::writable() const {
	switch(permissions_ & KNOWN_PERMISSIONS) { // ignore modifiers
		case PAGE_EXECUTE_READWRITE:
		case PAGE_EXECUTE_WRITECOPY:
		case PAGE_READWRITE:
		case PAGE_WRITECOPY:
			return true;
		default:
			return false;
	}
}

//------------------------------------------------------------------------------
// Name: executable() const
// Desc:
//------------------------------------------------------------------------------
bool MemRegion::executable() const {
	switch(permissions_ & KNOWN_PERMISSIONS) { // ignore modifiers
		case PAGE_EXECUTE:
		case PAGE_EXECUTE_READ:
		case PAGE_EXECUTE_READWRITE:
		case PAGE_EXECUTE_WRITECOPY:
			return true;
		default:
			return false;
	}
}

//------------------------------------------------------------------------------
// Name: size() const
// Desc:
//------------------------------------------------------------------------------
int MemRegion::size() const {
	return end - start;
}
