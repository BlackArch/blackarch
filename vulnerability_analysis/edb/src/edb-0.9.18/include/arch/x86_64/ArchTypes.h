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

#ifndef ARCHTYPES_20071127_H_
#define ARCHTYPES_20071127_H_

#include <QtGlobal>
#include "Instruction.h"

#define EDB_MAX_HEX 16
#define EDB_X86_64
#define EDB_FMT_PTR "%016llx"

namespace edb {
	typedef quint64                         reg_t;
	typedef quint64                         address_t;
	typedef Instruction<edisassm::x86_64>   Instruction;
	typedef Instruction::operand_t          Operand;
}

#endif
