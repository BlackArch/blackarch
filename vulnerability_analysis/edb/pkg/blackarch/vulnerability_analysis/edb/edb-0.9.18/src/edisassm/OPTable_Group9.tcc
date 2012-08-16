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

#ifndef OPTABLE_GROUP9_20080314_TCC_
#define OPTABLE_GROUP9_20080314_TCC_

#include "Operand.h"

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_Group9[8] = {
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "cmpxchg8b/cmpxchg16b", &Instruction::decode_invalid_cmpxchg8b_cmpxchg16b, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "vmptrld", &Instruction::decode_Mq, OP_VMPTRLD, FLAG_NONE, 1 },
	{ "vmptrst", &Instruction::decode_Mq, OP_VMPTRST, FLAG_NONE, 1 },
};

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_Group9_66[8] = {
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "cmpxchg8b/cmpxchg16b", &Instruction::decode_invalid_cmpxchg8b_cmpxchg16b, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "vmclear", &Instruction::decode_Mq, OP_VMCLEAR, FLAG_NONE, 1 },
	{ "vmptrst", &Instruction::decode_Mq, OP_VMPTRST, FLAG_NONE, 1 },
};

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_Group9_F3[8] = {
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "cmpxchg8b/cmpxchg16b", &Instruction::decode_invalid_cmpxchg8b_cmpxchg16b, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "vmxon", &Instruction::decode_Mq, OP_VMXON, FLAG_NONE, 1 },
	{ "vmptrst", &Instruction::decode_Mq, OP_VMPTRST, FLAG_NONE, 1 },
};

#endif

