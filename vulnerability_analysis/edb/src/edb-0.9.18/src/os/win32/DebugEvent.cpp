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

//------------------------------------------------------------------------------
// Name: DebugEvent()
// Desc: constructor
//------------------------------------------------------------------------------
DebugEvent::DebugEvent() : event(DEBUG_EVENT()){
}

//------------------------------------------------------------------------------
// Name: DebugEvent(DEBUG_EVENT &e)
// Desc:
//------------------------------------------------------------------------------
DebugEvent::DebugEvent(DEBUG_EVENT &e) {
	event = e;
}

//------------------------------------------------------------------------------
// Name: DebugEvent(const DebugEvent &other)
// Desc: copy constructor
//------------------------------------------------------------------------------
DebugEvent::DebugEvent(const DebugEvent &other) : event(other.event) {
}

//------------------------------------------------------------------------------
// Name: operator=(const DebugEvent &rhs)
// Desc:
//------------------------------------------------------------------------------
DebugEvent &DebugEvent::operator=(const DebugEvent &rhs) {
	event = rhs.event;
	return *this;
}

//------------------------------------------------------------------------------
// Name: exited() const
// Desc: was this even caused by an exit?
//------------------------------------------------------------------------------
bool DebugEvent::exited() const {
	return reason() == EVENT_EXITED;
}

//------------------------------------------------------------------------------
// Name: signaled() const
// Desc: was this event caused by a signal
//------------------------------------------------------------------------------
bool DebugEvent::signaled() const {
	return reason() == EVENT_SIGNALED;
}

//------------------------------------------------------------------------------
// Name: stopped() const
// Desc: was this event caused by stop
//------------------------------------------------------------------------------
bool DebugEvent::stopped() const {
	return reason() == EVENT_STOPPED;
}

//------------------------------------------------------------------------------
// Name: exit_code() const
// Desc: if it was an exit event, what was the status
//------------------------------------------------------------------------------
int DebugEvent::exit_code() const {
	return event.u.ExitProcess.dwExitCode;
}

//------------------------------------------------------------------------------
// Name: signal_code() const
// Desc: if it was a signal, what was the signal number
//------------------------------------------------------------------------------
int DebugEvent::signal_code() const {
	return event.u.Exception.ExceptionRecord.ExceptionCode;
}

//------------------------------------------------------------------------------
// Name: stop_code() const
// Desc: if it was a stop, what was the status
//------------------------------------------------------------------------------
int DebugEvent::stop_code() const {
	switch(event.u.Exception.ExceptionRecord.ExceptionCode) {
	case EXCEPTION_SINGLE_STEP:
	case EXCEPTION_BREAKPOINT:
			return sigtrap;
	default:
			return event.u.Exception.ExceptionRecord.ExceptionCode;
	}
}

//------------------------------------------------------------------------------
// Name: thread() const
// Desc: what was the TID for this event
//------------------------------------------------------------------------------
edb::tid_t DebugEvent::thread() const {
	return event.dwThreadId;
}

//------------------------------------------------------------------------------
// Name: process() const
// Desc: what was the PID for this event
//------------------------------------------------------------------------------
edb::pid_t DebugEvent::process() const {
	return event.dwProcessId;
}

//------------------------------------------------------------------------------
// Name: trap_reason() const
// Desc:
//------------------------------------------------------------------------------
DebugEvent::TRAP_REASON DebugEvent::trap_reason() const {
	switch(event.dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:
		switch(event.u.Exception.ExceptionRecord.ExceptionCode) {
		case EXCEPTION_BREAKPOINT:
			return TRAP_BREAKPOINT;
		case EXCEPTION_SINGLE_STEP:
			return TRAP_STEPPING;
		}
	}
	return TRAP_BREAKPOINT;
}

//------------------------------------------------------------------------------
// Name: reason() const
// Desc: what was the reason for this event
//------------------------------------------------------------------------------
DebugEvent::REASON DebugEvent::reason() const {
	switch(event.dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:
		return EVENT_STOPPED;
		/*
		switch(event.u.Exception.ExceptionRecord.ExceptionCode) {
		case EXCEPTION_BREAKPOINT:
		case EXCEPTION_SINGLE_STEP:
		case EXCEPTION_ACCESS_VIOLATION:
		case EXCEPTION_IN_PAGE_ERROR:
		case EXCEPTION_INVALID_DISPOSITION:
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		case EXCEPTION_ILLEGAL_INSTRUCTION:
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
		case EXCEPTION_INT_OVERFLOW:
		case EXCEPTION_PRIV_INSTRUCTION:
		case EXCEPTION_STACK_OVERFLOW:
		case EXCEPTION_FLT_DENORMAL_OPERAND:
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		case EXCEPTION_FLT_INEXACT_RESULT:
		case EXCEPTION_FLT_INVALID_OPERATION:
		case EXCEPTION_FLT_OVERFLOW:
		case EXCEPTION_FLT_STACK_CHECK:
		case EXCEPTION_FLT_UNDERFLOW:
		case EXCEPTION_DATATYPE_MISALIGNMENT:
		default:
			return EVENT_STOPPED;
		}
		*/
	case EXIT_PROCESS_DEBUG_EVENT:
		return EVENT_EXITED;
		/*
	case CREATE_THREAD_DEBUG_EVENT:
	case CREATE_PROCESS_DEBUG_EVENT:
	case EXIT_THREAD_DEBUG_EVENT:

	case LOAD_DLL_DEBUG_EVENT:
	case UNLOAD_DLL_DEBUG_EVENT:
	case OUTPUT_DEBUG_STRING_EVENT:
	case RIP_EVENT:
	*/
	default:
		return EVENT_UNKNOWN;
	}
}

//------------------------------------------------------------------------------
// Name: is_error() const
// Desc:
//------------------------------------------------------------------------------
bool DebugEvent::is_error() const {
	switch(event.dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:

		switch(event.u.Exception.ExceptionRecord.ExceptionCode) {
		/*
		case EXCEPTION_BREAKPOINT:
		case EXCEPTION_SINGLE_STEP:
			return false;*/
			/*
		case EXCEPTION_ACCESS_VIOLATION:
		case EXCEPTION_IN_PAGE_ERROR:
		case EXCEPTION_INVALID_DISPOSITION:
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		case EXCEPTION_ILLEGAL_INSTRUCTION:
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
		case EXCEPTION_INT_OVERFLOW:
		case EXCEPTION_PRIV_INSTRUCTION:
		case EXCEPTION_STACK_OVERFLOW:
		case EXCEPTION_FLT_DENORMAL_OPERAND:
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		case EXCEPTION_FLT_INEXACT_RESULT:
		case EXCEPTION_FLT_INVALID_OPERATION:
		case EXCEPTION_FLT_OVERFLOW:
		case EXCEPTION_FLT_STACK_CHECK:
		case EXCEPTION_FLT_UNDERFLOW:
		case EXCEPTION_DATATYPE_MISALIGNMENT:
		*/
		default:
			return (event.u.Exception.dwFirstChance == 0); //true;
		}
		/*
	case CREATE_THREAD_DEBUG_EVENT:
	case CREATE_PROCESS_DEBUG_EVENT:
	case EXIT_THREAD_DEBUG_EVENT:
	case EXIT_PROCESS_DEBUG_EVENT:
	case LOAD_DLL_DEBUG_EVENT:
	case UNLOAD_DLL_DEBUG_EVENT:
	case OUTPUT_DEBUG_STRING_EVENT:
	case RIP_EVENT:
	*/
	default:
		return false;
	}
}

//------------------------------------------------------------------------------
// Name: error_description() const
// Desc:
//------------------------------------------------------------------------------
DebugEvent::Message DebugEvent::error_description() const {
	Q_ASSERT(is_error());

	edb::address_t fault_address = static_cast<edb::address_t>(-1);
	if(event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
		fault_address = (edb::address_t)(event.u.Exception.ExceptionRecord.ExceptionInformation[1]);
	}

	switch(stop_code()) {
	case EXCEPTION_ACCESS_VIOLATION:
		return Message(
			tr("Illegal Access Fault"),
			tr(
				"<p>The debugged application encountered a segmentation fault.<br />The address <strong>0x%1</strong> could not be accessed.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>").arg(edb::v1::format_pointer(fault_address))
			);
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return Message(
			tr("Array Bounds Error"),
			tr(
				"<p>The debugged application tried to access an out of bounds array element.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>").arg(edb::v1::format_pointer(fault_address))
			);
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return Message(
			tr("Bus Error"),
			tr(
				"<p>The debugged application tried to read or write data that is misaligned.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return Message(
			tr("Floating Point Exception"),
			tr(
				"<p>One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return Message(
			tr("Floating Point Exception"),
			tr(
				"<p>The debugged application tried to divide a floating-point value by a floating-point divisor of zero.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_FLT_INEXACT_RESULT:
		return Message(
			tr("Floating Point Exception"),
			tr(
				"<p>The result of a floating-point operation cannot be represented exactly as a decimal fraction.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_FLT_INVALID_OPERATION:
		return Message(
			tr("Floating Point Exception"),
			tr(
				"<p>The application attempted an invalid floating point operation.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_FLT_OVERFLOW:
		return Message(
			tr("Floating Point Exception"),
			tr(
				"<p>The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_FLT_STACK_CHECK:
		return Message(
			tr("Floating Point Exception"),
			tr(
				"<p>The stack overflowed or underflowed as the result of a floating-point operation.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_FLT_UNDERFLOW:
		return Message(
			tr("Floating Point Exception"),
			tr(
				"<p>The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return Message(
			tr("Illegal Instruction Fault"),
			tr(
				"<p>The debugged application attempted to execute an illegal instruction.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_IN_PAGE_ERROR:
		return Message(
			tr("Page Error"),
			tr(
				"<p>The debugged application tried to access a page that was not present, and the system was unable to load the page.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return Message(
			tr("Divide By Zero"),
			tr(
				"<p>The debugged application tried to divide an integer value by an integer divisor of zero.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_INT_OVERFLOW:
		return Message(
			tr("Integer Overflow"),
			tr(
				"<p>The result of an integer operation caused a carry out of the most significant bit of the result.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_INVALID_DISPOSITION:
		return Message(
			tr("Invalid Disposition"),
			tr(
				"<p>An exception handler returned an invalid disposition to the exception dispatcher.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return Message(
			tr("Invalid Disposition"),
			tr(
				"<p>The debugged application tried to continue execution after a non-continuable exception occurred.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_PRIV_INSTRUCTION:
		return Message(
			tr("Privileged Instruction"),
			tr(
				"<p>The debugged application tried to execute an instruction whose operation is not allowed in the current machine mode.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	case EXCEPTION_STACK_OVERFLOW:
		return Message(
			tr("Stack Overflow"),
			tr(
				"<p>The debugged application has exhausted its stack.</p>"
				"<p>If you would like to pass this exception to the application press Shift+[F7/F8/F9]</p>")
			);
	default:
		return Message();
	}
}
