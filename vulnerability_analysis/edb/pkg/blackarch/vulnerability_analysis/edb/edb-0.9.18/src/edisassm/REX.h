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

#ifndef REX_20071217_H_
#define REX_20071217_H_

#include "edisassm_types.h"

class REX {
public:
	REX(uint8_t value) : value_(value) {
	}

	REX(const REX &other) : value_(other.value_) {
	}

	REX &operator=(const REX &rhs) {
		value_ = rhs.value_;
		return *this;
	}

private:
	uint8_t value_;

public:
	bool is_rex() const { return (value_ & 0xf0) == 0x40; }
	uint8_t w() const   { return (value_ >> 3) & 0x01; } // 64 bit mode
	uint8_t r() const   { return (value_ >> 2) & 0x01; } // modRM extension
	uint8_t x() const   { return (value_ >> 1) & 0x01; } // SIB extension
	uint8_t b() const   { return (value_ >> 0) & 0x01; } // ModRM OR SIB base or Opcode Reg extension
};

#endif

