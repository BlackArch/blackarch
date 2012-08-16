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

#ifndef OPTABLE_GROUP7_20080314_TCC_
#define OPTABLE_GROUP7_20080314_TCC_

#include "Operand.h"

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_Group7[8] = {
	{ "sgdt", &Instruction::decode_Ms, OP_SGDT, FLAG_NONE, 1 },
	{ "sidt", &Instruction::decode_Ms, OP_SIDT, FLAG_NONE, 1 },
	{ "lgdt", &Instruction::decode_Ms, OP_LGDT, FLAG_NONE, 1 },
	{ "lidt", &Instruction::decode_Ms, OP_LIDT, FLAG_NONE, 1 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "invlpg", &Instruction::decode_Mb, OP_INVLPG, FLAG_NONE, 1 },
};

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_Group7A[64] = {
	{ "vmcall", &Instruction::decode0, OP_VMCALL, FLAG_NONE, 0 },
	{ "vmlaunch", &Instruction::decode0, OP_VMLAUNCH, FLAG_NONE, 0 },
	{ "vmresume", &Instruction::decode0, OP_VMRESUME, FLAG_NONE, 0 },
	{ "vmxoff", &Instruction::decode0, OP_VMXOFF, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	{ "monitor", &Instruction::decode0, OP_MONITOR, FLAG_NONE, 0 },
	{ "mwait", &Instruction::decode0, OP_MWAIT, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	{ "xgetbv", &Instruction::decode0, OP_XGETBV, FLAG_NONE, 0 },
	{ "xsetbv", &Instruction::decode0, OP_XSETBV, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	{ "vmrun", &Instruction::decode0, OP_VMRUN, FLAG_AMD, 0 },
	{ "vmmcall", &Instruction::decode0, OP_VMMCALL, FLAG_AMD, 0 },
	{ "vmload", &Instruction::decode0, OP_VMLOAD, FLAG_AMD, 0 },
	{ "vmsave", &Instruction::decode0, OP_VMSAVE, FLAG_AMD, 0 },
	{ "stgi", &Instruction::decode0, OP_STGI, FLAG_AMD, 0 },
	{ "clgi", &Instruction::decode0, OP_CLGI, FLAG_AMD, 0 },
	{ "skinit", &Instruction::decode0, OP_SKINIT, FLAG_AMD, 0 },
	{ "invlpga", &Instruction::decode0, OP_INVLPGA, FLAG_AMD, 0 },

	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },
	{ "smsw", &Instruction::decode_Rv_Mw, OP_SMSW, FLAG_NONE, 2 },

	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },
	{ "lmsw", &Instruction::decode_Ew, OP_LMSW, FLAG_NONE, 1 },

	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },	// x86-64: swapgs
	{ "rdtscp", &Instruction::decode0, OP_RDTSCP, FLAG_AMD, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
};


#endif
