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

#ifndef OPTABLE_GROUP3_20080314_TCC_
#define OPTABLE_GROUP3_20080314_TCC_

#include "Operand.h"

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_Group3[16] = {
	{ "test", &Instruction::decode_Eb_Ib, OP_TEST, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "not", &Instruction::decode_Eb, OP_NOT, FLAG_NONE, 1 },
	{ "neg", &Instruction::decode_Eb, OP_NEG, FLAG_NONE, 1 },
	{ "mul", &Instruction::decode_Eb, OP_MUL, FLAG_NONE, 1 },
	{ "imul", &Instruction::decode_Eb, OP_IMUL, FLAG_NONE, 1 },
	{ "div", &Instruction::decode_Eb, OP_DIV, FLAG_NONE, 1 },
	{ "idiv", &Instruction::decode_Eb, OP_IDIV, FLAG_NONE, 1 },

	{ "test", &Instruction::decode_Ev_Iz, OP_TEST, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "not", &Instruction::decode_Ev, OP_NOT, FLAG_NONE, 1 },
	{ "neg", &Instruction::decode_Ev, OP_NEG, FLAG_NONE, 1 },
	{ "mul", &Instruction::decode_Ev, OP_MUL, FLAG_NONE, 1 },
	{ "imul", &Instruction::decode_Ev, OP_IMUL, FLAG_NONE, 1 },
	{ "div", &Instruction::decode_Ev, OP_DIV, FLAG_NONE, 1 },
	{ "idiv", &Instruction::decode_Ev, OP_IDIV, FLAG_NONE, 1 },
};

#endif

