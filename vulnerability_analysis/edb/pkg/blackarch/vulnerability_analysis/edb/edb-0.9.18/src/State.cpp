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

#include "State.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "StateInterface.h"

#include <QtAlgorithms>

//------------------------------------------------------------------------------
// Name: State()
// Desc: constructor
//------------------------------------------------------------------------------
State::State() : impl_(edb::v1::debugger_core ? edb::v1::debugger_core->create_state() : 0) {
}

//------------------------------------------------------------------------------
// Name: ~State()
// Desc:
//------------------------------------------------------------------------------
State::~State() {
	delete impl_;
}

//------------------------------------------------------------------------------
// Name: State(const State &other)
// Desc:
//------------------------------------------------------------------------------
State::State(const State &other) : impl_(other.impl_ ? other.impl_->copy() : 0) {
}

//------------------------------------------------------------------------------
// Name: swap(State &other)
// Desc:
//------------------------------------------------------------------------------
void State::swap(State &other) {
	qSwap(impl_, other.impl_);
}

//------------------------------------------------------------------------------
// Name: operator=(const State &other)
// Desc:
//------------------------------------------------------------------------------
State &State::operator=(const State &other) {
	if(this != &other) {
		State(other).swap(*this);
	}
	return *this;
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc:
//------------------------------------------------------------------------------
void State::clear() {
	if(impl_) {
		impl_->clear();
	}
}

//------------------------------------------------------------------------------
// Name: instruction_pointer() const
// Desc:
//------------------------------------------------------------------------------
edb::address_t State::instruction_pointer() const {
	if(impl_) {
		return impl_->instruction_pointer();
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: stack_pointer() const
// Desc:
//------------------------------------------------------------------------------
edb::address_t State::stack_pointer() const {
	if(impl_) {
		return impl_->stack_pointer();
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: frame_pointer() const
// Desc:
//------------------------------------------------------------------------------
edb::address_t State::frame_pointer() const {
	if(impl_) {
		return impl_->frame_pointer();
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: flags() const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t State::flags() const {
	if(impl_) {
		return impl_->flags();
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: value(const QString &reg)
// Desc: a function to return the value of a register based on it's name
//------------------------------------------------------------------------------
Register State::value(const QString &reg) const {
	if(impl_) {
		return impl_->value(reg);
	}
	return Register();
}

//------------------------------------------------------------------------------
// Name: operator[](const QString &reg) const
// Desc:
//------------------------------------------------------------------------------
Register State::operator[](const QString &reg) const {
	if(impl_) {
		return impl_->value(reg);
	}
	return Register();
}

//------------------------------------------------------------------------------
// Name: set_register(const QString &name, edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void State::set_register(const QString &name, edb::reg_t value) {
	if(impl_) {
		impl_->set_register(name, value);
	}
}

//------------------------------------------------------------------------------
// Name: adjust_stack(int bytes)
// Desc:
//------------------------------------------------------------------------------
void State::adjust_stack(int bytes) {
	if(impl_) {
		impl_->adjust_stack(bytes);
	}
}

//------------------------------------------------------------------------------
// Name: set_instruction_pointer(edb::address_t value)
// Desc:
//------------------------------------------------------------------------------
void State::set_instruction_pointer(edb::address_t value) {
	if(impl_) {
		impl_->set_instruction_pointer(value);
	}
}

//------------------------------------------------------------------------------
// Name: flags_to_string() const
// Desc:
//------------------------------------------------------------------------------
QString State::flags_to_string() const {
	if(impl_) {
		return impl_->flags_to_string();
	}
	return QString();
}

//------------------------------------------------------------------------------
// Name: flags_to_string(edb::reg_t flags) const
// Desc:
//------------------------------------------------------------------------------
QString State::flags_to_string(edb::reg_t flags) const {
	if(impl_) {
		return impl_->flags_to_string(flags);
	}
	return QString();
}

//------------------------------------------------------------------------------
// Name: set_flags(edb::reg_t flags)
// Desc:
//------------------------------------------------------------------------------
void State::set_flags(edb::reg_t flags) {
	if(impl_) {
		return impl_->set_flags(flags);
	}
}

//------------------------------------------------------------------------------
// Name: debug_register(int n) const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t State::debug_register(int n) const {
	if(impl_) {
		return impl_->debug_register(n);
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: set_debug_register(int n, edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void State::set_debug_register(int n, edb::reg_t value) {
	if(impl_) {
		impl_->set_debug_register(n, value);
	}
}

//------------------------------------------------------------------------------
// Name: fpu_register(int n) const
// Desc:
//------------------------------------------------------------------------------
long double State::fpu_register(int n) const {
	if(impl_) {
		return impl_->fpu_register(n);
	}
	return 0.0;
}

quint64 State::mmx_register(int n) const {
	if(impl_) {
		return impl_->mmx_register(n);
	}
	return 0;
}

QByteArray State::xmm_register(int n) const {
	if(impl_) {
		return impl_->xmm_register(n);
	}
	return QByteArray(16, 0);
}
