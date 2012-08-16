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

#ifndef REGISTER_20100329_H_
#define REGISTER_20100329_H_

#include "Types.h"
#include "API.h"
#include <QString>

class EDB_EXPORT Register {
public:
	enum Type{
		// groups, these catagories should remain as portable as possible
		TYPE_INVALID = 0x0000,
		TYPE_GPR     = 0x0001,
		TYPE_IP      = 0x0002,
		TYPE_SEG     = 0x0004,
		TYPE_COND    = 0x0008,
		TYPE_FPU     = 0x0010,
		TYPE_ANY     = -1 // type_ can never be one of these
	};

public:
	Register();
	Register(const QString &name, edb::reg_t value, Type type, edb::address_t segment_base = 0);
	Register(const Register &other);
	Register &operator=(const Register &rhs);

public:
	bool operator==(const Register &rhs) const;
	bool operator!=(const Register &rhs) const;

public:
	operator void*() const       { return reinterpret_cast<void*>(valid()); }
	edb::reg_t operator*() const { return value_; }
	Type type() const            { return type_; }
	QString name() const         { return name_; }

public:
	// this only makes sense for TYPE_SEG
	edb::address_t segment_base() const { return segment_base_; }

private:
	bool valid() const { return type_ != TYPE_INVALID; }

private:
	QString        name_;
	edb::reg_t     value_;
	Type           type_;
	edb::address_t segment_base_;
};

#endif
