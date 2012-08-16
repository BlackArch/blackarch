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

#ifndef SIB_20070530_H_
#define SIB_20070530_H_

#include "edisassm_types.h"

class SIB {
public:
	SIB(uint8_t value) : value_(value) {
	}

	SIB(const SIB &other) : value_(other.value_) {
	}

	SIB &operator=(const SIB &rhs) {
		value_ = rhs.value_;
		return *this;
	}

private:
	uint8_t value_;

public:
	uint8_t base() const  { return value_ & 0x07; }
	uint8_t index() const { return (value_ >> 3) & 0x07; }
	uint8_t scale() const { return (value_ >> 6) & 0x03; }
};

#endif

