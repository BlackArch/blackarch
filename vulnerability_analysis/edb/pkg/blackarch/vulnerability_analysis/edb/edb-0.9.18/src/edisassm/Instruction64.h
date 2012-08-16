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

#include "Instruction.h"

#ifndef INSTRUCTION64_20070427_TCC_
#define INSTRUCTION64_20070427_TCC_

template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_cmpsw_cmpsd_cmpsq[3] = {
	{ "cmpsw",  &Instruction::decode0, OP_CMPS, FLAG_NONE, 0 },
	{ "cmpsd",  &Instruction::decode0, OP_CMPS, FLAG_NONE, 0 },
	{ "cmpsq",  &Instruction::decode0, OP_CMPS, FLAG_AMD, 0 }, // x86: invalid
};

template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_Group7A[64] = {
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

	{ "swapgs", &Instruction::decode0, OP_SWAPGS, FLAG_NONE, 0 }, // x86: invalid
	{ "rdtscp", &Instruction::decode0, OP_RDTSCP, FLAG_AMD, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
};

template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes[0x100] = {

	/* 0x00 - 0x0f */
	{ "add", &Instruction::decode_Eb_Gb, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_Ev_Gv, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_Gb_Eb, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_Gv_Ev, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_AL_Ib, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_rAX_Iz, OP_ADD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "or", &Instruction::decode_Eb_Gb, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_Ev_Gv, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_Gb_Eb, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_Gv_Ev, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_AL_Ib, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_rAX_Iz, OP_OR, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "2byte", &Instruction::decode_2byte, OP_2BYTE, FLAG_NONE, -1 },

	/* 0x10 - 0x1f */
	{ "adc", &Instruction::decode_Eb_Gb, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_Ev_Gv, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_Gb_Eb, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_Gv_Ev, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_AL_Ib, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_rAX_Iz, OP_ADC, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "sbb", &Instruction::decode_Eb_Gb, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_Ev_Gv, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_Gb_Eb, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_Gv_Ev, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_AL_Ib, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_rAX_Iz, OP_SBB, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only

	/* 0x20 - 0x2f */
	{ "and", &Instruction::decode_Eb_Gb, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_Ev_Gv, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_Gb_Eb, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_Gv_Ev, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_AL_Ib, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_rAX_Iz, OP_AND, FLAG_NONE, 2 },
	{ "prefix es", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "sub", &Instruction::decode_Eb_Gb, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_Ev_Gv, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_Gb_Eb, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_Gv_Ev, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_AL_Ib, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_rAX_Iz, OP_SUB, FLAG_NONE, 2 },
	{ "prefix cs", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only

	/* 0x30 - 0x3f */
	{ "xor", &Instruction::decode_Eb_Gb, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_Ev_Gv, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_Gb_Eb, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_Gv_Ev, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_AL_Ib, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_rAX_Iz, OP_XOR, FLAG_NONE, 2 },
	{ "prefix ss", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "cmp", &Instruction::decode_Eb_Gb, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_Ev_Gv, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_Gb_Eb, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_Gv_Ev, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_AL_Ib, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_rAX_Iz, OP_CMP, FLAG_NONE, 2 },
	{ "prefix ds", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only

	/* 0x40 - 0x4f */
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)
	{ "prefix rex", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 }, // ia-32 only (REX byte)

	/* 0x50 - 0x5f */
	{ "push", &Instruction::decode_rAX, OP_PUSH, FLAG_NONE, 1 },
	{ "push", &Instruction::decode_rCX, OP_PUSH, FLAG_NONE, 1 },
	{ "push", &Instruction::decode_rDX, OP_PUSH, FLAG_NONE, 1 },
	{ "push", &Instruction::decode_rBX, OP_PUSH, FLAG_NONE, 1 },
	{ "push", &Instruction::decode_rSP, OP_PUSH, FLAG_NONE, 1 },
	{ "push", &Instruction::decode_rBP, OP_PUSH, FLAG_NONE, 1 },
	{ "push", &Instruction::decode_rSI, OP_PUSH, FLAG_NONE, 1 },
	{ "push", &Instruction::decode_rDI, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rAX, OP_POP, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rCX, OP_POP, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rDX, OP_POP, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rBX, OP_POP, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rSP, OP_POP, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rBP, OP_POP, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rSI, OP_POP, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_rDI, OP_POP, FLAG_NONE, 1 },

	/* 0x60 - 0x6f */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "movsxd", &Instruction::decode_Gv_Ev, OP_MOVSXD, FLAG_NONE, 2 },      // x86: arpl Ew,Gw
	{ "prefix fs", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "prefix gs", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "prefix data16", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "prefix addr16", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "push", &Instruction::decode_Iz, OP_PUSH, FLAG_NONE, 1 },
	{ "imul", &Instruction::decode_Gv_Ev_Iz, OP_IMUL, FLAG_NONE, 3 },
	{ "push", &Instruction::decode_Ib, OP_PUSH, FLAG_NONE, 1 },
	{ "imul", &Instruction::decode_Gv_Ev_Ib, OP_IMUL, FLAG_NONE, 3 },
	{ "insb",  &Instruction::decode0, OP_INS, FLAG_NONE, 0 },
	{ "insw/insd",  &Instruction::decode_insw_insd_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "outsb",  &Instruction::decode0, OP_OUTS, FLAG_NONE, 0 },
	{ "outsw/outsd",  &Instruction::decode_outsw_outsd_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x70 - 0x7f */
	{ "jo", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jno", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jb", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jnb", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jz", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jnz", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jbe", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jnbe", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "js", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jns", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jp", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jnp", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jl", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jnl", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jle", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "jnle", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },

	/* 0x80 - 0x8f */
	{ "group1", &Instruction::decode_group1, OP_GROUP1, FLAG_NONE, -1 },
	{ "group1", &Instruction::decode_group1, OP_GROUP1, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "group1", &Instruction::decode_group1, OP_GROUP1, FLAG_NONE, -1 },
	{ "test",  &Instruction::decode_Eb_Gb, OP_TEST, FLAG_NONE, 2 },
	{ "test",  &Instruction::decode_Ev_Gv, OP_TEST, FLAG_NONE, 2 },
	{ "xchg", &Instruction::decode_Gb_Eb, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg", &Instruction::decode_Gv_Ev, OP_XCHG, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Eb_Gb, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Ev_Gv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Gb_Eb, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Gv_Ev, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Ev_Sw, OP_MOV, FLAG_NONE, 2 },
	{ "lea", &Instruction::decode_Gv_M, OP_LEA, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Sw_Ew, OP_MOV, FLAG_NONE, 2 },
	{ "group10", &Instruction::decode_group10, OP_GROUP10, FLAG_NONE, -1 },

	/* 0x90 - 0x9f */
	{ "xchg",  &Instruction::decode_rAX_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg",  &Instruction::decode_rCX_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg",  &Instruction::decode_rDX_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg",  &Instruction::decode_rBX_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg",  &Instruction::decode_rSP_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg",  &Instruction::decode_rBP_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg",  &Instruction::decode_rSI_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "xchg",  &Instruction::decode_rDI_rAX_NOREX, OP_XCHG, FLAG_NONE, 2 },
	{ "cbw/cwde/cdqe",  &Instruction::decode_cbw_cwde_cdqe, OP_INVALID, FLAG_NONE, -1 },
	{ "cwd/cdq/cqo",  &Instruction::decode_cwd_cdq_cqo, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "wait", &Instruction::decode0, OP_WAIT, FLAG_NONE, 0 },
	{ "pushfw/pushfd/pushfq", &Instruction::decode_pushfw_pushfd_pushfq, OP_INVALID, FLAG_NONE, -1 },
	{ "popfw/popfd/popfq", &Instruction::decode_popfw_popfd_popfq, OP_INVALID, FLAG_NONE, -1 },
	{ "sahf", &Instruction::decode0, OP_SAHF, FLAG_NONE, 0 },
	{ "lahf", &Instruction::decode0, OP_LAHF, FLAG_NONE, 0 },

	/* 0xa0 - 0xaf */
	{ "mov", &Instruction::decode_AL_Ob, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rAX_Ov, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Ob_AL, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Ov_rAX, OP_MOV, FLAG_NONE, 2 },
	{ "movsb", &Instruction::decode0, OP_MOVS, FLAG_NONE, 0 },
	{ "movsw/movsd/movsq", &Instruction::decode_movsw_movsd_movsq, OP_INVALID, FLAG_NONE, -1 },
	{ "cmpsb", &Instruction::decode0, OP_CMPS, FLAG_NONE, 0 },
	{ "cmpsw/cmpsd/cmpsq", &Instruction::decode_cmpsw_cmpsd_cmpsq, OP_INVALID, FLAG_NONE, -1 },
	{ "test", &Instruction::decode_AL_Ib, OP_TEST, FLAG_NONE, 2 },
	{ "test", &Instruction::decode_rAX_Iz, OP_TEST, FLAG_NONE, 2 },
	{ "stosb", &Instruction::decode0, OP_STOS, FLAG_NONE, 0 },
	{ "stosw/stosd/stosq", &Instruction::decode_stosw_stosd_stosq, OP_INVALID, FLAG_NONE, -1 },
	{ "lodsb", &Instruction::decode0, OP_LODS, FLAG_NONE, 0 },
	{ "lodsw/lodsd/lodsq", &Instruction::decode_lodsw_lodsd_lodsq, OP_INVALID, FLAG_NONE, -1 },
	{ "scasb", &Instruction::decode0, OP_SCAS, FLAG_NONE, 0 },
	{ "scasw/scasd/scasq", &Instruction::decode_scasw_scasd_scasq, OP_INVALID, FLAG_NONE, -1 },

	/* 0xb0 - 0xbf */
	{ "mov", &Instruction::decode_AL_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_CL_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_DL_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_BL_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_AH_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_CH_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_DH_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_BH_Ib, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rAX_Iv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rCX_Iv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rDX_Iv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rBX_Iv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rSP_Iv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rBP_Iv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rSI_Iv, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_rDI_Iv, OP_MOV, FLAG_NONE, 2 },

	/* 0xc0 - 0xcf */
	{ "group2", &Instruction::decode_group2, OP_GROUP2, FLAG_NONE, -1 },
	{ "group2", &Instruction::decode_group2, OP_GROUP2, FLAG_NONE, -1 },
	{ "ret", &Instruction::decode_Iw, OP_RET, FLAG_NONE, 1 },
	{ "ret", &Instruction::decode0, OP_RET, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "group12", &Instruction::decode_group12, OP_GROUP12, FLAG_NONE, -1 },
	{ "group12", &Instruction::decode_group12, OP_GROUP12, FLAG_NONE, -1 },
	{ "enter", &Instruction::decode_Iw_Ib, OP_ENTER, FLAG_NONE, 2 },
	{ "leave", &Instruction::decode0, OP_LEAVE, FLAG_NONE, 0 },
	{ "retf", &Instruction::decode_Iw, OP_RETF, FLAG_NONE, 1 },
	{ "retf", &Instruction::decode0, OP_RETF, FLAG_NONE, 0 },
	{ "int3", &Instruction::decode0, OP_INT3, FLAG_NONE, 0 },
	{ "int", &Instruction::decode_Ib, OP_INT, FLAG_NONE, 1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "iretw/iret/iretq", &Instruction::decode_iretw_iret_iretq, OP_INVALID, FLAG_NONE, -1 },

	/* 0xd0 - 0xdf */
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "xlat", &Instruction::decode0, OP_XLAT, FLAG_NONE, 0 },
	{ "esc0", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },
	{ "esc1", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },
	{ "esc2", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },
	{ "esc3", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },
	{ "esc4", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },
	{ "esc5", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },
	{ "esc6", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },
	{ "esc7", &Instruction::decode_x87, OP_FPU, FLAG_NONE, -1 },

	/* 0xe0 - 0xef */
	{ "loopne", &Instruction::decode_Jb, OP_LOOPNE, FLAG_NONE, 1 },
	{ "loope", &Instruction::decode_Jb, OP_LOOPE, FLAG_NONE, 1 },
	{ "loop", &Instruction::decode_Jb, OP_LOOP, FLAG_NONE, 1 },
	{ "jcez", &Instruction::decode_Jb, OP_JCC, FLAG_NONE, 1 },
	{ "in", &Instruction::decode_AL_Ib, OP_IN, FLAG_NONE, 2 },
	{ "in", &Instruction::decode_eAX_Ib, OP_IN, FLAG_NONE, 2 },
	{ "out", &Instruction::decode_Ib_AL, OP_OUT, FLAG_NONE, 2 },
	{ "out", &Instruction::decode_Ib_eAX, OP_OUT, FLAG_NONE, 2 },
	{ "call", &Instruction::decode_Jz, OP_CALL, FLAG_NONE, 1 },
	{ "jmp", &Instruction::decode_Jz, OP_JMP, FLAG_NONE, 1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "jmp", &Instruction::decode_Jb, OP_JMP, FLAG_NONE, 1 },
	{ "in", &Instruction::decode_AL_DX, OP_IN, FLAG_NONE, 2 },
	{ "in", &Instruction::decode_eAX_DX, OP_IN, FLAG_NONE, 2 },
	{ "out", &Instruction::decode_DX_AL, OP_OUT, FLAG_NONE, 2 },
	{ "out", &Instruction::decode_DX_eAX, OP_OUT, FLAG_NONE, 2 },

	/* 0xf0 - 0xff */
	{ "prefix lock", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "prefix repne", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "prefix rep", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "hlt", &Instruction::decode0, OP_HLT, FLAG_NONE, 0 },
	{ "cmc", &Instruction::decode0, OP_CMC, FLAG_NONE, 0 },
	{ "group3", &Instruction::decode_group3, OP_GROUP3, FLAG_NONE, -1 },
	{ "group3", &Instruction::decode_group3, OP_GROUP3, FLAG_NONE, -1 },
	{ "clc", &Instruction::decode0, OP_CLC, FLAG_NONE, 0 },
	{ "stc", &Instruction::decode0, OP_STC, FLAG_NONE, 0 },
	{ "cli", &Instruction::decode0, OP_CLI, FLAG_NONE, 0 },
	{ "sti", &Instruction::decode0, OP_STI, FLAG_NONE, 0 },
	{ "cld", &Instruction::decode0, OP_CLD, FLAG_NONE, 0 },
	{ "std", &Instruction::decode0, OP_STD, FLAG_NONE, 0 },
	{ "group4", &Instruction::decode_group4, OP_GROUP4, FLAG_NONE, -1 },
	{ "group5", &Instruction::decode_group5, OP_GROUP5, FLAG_NONE, -1 },
};


template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_2Byte_NA[0x100] = {

	/* 0x0f 0x00 - 0x0f 0x0f */
	{ "group6", &Instruction::decode_group6, OP_GROUP6, FLAG_NONE, -1 },
	{ "group7", &Instruction::decode_group7, OP_GROUP7, FLAG_NONE, -1 },
	{ "lar", &Instruction::decode_Gv_Ew, OP_LAR, FLAG_NONE, 2 },
	{ "lsl", &Instruction::decode_Gv_Ew, OP_LSL, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "syscall", &Instruction::decode0, OP_SYSCALL, FLAG_NONE, 0 },
	{ "clts", &Instruction::decode0, OP_CLTS, FLAG_NONE, 0 },
	{ "sysret", &Instruction::decode0, OP_SYSRET, FLAG_NONE, 0 },
	{ "invd", &Instruction::decode0, OP_INVD, FLAG_NONE, 0 },
	{ "wbinvd", &Instruction::decode0, OP_WBINVD, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "ud2", &Instruction::decode0, OP_UD2, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x10 - 0x0f 0x1f */
	{ "movups", &Instruction::decode_Vo_Wo, OP_MOVUPS, FLAG_NONE, 2 },
	{ "movups", &Instruction::decode_Wo_Vo, OP_MOVUPS, FLAG_NONE, 2 },
	{ "movlps", &Instruction::decode_Vq_Mq, OP_MOVLPS, FLAG_NONE, 2 },
	{ "movlps", &Instruction::decode_Mq_Vq, OP_MOVLPS, FLAG_NONE, 2 },
	{ "unpcklps", &Instruction::decode_Vo_Wq, OP_UNPCKLPS, FLAG_NONE, 2 },
	{ "unpckhps", &Instruction::decode_Vo_Wq, OP_UNPCKHPS, FLAG_NONE, 2 },
	{ "movhps", &Instruction::decode_Vq_Mq, OP_MOVHPS, FLAG_NONE, 2 },
	{ "movhps", &Instruction::decode_Mq_Vq, OP_MOVHPS, FLAG_NONE, 2 },
	{ "group17", &Instruction::decode_group17, OP_GROUP17, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },

	/* 0x0f 0x20 - 0x0f 0x2f */
	{ "mov", &Instruction::decode_Rd_Cd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Rd_Dd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Cd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Dd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movaps", &Instruction::decode_Vo_Wo, OP_MOVAPS, FLAG_NONE, 2 },
	{ "movaps", &Instruction::decode_Wo_Vo, OP_MOVAPS, FLAG_NONE, 2 },
	{ "cvtpi2ps", &Instruction::decode_Vo_Qq, OP_CVTPI2PS, FLAG_NONE, 2 },
	{ "movntps",  &Instruction::decode_Mo_Vo, OP_MOVNTPS, FLAG_NONE, 2 },
	{ "cvttps2pi",  &Instruction::decode_Pq_Wo, OP_CVTTPS2PI, FLAG_NONE, 2 },
	{ "cvtps2pi",  &Instruction::decode_Pq_Wo, OP_CVTPS2PI, FLAG_NONE, 2 },
	{ "ucomiss",  &Instruction::decode_Vo_Wo, OP_UCOMISS, FLAG_NONE, 2 },
	{ "comiss",  &Instruction::decode_Vo_Wo, OP_COMISS, FLAG_NONE, 2 },

	/* 0x0f 0x30 - 0x0f 0x3f */
	{ "wrmsr", &Instruction::decode0, OP_WRMSR, FLAG_NONE, 0 },
	{ "rdtsc", &Instruction::decode0, OP_RDTSC, FLAG_NONE, 0 },
	{ "rdmsr", &Instruction::decode0, OP_RDMSR, FLAG_NONE, 0 },
	{ "rdpmc", &Instruction::decode0, OP_RDPMC, FLAG_NONE, 0 },
	{ "sysenter", &Instruction::decode0, OP_SYSENTER, FLAG_NONE, 0 },
	{ "sysexit", &Instruction::decode0, OP_SYSEXIT, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "getsec", &Instruction::decode0, OP_GETSEC, FLAG_NONE, 0 },
	{ "3byte", &Instruction::decode_3byte_38, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "3byte", &Instruction::decode_3byte_3A, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x40 - 0x0f 0x4f */
	{ "cmovo", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovno", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnbe", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovs", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovns", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },

	/* 0x0f 0x50 - 0x0f 0x5f */
	{ "movmskps", &Instruction::decode_Gq_Uo, OP_MOVMSKPS, FLAG_NONE, 2 }, // x86: movmskps Gd, Uo
	{ "sqrtps", &Instruction::decode_Vo_Wo, OP_SQRTPS, FLAG_NONE, 2 },
	{ "rsqrtps", &Instruction::decode_Vo_Wo, OP_RSQRTPS, FLAG_NONE, 2 },
	{ "rcpps", &Instruction::decode_Vo_Wo, OP_RCPPS, FLAG_NONE, 2 },
	{ "andps", &Instruction::decode_Vo_Wo, OP_ANDPS, FLAG_NONE, 2 },
	{ "andnps", &Instruction::decode_Vo_Wo, OP_ANDNPS, FLAG_NONE, 2 },
	{ "orps", &Instruction::decode_Vo_Wo, OP_ORPS, FLAG_NONE, 2 },
	{ "xorps", &Instruction::decode_Vo_Wo, OP_XORPS, FLAG_NONE, 2 },
	{ "addps", &Instruction::decode_Vo_Wo, OP_ADDPS, FLAG_NONE, 2 },
	{ "mulps", &Instruction::decode_Vo_Wo, OP_MULPS, FLAG_NONE, 2 },
	{ "cvtps2pd", &Instruction::decode_Vo_Wo, OP_CVTPS2PD, FLAG_NONE, 2 },
	{ "cvtdq2ps", &Instruction::decode_Vo_Wo, OP_CVTDQ2PS, FLAG_NONE, 2 },
	{ "subps", &Instruction::decode_Vo_Wo, OP_SUBPS, FLAG_NONE, 2 },
	{ "minps", &Instruction::decode_Vo_Wo, OP_MINPS, FLAG_NONE, 2 },
	{ "divps", &Instruction::decode_Vo_Wo, OP_DIVPS, FLAG_NONE, 2 },
	{ "maxps", &Instruction::decode_Vo_Wo, OP_MAXPS, FLAG_NONE, 2 },

	/* 0x0f 0x60 - 0x0f 0x6f */
	{ "punpcklbw", &Instruction::decode_Pq_Qd, OP_PUNPCKLBW, FLAG_NONE, 2 },
	{ "punpcklwd", &Instruction::decode_Pq_Qd, OP_PUNPCKLWD, FLAG_NONE, 2 },
	{ "punpckldq", &Instruction::decode_Pq_Qd, OP_PUNPCKLDQ, FLAG_NONE, 2 },
	{ "packsswb", &Instruction::decode_Pq_Qq, OP_PACKSSWB, FLAG_NONE, 2 },
	{ "pcmpgtb", &Instruction::decode_Pq_Qq, OP_PCMPGTB, FLAG_NONE, 2 },
	{ "pcmpgtw", &Instruction::decode_Pq_Qq, OP_PCMPGTW, FLAG_NONE, 2 },
	{ "pcmpgtd", &Instruction::decode_Pq_Qq, OP_PCMPGTD, FLAG_NONE, 2 },
	{ "packuswb", &Instruction::decode_Pq_Qq, OP_PACKUSWB, FLAG_NONE, 2 },
	{ "punpckhbw", &Instruction::decode_Pq_Qd, OP_PUNPCKHBW, FLAG_NONE, 2 },
	{ "punpckhwd", &Instruction::decode_Pq_Qd, OP_PUNPCKHWD, FLAG_NONE, 2 },
	{ "punpckhdq", &Instruction::decode_Pq_Qd, OP_PUNPCKHDQ, FLAG_NONE, 2 },
	{ "packssdw", &Instruction::decode_Pq_Qq, OP_PACKSSDW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movq", &Instruction::decode_Pd_Eq, OP_MOVQ, FLAG_NONE, 2 }, // x86: movd Pd, Ed
	{ "movq", &Instruction::decode_Pq_Qq, OP_MOVQ, FLAG_NONE, 2 },

	/* 0x0f 0x70 - 0x0f 0x7f */
	{ "pshufw", &Instruction::decode_Pq_Qq_Ib, OP_PSHUFW, FLAG_NONE, 3 },
	{ "group13", &Instruction::decode_group13, OP_GROUP13, FLAG_NONE, -1 },
	{ "group14", &Instruction::decode_group14, OP_GROUP14, FLAG_NONE, -1 },
	{ "group15", &Instruction::decode_group15, OP_GROUP15, FLAG_NONE, -1 },
	{ "pcmpeqb", &Instruction::decode_Pq_Qq, OP_PCMPEQB, FLAG_NONE, 2 },
	{ "pcmpeqw", &Instruction::decode_Pq_Qq, OP_PCMPEQW, FLAG_NONE, 2 },
	{ "pcmpeqd", &Instruction::decode_Pq_Qq, OP_PCMPEQD, FLAG_NONE, 2 },
	{ "emms", &Instruction::decode0, OP_EMMS, FLAG_NONE, 0 },
	{ "vmread", &Instruction::decode_Eq_Gq, OP_VMREAD, FLAG_NONE, 2 },   // x86: vmread Ed, Gq
	{ "vmwrite", &Instruction::decode_Gq_Eq, OP_VMWRITE, FLAG_NONE, 2 }, // x86: vmwrite Gd, Ed
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movq", &Instruction::decode_Eq_Pd, OP_MOVQ, FLAG_NONE, 2 },       // x86: movq Ed,Pd
	{ "movq", &Instruction::decode_Qq_Pq, OP_MOVQ, FLAG_NONE, 2 },

	/* 0x0f 0x80 - 0x0f 0x8f */
	{ "jo", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jno", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "js", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jns", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },

	/* 0x0f 0x90 - 0x0f 0x9f */
	{ "seto", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setno", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "sets", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setns", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },

	/* 0x0f 0xa0 - 0x0f 0xaf */
	{ "push", &Instruction::decode_Reg<operand_t::REG_FS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_FS>, OP_POP, FLAG_NONE, 1 },
	{ "cpuid", &Instruction::decode0, OP_CPUID, FLAG_NONE, 0 },
	{ "bt", &Instruction::decode_Ev_Gv, OP_BT, FLAG_NONE, 2 },
	{ "shld", &Instruction::decode_Ev_Gv_Ib, OP_SHLD, FLAG_NONE, 3 },
	{ "shld", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHLD, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "push", &Instruction::decode_Reg<operand_t::REG_GS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_GS>, OP_POP, FLAG_NONE, 1 },
	{ "rsm", &Instruction::decode0, OP_RSM, FLAG_NONE, 0 },
	{ "bts", &Instruction::decode_Ev_Gv, OP_BTS, FLAG_NONE, 2 },
	{ "shrd", &Instruction::decode_Ev_Gv_Ib, OP_SHRD, FLAG_NONE, 3 },
	{ "shrd", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHRD, FLAG_NONE, 3 },
	{ "group16", &Instruction::decode_group16, OP_GROUP16, FLAG_NONE, -1 },
	{ "imul",  &Instruction::decode_Gv_Ev, OP_IMUL, FLAG_NONE, 2 },

	/* 0x0f 0xb0 - 0x0f 0xbf */
	{ "cmpxchg", &Instruction::decode_Eb_Gb, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "cmpxchg", &Instruction::decode_Ev_Gv, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "lss", &Instruction::decode_Gv_Mp, OP_LSS, FLAG_NONE, 2 },
	{ "btr", &Instruction::decode_Ev_Gv, OP_BTR, FLAG_NONE, 2 },
	{ "lfs", &Instruction::decode_Gv_Mp, OP_LFS, FLAG_NONE, 2 },
	{ "lgs", &Instruction::decode_Gv_Mp, OP_LGS, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Eb, OP_MOVZX, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Ew, OP_MOVZX, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "group11", &Instruction::decode_group11, OP_GROUP11, FLAG_NONE, -1 },
	{ "group8", &Instruction::decode_group8, OP_GROUP8, FLAG_NONE, -1 },
	{ "btc", &Instruction::decode_Ev_Gv, OP_BTC, FLAG_NONE, 2 },
	{ "bsf", &Instruction::decode_Gv_Ev, OP_BSF, FLAG_NONE, 2 },
	{ "bsr", &Instruction::decode_Gv_Ev, OP_BSR, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Eb, OP_MOVSX, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Ew, OP_MOVSX, FLAG_NONE, 2 },

	/* 0x0f 0xc0 - 0x0f 0xcf */
	{ "xadd", &Instruction::decode_Eb_Gb, OP_XADD, FLAG_NONE, 2 },
	{ "xadd", &Instruction::decode_Ev_Gv, OP_XADD, FLAG_NONE, 2 },
	{ "cmpps", &Instruction::decode_Vo_Wo_Ib, OP_CMPPS, FLAG_NONE, 3 },
	{ "movnti", &Instruction::decode_Nq_Gq, OP_MOVNTI, FLAG_NONE, 2 },       // x86: movnti Md, Gd
	{ "pinsrw", &Instruction::decode_Pq_Rq_Mw_Ib, OP_PINSRW, FLAG_NONE, 3 }, // x86: pinsrw Pq, Rd/Mw,Ib
	{ "pextrw", &Instruction::decode_Gd_Nq_Ib, OP_PEXTRW, FLAG_NONE, 3 },
	{ "shufps", &Instruction::decode_Vo_Wo_Ib, OP_SHUFPS, FLAG_NONE, 3 },
	{ "group9", &Instruction::decode_group9, OP_GROUP9, FLAG_NONE, -1 },
	{ "bswap", &Instruction::decode_rAX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rCX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSI, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDI, OP_BSWAP, FLAG_NONE, 1 },

	/* 0x0f 0xd0 - 0x0f 0xdf */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "psrlw", &Instruction::decode_Pq_Qq, OP_PSRLW, FLAG_NONE, 2 },
	{ "psrld", &Instruction::decode_Pq_Qq, OP_PSRLD, FLAG_NONE, 2 },
	{ "psrlq", &Instruction::decode_Pq_Qq, OP_PSRLQ, FLAG_NONE, 2 },
	{ "paddq", &Instruction::decode_Pq_Qq, OP_PADDQ, FLAG_NONE, 2 },
	{ "pmullw", &Instruction::decode_Pq_Qq, OP_PMULLW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "pmovmskb", &Instruction::decode_Gd_Nq, OP_PMOVMSKB, FLAG_NONE, 2 },
    { "psubusb", &Instruction::decode_Pq_Qq, OP_PSUBUSB, FLAG_NONE, 2 },
    { "psubusw", &Instruction::decode_Pq_Qq, OP_PSUBUSW, FLAG_NONE, 2 },
    { "pminub", &Instruction::decode_Pq_Qq, OP_PMINUB, FLAG_NONE, 2 },
    { "pand", &Instruction::decode_Pq_Qq, OP_PAND, FLAG_NONE, 2 },
    { "paddusb", &Instruction::decode_Pq_Qq, OP_PADDUSB, FLAG_NONE, 2 },
    { "paddusw", &Instruction::decode_Pq_Qq, OP_PADDUSW, FLAG_NONE, 2 },
    { "pmaxub", &Instruction::decode_Pq_Qq, OP_PMAXUB, FLAG_NONE, 2 },
    { "pandn", &Instruction::decode_Pq_Qq, OP_PANDN, FLAG_NONE, 2 },

	/* 0x0f 0xe0 - 0x0f 0xef */
	{ "pavgb", &Instruction::decode_Pq_Qq, OP_PAVGB, FLAG_NONE, 2 },
	{ "psraw", &Instruction::decode_Pq_Qq, OP_PSRAW, FLAG_NONE, 2 },
	{ "psrad", &Instruction::decode_Pq_Qq, OP_PSRAD, FLAG_NONE, 2 },
	{ "pavgw", &Instruction::decode_Pq_Qq, OP_PAVGW, FLAG_NONE, 2 },
	{ "pmulhuw", &Instruction::decode_Pq_Qq, OP_PMULHUW, FLAG_NONE, 2 },
	{ "pmulhw", &Instruction::decode_Pq_Qq, OP_PMULHW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movntq", &Instruction::decode_Mq_Pq, OP_MOVNTQ, FLAG_NONE, 2 },
	{ "psubsb", &Instruction::decode_Pq_Qq, OP_PSUBSB, FLAG_NONE, 2 },
	{ "psubsw", &Instruction::decode_Pq_Qq, OP_PSUBSW, FLAG_NONE, 2 },
	{ "pminsw", &Instruction::decode_Pq_Qq, OP_PMINSW, FLAG_NONE, 2 },
	{ "por", &Instruction::decode_Pq_Qq, OP_POR, FLAG_NONE, 2 },
	{ "paddsb", &Instruction::decode_Pq_Qq, OP_PADDSB, FLAG_NONE, 2 },
	{ "paddsw", &Instruction::decode_Pq_Qq, OP_PADDSW, FLAG_NONE, 2 },
	{ "pmaxsw", &Instruction::decode_Pq_Qq, OP_PMAXSW, FLAG_NONE, 2 },
	{ "pxor", &Instruction::decode_Pq_Qq, OP_PXOR, FLAG_NONE, 2 },

	/* 0x0f 0xf0 - 0x0f 0xff */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "psllw", &Instruction::decode_Pq_Qq, OP_PSLLW, FLAG_NONE, 2 },
	{ "pslld", &Instruction::decode_Pq_Qq, OP_PSLLD, FLAG_NONE, 2 },
	{ "psllq", &Instruction::decode_Pq_Qq, OP_PSLLQ, FLAG_NONE, 2 },
	{ "pmuludq", &Instruction::decode_Pq_Qq, OP_PMULUDQ, FLAG_NONE, 2 },
	{ "pmaddwd", &Instruction::decode_Pq_Qq, OP_PMADDWD, FLAG_NONE, 2 },
	{ "psadbw", &Instruction::decode_Pq_Qq, OP_PSADBW, FLAG_NONE, 2 },
	{ "maskmovq", &Instruction::decode_Pq_Nq, OP_MASKMOVQ, FLAG_NONE, 2 },
	{ "psubb", &Instruction::decode_Pq_Qq, OP_PSUBB, FLAG_NONE, 2 },
	{ "psubw", &Instruction::decode_Pq_Qq, OP_PSUBW, FLAG_NONE, 2 },
	{ "psubd", &Instruction::decode_Pq_Qq, OP_PSUBD, FLAG_NONE, 2 },
	{ "psubq", &Instruction::decode_Pq_Qq, OP_PSUBQ, FLAG_NONE, 2 },
	{ "paddb", &Instruction::decode_Pq_Qq, OP_PADDB, FLAG_NONE, 2 },
	{ "paddw", &Instruction::decode_Pq_Qq, OP_PADDW, FLAG_NONE, 2 },
	{ "paddd", &Instruction::decode_Pq_Qq, OP_PADDD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
};


// 2 byte opcodes with a 66 prefix
template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_2Byte_66[0x100] = {

	/* 0x0f 0x00 - 0x0f 0x0f */
	{ "group6", &Instruction::decode_group6, OP_GROUP6, FLAG_NONE, -1 },
	{ "group7", &Instruction::decode_group7, OP_GROUP7, FLAG_NONE, -1 },
	{ "lar", &Instruction::decode_Gv_Ew, OP_LAR, FLAG_NONE, 2 },
	{ "lsl", &Instruction::decode_Gv_Ew, OP_LSL, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "syscall", &Instruction::decode0, OP_SYSCALL, FLAG_NONE, 0 },
	{ "clts", &Instruction::decode0, OP_CLTS, FLAG_NONE, 0 },
	{ "sysret", &Instruction::decode0, OP_SYSRET, FLAG_NONE, 0 },
	{ "invd", &Instruction::decode0, OP_INVD, FLAG_NONE, 0 },
	{ "wbinvd", &Instruction::decode0, OP_WBINVD, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "ud2", &Instruction::decode0, OP_UD2, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x10 - 0x0f 0x1f */
	{ "movupd", &Instruction::decode_Vo_Wo, OP_MOVUPD, FLAG_NONE, 2 },
	{ "movupd", &Instruction::decode_Wo_Vo, OP_MOVUPD, FLAG_NONE, 2 },
	{ "movlpd", &Instruction::decode_Vq_Mq, OP_MOVLPD, FLAG_NONE, 2 },
	{ "movlpd", &Instruction::decode_Mq_Vq, OP_MOVLPD, FLAG_NONE, 2 },
	{ "unpcklpd", &Instruction::decode_Vo_Wq, OP_UNPCKLPD, FLAG_NONE, 2 },
	{ "unpckhpd", &Instruction::decode_Vo_Wq, OP_UNPCKHPD, FLAG_NONE, 2 },
	{ "movhpd", &Instruction::decode_Vq_Mq, OP_MOVHPD, FLAG_NONE, 2 },
	{ "movhpd", &Instruction::decode_Mq_Vq, OP_MOVHPD, FLAG_NONE, 2 },
	{ "group17", &Instruction::decode_group17, OP_GROUP17, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },

	/* 0x0f 0x20 - 0x0f 0x2f */
	{ "mov", &Instruction::decode_Rd_Cd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Rd_Dd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Cd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Dd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movapd", &Instruction::decode_Vo_Wo, OP_MOVAPD, FLAG_NONE, 2 },
	{ "movapd", &Instruction::decode_Wo_Vo, OP_MOVAPD, FLAG_NONE, 2 },
	{ "cvtpi2pd", &Instruction::decode_Vo_Qq, OP_CVTPI2PD, FLAG_NONE, 2 },
	{ "movntpd",  &Instruction::decode_Mo_Vo, OP_MOVNTPD, FLAG_NONE, 2 },
	{ "cvttpd2pi",  &Instruction::decode_Pq_Wo, OP_CVTTPD2PI, FLAG_NONE, 2 },
	{ "cvtpd2pi",  &Instruction::decode_Qq_Wo, OP_CVTPD2PI, FLAG_NONE, 2 },
	{ "ucomisd",  &Instruction::decode_Vo_Wo, OP_UCOMISD, FLAG_NONE, 2 },
	{ "comisd",  &Instruction::decode_Vo_Wo, OP_COMISD, FLAG_NONE, 2 },

	/* 0x0f 0x30 - 0x0f 0x3f */
	{ "wrmsr", &Instruction::decode0, OP_WRMSR, FLAG_NONE, 0 },
	{ "rdtsc", &Instruction::decode0, OP_RDTSC, FLAG_NONE, 0 },
	{ "rdmsr", &Instruction::decode0, OP_RDMSR, FLAG_NONE, 0 },
	{ "rdpmc", &Instruction::decode0, OP_RDPMC, FLAG_NONE, 0 },
	{ "sysenter", &Instruction::decode0, OP_SYSENTER, FLAG_NONE, 0 },
	{ "sysexit", &Instruction::decode0, OP_SYSEXIT, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "getsec", &Instruction::decode0, OP_GETSEC, FLAG_NONE, 0 },
	{ "3byte", &Instruction::decode_3byte_38, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "3byte", &Instruction::decode_3byte_3A, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x40 - 0x0f 0x4f */
	{ "cmovo", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovno", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnbe", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovs", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovns", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },

	/* 0x0f 0x50 - 0x0f 0x5f */
	{ "movmskpd", &Instruction::decode_Gq_Uo, OP_MOVMSKPD, FLAG_NONE, 2 }, // x86: movmskpd Gd, Uo
	{ "sqrtpd", &Instruction::decode_Vo_Wo, OP_SQRTPD, FLAG_NONE, 2 },
	{ "rsqrtps", &Instruction::decode_Vo_Wo, OP_RSQRTPS, FLAG_NONE, 2 },
	{ "rcpps", &Instruction::decode_Vo_Wo, OP_RCPPS, FLAG_NONE, 2 },
	{ "andpd", &Instruction::decode_Vo_Wo, OP_ANDPD, FLAG_NONE, 2 },
	{ "andnpd", &Instruction::decode_Vo_Wo, OP_ANDNPD, FLAG_NONE, 2 },
	{ "orpd", &Instruction::decode_Vo_Wo, OP_ORPD, FLAG_NONE, 2 },
	{ "xorpd", &Instruction::decode_Vo_Wo, OP_XORPD, FLAG_NONE, 2 },
	{ "addpd", &Instruction::decode_Vo_Wo, OP_ADDPD, FLAG_NONE, 2 },
	{ "mulpd", &Instruction::decode_Vo_Wo, OP_MULPD, FLAG_NONE, 2 },
	{ "cvtpd2ps", &Instruction::decode_Vo_Wo, OP_CVTPD2PS, FLAG_NONE, 2 },
	{ "cvtps2dq", &Instruction::decode_Vo_Wo, OP_CVTPS2DQ, FLAG_NONE, 2 },
	{ "subpd", &Instruction::decode_Vo_Wo, OP_SUBPD, FLAG_NONE, 2 },
	{ "minpd", &Instruction::decode_Vo_Wo, OP_MINPD, FLAG_NONE, 2 },
	{ "divpd", &Instruction::decode_Vo_Wo, OP_DIVPD, FLAG_NONE, 2 },
	{ "maxpd", &Instruction::decode_Vo_Wo, OP_MAXPD, FLAG_NONE, 2 },

	/* 0x0f 0x60 - 0x0f 0x6f */
	{ "punpcklbw", &Instruction::decode_Vo_Wo, OP_PUNPCKLBW, FLAG_NONE, 2 },
	{ "punpcklwd", &Instruction::decode_Vo_Wo, OP_PUNPCKLWD, FLAG_NONE, 2 },
	{ "punpckldq", &Instruction::decode_Vo_Wo, OP_PUNPCKLDQ, FLAG_NONE, 2 },
	{ "packsswb", &Instruction::decode_Vo_Wo, OP_PACKSSWB, FLAG_NONE, 2 },
	{ "pcmpgtb", &Instruction::decode_Vo_Wo, OP_PCMPGTB, FLAG_NONE, 2 },
	{ "pcmpgtw", &Instruction::decode_Vo_Wo, OP_PCMPGTW, FLAG_NONE, 2 },
	{ "pcmpgtd", &Instruction::decode_Vo_Wo, OP_PCMPGTD, FLAG_NONE, 2 },
	{ "packuswb", &Instruction::decode_Vo_Wo, OP_PACKUSWB, FLAG_NONE, 2 },
	{ "punpckhbw", &Instruction::decode_Vo_Wo, OP_PUNPCKHBW, FLAG_NONE, 2 },
	{ "punpckhwd", &Instruction::decode_Vo_Wo, OP_PUNPCKHWD, FLAG_NONE, 2 },
	{ "punpckhdq", &Instruction::decode_Vo_Wo, OP_PUNPCKHDQ, FLAG_NONE, 2 },
	{ "packssdw", &Instruction::decode_Vo_Wo, OP_PACKSSDW, FLAG_NONE, 2 },
	{ "punpcklqdq", &Instruction::decode_Vo_Wo, OP_PUNPCKLQDQ, FLAG_NONE, 2 },
	{ "punpckhqdq", &Instruction::decode_Vo_Wo, OP_PUNPCKHQDQ, FLAG_NONE, 2 },
	{ "movq", &Instruction::decode_Vo_Eq, OP_MOVQ, FLAG_NONE, 2 }, // x86: movd Vo, Ed
	{ "movq", &Instruction::decode_Vo_Qq, OP_MOVQ, FLAG_NONE, 2 },

	/* 0x0f 0x70 - 0x0f 0x7f */
	{ "pshufd", &Instruction::decode_Vo_Wo_Ib, OP_PSHUFD, FLAG_NONE, 3 },
	{ "group13", &Instruction::decode_group13, OP_GROUP13, FLAG_NONE, -1 },
	{ "group14", &Instruction::decode_group14, OP_GROUP14, FLAG_NONE, -1 },
	{ "group15", &Instruction::decode_group15, OP_GROUP15, FLAG_NONE, -1 },
	{ "pcmpeqb", &Instruction::decode_Vo_Wo, OP_PCMPEQB, FLAG_NONE, 2 },
	{ "pcmpeqw", &Instruction::decode_Vo_Wo, OP_PCMPEQW, FLAG_NONE, 2 },
	{ "pcmpeqd", &Instruction::decode_Vo_Wo, OP_PCMPEQD, FLAG_NONE, 2 },
	{ "emms", &Instruction::decode0, OP_EMMS, FLAG_NONE, 0 },
	{ "vmread", &Instruction::decode_Eq_Gq, OP_VMREAD, FLAG_NONE, 2 },   // x86: vmread Ed, Gd
	{ "vmwrite", &Instruction::decode_Gq_Eq, OP_VMWRITE, FLAG_NONE, 2 }, // x86: vmwrite Gd, Ed
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "haddpd", &Instruction::decode_Vo_Wo, OP_HADDPD, FLAG_NONE, 2 },
	{ "hsubpd", &Instruction::decode_Vo_Wo, OP_HSUBPD, FLAG_NONE, 2 },
	{ "movq", &Instruction::decode_Eq_Vo, OP_MOVD, FLAG_NONE, 2 },       // x86: movd Ed, Vo
	{ "movdqa", &Instruction::decode_Wo_Vo, OP_MOVDQA, FLAG_NONE, 2 },

	/* 0x0f 0x80 - 0x0f 0x8f */
	{ "jo", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jno", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "js", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jns", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },

	/* 0x0f 0x90 - 0x0f 0x9f */
	{ "seto", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setno", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "sets", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setns", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },

	/* 0x0f 0xa0 - 0x0f 0xaf */
	{ "push", &Instruction::decode_Reg<operand_t::REG_FS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_FS>, OP_POP, FLAG_NONE, 1 },
	{ "cpuid", &Instruction::decode0, OP_CPUID, FLAG_NONE, 0 },
	{ "bt", &Instruction::decode_Ev_Gv, OP_BT, FLAG_NONE, 2 },
	{ "shld", &Instruction::decode_Ev_Gv_Ib, OP_SHLD, FLAG_NONE, 3 },
	{ "shld", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHLD, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "push", &Instruction::decode_Reg<operand_t::REG_GS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_GS>, OP_POP, FLAG_NONE, 1 },
	{ "rsm", &Instruction::decode0, OP_RSM, FLAG_NONE, 0 },
	{ "bts", &Instruction::decode_Ev_Gv, OP_BTS, FLAG_NONE, 2 },
	{ "shrd", &Instruction::decode_Ev_Gv_Ib, OP_SHRD, FLAG_NONE, 3 },
	{ "shrd", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHRD, FLAG_NONE, 3 },
	{ "group16", &Instruction::decode_group16, OP_GROUP16, FLAG_NONE, -1 },
	{ "imul",  &Instruction::decode_Gv_Ev, OP_IMUL, FLAG_NONE, 2 },

	/* 0x0f 0xb0 - 0x0f 0xbf */
	{ "cmpxchg", &Instruction::decode_Eb_Gb, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "cmpxchg", &Instruction::decode_Ev_Gv, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "lss", &Instruction::decode_Gv_Mp, OP_LSS, FLAG_NONE, 2 },
	{ "btr", &Instruction::decode_Ev_Gv, OP_BTR, FLAG_NONE, 2 },
	{ "lfs", &Instruction::decode_Gv_Mp, OP_LFS, FLAG_NONE, 2 },
	{ "lgs", &Instruction::decode_Gv_Mp, OP_LGS, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Eb, OP_MOVZX, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Ew, OP_MOVZX, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "group11", &Instruction::decode_group11, OP_GROUP11, FLAG_NONE, -1 },
	{ "group8", &Instruction::decode_group8, OP_GROUP8, FLAG_NONE, -1 },
	{ "btc", &Instruction::decode_Ev_Gv, OP_BTC, FLAG_NONE, 2 },
	{ "bsf", &Instruction::decode_Gv_Ev, OP_BSF, FLAG_NONE, 2 },
	{ "bsr", &Instruction::decode_Gv_Ev, OP_BSR, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Eb, OP_MOVSX, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Ew, OP_MOVSX, FLAG_NONE, 2 },

	/* 0x0f 0xc0 - 0x0f 0xcf */
	{ "xadd", &Instruction::decode_Eb_Gb, OP_XADD, FLAG_NONE, 2 },
	{ "xadd", &Instruction::decode_Ev_Gv, OP_XADD, FLAG_NONE, 2 },
	{ "cmppd", &Instruction::decode_Vo_Wo_Ib, OP_CMPPD, FLAG_NONE, 3 },
	{ "movnti", &Instruction::decode_Nq_Gq, OP_MOVNTI, FLAG_NONE, 2 },       // x86: movnti Md, Gd
	{ "pinsrw", &Instruction::decode_Vo_Rq_Mw_Ib, OP_PINSRW, FLAG_NONE, 3 }, // x86: pinsrw Vo, Rd/Mw,Ib
	{ "pextrw", &Instruction::decode_Gd_Uo_Ib, OP_PEXTRW, FLAG_NONE, 3 },
	{ "shufpd", &Instruction::decode_Vo_Wo_Ib, OP_SHUFPD, FLAG_NONE, 3 },
	{ "group9", &Instruction::decode_group9, OP_GROUP9, FLAG_NONE, -1 },
	{ "bswap", &Instruction::decode_rAX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rCX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSI, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDI, OP_BSWAP, FLAG_NONE, 1 },

	/* 0x0f 0xd0 - 0x0f 0xdf */
	{ "addsubpd", &Instruction::decode_Vo_Wo, OP_ADDSUBPD, FLAG_NONE, 2 },
	{ "psrlw", &Instruction::decode_Vo_Wo, OP_PSRLW, FLAG_NONE, 2 },
	{ "psrld", &Instruction::decode_Vo_Wo, OP_PSRLD, FLAG_NONE, 2 },
	{ "psrlq", &Instruction::decode_Vo_Wo, OP_PSRLQ, FLAG_NONE, 2 },
	{ "paddq", &Instruction::decode_Vo_Wo, OP_PADDQ, FLAG_NONE, 2 },
	{ "pmullw", &Instruction::decode_Vo_Wo, OP_PMULLW, FLAG_NONE, 2 },
	{ "movq", &Instruction::decode_Vo_Wo, OP_MOVQ, FLAG_NONE, 2 },
	{ "pmovmskb", &Instruction::decode_Gd_Uo, OP_PMOVMSKB, FLAG_NONE, 2 },
    { "psubusb", &Instruction::decode_Vo_Wo, OP_PSUBUSB, FLAG_NONE, 2 },
    { "psubusw", &Instruction::decode_Vo_Wo, OP_PSUBUSW, FLAG_NONE, 2 },
    { "pminub", &Instruction::decode_Vo_Wo, OP_PMINUB, FLAG_NONE, 2 },
    { "pand", &Instruction::decode_Vo_Wo, OP_PAND, FLAG_NONE, 2 },
    { "paddusb", &Instruction::decode_Vo_Wo, OP_PADDUSB, FLAG_NONE, 2 },
    { "paddusw", &Instruction::decode_Vo_Wo, OP_PADDUSW, FLAG_NONE, 2 },
    { "pmaxub", &Instruction::decode_Vo_Wo, OP_PMAXUB, FLAG_NONE, 2 },
    { "pandn", &Instruction::decode_Vo_Wo, OP_PANDN, FLAG_NONE, 2 },

	/* 0x0f 0xe0 - 0x0f 0xef */
	{ "pavgb", &Instruction::decode_Vo_Wo, OP_PAVGB, FLAG_NONE, 2 },
	{ "psraw", &Instruction::decode_Vo_Wo, OP_PSRAW, FLAG_NONE, 2 },
	{ "psrad", &Instruction::decode_Vo_Wo, OP_PSRAD, FLAG_NONE, 2 },
	{ "pavgw", &Instruction::decode_Vo_Wo, OP_PAVGW, FLAG_NONE, 2 },
	{ "pmulhuw", &Instruction::decode_Vo_Wo, OP_PMULHUW, FLAG_NONE, 2 },
	{ "pmulhw", &Instruction::decode_Vo_Wo, OP_PMULHW, FLAG_NONE, 2 },
	{ "cvttpd2dq", &Instruction::decode_Vo_Wo, OP_CVTTPD2DQ, FLAG_NONE, 2 },
	{ "movntdq", &Instruction::decode_Mo_Vo, OP_MOVNTDQ, FLAG_NONE, 2 },
	{ "psubsb", &Instruction::decode_Vo_Wo, OP_PSUBSB, FLAG_NONE, 2 },
	{ "psubsw", &Instruction::decode_Vo_Wo, OP_PSUBSW, FLAG_NONE, 2 },
	{ "pminsw", &Instruction::decode_Vo_Wo, OP_PMINSW, FLAG_NONE, 2 },
	{ "por", &Instruction::decode_Vo_Wo, OP_POR, FLAG_NONE, 2 },
	{ "paddsb", &Instruction::decode_Vo_Wo, OP_PADDSB, FLAG_NONE, 2 },
	{ "paddsw", &Instruction::decode_Vo_Wo, OP_PADDSW, FLAG_NONE, 2 },
	{ "pmaxsw", &Instruction::decode_Vo_Wo, OP_PMAXSW, FLAG_NONE, 2 },
	{ "pxor", &Instruction::decode_Vo_Wo, OP_PXOR, FLAG_NONE, 2 },

	/* 0x0f 0xf0 - 0x0f 0xff */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "psllw", &Instruction::decode_Vo_Wo, OP_PSLLW, FLAG_NONE, 2 },
	{ "pslld", &Instruction::decode_Vo_Wo, OP_PSLLD, FLAG_NONE, 2 },
	{ "psllq", &Instruction::decode_Vo_Wo, OP_PSLLQ, FLAG_NONE, 2 },
	{ "pmuludq", &Instruction::decode_Vo_Wo, OP_PMULUDQ, FLAG_NONE, 2 },
	{ "pmaddwd", &Instruction::decode_Vo_Wo, OP_PMADDWD, FLAG_NONE, 2 },
	{ "psadbw", &Instruction::decode_Vo_Wo, OP_PSADBW, FLAG_NONE, 2 },
	{ "maskmovdqu", &Instruction::decode_Vo_Uo, OP_MASKMOVDQU, FLAG_NONE, 2 },
	{ "psubb", &Instruction::decode_Vo_Wo, OP_PSUBB, FLAG_NONE, 2 },
	{ "psubw", &Instruction::decode_Vo_Wo, OP_PSUBW, FLAG_NONE, 2 },
	{ "psubd", &Instruction::decode_Vo_Wo, OP_PSUBD, FLAG_NONE, 2 },
	{ "psubq", &Instruction::decode_Vo_Wo, OP_PSUBQ, FLAG_NONE, 2 },
	{ "paddb", &Instruction::decode_Vo_Wo, OP_PADDB, FLAG_NONE, 2 },
	{ "paddw", &Instruction::decode_Vo_Wo, OP_PADDW, FLAG_NONE, 2 },
	{ "paddd", &Instruction::decode_Vo_Wo, OP_PADDD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
};

// 2 byte opcodes with a f2 prefix
template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_2Byte_F2[0x100] = {

	/* 0x0f 0x00 - 0x0f 0x0f */
	{ "group6", &Instruction::decode_group6, OP_GROUP6, FLAG_NONE, -1 },
	{ "group7", &Instruction::decode_group7, OP_GROUP7, FLAG_NONE, -1 },
	{ "lar", &Instruction::decode_Gv_Ew, OP_LAR, FLAG_NONE, 2 },
	{ "lsl", &Instruction::decode_Gv_Ew, OP_LSL, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "syscall", &Instruction::decode0, OP_SYSCALL, FLAG_NONE, 0 },
	{ "clts", &Instruction::decode0, OP_CLTS, FLAG_NONE, 0 },
	{ "sysret", &Instruction::decode0, OP_SYSRET, FLAG_NONE, 0 },
	{ "invd", &Instruction::decode0, OP_INVD, FLAG_NONE, 0 },
	{ "wbinvd", &Instruction::decode0, OP_WBINVD, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "ud2", &Instruction::decode0, OP_UD2, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x10 - 0x0f 0x1f */
	{ "movsd", &Instruction::decode_Vo_Wo, OP_MOVSD, FLAG_NONE, 2 },
	{ "movsd", &Instruction::decode_Wo_Vo, OP_MOVSD, FLAG_NONE, 2 },
	{ "movddup", &Instruction::decode_Vq_Mq, OP_MOVDDUP, FLAG_NONE, 2 },
	{ "movlps", &Instruction::decode_Mq_Vq, OP_MOVLPS, FLAG_NONE, 2 },
	{ "unpcklps", &Instruction::decode_Vo_Wq, OP_UNPCKLPS, FLAG_NONE, 2 },
	{ "unpckhps", &Instruction::decode_Vo_Wq, OP_UNPCKHPS, FLAG_NONE, 2 },
	{ "movhps", &Instruction::decode_Vq_Mq, OP_MOVHPS, FLAG_NONE, 2 },
	{ "movhps", &Instruction::decode_Mq_Vq, OP_MOVHPS, FLAG_NONE, 2 },
	{ "group17", &Instruction::decode_group17, OP_GROUP17, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },

	/* 0x0f 0x20 - 0x0f 0x2f */
	{ "mov", &Instruction::decode_Rd_Cd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Rd_Dd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Cd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Dd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movaps", &Instruction::decode_Vo_Wo, OP_MOVAPS, FLAG_NONE, 2 },
	{ "movaps", &Instruction::decode_Wo_Vo, OP_MOVAPS, FLAG_NONE, 2 },
	{ "cvtsi2sd", &Instruction::decode_Vo_Eq, OP_CVTSI2SD, FLAG_NONE, 2 },    // x86: cvtsi2sd Vo, Ed
	{ "movntps",  &Instruction::decode_Mo_Vo, OP_MOVNTPS, FLAG_NONE, 2 },
	{ "cvttsd2si",  &Instruction::decode_Gq_Wo, OP_CVTTSD2SI, FLAG_NONE, 2 }, // x86: cvttsd2si Gd, Wo
	{ "cvtsd2si",  &Instruction::decode_Gq_Wo, OP_CVTSD2SI, FLAG_NONE, 2 },   // x86: cvtsd2si Gd, Wo
	{ "ucomiss",  &Instruction::decode_Vo_Wo, OP_UCOMISS, FLAG_NONE, 2 },
	{ "comiss",  &Instruction::decode_Vo_Wo, OP_COMISS, FLAG_NONE, 2 },

	/* 0x0f 0x30 - 0x0f 0x3f */
	{ "wrmsr", &Instruction::decode0, OP_WRMSR, FLAG_NONE, 0 },
	{ "rdtsc", &Instruction::decode0, OP_RDTSC, FLAG_NONE, 0 },
	{ "rdmsr", &Instruction::decode0, OP_RDMSR, FLAG_NONE, 0 },
	{ "rdpmc", &Instruction::decode0, OP_RDPMC, FLAG_NONE, 0 },
	{ "sysenter", &Instruction::decode0, OP_SYSENTER, FLAG_NONE, 0 },
	{ "sysexit", &Instruction::decode0, OP_SYSEXIT, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "getsec", &Instruction::decode0, OP_GETSEC, FLAG_NONE, 0 },
	{ "3byte", &Instruction::decode_3byte_38, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "3byte", &Instruction::decode_3byte_3A, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x40 - 0x0f 0x4f */
	{ "cmovo", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovno", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnbe", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovs", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovns", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },

	/* 0x0f 0x50 - 0x0f 0x5f */
	{ "movmskps", &Instruction::decode_Gq_Uo, OP_MOVMSKPS, FLAG_NONE, 2 }, // x86: movmskps Gd, Uo
	{ "sqrtsd", &Instruction::decode_Vo_Wo, OP_SQRTSD, FLAG_NONE, 2 },
	{ "rsqrtps", &Instruction::decode_Vo_Wo, OP_RSQRTPS, FLAG_NONE, 2 },
	{ "rcpps", &Instruction::decode_Vo_Wo, OP_RCPPS, FLAG_NONE, 2 },
	{ "andps", &Instruction::decode_Vo_Wo, OP_ANDPS, FLAG_NONE, 2 },
	{ "andnps", &Instruction::decode_Vo_Wo, OP_ANDNPS, FLAG_NONE, 2 },
	{ "orps", &Instruction::decode_Vo_Wo, OP_ORPS, FLAG_NONE, 2 },
	{ "xorps", &Instruction::decode_Vo_Wo, OP_XORPS, FLAG_NONE, 2 },
	{ "addsd", &Instruction::decode_Vo_Wo, OP_ADDSD, FLAG_NONE, 2 },
	{ "mulsd", &Instruction::decode_Vo_Wo, OP_MULSD, FLAG_NONE, 2 },
	{ "cvtsd2ss", &Instruction::decode_Vo_Wo, OP_CVTSD2SS, FLAG_NONE, 2 },
	{ "cvtdq2ps", &Instruction::decode_Vo_Wo, OP_CVTDQ2PS, FLAG_NONE, 2 },
	{ "subsd", &Instruction::decode_Vo_Wo, OP_SUBSD, FLAG_NONE, 2 },
	{ "minsd", &Instruction::decode_Vo_Wo, OP_MINSD, FLAG_NONE, 2 },
	{ "divsd", &Instruction::decode_Vo_Wo, OP_DIVSD, FLAG_NONE, 2 },
	{ "maxsd", &Instruction::decode_Vo_Wo, OP_MAXSD, FLAG_NONE, 2 },

	/* 0x0f 0x60 - 0x0f 0x6f */
	{ "punpcklbw", &Instruction::decode_Pq_Qd, OP_PUNPCKLBW, FLAG_NONE, 2 },
	{ "punpcklwd", &Instruction::decode_Pq_Qd, OP_PUNPCKLWD, FLAG_NONE, 2 },
	{ "punpckldq", &Instruction::decode_Pq_Qd, OP_PUNPCKLDQ, FLAG_NONE, 2 },
	{ "packsswb", &Instruction::decode_Pq_Qq, OP_PACKSSWB, FLAG_NONE, 2 },
	{ "pcmpgtb", &Instruction::decode_Pq_Qq, OP_PCMPGTB, FLAG_NONE, 2 },
	{ "pcmpgtw", &Instruction::decode_Pq_Qq, OP_PCMPGTW, FLAG_NONE, 2 },
	{ "pcmpgtd", &Instruction::decode_Pq_Qq, OP_PCMPGTD, FLAG_NONE, 2 },
	{ "packuswb", &Instruction::decode_Pq_Qq, OP_PACKUSWB, FLAG_NONE, 2 },
	{ "punpckhbw", &Instruction::decode_Pq_Qd, OP_PUNPCKHBW, FLAG_NONE, 2 },
	{ "punpckhwd", &Instruction::decode_Pq_Qd, OP_PUNPCKHWD, FLAG_NONE, 2 },
	{ "punpckhdq", &Instruction::decode_Pq_Qd, OP_PUNPCKHDQ, FLAG_NONE, 2 },
	{ "packssdw", &Instruction::decode_Pq_Qq, OP_PACKSSDW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movq", &Instruction::decode_Pd_Eq, OP_MOVQ, FLAG_NONE, 2 }, // x86: movq Pd, Ed
	{ "movq", &Instruction::decode_Pq_Qq, OP_MOVQ, FLAG_NONE, 2 },

	/* 0x0f 0x70 - 0x0f 0x7f */
	{ "pshuflw", &Instruction::decode_Vo_Wo_Ib, OP_PSHUFLW, FLAG_NONE, 3 },
	{ "group13", &Instruction::decode_group13, OP_GROUP13, FLAG_NONE, -1 },
	{ "group14", &Instruction::decode_group14, OP_GROUP14, FLAG_NONE, -1 },
	{ "group15", &Instruction::decode_group15, OP_GROUP15, FLAG_NONE, -1 },
	{ "pcmpeqb", &Instruction::decode_Pq_Qq, OP_PCMPEQB, FLAG_NONE, 2 },
	{ "pcmpeqw", &Instruction::decode_Pq_Qq, OP_PCMPEQW, FLAG_NONE, 2 },
	{ "pcmpeqd", &Instruction::decode_Pq_Qq, OP_PCMPEQD, FLAG_NONE, 2 },
	{ "emms", &Instruction::decode0, OP_EMMS, FLAG_NONE, 0 },
	{ "vmread", &Instruction::decode_Eq_Gq, OP_VMREAD, FLAG_NONE, 2 },     // x86: vmread Ed, Gd
	{ "vmwrite", &Instruction::decode_Gq_Eq, OP_VMWRITE, FLAG_NONE, 2 },   // x86: vmwrite Gd, Ed
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "haddps", &Instruction::decode_Vo_Wo, OP_HADDPS, FLAG_NONE, 2 },
	{ "hsubps", &Instruction::decode_Vo_Wo, OP_HSUBPS, FLAG_NONE, 2 },
	{ "movq", &Instruction::decode_Eq_Pd, OP_MOVD, FLAG_NONE, 2 },         // x86: movq Ed, Pd
	{ "movq", &Instruction::decode_Qq_Pq, OP_MOVQ, FLAG_NONE, 2 },

	/* 0x0f 0x80 - 0x0f 0x8f */
	{ "jo", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jno", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "js", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jns", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },

	/* 0x0f 0x90 - 0x0f 0x9f */
	{ "seto", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setno", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "sets", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setns", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },

	/* 0x0f 0xa0 - 0x0f 0xaf */
	{ "push", &Instruction::decode_Reg<operand_t::REG_FS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_FS>, OP_POP, FLAG_NONE, 1 },
	{ "cpuid", &Instruction::decode0, OP_CPUID, FLAG_NONE, 0 },
	{ "bt", &Instruction::decode_Ev_Gv, OP_BT, FLAG_NONE, 2 },
	{ "shld", &Instruction::decode_Ev_Gv_Ib, OP_SHLD, FLAG_NONE, 3 },
	{ "shld", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHLD, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "push", &Instruction::decode_Reg<operand_t::REG_GS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_GS>, OP_POP, FLAG_NONE, 1 },
	{ "rsm", &Instruction::decode0, OP_RSM, FLAG_NONE, 0 },
	{ "bts", &Instruction::decode_Ev_Gv, OP_BTS, FLAG_NONE, 2 },
	{ "shrd", &Instruction::decode_Ev_Gv_Ib, OP_SHRD, FLAG_NONE, 3 },
	{ "shrd", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHRD, FLAG_NONE, 3 },
	{ "group16", &Instruction::decode_group16, OP_GROUP16, FLAG_NONE, -1 },
	{ "imul",  &Instruction::decode_Gv_Ev, OP_IMUL, FLAG_NONE, 2 },

	/* 0x0f 0xb0 - 0x0f 0xbf */
	{ "cmpxchg", &Instruction::decode_Eb_Gb, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "cmpxchg", &Instruction::decode_Ev_Gv, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "lss", &Instruction::decode_Gv_Mp, OP_LSS, FLAG_NONE, 2 },
	{ "btr", &Instruction::decode_Ev_Gv, OP_BTR, FLAG_NONE, 2 },
	{ "lfs", &Instruction::decode_Gv_Mp, OP_LFS, FLAG_NONE, 2 },
	{ "lgs", &Instruction::decode_Gv_Mp, OP_LGS, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Eb, OP_MOVZX, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Ew, OP_MOVZX, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "group11", &Instruction::decode_group11, OP_GROUP11, FLAG_NONE, -1 },
	{ "group8", &Instruction::decode_group8, OP_GROUP8, FLAG_NONE, -1 },
	{ "btc", &Instruction::decode_Ev_Gv, OP_BTC, FLAG_NONE, 2 },
	{ "bsf", &Instruction::decode_Gv_Ev, OP_BSF, FLAG_NONE, 2 },
	{ "bsr", &Instruction::decode_Gv_Ev, OP_BSR, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Eb, OP_MOVSX, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Ew, OP_MOVSX, FLAG_NONE, 2 },

	/* 0x0f 0xc0 - 0x0f 0xcf */
	{ "xadd", &Instruction::decode_Eb_Gb, OP_XADD, FLAG_NONE, 2 },
	{ "xadd", &Instruction::decode_Ev_Gv, OP_XADD, FLAG_NONE, 2 },
	{ "cmpsd", &Instruction::decode_Vo_Wo_Ib, OP_CMPSD, FLAG_NONE, 3 },
	{ "movnti", &Instruction::decode_Nq_Gq, OP_MOVNTI, FLAG_NONE, 2 },       // x86: movnti Md, Gd
	{ "pinsrw", &Instruction::decode_Pq_Rq_Mw_Ib, OP_PINSRW, FLAG_NONE, 3 }, // x86: pinsrw Pq, Rd/Mw,Ib
	{ "pextrw", &Instruction::decode_Gd_Nq_Ib, OP_PEXTRW, FLAG_NONE, 3 },
	{ "shufps", &Instruction::decode_Vo_Wo_Ib, OP_SHUFPS, FLAG_NONE, 3 },
	{ "group9", &Instruction::decode_group9, OP_GROUP9, FLAG_NONE, -1 },
	{ "bswap", &Instruction::decode_rAX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rCX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSI, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDI, OP_BSWAP, FLAG_NONE, 1 },

	/* 0x0f 0xd0 - 0x0f 0xdf */
	{ "addsubps", &Instruction::decode_Vo_Wo, OP_ADDSUBPS, FLAG_NONE, 2 },
	{ "psrlw", &Instruction::decode_Pq_Qq, OP_PSRLW, FLAG_NONE, 2 },
	{ "psrld", &Instruction::decode_Pq_Qq, OP_PSRLD, FLAG_NONE, 2 },
	{ "psrlq", &Instruction::decode_Pq_Qq, OP_PSRLQ, FLAG_NONE, 2 },
	{ "paddq", &Instruction::decode_Pq_Qq, OP_PADDQ, FLAG_NONE, 2 },
	{ "pmullw", &Instruction::decode_Pq_Qq, OP_PMULLW, FLAG_NONE, 2 },
	{ "movdq2q", &Instruction::decode_Pq_Qq, OP_MOVDQ2Q, FLAG_NONE, 2 },
	{ "pmovmskb", &Instruction::decode_Gd_Nq, OP_PMOVMSKB, FLAG_NONE, 2 },
    { "psubusb", &Instruction::decode_Pq_Qq, OP_PSUBUSB, FLAG_NONE, 2 },
    { "psubusw", &Instruction::decode_Pq_Qq, OP_PSUBUSW, FLAG_NONE, 2 },
    { "pminub", &Instruction::decode_Pq_Qq, OP_PMINUB, FLAG_NONE, 2 },
    { "pand", &Instruction::decode_Pq_Qq, OP_PAND, FLAG_NONE, 2 },
    { "paddusb", &Instruction::decode_Pq_Qq, OP_PADDUSB, FLAG_NONE, 2 },
    { "paddusw", &Instruction::decode_Pq_Qq, OP_PADDUSW, FLAG_NONE, 2 },
    { "pmaxub", &Instruction::decode_Pq_Qq, OP_PMAXUB, FLAG_NONE, 2 },
    { "pandn", &Instruction::decode_Pq_Qq, OP_PANDN, FLAG_NONE, 2 },

	/* 0x0f 0xe0 - 0x0f 0xef */
	{ "pavgb", &Instruction::decode_Pq_Qq, OP_PAVGB, FLAG_NONE, 2 },
	{ "psraw", &Instruction::decode_Pq_Qq, OP_PSRAW, FLAG_NONE, 2 },
	{ "psrad", &Instruction::decode_Pq_Qq, OP_PSRAD, FLAG_NONE, 2 },
	{ "pavgw", &Instruction::decode_Pq_Qq, OP_PAVGW, FLAG_NONE, 2 },
	{ "pmulhuw", &Instruction::decode_Pq_Qq, OP_PMULHUW, FLAG_NONE, 2 },
	{ "pmulhw", &Instruction::decode_Pq_Qq, OP_PMULHW, FLAG_NONE, 2 },
	{ "cvtpd2dq", &Instruction::decode_Vo_Wo, OP_CVTPD2DQ, FLAG_NONE, 2 },
	{ "movntq", &Instruction::decode_Mq_Pq, OP_MOVNTQ, FLAG_NONE, 2 },
	{ "psubsb", &Instruction::decode_Pq_Qq, OP_PSUBSB, FLAG_NONE, 2 },
	{ "psubsw", &Instruction::decode_Pq_Qq, OP_PSUBSW, FLAG_NONE, 2 },
	{ "pminsw", &Instruction::decode_Pq_Qq, OP_PMINSW, FLAG_NONE, 2 },
	{ "por", &Instruction::decode_Pq_Qq, OP_POR, FLAG_NONE, 2 },
	{ "paddsb", &Instruction::decode_Pq_Qq, OP_PADDSB, FLAG_NONE, 2 },
	{ "paddsw", &Instruction::decode_Pq_Qq, OP_PADDSW, FLAG_NONE, 2 },
	{ "pmaxsw", &Instruction::decode_Pq_Qq, OP_PMAXSW, FLAG_NONE, 2 },
	{ "pxor", &Instruction::decode_Pq_Qq, OP_PXOR, FLAG_NONE, 2 },

	/* 0x0f 0xf0 - 0x0f 0xff */
	{ "lddqu", &Instruction::decode_Vo_Mo, OP_LDDQU, FLAG_NONE, 2 },
	{ "psllw", &Instruction::decode_Pq_Qq, OP_PSLLW, FLAG_NONE, 2 },
	{ "pslld", &Instruction::decode_Pq_Qq, OP_PSLLD, FLAG_NONE, 2 },
	{ "psllq", &Instruction::decode_Pq_Qq, OP_PSLLQ, FLAG_NONE, 2 },
	{ "pmuludq", &Instruction::decode_Pq_Qq, OP_PMULUDQ, FLAG_NONE, 2 },
	{ "pmaddwd", &Instruction::decode_Pq_Qq, OP_PMADDWD, FLAG_NONE, 2 },
	{ "psadbw", &Instruction::decode_Pq_Qq, OP_PSADBW, FLAG_NONE, 2 },
	{ "maskmovq", &Instruction::decode_Pq_Nq, OP_MASKMOVQ, FLAG_NONE, 2 },
	{ "psubb", &Instruction::decode_Pq_Qq, OP_PSUBB, FLAG_NONE, 2 },
	{ "psubw", &Instruction::decode_Pq_Qq, OP_PSUBW, FLAG_NONE, 2 },
	{ "psubd", &Instruction::decode_Pq_Qq, OP_PSUBD, FLAG_NONE, 2 },
	{ "psubq", &Instruction::decode_Pq_Qq, OP_PSUBQ, FLAG_NONE, 2 },
	{ "paddb", &Instruction::decode_Pq_Qq, OP_PADDB, FLAG_NONE, 2 },
	{ "paddw", &Instruction::decode_Pq_Qq, OP_PADDW, FLAG_NONE, 2 },
	{ "paddd", &Instruction::decode_Pq_Qq, OP_PADDD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
};

// 2 byte opcodes with a f3 prefix
template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_2Byte_F3[0x100] = {

	/* 0x0f 0x00 - 0x0f 0x0f */
	{ "group6", &Instruction::decode_group6, OP_GROUP6, FLAG_NONE, -1 },
	{ "group7", &Instruction::decode_group7, OP_GROUP7, FLAG_NONE, -1 },
	{ "lar", &Instruction::decode_Gv_Ew, OP_LAR, FLAG_NONE, 2 },
	{ "lsl", &Instruction::decode_Gv_Ew, OP_LSL, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "syscall", &Instruction::decode0, OP_SYSCALL, FLAG_NONE, 0 },
	{ "clts", &Instruction::decode0, OP_CLTS, FLAG_NONE, 0 },
	{ "sysret", &Instruction::decode0, OP_SYSRET, FLAG_NONE, 0 },
	{ "invd", &Instruction::decode0, OP_INVD, FLAG_NONE, 0 },
	{ "wbinvd", &Instruction::decode0, OP_WBINVD, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "ud2", &Instruction::decode0, OP_UD2, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x10 - 0x0f 0x1f */
	{ "movss", &Instruction::decode_Vo_Wo, OP_MOVSS, FLAG_NONE, 2 },
	{ "movss", &Instruction::decode_Wo_Vo, OP_MOVSS, FLAG_NONE, 2 },
	{ "movsldup", &Instruction::decode_Vq_Wq, OP_MOVSLDUP, FLAG_NONE, 2 },
	{ "movlps", &Instruction::decode_Mq_Vq, OP_MOVLPS, FLAG_NONE, 2 },
	{ "unpcklps", &Instruction::decode_Vo_Wq, OP_UNPCKLPS, FLAG_NONE, 2 },
	{ "unpckhps", &Instruction::decode_Vo_Wq, OP_UNPCKHPS, FLAG_NONE, 2 },
	{ "movshdup", &Instruction::decode_Vq_Wq, OP_MOVSHDUP, FLAG_NONE, 2 },
	{ "movhps", &Instruction::decode_Mq_Vq, OP_MOVHPS, FLAG_NONE, 2 },
	{ "group17", &Instruction::decode_group17, OP_GROUP17, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "nop", &Instruction::decode_Ev, OP_NOP, FLAG_NONE, 1 },

	/* 0x0f 0x20 - 0x0f 0x2f */
	{ "mov", &Instruction::decode_Rd_Cd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Rd_Dd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Cd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "mov", &Instruction::decode_Dd_Rd, OP_MOV, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movaps", &Instruction::decode_Vo_Wo, OP_MOVAPS, FLAG_NONE, 2 },
	{ "movaps", &Instruction::decode_Wo_Vo, OP_MOVAPS, FLAG_NONE, 2 },
	{ "cvtsi2ss", &Instruction::decode_Vo_Eq, OP_CVTSI2SS, FLAG_NONE, 2 },    // x86: cvtsi2ss Vo, Ed
	{ "movntps",  &Instruction::decode_Mo_Vo, OP_MOVNTPS, FLAG_NONE, 2 },
	{ "cvttss2si",  &Instruction::decode_Gq_Wo, OP_CVTTSS2SI, FLAG_NONE, 2 }, // x86: cvttss2si Gd, Wo
	{ "cvtss2si",  &Instruction::decode_Gq_Wo, OP_CVTSS2SI, FLAG_NONE, 2 },   // x86: cvtss2si Gd, Wo
	{ "ucomiss",  &Instruction::decode_Vo_Wo, OP_UCOMISS, FLAG_NONE, 2 },
	{ "comiss",  &Instruction::decode_Vo_Wo, OP_COMISS, FLAG_NONE, 2 },

	/* 0x0f 0x30 - 0x0f 0x3f */
	{ "wrmsr", &Instruction::decode0, OP_WRMSR, FLAG_NONE, 0 },
	{ "rdtsc", &Instruction::decode0, OP_RDTSC, FLAG_NONE, 0 },
	{ "rdmsr", &Instruction::decode0, OP_RDMSR, FLAG_NONE, 0 },
	{ "rdpmc", &Instruction::decode0, OP_RDPMC, FLAG_NONE, 0 },
	{ "sysenter", &Instruction::decode0, OP_SYSENTER, FLAG_NONE, 0 },
	{ "sysexit", &Instruction::decode0, OP_SYSEXIT, FLAG_NONE, 0 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "getsec", &Instruction::decode0, OP_GETSEC, FLAG_NONE, 0 },
	{ "3byte", &Instruction::decode_3byte_38, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "3byte", &Instruction::decode_3byte_3A, OP_3BYTE, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x40 - 0x0f 0x4f */
	{ "cmovo", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovno", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnz", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovb", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnbe", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovs", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovns", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnp", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnl", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },
	{ "cmovnle", &Instruction::decode_Gv_Ev, OP_CMOVCC, FLAG_NONE, 2 },

	/* 0x0f 0x50 - 0x0f 0x5f */
	{ "movmskps", &Instruction::decode_Gq_Uo, OP_MOVMSKPS, FLAG_NONE, 2 }, // x86: movmskps Gd, Uo
	{ "sqrtss", &Instruction::decode_Vo_Wo, OP_SQRTSS, FLAG_NONE, 2 },
	{ "rsqrtss", &Instruction::decode_Vo_Wo, OP_RSQRTSS, FLAG_NONE, 2 },
	{ "rcpss", &Instruction::decode_Vo_Wo, OP_RCPSS, FLAG_NONE, 2 },
	{ "andps", &Instruction::decode_Vo_Wo, OP_ANDPS, FLAG_NONE, 2 },
	{ "andnps", &Instruction::decode_Vo_Wo, OP_ANDNPS, FLAG_NONE, 2 },
	{ "orps", &Instruction::decode_Vo_Wo, OP_ORPS, FLAG_NONE, 2 },
	{ "xorps", &Instruction::decode_Vo_Wo, OP_XORPS, FLAG_NONE, 2 },
	{ "addss", &Instruction::decode_Vo_Wo, OP_ADDSS, FLAG_NONE, 2 },
	{ "mulss", &Instruction::decode_Vo_Wo, OP_MULSS, FLAG_NONE, 2 },
	{ "cvtss2sd", &Instruction::decode_Vo_Wo, OP_CVTSS2SD, FLAG_NONE, 2 },
	{ "cvttps2dq", &Instruction::decode_Vo_Wo, OP_CVTTPS2DQ, FLAG_NONE, 2 },
	{ "subss", &Instruction::decode_Vo_Wo, OP_SUBSS, FLAG_NONE, 2 },
	{ "minss", &Instruction::decode_Vo_Wo, OP_MINSS, FLAG_NONE, 2 },
	{ "divss", &Instruction::decode_Vo_Wo, OP_DIVSS, FLAG_NONE, 2 },
	{ "maxss", &Instruction::decode_Vo_Wo, OP_MAXSS, FLAG_NONE, 2 },

	/* 0x0f 0x60 - 0x0f 0x6f */
	{ "punpcklbw", &Instruction::decode_Pq_Qd, OP_PUNPCKLBW, FLAG_NONE, 2 },
	{ "punpcklwd", &Instruction::decode_Pq_Qd, OP_PUNPCKLWD, FLAG_NONE, 2 },
	{ "punpckldq", &Instruction::decode_Pq_Qd, OP_PUNPCKLDQ, FLAG_NONE, 2 },
	{ "packsswb", &Instruction::decode_Pq_Qq, OP_PACKSSWB, FLAG_NONE, 2 },
	{ "pcmpgtb", &Instruction::decode_Pq_Qq, OP_PCMPGTB, FLAG_NONE, 2 },
	{ "pcmpgtw", &Instruction::decode_Pq_Qq, OP_PCMPGTW, FLAG_NONE, 2 },
	{ "pcmpgtd", &Instruction::decode_Pq_Qq, OP_PCMPGTD, FLAG_NONE, 2 },
	{ "packuswb", &Instruction::decode_Pq_Qq, OP_PACKUSWB, FLAG_NONE, 2 },
	{ "punpckhbw", &Instruction::decode_Pq_Qd, OP_PUNPCKHBW, FLAG_NONE, 2 },
	{ "punpckhwd", &Instruction::decode_Pq_Qd, OP_PUNPCKHWD, FLAG_NONE, 2 },
	{ "punpckhdq", &Instruction::decode_Pq_Qd, OP_PUNPCKHDQ, FLAG_NONE, 2 },
	{ "packssdw", &Instruction::decode_Pq_Qq, OP_PACKSSDW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movq", &Instruction::decode_Pd_Eq, OP_MOVQ, FLAG_NONE, 2 }, // x86: movq Pd, Ed
	{ "movdqu", &Instruction::decode_Vo_Wo, OP_MOVDQU, FLAG_NONE, 2 },

	/* 0x0f 0x70 - 0x0f 0x7f */
	{ "pshufhw", &Instruction::decode_Vo_Wo_Ib, OP_PSHUFHW, FLAG_NONE, 3 },
	{ "group13", &Instruction::decode_group13, OP_GROUP13, FLAG_NONE, -1 },
	{ "group14", &Instruction::decode_group14, OP_GROUP14, FLAG_NONE, -1 },
	{ "group15", &Instruction::decode_group15, OP_GROUP15, FLAG_NONE, -1 },
	{ "pcmpeqb", &Instruction::decode_Pq_Qq, OP_PCMPEQB, FLAG_NONE, 2 },
	{ "pcmpeqw", &Instruction::decode_Pq_Qq, OP_PCMPEQW, FLAG_NONE, 2 },
	{ "pcmpeqd", &Instruction::decode_Pq_Qq, OP_PCMPEQD, FLAG_NONE, 2 },
	{ "emms", &Instruction::decode0, OP_EMMS, FLAG_NONE, 0 },
	{ "vmread", &Instruction::decode_Eq_Gq, OP_VMREAD, FLAG_NONE, 2 },   // x86: vmread Ed, Gd
	{ "vmwrite", &Instruction::decode_Gq_Eq, OP_VMWRITE, FLAG_NONE, 2 }, // x86: vmwrite Gd, Ed
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "movq", &Instruction::decode_Vq_Wq, OP_MOVQ, FLAG_NONE, 2 },
	{ "movdqu", &Instruction::decode_Wo_Vo, OP_MOVDQU, FLAG_NONE, 2 },

	/* 0x0f 0x80 - 0x0f 0x8f */
	{ "jo", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jno", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnb", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnz", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnbe", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "js", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jns", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnp", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnl", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },
	{ "jnle", &Instruction::decode_Jz, OP_JCC, FLAG_NONE, 1 },

	/* 0x0f 0x90 - 0x0f 0x9f */
	{ "seto", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setno", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnb", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnz", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnbe", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "sets", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setns", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnp", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnl", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },
	{ "setnle", &Instruction::decode_Eb, OP_SETCC, FLAG_NONE, 1 },

	/* 0x0f 0xa0 - 0x0f 0xaf */
	{ "push", &Instruction::decode_Reg<operand_t::REG_FS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_FS>, OP_POP, FLAG_NONE, 1 },
	{ "cpuid", &Instruction::decode0, OP_CPUID, FLAG_NONE, 0 },
	{ "bt", &Instruction::decode_Ev_Gv, OP_BT, FLAG_NONE, 2 },
	{ "shld", &Instruction::decode_Ev_Gv_Ib, OP_SHLD, FLAG_NONE, 3 },
	{ "shld", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHLD, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "push", &Instruction::decode_Reg<operand_t::REG_GS>, OP_PUSH, FLAG_NONE, 1 },
	{ "pop", &Instruction::decode_Reg<operand_t::REG_GS>, OP_POP, FLAG_NONE, 1 },
	{ "rsm", &Instruction::decode0, OP_RSM, FLAG_NONE, 0 },
	{ "bts", &Instruction::decode_Ev_Gv, OP_BTS, FLAG_NONE, 2 },
	{ "shrd", &Instruction::decode_Ev_Gv_Ib, OP_SHRD, FLAG_NONE, 3 },
	{ "shrd", &Instruction::decode_Ev_Gv_Reg<operand_t::REG_CL>, OP_SHRD, FLAG_NONE, 3 },
	{ "group16", &Instruction::decode_group16, OP_GROUP16, FLAG_NONE, -1 },
	{ "imul",  &Instruction::decode_Gv_Ev, OP_IMUL, FLAG_NONE, 2 },

	/* 0x0f 0xb0 - 0x0f 0xbf */
	{ "cmpxchg", &Instruction::decode_Eb_Gb, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "cmpxchg", &Instruction::decode_Ev_Gv, OP_CMPXCHG, FLAG_NONE, 2 },
	{ "lss", &Instruction::decode_Gv_Mp, OP_LSS, FLAG_NONE, 2 },
	{ "btr", &Instruction::decode_Ev_Gv, OP_BTR, FLAG_NONE, 2 },
	{ "lfs", &Instruction::decode_Gv_Mp, OP_LFS, FLAG_NONE, 2 },
	{ "lgs", &Instruction::decode_Gv_Mp, OP_LGS, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Eb, OP_MOVZX, FLAG_NONE, 2 },
	{ "movzx", &Instruction::decode_Gv_Ew, OP_MOVZX, FLAG_NONE, 2 },
	{ "popcnt", &Instruction::decode_Gv_Ev, OP_POPCNT, FLAG_NONE, 2 },
	{ "group11", &Instruction::decode_group11, OP_GROUP11, FLAG_NONE, -1 },
	{ "group8", &Instruction::decode_group8, OP_GROUP8, FLAG_NONE, -1 },
	{ "btc", &Instruction::decode_Ev_Gv, OP_BTC, FLAG_NONE, 2 },
	{ "bsf", &Instruction::decode_Gv_Ev, OP_BSF, FLAG_NONE, 2 },
	{ "bsr", &Instruction::decode_Gv_Ev, OP_BSR, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Eb, OP_MOVSX, FLAG_NONE, 2 },
	{ "movsx", &Instruction::decode_Gv_Ew, OP_MOVSX, FLAG_NONE, 2 },

	/* 0x0f 0xc0 - 0x0f 0xcf */
	{ "xadd", &Instruction::decode_Eb_Gb, OP_XADD, FLAG_NONE, 2 },
	{ "xadd", &Instruction::decode_Ev_Gv, OP_XADD, FLAG_NONE, 2 },
	{ "cmpss", &Instruction::decode_Vo_Wo_Ib, OP_CMPSS, FLAG_NONE, 3 },
	{ "movnti", &Instruction::decode_Nq_Gq, OP_MOVNTI, FLAG_NONE, 2 },       // x86: movnti Md, Gd
	{ "pinsrw", &Instruction::decode_Pq_Rq_Mw_Ib, OP_PINSRW, FLAG_NONE, 3 }, // x86: pinsrw Pq, Rd/Mw,Ib
	{ "pextrw", &Instruction::decode_Gd_Nq_Ib, OP_PEXTRW, FLAG_NONE, 3 },
	{ "shufps", &Instruction::decode_Vo_Wo_Ib, OP_SHUFPS, FLAG_NONE, 3 },
	{ "group9", &Instruction::decode_group9, OP_GROUP9, FLAG_NONE, -1 },
	{ "bswap", &Instruction::decode_rAX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rCX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBX, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rBP, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rSI, OP_BSWAP, FLAG_NONE, 1 },
	{ "bswap", &Instruction::decode_rDI, OP_BSWAP, FLAG_NONE, 1 },

	/* 0x0f 0xd0 - 0x0f 0xdf */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "psrlw", &Instruction::decode_Pq_Qq, OP_PSRLW, FLAG_NONE, 2 },
	{ "psrld", &Instruction::decode_Pq_Qq, OP_PSRLD, FLAG_NONE, 2 },
	{ "psrlq", &Instruction::decode_Pq_Qq, OP_PSRLQ, FLAG_NONE, 2 },
	{ "paddq", &Instruction::decode_Pq_Qq, OP_PADDQ, FLAG_NONE, 2 },
	{ "pmullw", &Instruction::decode_Pq_Qq, OP_PMULLW, FLAG_NONE, 2 },
	{ "movq2dq", &Instruction::decode_Vo_Nq, OP_MOVQ2DQ, FLAG_NONE, 2 },
	{ "pmovmskb", &Instruction::decode_Gd_Nq, OP_PMOVMSKB, FLAG_NONE, 2 },
    { "psubusb", &Instruction::decode_Pq_Qq, OP_PSUBUSB, FLAG_NONE, 2 },
    { "psubusw", &Instruction::decode_Pq_Qq, OP_PSUBUSW, FLAG_NONE, 2 },
    { "pminub", &Instruction::decode_Pq_Qq, OP_PMINUB, FLAG_NONE, 2 },
    { "pand", &Instruction::decode_Pq_Qq, OP_PAND, FLAG_NONE, 2 },
    { "paddusb", &Instruction::decode_Pq_Qq, OP_PADDUSB, FLAG_NONE, 2 },
    { "paddusw", &Instruction::decode_Pq_Qq, OP_PADDUSW, FLAG_NONE, 2 },
    { "pmaxub", &Instruction::decode_Pq_Qq, OP_PMAXUB, FLAG_NONE, 2 },
    { "pandn", &Instruction::decode_Pq_Qq, OP_PANDN, FLAG_NONE, 2 },

	/* 0x0f 0xe0 - 0x0f 0xef */
	{ "pavgb", &Instruction::decode_Pq_Qq, OP_PAVGB, FLAG_NONE, 2 },
	{ "psraw", &Instruction::decode_Pq_Qq, OP_PSRAW, FLAG_NONE, 2 },
	{ "psrad", &Instruction::decode_Pq_Qq, OP_PSRAD, FLAG_NONE, 2 },
	{ "pavgw", &Instruction::decode_Pq_Qq, OP_PAVGW, FLAG_NONE, 2 },
	{ "pmulhuw", &Instruction::decode_Pq_Qq, OP_PMULHUW, FLAG_NONE, 2 },
	{ "pmulhw", &Instruction::decode_Pq_Qq, OP_PMULHW, FLAG_NONE, 2 },
	{ "cvtdq2pd", &Instruction::decode_Vo_Wo, OP_CVTDQ2PD, FLAG_NONE, 2 },
	{ "movntq", &Instruction::decode_Mq_Pq, OP_MOVNTQ, FLAG_NONE, 2 },
	{ "psubsb", &Instruction::decode_Pq_Qq, OP_PSUBSB, FLAG_NONE, 2 },
	{ "psubsw", &Instruction::decode_Pq_Qq, OP_PSUBSW, FLAG_NONE, 2 },
	{ "pminsw", &Instruction::decode_Pq_Qq, OP_PMINSW, FLAG_NONE, 2 },
	{ "por", &Instruction::decode_Pq_Qq, OP_POR, FLAG_NONE, 2 },
	{ "paddsb", &Instruction::decode_Pq_Qq, OP_PADDSB, FLAG_NONE, 2 },
	{ "paddsw", &Instruction::decode_Pq_Qq, OP_PADDSW, FLAG_NONE, 2 },
	{ "pmaxsw", &Instruction::decode_Pq_Qq, OP_PMAXSW, FLAG_NONE, 2 },
	{ "pxor", &Instruction::decode_Pq_Qq, OP_PXOR, FLAG_NONE, 2 },

	/* 0x0f 0xf0 - 0x0f 0xff */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "psllw", &Instruction::decode_Pq_Qq, OP_PSLLW, FLAG_NONE, 2 },
	{ "pslld", &Instruction::decode_Pq_Qq, OP_PSLLD, FLAG_NONE, 2 },
	{ "psllq", &Instruction::decode_Pq_Qq, OP_PSLLQ, FLAG_NONE, 2 },
	{ "pmuludq", &Instruction::decode_Pq_Qq, OP_PMULUDQ, FLAG_NONE, 2 },
	{ "pmaddwd", &Instruction::decode_Pq_Qq, OP_PMADDWD, FLAG_NONE, 2 },
	{ "psadbw", &Instruction::decode_Pq_Qq, OP_PSADBW, FLAG_NONE, 2 },
	{ "maskmovq", &Instruction::decode_Pq_Nq, OP_MASKMOVQ, FLAG_NONE, 2 },
	{ "psubb", &Instruction::decode_Pq_Qq, OP_PSUBB, FLAG_NONE, 2 },
	{ "psubw", &Instruction::decode_Pq_Qq, OP_PSUBW, FLAG_NONE, 2 },
	{ "psubd", &Instruction::decode_Pq_Qq, OP_PSUBD, FLAG_NONE, 2 },
	{ "psubq", &Instruction::decode_Pq_Qq, OP_PSUBQ, FLAG_NONE, 2 },
	{ "paddb", &Instruction::decode_Pq_Qq, OP_PADDB, FLAG_NONE, 2 },
	{ "paddw", &Instruction::decode_Pq_Qq, OP_PADDW, FLAG_NONE, 2 },
	{ "paddd", &Instruction::decode_Pq_Qq, OP_PADDD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
};


template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_3Byte_38_66[0x100] = {

	/* 0x0f 0x38 0x00 - 0x0f 0x38 0x0f */
	{ "pshufb", &Instruction::decode_Vo_Wo, OP_PSHUFB, FLAG_NONE, 2 },
	{ "phaddw", &Instruction::decode_Vo_Wo, OP_PHADDW, FLAG_NONE, 2 },
	{ "phaddd", &Instruction::decode_Vo_Wo, OP_PHADDD, FLAG_NONE, 2 },
	{ "phaddsw", &Instruction::decode_Vo_Wo, OP_PHADDSW, FLAG_NONE, 2 },
	{ "pmaddubsw", &Instruction::decode_Vo_Wo, OP_PMADDUBSW, FLAG_NONE, 2 },
	{ "phsubw", &Instruction::decode_Vo_Wo, OP_PHSUBW, FLAG_NONE, 2 },
	{ "phsubd", &Instruction::decode_Vo_Wo, OP_PHSUBD, FLAG_NONE, 2 },
	{ "phsubsw", &Instruction::decode_Vo_Wo, OP_PHSUBSW, FLAG_NONE, 2 },
	{ "psignb", &Instruction::decode_Vo_Wo, OP_PSIGNB, FLAG_NONE, 2 },
	{ "psignw", &Instruction::decode_Vo_Wo, OP_PSIGNW, FLAG_NONE, 2 },
	{ "psignd", &Instruction::decode_Vo_Wo, OP_PSIGND, FLAG_NONE, 2 },
	{ "pmulhrsw", &Instruction::decode_Vo_Wo, OP_PMULHRSW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x10 - 0x0f 0x38 0x1f */
	{ "pblendvb", &Instruction::decode_Vo_Wo, OP_PBLENDVB, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "blendvps", &Instruction::decode_Vo_Wo, OP_BLENDVPS, FLAG_NONE, 2 },
	{ "blendvpd", &Instruction::decode_Vo_Wo, OP_BLENDVPD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "ptest", &Instruction::decode_Vo_Wo, OP_PTEST, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "pabsb", &Instruction::decode_Vo_Wo, OP_PABSB, FLAG_NONE, 2 },
	{ "pabsw", &Instruction::decode_Vo_Wo, OP_PABSW, FLAG_NONE, 2 },
	{ "pabsd", &Instruction::decode_Vo_Wo, OP_PABSD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x20 - 0x0f 0x38 0x2f */
	{ "pmovsxbw", &Instruction::decode_Vo_Uo_Mq, OP_PMOVSXBW, FLAG_NONE, 3 },
	{ "pmovsxbd", &Instruction::decode_Vo_Uo_Md, OP_PMOVSXBD, FLAG_NONE, 3 },
	{ "pmovsxbq", &Instruction::decode_Vo_Uo_Mw, OP_PMOVSXBQ, FLAG_NONE, 3 },
	{ "pmovsxwd", &Instruction::decode_Vo_Uo_Mq, OP_PMOVSXWD, FLAG_NONE, 3 },
	{ "pmovsxwq", &Instruction::decode_Vo_Uo_Md, OP_PMOVSXWQ, FLAG_NONE, 3 },
	{ "pmovsxdq", &Instruction::decode_Vo_Uo_Mq, OP_PMOVSXDQ, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "pmuldq", &Instruction::decode_Vo_Wo, OP_PMULDQ, FLAG_NONE, 2 },
	{ "pcmpeqq", &Instruction::decode_Vo_Wo, OP_PCMPEQQ, FLAG_NONE, 2 },
	{ "movntdqa", &Instruction::decode_Vo_Mo, OP_MOVNTDQA, FLAG_NONE, 2 },
	{ "packusdw", &Instruction::decode_Vo_Wo, OP_PACKUSDW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x30 - 0x0f 0x38 0x3f */
	{ "pmovzxbw", &Instruction::decode_Vo_Uo_Mq, OP_PMOVZXBW, FLAG_NONE, 3 },
	{ "pmovzxbd", &Instruction::decode_Vo_Uo_Md, OP_PMOVZXBD, FLAG_NONE, 3 },
	{ "pmovzxbq", &Instruction::decode_Vo_Uo_Mw, OP_PMOVZXBQ, FLAG_NONE, 3 },
	{ "pmovzxwd", &Instruction::decode_Vo_Uo_Mq, OP_PMOVZXWD, FLAG_NONE, 3 },
	{ "pmovzxwq", &Instruction::decode_Vo_Uo_Md, OP_PMOVZXWQ, FLAG_NONE, 3 },
	{ "pmovzxdq", &Instruction::decode_Vo_Uo_Mq, OP_PMOVZXDQ, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "pcmpgtq", &Instruction::decode_Vo_Wo, OP_PCMPGTQ, FLAG_NONE, 2 },
	{ "pminsb", &Instruction::decode_Vo_Wo, OP_PMINSB, FLAG_NONE, 2 },
	{ "pminsd", &Instruction::decode_Vo_Wo, OP_PMINSD, FLAG_NONE, 2 },
	{ "pminuw", &Instruction::decode_Vo_Wo, OP_PMINUW, FLAG_NONE, 2 },
	{ "pminud", &Instruction::decode_Vo_Wo, OP_PMINUD, FLAG_NONE, 2 },
	{ "pmaxsb", &Instruction::decode_Vo_Wo, OP_PMAXSB, FLAG_NONE, 2 },
	{ "pmaxsd", &Instruction::decode_Vo_Wo, OP_PMAXSD, FLAG_NONE, 2 },
	{ "pmaxuw", &Instruction::decode_Vo_Wo, OP_PMAXUW, FLAG_NONE, 2 },
	{ "pmaxud", &Instruction::decode_Vo_Wo, OP_PMAXUD, FLAG_NONE, 2 },

	/* 0x0f 0x38 0x40 - 0x0f 0x38 0x4f */
	{ "pmulld", &Instruction::decode_Vo_Wo, OP_PMULLD, FLAG_NONE, 2 },
	{ "phminposuw", &Instruction::decode_Vo_Wo, OP_PHMINPOSUW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x50 - 0x0f 0x38 0x5f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x60 - 0x0f 0x38 0x6f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x70 - 0x0f 0x38 0x7f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x80 - 0x0f 0x38 0x8f */
	{ "nvept", &Instruction::decode_Gq_Mo, OP_NVEPT, FLAG_NONE, 2 },   // x86: nvept Gd, Mo
	{ "nvvpid", &Instruction::decode_Gq_Mo, OP_NVVPID, FLAG_NONE, 2 }, // x86: nvvpid Gd, Mo
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x90 - 0x0f 0x38 0x9f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0xa0 - 0x0f 0x38 0xaf */
	INVALID_BLOCK,

	/* 0x0f 0x38 0xb0 - 0x0f 0x38 0xbf */
	INVALID_BLOCK,

	/* 0x0f 0x38 0xc0 - 0x0f 0x38 0xcf */
	INVALID_BLOCK,

	/* 0x0f 0x38 0xd0 - 0x0f 0x38 0xdf */
	INVALID_BLOCK,

	/* 0x0f 0x38 0xe0 - 0x0f 0x38 0xef */
	INVALID_BLOCK,

	/* 0x0f 0x38 0xf0 - 0x0f 0x38 0xff */
	{ "movbe", &Instruction::decode_Gv_Mv, OP_MOVBE, FLAG_NONE, 2 },
	{ "movbe", &Instruction::decode_Mv_Gv, OP_MOVBE, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
};

template <>
const Instruction<edisassm::x86_64>::opcode_entry Instruction<edisassm::x86_64>::Opcodes_3Byte_3A_66[0x100] = {
	/* 0x0f 0x3a 0x00 - 0x0f 0x3a 0x0f */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "roundps", &Instruction::decode_Vo_Wo_Ib, OP_ROUNDPS, FLAG_NONE, 3 },
	{ "roundpd", &Instruction::decode_Vo_Wo_Ib, OP_ROUNDPD, FLAG_NONE, 3 },
	{ "roundss", &Instruction::decode_Vo_Wo_Ib, OP_ROUNDSS, FLAG_NONE, 3 },
	{ "roundsd", &Instruction::decode_Vo_Wo_Ib, OP_ROUNDSD, FLAG_NONE, 3 },
	{ "blendps", &Instruction::decode_Vo_Wo_Ib, OP_BLENDPS, FLAG_NONE, 3 },
	{ "blendpd", &Instruction::decode_Vo_Wo_Ib, OP_BLENDPD, FLAG_NONE, 3 },
	{ "pblendw", &Instruction::decode_Vo_Wo_Ib, OP_PBLENDW, FLAG_NONE, 3 },
	{ "palignr", &Instruction::decode_Vo_Wo_Ib, OP_PALIGNR, FLAG_NONE, 3 },

	/* 0x0f 0x3a 0x10 - 0x0f 0x3a 0x1f */
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "pextrb", &Instruction::decode_Rd_Mb_Vo_Ib, OP_PEXTRB, FLAG_NONE, 3 },
	{ "pextrw", &Instruction::decode_Rd_Mw_Vo_Ib, OP_PEXTRW, FLAG_NONE, 3 },
	{ "pextrq", &Instruction::decode_Eq_Vo_Ib, OP_PEXTRQ, FLAG_NONE, 3 },       // x86: pextrq Eq, Vo, Ib
	{ "extractps", &Instruction::decode_Ed_Vo_Ib, OP_EXTRACTPS, FLAG_NONE, 3 }, // x86: extractps Ed, Vo, Ib // TODO: figure out if this should allow RAX or not?
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x3a 0x20 - 0x0f 0x3a 0x2f */
	{ "pinsrb", &Instruction::decode_Vo_Mb_Ib, OP_PINSRB, FLAG_NONE, 3 },
	{ "insertps", &Instruction::decode_Vo_Md_Ib, OP_INSERTPS, FLAG_NONE, 3 },
	{ "pinsrq", &Instruction::decode_Vo_Eq_Ib, OP_PINSRQ, FLAG_NONE, 3 }, // x86: pinsrq Vo, Eq, Ib
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x3a 0x30 - 0x0f 0x3a 0x3f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x40 - 0x0f 0x3a 0x4f */
	{ "dpps", &Instruction::decode_Vo_Wo_Ib, OP_DPPS, FLAG_NONE, 3 },
	{ "dppd", &Instruction::decode_Vo_Wo_Ib, OP_DPPD, FLAG_NONE, 3 },
	{ "mpsadbw", &Instruction::decode_Vo_Wo_Ib, OP_MPSADBW, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x3a 0x50 - 0x0f 0x3a 0x5f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x60 - 0x0f 0x3a 0x6f */
	{ "pcmpestrm", &Instruction::decode_Vo_Wo_Ib, OP_PCMPESTRM, FLAG_NONE, 3 },
	{ "pcmpestri", &Instruction::decode_Vo_Wo_Ib, OP_PCMPESTRI, FLAG_NONE, 3 },
	{ "pcmpistrm", &Instruction::decode_Vo_Wo_Ib, OP_PCMPISTRM, FLAG_NONE, 3 },
	{ "pcmpistri", &Instruction::decode_Vo_Wo_Ib, OP_PCMPISTRI, FLAG_NONE, 3 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x3a 0x70 - 0x0f 0x3a 0x7f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x80 - 0x0f 0x3a 0x8f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x90 - 0x0f 0x3a 0x9f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0xa0 - 0x0f 0x3a 0xaf */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0xb0 - 0x0f 0x3a 0xbf */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0xc0 - 0x0f 0x3a 0xcf */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0xd0 - 0x0f 0x3a 0xdf */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0xe0 - 0x0f 0x3a 0xef */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0xf0 - 0x0f 0x3a 0xff */
	INVALID_BLOCK
};

#endif
