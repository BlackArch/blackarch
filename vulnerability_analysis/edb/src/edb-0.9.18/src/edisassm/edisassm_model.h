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

#ifndef EDISASSM_MODEL_20100915_H_
#define EDISASSM_MODEL_20100915_H_

#include "edisassm_types.h"

namespace edisassm {

struct x86 {
public:
	static const int BITS         = 32;
	static const int MAX_SIZE     = 20;
	static const int MAX_OPERANDS = 3;

public:
	typedef uint32_t address_t;
};

struct x86_64 : x86 {
public:
	static const int BITS = 64;

public:
	typedef uint64_t address_t;
};

}

#endif
