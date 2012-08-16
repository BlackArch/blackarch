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

#ifndef OPTABLE_GROUP11_20080314_TCC_
#define OPTABLE_GROUP11_20080314_TCC_

#include "Operand.h"

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_Group11[8] = {
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
	{ "ud", &Instruction::decode0, OP_UD, FLAG_NONE, 0 },
};

#endif

