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

#include "DebugEvent.h"
#include "Debugger.h"
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <cstdio>

// doesn't always seem to be defined in the headers
#ifndef PTRACE_GETSIGINFO
#define PTRACE_GETSIGINFO static_cast<__ptrace_request>(0x4202)
#endif

//------------------------------------------------------------------------------
// Name: DebugEvent()
// Desc: constructor
//------------------------------------------------------------------------------
DebugEvent::DebugEvent() : status(0), pid(-1), tid(-1) {
	std::memset(&siginfo, 0, sizeof(siginfo));
}

//------------------------------------------------------------------------------
// Name: DebugEvent(const DebugEvent &other)
// Desc: copy constructor
//------------------------------------------------------------------------------
DebugEvent::DebugEvent(const DebugEvent &other) : status(other.status), pid(other.pid), tid(other.tid) {
	std::memcpy(&siginfo, &other.siginfo, sizeof(siginfo));
}

//------------------------------------------------------------------------------
// Name: operator=(const DebugEvent &rhs)
// Desc:
//------------------------------------------------------------------------------
DebugEvent &DebugEvent::operator=(const DebugEvent &rhs) {
	status = rhs.status;
	pid    = rhs.pid;
	tid    = rhs.tid;
	std::memcpy(&siginfo, &rhs.siginfo, sizeof(siginfo));
	return *this;
}

//------------------------------------------------------------------------------
// Name: DebugEvent(int s, edb::pid_t p, edb::tid_t t)
// Desc: constructor
//------------------------------------------------------------------------------
DebugEvent::DebugEvent(int s, edb::pid_t p, edb::tid_t t) : status(s), pid(p), tid(t) {
	if(ptrace(PTRACE_GETSIGINFO, tid, 0, &siginfo) == -1) {
		std::memset(&siginfo, 0, sizeof(siginfo));
	}
}

//------------------------------------------------------------------------------
// Name: exited() const
// Desc: was this even caused by an exit?
//------------------------------------------------------------------------------
bool DebugEvent::exited() const {
	return WIFEXITED(status) != 0;
}

//------------------------------------------------------------------------------
// Name: signaled() const
// Desc: was this event caused by a signal
//------------------------------------------------------------------------------
bool DebugEvent::signaled() const {
	return WIFSIGNALED(status) != 0;
}

//------------------------------------------------------------------------------
// Name: stopped() const
// Desc: was this event caused by stop
//------------------------------------------------------------------------------
bool DebugEvent::stopped() const {
	return WIFSTOPPED(status) != 0;
}

//------------------------------------------------------------------------------
// Name: exit_code() const
// Desc: if it was an exit event, what was the status
//------------------------------------------------------------------------------
int DebugEvent::exit_code() const {
	return WEXITSTATUS(status);
}

//------------------------------------------------------------------------------
// Name: signal_code() const
// Desc: if it was a signal, what was the signal number
//------------------------------------------------------------------------------
int DebugEvent::signal_code() const {
	return WTERMSIG(status);
}

//------------------------------------------------------------------------------
// Name: stop_code() const
// Desc: if it was a stop, what was the status
//------------------------------------------------------------------------------
int DebugEvent::stop_code() const {
	return WSTOPSIG(status);
}

//------------------------------------------------------------------------------
// Name: thread() const
// Desc: what was the TID for this event
//------------------------------------------------------------------------------
edb::tid_t DebugEvent::thread() const {
	return tid;
}

//------------------------------------------------------------------------------
// Name: process() const
// Desc: what was the TID for this event
//------------------------------------------------------------------------------
edb::pid_t DebugEvent::process() const {
	return pid;
}

//------------------------------------------------------------------------------
// Name: reason() const
// Desc: what was the reason for this event
//------------------------------------------------------------------------------
DebugEvent::REASON DebugEvent::reason() const {

	// this basically converts our value into a 'switchable' value for convenience

	if(stopped()) {
		return EVENT_STOPPED;
	} else if(signaled()) {
		return EVENT_SIGNALED;
	} else if(exited()) {
		return EVENT_EXITED;
	} else {
		return EVENT_UNKNOWN;
	}
}

//------------------------------------------------------------------------------
// Name: trap_reason() const
// Desc:
//------------------------------------------------------------------------------
DebugEvent::TRAP_REASON DebugEvent::trap_reason() const {
	switch(siginfo.si_code) {
	case TRAP_TRACE: return TRAP_STEPPING;
	default:         return TRAP_BREAKPOINT;
	}
}

//------------------------------------------------------------------------------
// Name: is_error() const
// Desc:
//------------------------------------------------------------------------------
bool DebugEvent::is_error() const {
	if(stopped()) {
		switch(stop_code()) {
		case SIGTRAP:
		case SIGSTOP:
			return false;
		case SIGSEGV:
		case SIGILL:
		case SIGFPE:
		case SIGABRT:
		case SIGBUS:
#ifdef SIGSTKFLT
		case SIGSTKFLT:
#endif
		case SIGPIPE:
			return true;
		default:
			return false;
		}
	} else {
		return false;
	}
}

//------------------------------------------------------------------------------
// Name: error_description() const
// Desc:
//------------------------------------------------------------------------------
DebugEvent::Message DebugEvent::error_description() const {
	Q_ASSERT(is_error());

	const edb::address_t fault_address = reinterpret_cast<edb::address_t>(siginfo.si_addr);

	switch(stop_code()) {
	case SIGSEGV:
		return Message(
			tr("Illegal Access Fault"),
			tr(
				"<p>The debugged application encountered a segmentation fault.<br />The address <strong>0x%1</strong> could not be accessed.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>").arg(edb::v1::format_pointer(fault_address))
			);
	case SIGILL:
		return Message(
			tr("Illegal Instruction Fault"),
			tr(
				"<p>The debugged application attempted to execute an illegal instruction.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case SIGFPE:
		switch(siginfo.si_code) {
		case FPE_INTDIV:
		return Message(
			tr("Divide By Zero"),
			tr(
				"<p>The debugged application tried to divide an integer value by an integer divisor of zero.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
		default:
			return Message(
				tr("Floating Point Exception"),
				tr(
					"<p>The debugged application encountered a floating-point exception.</p>"
					"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
				);
		}

	case SIGABRT:
		return Message(
			tr("Application Aborted"),
			tr(
				"<p>The debugged application has aborted.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case SIGBUS:
		return Message(
			tr("Bus Error"),
			tr(
				"<p>The debugged application tried to read or write data that is misaligned.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
#ifdef SIGSTKFLT
	case SIGSTKFLT:
		return Message(
			tr("Stack Fault"),
			tr(
				"<p>The debugged application encountered a stack fault.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
#endif
	case SIGPIPE:
		return Message(
			tr("Broken Pipe Fault"),
			tr(
				"<p>The debugged application encountered a broken pipe fault.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	default:
		return Message();
	}
}
