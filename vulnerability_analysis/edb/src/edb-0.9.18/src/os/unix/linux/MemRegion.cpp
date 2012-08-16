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
#include "DebugEventHandlerInterface.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "MemoryRegions.h"
#include "State.h"
#include "Types.h"
#include "Util.h"

#include <QtGlobal>
#include <QAtomicInt>
#include <QMessageBox>
#include <QApplication>
#include <cstring>

#include <asm/unistd.h>
#include <sys/mman.h>

namespace {
	//------------------------------------------------------------------------------
	// Name: permissions_value(bool read, bool write, bool execute)
	// Desc:
	//------------------------------------------------------------------------------
	MemRegion::permissions_t permissions_value(bool read, bool write, bool execute) {
		MemRegion::permissions_t perms = 0;
		if(read)    perms |= PROT_READ;
		if(write)   perms |= PROT_WRITE;
		if(execute) perms |= PROT_EXEC;
		return perms;
	}
}


template <size_t N>
class BackupInfo : public DebugEventHandlerInterface {
public:
	BackupInfo(edb::address_t address, quint8 perms, MemRegion &region);

private:
	Q_DISABLE_COPY(BackupInfo)

public:
	quint8 perms() const { return premissions_; }
	bool locked()        { return !lock_.testAndSetAcquire(0, 1); }

public:
	bool backup();
	bool restore();

public:
	virtual edb::EVENT_STATUS handle_event(const DebugEvent &event);

public:
	QAtomicInt     lock_;
	edb::address_t address_;
	quint8         premissions_;
	State          state_;
	quint8         buffer_[N];
	MemRegion&     region_;

public:
	DebugEventHandlerInterface *event_handler_;
};

//------------------------------------------------------------------------------
// Name: BackupInfo(td::size_t size, edb::address_t address, quint8 perms, MemRegion &region)
// Desc:
//------------------------------------------------------------------------------
template <size_t N>
BackupInfo<N>::BackupInfo(edb::address_t address, quint8 perms, MemRegion &region) :
		lock_(1), address_(address), premissions_(perms), region_(region), event_handler_(0) {
}

//------------------------------------------------------------------------------
// Name: backup()
// Desc:
//------------------------------------------------------------------------------
template <size_t N>
bool BackupInfo<N>::backup() {
	edb::v1::debugger_core->get_state(state_);
	return edb::v1::debugger_core->read_bytes(address_, buffer_, N);
}

//------------------------------------------------------------------------------
// Name: restore()
// Desc:
//------------------------------------------------------------------------------
template <size_t N>
bool BackupInfo<N>::restore() {
	edb::v1::debugger_core->set_state(state_);
	return edb::v1::debugger_core->write_bytes(address_, buffer_, N);
}

//------------------------------------------------------------------------------
// Name: handle_event(const DebugEvent &event)
// Desc:
//------------------------------------------------------------------------------
template <size_t N>
edb::EVENT_STATUS BackupInfo<N>::handle_event(const DebugEvent &event) {
	Q_UNUSED(event);

	// TODO: check that the event was caused by a hlt in our shellcode
	lock_.testAndSetRelease(1, 0);

	// restore the original code and register state
	restore();

	// update permissions mask
	region_.permissions_ = perms();

	// restore the event handler
	edb::v1::set_debug_event_handler(event_handler_);

	// really shouldn't matter since the return value isn't used at all
	// we simply want tot catch the event and set the lock to 0
	return edb::DEBUG_STOP;
}

//------------------------------------------------------------------------------
// Name: swap(MemRegion &other)
// Desc:
//------------------------------------------------------------------------------
void MemRegion::swap(MemRegion &other) {
	qSwap(start, other.start);
	qSwap(end, other.end);
	qSwap(base, other.base);
	qSwap(name, other.name);
	qSwap(permissions_, other.permissions_);
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
// Name: set_permissions(bool read, bool write, bool execute)
// Desc: attempts to change the permissions of a given region by injecting
//       shellcode into the debugged process, and executing a mprotect syscall
//       we probably need to ensure that the debug event server thread is not
//       waiting for an event at this moment because we need to catch the 'hlt'
//       instruction we injected in order to restore the original code
//       we also probably need this code to work perfectly in order to implement
//       memory access breakpoints
//------------------------------------------------------------------------------
void MemRegion::set_permissions(bool read, bool write, bool execute, edb::address_t temp_address) {

	const permissions_t perms              = permissions_value(read, write, execute);
	const edb::address_t len               = size();
	const edb::address_t addr              = start;
	static const edb::address_t syscallnum = __NR_mprotect;

	// start of nowhere near portable code
#if defined(EDB_X86)
	quint8 shellcode[] = {
		"\xcd\x80" // int $0x80
		"\xf4"     // hlt
	};
#elif defined(EDB_X86_64)
	quint8 shellcode[] = {
		"\x0f\x05" // syscall
		"\xf4"     // hlt
	};
#else
#error "invalid architecture"
#endif
	// end nowhere near portable code

	typedef BackupInfo<sizeof(shellcode)> BI;
	try {
		BI backup_info(temp_address, perms, *this);

		if(backup_info.backup()) {
			// write out our shellcode
			if(edb::v1::debugger_core->write_bytes(temp_address, shellcode, sizeof(shellcode))) {

				State state;
				state.set_instruction_pointer(temp_address);

#if defined(EDB_X86)
				state.set_register("ebx", len);
				state.set_register("ecx", addr);
				state.set_register("edx", perms);
				state.set_register("eax", syscallnum);
#elif defined(EDB_X86_64)
				state.set_register("rsi", len);
				state.set_register("rdi", addr);
				state.set_register("rdx", perms);
				state.set_register("rax", syscallnum);
#else
#error "invalid architecture"
#endif
				edb::v1::debugger_core->set_state(state);

				backup_info.event_handler_ = edb::v1::set_debug_event_handler(&backup_info);

				// run and wait for the 'crash' caused by the hlt instruction
				// should be a SIGSEGV on Linux
				edb::v1::debugger_core->resume(edb::DEBUG_CONTINUE);

				// we use a spinlock here because we want to be able to
				// process events while waiting
				while(backup_info.locked()) {
					QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
				}
			}
		}
	} catch(const std::bad_alloc &) {
		QMessageBox::information(
			0,
			tr("Memroy Allocation Error"),
			tr("Unable to satisfy memory allocation request for backup code."));
	}
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
	edb::address_t temp_address     = 0;
	int count                       = 0;
	int ret                         = QMessageBox::Yes;
	const QList<MemRegion> &regions = edb::v1::memory_regions().regions();

	// search for an executable region to run our shell code
	Q_FOREACH(const MemRegion &region, regions) {
		if(region.executable()) {
			if(temp_address == 0) {
				temp_address = region.start;
			}

			if(++count > 1) {
				break;
			}
		}
	}

	if(executable() && count == 1 && !execute) {
		ret = QMessageBox::question(0,
			tr("Removing Execute Permissions On Last Executable MemRegion"),
			tr("You are about to remove execute permissions from the last executable region. Because of the need "
			"to run code in the process to change permissions, there will be no way to undo this. In addition, "
			"the process will no longer be able to run as it will have no execute permissions in any regions. "
			"Odds are this is not what you want to do."
			"Are you sure you want to remove execute permissions from this region?"),
			QMessageBox::Yes, QMessageBox::No);
	}

	if(ret == QMessageBox::Yes) {
		if(temp_address != 0) {
			set_permissions(read, write, execute, temp_address);
		} else {
			QMessageBox::information(
				0,
				tr("No Suitable Address Found"),
				tr("This feature relies on running shellcode in the debugged process, no executable memory region was found. Unfortunately, this means that no more region permission changes can be made (it also means that there is nothing the process can continue to do since it cannot execute at all)."));
		}
	}
}
