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

#ifndef OPTABLE_1BYTE_20080314_TCC_
#define OPTABLE_1BYTE_20080314_TCC_

#include "Operand.h"

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes[0x100] = {

	/* 0x00 - 0x0f */
	{ "add", &Instruction::decode_Eb_Gb, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_Ev_Gv, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_Gb_Eb, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_Gv_Ev, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_AL_Ib, OP_ADD, FLAG_NONE, 2 },
	{ "add", &Instruction::decode_rAX_Iz, OP_ADD, FLAG_NONE, 2 },
	{ "push", &Instruction::decode_SegES, OP_PUSH, FLAG_NONE, 1 }, // ia-32 only
	{ "pop", &Instruction::decode_SegES, OP_POP, FLAG_NONE, 1 }, // ia-32 only
	{ "or", &Instruction::decode_Eb_Gb, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_Ev_Gv, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_Gb_Eb, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_Gv_Ev, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_AL_Ib, OP_OR, FLAG_NONE, 2 },
	{ "or", &Instruction::decode_rAX_Iz, OP_OR, FLAG_NONE, 2 },
	{ "push", &Instruction::decode_SegCS, OP_PUSH, FLAG_NONE, 1 }, // ia-32 only
	{ "2byte", &Instruction::decode_2byte, OP_2BYTE, FLAG_NONE, -1 },

	/* 0x10 - 0x1f */
	{ "adc", &Instruction::decode_Eb_Gb, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_Ev_Gv, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_Gb_Eb, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_Gv_Ev, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_AL_Ib, OP_ADC, FLAG_NONE, 2 },
	{ "adc", &Instruction::decode_rAX_Iz, OP_ADC, FLAG_NONE, 2 },
	{ "push", &Instruction::decode_SegSS, OP_PUSH, FLAG_NONE, 1 }, // ia-32 only
	{ "pop", &Instruction::decode_SegSS, OP_POP, FLAG_NONE, 1 }, // ia-32 only
	{ "sbb", &Instruction::decode_Eb_Gb, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_Ev_Gv, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_Gb_Eb, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_Gv_Ev, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_AL_Ib, OP_SBB, FLAG_NONE, 2 },
	{ "sbb", &Instruction::decode_rAX_Iz, OP_SBB, FLAG_NONE, 2 },
	{ "push", &Instruction::decode_SegDS, OP_PUSH, FLAG_NONE, 1 }, // ia-32 only
	{ "pop", &Instruction::decode_SegDS, OP_POP, FLAG_NONE, 1 }, // ia-32 only

	/* 0x20 - 0x2f */
	{ "and", &Instruction::decode_Eb_Gb, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_Ev_Gv, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_Gb_Eb, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_Gv_Ev, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_AL_Ib, OP_AND, FLAG_NONE, 2 },
	{ "and", &Instruction::decode_rAX_Iz, OP_AND, FLAG_NONE, 2 },
	{ "prefix es", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "daa", &Instruction::decode0, OP_DAA, FLAG_NONE, 0 }, 						// ia-32 only
	{ "sub", &Instruction::decode_Eb_Gb, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_Ev_Gv, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_Gb_Eb, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_Gv_Ev, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_AL_Ib, OP_SUB, FLAG_NONE, 2 },
	{ "sub", &Instruction::decode_rAX_Iz, OP_SUB, FLAG_NONE, 2 },
	{ "prefix cs", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "das", &Instruction::decode0, OP_DAS, FLAG_NONE, 0 }, // ia-32 only

	/* 0x30 - 0x3f */
	{ "xor", &Instruction::decode_Eb_Gb, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_Ev_Gv, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_Gb_Eb, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_Gv_Ev, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_AL_Ib, OP_XOR, FLAG_NONE, 2 },
	{ "xor", &Instruction::decode_rAX_Iz, OP_XOR, FLAG_NONE, 2 },
	{ "prefix ss", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "aaa", &Instruction::decode0, OP_AAA, FLAG_NONE, 0 }, // ia-32 only
	{ "cmp", &Instruction::decode_Eb_Gb, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_Ev_Gv, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_Gb_Eb, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_Gv_Ev, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_AL_Ib, OP_CMP, FLAG_NONE, 2 },
	{ "cmp", &Instruction::decode_rAX_Iz, OP_CMP, FLAG_NONE, 2 },
	{ "prefix ds", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "aas", &Instruction::decode0, OP_AAS, FLAG_NONE, 0 }, // ia-32 only

	/* 0x40 - 0x4f */
	{ "inc", &Instruction::decode_rAX, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "inc", &Instruction::decode_rCX, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "inc", &Instruction::decode_rDX, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "inc", &Instruction::decode_rBX, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "inc", &Instruction::decode_rSP, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "inc", &Instruction::decode_rBP, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "inc", &Instruction::decode_rSI, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "inc", &Instruction::decode_rDI, OP_INC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rAX, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rCX, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rDX, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rBX, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rSP, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rBP, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rSI, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)
	{ "dec", &Instruction::decode_rDI, OP_DEC, FLAG_NONE, 1 }, // ia-32 only (REX byte)

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
	{ "pushaw/pushad", &Instruction::decode_pushaw_pushad_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "popaw/popad", &Instruction::decode_popaw_popad_invalid, OP_INVALID, FLAG_NONE, -1 }, // ia-32 only
	{ "bound", &Instruction::decode_Gv_Ma, OP_BOUND, FLAG_NONE, 2 }, // ia-32 only
	{ "arpl", &Instruction::decode_Ew_Gw, OP_ARPL, FLAG_NONE, 2 }, // x86-64: movsxd Gv,Ev
	{ "prefix fs", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "prefix gs", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "prefix data16", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "prefix addr16", &Instruction::decode_invalid, OP_PREFIX, FLAG_NONE, -1 },
	{ "push", &Instruction::decode_Iz, OP_PUSH, FLAG_NONE, 1 },
	{ "imul", &Instruction::decode_Gv_Ev_Iz, OP_IMUL, FLAG_NONE, 3 },
	{ "push", &Instruction::decode_Ib, OP_PUSH, FLAG_NONE, 1 },
	{ "imul", &Instruction::decode_Gv_Ev_Ib, OP_IMUL, FLAG_NONE, 3 },
	{ "insb", &Instruction::decode0, OP_INS, FLAG_NONE, 0 },
	{ "insw/insd", &Instruction::decode_insw_insd_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "outsb", &Instruction::decode0, OP_OUTS, FLAG_NONE, 0 },
	{ "outsw/outsd", &Instruction::decode_outsw_outsd_invalid, OP_INVALID, FLAG_NONE, -1 },

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
	{ "group1", &Instruction::decode_group1, OP_GROUP1, FLAG_NONE, -1 }, // ia-32 only
	{ "group1", &Instruction::decode_group1, OP_GROUP1, FLAG_NONE, -1 },
	{ "test", &Instruction::decode_Eb_Gb, OP_TEST, FLAG_NONE, 2 },
	{ "test", &Instruction::decode_Ev_Gv, OP_TEST, FLAG_NONE, 2 },
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
	{ "xchg", &Instruction::decode_rAX_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "xchg", &Instruction::decode_rCX_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "xchg", &Instruction::decode_rDX_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "xchg", &Instruction::decode_rBX_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "xchg", &Instruction::decode_rSP_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "xchg", &Instruction::decode_rBP_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "xchg", &Instruction::decode_rSI_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "xchg", &Instruction::decode_rDI_rAX_NOREX, OP_XCHG, FLAG_NONE, 3 },
	{ "cbw/cwde/cdqe", &Instruction::decode_cbw_cwde_cdqe, OP_INVALID, FLAG_NONE, -1 },
	{ "cwd/cdq/cqo", &Instruction::decode_cwd_cdq_cqo, OP_INVALID, FLAG_NONE, -1 },
	{ "callf", &Instruction::decode_Ap, OP_CALL, FLAG_NONE, 1 }, // ia-32 only
	{ "wait/feni/fdisi/finit/fclex", &Instruction::wait_feni_fdisi_finit_fclex, OP_INVALID, FLAG_NONE, -1 },
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
	{ "les", &Instruction::decode_Gz_Mp, OP_LES, FLAG_NONE, 2 }, // ia-32 only
	{ "lds", &Instruction::decode_Gz_Mp, OP_LDS, FLAG_NONE, 2 }, // ia-32 only
	{ "group12", &Instruction::decode_group12, OP_GROUP12, FLAG_NONE, -1 },
	{ "group12", &Instruction::decode_group12, OP_GROUP12, FLAG_NONE, -1 },
	{ "enter", &Instruction::decode_Iw_Ib, OP_ENTER, FLAG_NONE, 2 },
	{ "leave", &Instruction::decode0, OP_LEAVE, FLAG_NONE, 0 },
	{ "retf", &Instruction::decode_Iw, OP_RETF, FLAG_NONE, 1 },
	{ "retf", &Instruction::decode0, OP_RETF, FLAG_NONE, 0 },
	{ "int3", &Instruction::decode0, OP_INT3, FLAG_NONE, 0 },
	{ "int", &Instruction::decode_Ib, OP_INT, FLAG_NONE, 1 },
	{ "into", &Instruction::decode0, OP_INTO, FLAG_NONE, 0 }, // ia-32 only
	{ "iretw/iret/iretq", &Instruction::decode_iretw_iret_iretq, OP_INVALID, FLAG_NONE, -1 },

	/* 0xd0 - 0xdf */
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "group2", &Instruction::decode_group2D, OP_GROUP2, FLAG_NONE, -1 },
	{ "aam", &Instruction::decode_Ib, OP_AAM, FLAG_NONE, 1 }, // ia-32 only
	{ "aad", &Instruction::decode_Ib, OP_AAD, FLAG_NONE, 1 }, // ia-32 only
	{ "salc", &Instruction::decode0, OP_SALC, FLAG_UNDOCUMENTED, 0 },
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
	{ "jmp", &Instruction::decode_Ap, OP_JMP, FLAG_NONE, 1 }, // ia-32 only
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

#endif
