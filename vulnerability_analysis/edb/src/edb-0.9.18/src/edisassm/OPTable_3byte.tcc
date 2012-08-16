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

#ifndef OPTABLE_3BYTE_20080314_TCC_
#define OPTABLE_3BYTE_20080314_TCC_

#include "Operand.h"

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_3Byte_38_NA[0x100] = {

	/* 0x0f 0x38 0x00 - 0x0f 0x38 0x0f */
	{ "pshufb", &Instruction::decode_Pq_Qq, OP_PSHUFB, FLAG_NONE, 2 },
	{ "phaddw", &Instruction::decode_Pq_Qq, OP_PHADDW, FLAG_NONE, 2 },
	{ "phaddd", &Instruction::decode_Pq_Qq, OP_PHADDD, FLAG_NONE, 2 },
	{ "phaddsw", &Instruction::decode_Pq_Qq, OP_PHADDSW, FLAG_NONE, 2 },
	{ "pmaddubsw", &Instruction::decode_Pq_Qq, OP_PMADDUBSW, FLAG_NONE, 2 },
	{ "phsubw", &Instruction::decode_Pq_Qq, OP_PHSUBW, FLAG_NONE, 2 },
	{ "phsubd", &Instruction::decode_Pq_Qq, OP_PHSUBD, FLAG_NONE, 2 },
	{ "phsubsw", &Instruction::decode_Pq_Qq, OP_PHSUBSW, FLAG_NONE, 2 },
	{ "psignb", &Instruction::decode_Pq_Qq, OP_PSIGNB, FLAG_NONE, 2 },
	{ "psignw", &Instruction::decode_Pq_Qq, OP_PSIGNW, FLAG_NONE, 2 },
	{ "psignd", &Instruction::decode_Pq_Qq, OP_PSIGND, FLAG_NONE, 2 },
	{ "pmulhrsw", &Instruction::decode_Pq_Qq, OP_PMULHRSW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x10 - 0x0f 0x38 0x1f */
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
	{ "pabsb", &Instruction::decode_Pq_Qq, OP_PABSB, FLAG_NONE, 2 },
	{ "pabsw", &Instruction::decode_Pq_Qq, OP_PABSW, FLAG_NONE, 2 },
	{ "pabsd", &Instruction::decode_Pq_Qq, OP_PABSD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x20 - 0x0f 0x38 0x2f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x30 - 0x0f 0x38 0x3f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x40 - 0x0f 0x38 0x4f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x50 - 0x0f 0x38 0x5f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x60 - 0x0f 0x38 0x6f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x70 - 0x0f 0x38 0x7f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x80 - 0x0f 0x38 0x8f */
	INVALID_BLOCK,

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

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_3Byte_38_F2[0x100] = {

	/* 0x0f 0x38 0x00 - 0x0f 0x38 0x0f */
	{ "pshufb", &Instruction::decode_Pq_Qq, OP_PSHUFB, FLAG_NONE, 2 },
	{ "phaddw", &Instruction::decode_Pq_Qq, OP_PHADDW, FLAG_NONE, 2 },
	{ "phaddd", &Instruction::decode_Pq_Qq, OP_PHADDD, FLAG_NONE, 2 },
	{ "phaddsw", &Instruction::decode_Pq_Qq, OP_PHADDSW, FLAG_NONE, 2 },
	{ "pmaddubsw", &Instruction::decode_Pq_Qq, OP_PMADDUBSW, FLAG_NONE, 2 },
	{ "phsubw", &Instruction::decode_Pq_Qq, OP_PHSUBW, FLAG_NONE, 2 },
	{ "phsubd", &Instruction::decode_Pq_Qq, OP_PHSUBD, FLAG_NONE, 2 },
	{ "phsubsw", &Instruction::decode_Pq_Qq, OP_PHSUBSW, FLAG_NONE, 2 },
	{ "psignb", &Instruction::decode_Pq_Qq, OP_PSIGNB, FLAG_NONE, 2 },
	{ "psignw", &Instruction::decode_Pq_Qq, OP_PSIGNW, FLAG_NONE, 2 },
	{ "psignd", &Instruction::decode_Pq_Qq, OP_PSIGND, FLAG_NONE, 2 },
	{ "pmulhrsw", &Instruction::decode_Pq_Qq, OP_PMULHRSW, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x10 - 0x0f 0x38 0x1f */
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
	{ "pabsb", &Instruction::decode_Pq_Qq, OP_PABSB, FLAG_NONE, 2 },
	{ "pabsw", &Instruction::decode_Pq_Qq, OP_PABSW, FLAG_NONE, 2 },
	{ "pabsd", &Instruction::decode_Pq_Qq, OP_PABSD, FLAG_NONE, 2 },
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },

	/* 0x0f 0x38 0x20 - 0x0f 0x38 0x2f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x30 - 0x0f 0x38 0x3f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x40 - 0x0f 0x38 0x4f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x50 - 0x0f 0x38 0x5f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x60 - 0x0f 0x38 0x6f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x70 - 0x0f 0x38 0x7f */
	INVALID_BLOCK,

	/* 0x0f 0x38 0x80 - 0x0f 0x38 0x8f */
	INVALID_BLOCK,

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
	{ "crc32", &Instruction::decode_Gq_Eb, OP_CRC32, FLAG_NONE, 2 },
	{ "crc32", &Instruction::decode_Gq_Eq, OP_CRC32, FLAG_NONE, 2 },
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

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_3Byte_38_66[0x100] = {

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
	{ "nvept", &Instruction::decode_Gd_Mo, OP_NVEPT, FLAG_NONE, 2 },		// x86-64: nvept Gq, Mo
	{ "nvvpid", &Instruction::decode_Gd_Mo, OP_NVVPID, FLAG_NONE, 2 },	// x86-64: nvvpid Gq, Mo
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

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_3Byte_3A_66[0x100] = {
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
	{ "pextrd", &Instruction::decode_Ed_Vo_Ib, OP_PEXTRD, FLAG_NONE, 3 },		// x86-64: pextrq Eq, Vo, Ib
	{ "extractps", &Instruction::decode_Ed_Vo_Ib, OP_EXTRACTPS, FLAG_NONE, 3 },	// x86-64: extractps Ev, Vo, Ib
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
	{ "pinsrd", &Instruction::decode_Vo_Ed_Ib, OP_PINSRD, FLAG_NONE, 3 },		// x86-64: pinsrq Vo, Eq, Ib
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

template <class M>
const typename Instruction<M>::opcode_entry Instruction<M>::Opcodes_3Byte_3A_NA[0x100] = {
	/* 0x0f 0x3a 0x00 - 0x0f 0x3a 0x0f */
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
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },
	{ "palignr", &Instruction::decode_Pq_Qq_Ib, OP_PALIGNR, FLAG_NONE, 3 },

	/* 0x0f 0x3a 0x10 - 0x0f 0x3a 0x1f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x20 - 0x0f 0x3a 0x2f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x30 - 0x0f 0x3a 0x3f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x40 - 0x0f 0x3a 0x4f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x50 - 0x0f 0x3a 0x5f */
	INVALID_BLOCK,

	/* 0x0f 0x3a 0x60 - 0x0f 0x3a 0x6f */
	INVALID_BLOCK,

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
