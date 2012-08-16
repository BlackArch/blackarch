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

#ifndef PLATFORMSTATE_20110330_H_
#define PLATFORMSTATE_20110330_H_

#include "StateInterface.h"
#include "Types.h"
#include <sys/user.h>
#include <mach/mach.h>

class PlatformState : public StateInterface {
	friend class DebuggerCore;

public:
	PlatformState();

public:
	virtual StateInterface *copy() const;

public:
	virtual QString flags_to_string() const;
	virtual QString flags_to_string(edb::reg_t flags) const;
	virtual Register value(const QString &reg) const;
	virtual edb::address_t frame_pointer() const;
	virtual edb::address_t instruction_pointer() const;
	virtual edb::address_t stack_pointer() const;
	virtual edb::reg_t debug_register(int n) const;
	virtual edb::reg_t flags() const;
	virtual long double fpu_register(int n) const;
	virtual void adjust_stack(int bytes);
	virtual void clear();
	virtual void set_debug_register(int n, edb::reg_t value);
	virtual void set_flags(edb::reg_t flags);
	virtual void set_instruction_pointer(edb::address_t value);
	virtual void set_register(const QString &name, edb::reg_t value);
	virtual quint64 mmx_register(int n) const;
	virtual QByteArray xmm_register(int n) const;

private:
#if defined(EDB_X86)
	x86_thread_state32_t    thread_state_;
	x86_float_state32_t     float_state_;
	x86_debug_state32_t     debug_state_;
	x86_exception_state32_t exception_state_;
#elif defined(EDB_X86_64)
	x86_thread_state64_t    thread_state_;
	x86_float_state64_t     float_state_;
	x86_debug_state64_t     debug_state_;
	x86_exception_state64_t exception_state_;
#endif
};

#endif

