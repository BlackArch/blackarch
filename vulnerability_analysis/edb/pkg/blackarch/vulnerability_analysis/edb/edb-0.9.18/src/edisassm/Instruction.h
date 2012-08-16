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

#ifndef INSTRUCTION_20070427_H_
#define INSTRUCTION_20070427_H_

#include "edisassm_util.h"
#include "edisassm_exception.h"
#include "edisassm_model.h"
#include "edisassm_types.h"
#include "edisassm_string.h"
#include "edisassm_ops.h"
#include <cstddef>
#include "ModRM.h"
#include "SIB.h"
#include "Operand.h"
#include "REX.h"

#ifdef QT_CORE_LIB
#include <QtGlobal>
#ifndef EDB_PLUGIN
#define EDB_EXPORT Q_DECL_EXPORT
#else
#define EDB_EXPORT Q_DECL_IMPORT
#endif
#else
#define EDB_EXPORT
#endif

#ifndef INVALID_BLOCK
#define INVALID_BLOCK \
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 },\
	{ "invalid", &Instruction::decode_invalid, OP_INVALID, FLAG_NONE, -1 }
#endif

template <class M>
class EDB_EXPORT Instruction {
public:
	static const int MAX_OPERANDS = M::MAX_OPERANDS;
	static const int MAX_SIZE     = M::MAX_SIZE;
	static const int BITS         = M::BITS;

public:
	typedef Operand<M>            operand_t;
	typedef typename M::address_t address_t;
	typedef Instruction<M>        instruction_t;

public:
	typedef void (instruction_t::*decoder_t)(const uint8_t *);

public:
	Instruction(const uint8_t *buf, std::size_t size, address_t rva, const std::nothrow_t&) throw();
	Instruction(const uint8_t *buf, std::size_t size, address_t rva);
	~Instruction();

public:
	Instruction(const Instruction &);
	Instruction &operator=(const Instruction &);

public:
	void swap(Instruction &other);

private:
	void initialize(const uint8_t *buf, std::size_t size);

public:
	enum Flags {
		FLAG_NONE           = 0x00000000,
		FLAG_TEST_FLAGS     = 0x00000001,
		FLAG_WRITES_FLAGS   = 0x00000002,
		FLAG_MODIFIES_STACK = 0x00000004,
		FLAG_RING0          = 0x00000008,
		FLAG_UNDOCUMENTED   = 0x00000010,
		FLAG_AMD            = 0x00000020,
		FLAG_INTEL          = 0x00000040
	};

	enum Prefix {
		PREFIX_NONE             = 0x00000000,
		PREFIX_LOCK             = 0x00000001,
		PREFIX_REPNE            = 0x00000002,
		PREFIX_REP              = 0x00000004,

		PREFIX_CS               = 0x00000100,
		PREFIX_SS               = 0x00000200,
		PREFIX_DS               = 0x00000400,
		PREFIX_ES               = 0x00000800,
		PREFIX_FS               = 0x00001000,
		PREFIX_GS               = 0x00002000,
		PREFIX_BRANCH_NOT_TAKEN = 0x00004000,
		PREFIX_BRANCH_TAKEN     = 0x00008000,

		PREFIX_OPERAND          = 0x00010000,

		PREFIX_ADDRESS          = 0x01000000
	};

	enum Type {
		// invalid
		OP_INVALID = 0x00000000,

		// data transfer
		OP_MOV,
		OP_CMOVCC,
		OP_XCHG,
		OP_BSWAP,
		OP_XADD,
		OP_CMPXCHG,
		OP_PUSH,
		OP_PUSHA,
		OP_POP,
		OP_POPA,
		OP_CWD,
		OP_CDQ,
		OP_CQO,
		OP_CDQE,
		OP_CBW,
		OP_CWDE,
		OP_MOVSX,
		OP_MOVZX,
		OP_MOVSXD,
		OP_MOVBE,

		// arithmetic
		OP_ADD,
		OP_ADC,
		OP_SUB,
		OP_SBB,
		OP_IMUL,
		OP_MUL,
		OP_IDIV,
		OP_DIV,
		OP_INC,
		OP_DEC,
		OP_NEG,
		OP_CMP,
		OP_DAA,
		OP_DAS,
		OP_AAA,
		OP_AAS,
		OP_AAM,
		OP_AAD,

		// logic
		OP_AND,
		OP_OR,
		OP_XOR,
		OP_NOT,

		// shift/rotate
		OP_SAR,
		OP_SAL,
		OP_SHR,
		OP_SHL,
		OP_SHRD,
		OP_SHLD,
		OP_ROR,
		OP_ROL,
		OP_RCR,
		OP_RCL,

		// bit/byte
		OP_BT,
		OP_BTS,
		OP_BTR,
		OP_BTC,
		OP_BSF,
		OP_BSR,
		OP_SETCC,
		OP_TEST,

		// control flow
		OP_JMP,
		OP_JCC,
		OP_LOOP,
		OP_LOOPE,
		OP_LOOPNE,
		OP_CALL,
		OP_RET,
		OP_RETF,
		OP_IRET,
		OP_INT,
		OP_INT3,
		OP_INTO,
		OP_BOUND,
		OP_ENTER,
		OP_LEAVE,

		// string
		OP_MOVS,
		OP_CMPS,
		OP_CMPSW,
		OP_SCAS,
		OP_LODS,
		OP_STOS,

		// I/O
		OP_IN,
		OP_OUT,
		OP_INS,
		OP_OUTS,

		// flag control
		OP_STC,
		OP_CLC,
		OP_CMC,
		OP_CLD,
		OP_STD,
		OP_LAHF,
		OP_SAHF,
		OP_PUSHF,
		OP_POPF,
		OP_STI,
		OP_CLI,

		// segment manipulation
		OP_LDS,
		OP_LES,
		OP_LFS,
		OP_LGS,
		OP_LSS,
		OP_SWAPGS,

		// misc
		OP_PEXTRQ,
		OP_PINSRQ,
		OP_POPCNT,
		OP_LEA,
		OP_NOP,
		OP_UD,
		OP_UD2,
		OP_XLAT,
		OP_CPUID,
		OP_CRC32,
		OP_XSAVE,
		OP_XRSTOR,
		OP_GETSEC,
		OP_PBLENDVB,
		OP_BLENDVPS,
		OP_BLENDVPD,
		OP_PTEST,
		OP_PMOVSXBW,
		OP_PMOVSXBD,
		OP_PMOVSXBQ,
		OP_PMOVSXWD,
		OP_PMOVSXWQ,
		OP_PMOVSXDQ,
		OP_PMULDQ,
		OP_PCMPEQQ,
		OP_MOVNTDQA,
		OP_PACKUSDW,
		OP_PMOVZXBW,
		OP_PMOVZXBD,
		OP_PMOVZXBQ,
		OP_PMOVZXWD,
		OP_PMOVZXWQ,
		OP_PMOVZXDQ,
		OP_PCMPGTQ,
		OP_PMINSB,
		OP_PMINSD,
		OP_PMINUW,
		OP_PMINUD,
		OP_PMAXSB,
		OP_PMAXSD,
		OP_PMAXUW,
		OP_PMAXUD,
		OP_PMULLD,
		OP_PHMINPOSUW,
		OP_NVEPT,
		OP_NVVPID,
		OP_ROUNDPS,
		OP_ROUNDPD,
		OP_ROUNDSS,
		OP_ROUNDSD,
		OP_BLENDPS,
		OP_BLENDPD,
		OP_PBLENDW,
		OP_PEXTRB,
		OP_PEXTRD,
		OP_EXTRACTPS,
		OP_PINSRB,
		OP_INSERTPS,
		OP_PINSRD,
		OP_DPPS,
		OP_DPPD,
		OP_MPSADBW,
		OP_PCMPESTRM,
		OP_PCMPESTRI,
		OP_PCMPISTRM,
		OP_PCMPISTRI,

		// misc
		OP_CMPXCHG8B,
		OP_CMPXCHG16B,
		OP_MOVNTSD,
		OP_MOVNTSS,

		// VM
		OP_VMCALL,
		OP_VMCLEAR,
		OP_VMLAUNCH,
		OP_VMPTRLD,
		OP_VMPTRST,
		OP_VMRESUME,
		OP_VMXOFF,
		OP_VMXON,
		OP_VMREAD,
		OP_VMWRITE,
		OP_XGETBV,
		OP_XSETBV,

		// system
		OP_LGDT,
		OP_SGDT,
		OP_LLDT,
		OP_SLDT,
		OP_LTR,
		OP_STR,
		OP_LIDT,
		OP_SIDT,
		OP_LMSW,
		OP_SMSW,
		OP_CLTS,
		OP_ARPL,
		OP_LAR,
		OP_LSL,
		OP_VERR,
		OP_VERW,
		OP_INVD,
		OP_WBINVD,
		OP_INVLPG,
		OP_HLT,
		OP_RSM,
		OP_RDMSR,
		OP_WRMSR,
		OP_RDPMC,
		OP_RDTSC,
		OP_SYSENTER,
		OP_SYSEXIT,
		OP_SYSCALL,
		OP_SYSRET,

		// MMX
		OP_MOVD,
		OP_MOVQ,
		OP_PACKSSDW,
		OP_PACKSSWB,
		OP_PACKUSWB,
		OP_PUNPCKHBW,
		OP_PUNPCKHWD,
		OP_PUNPCKHDQ,
		OP_PUNPCKLBW,
		OP_PUNPCKLWD,
		OP_PUNPCKLDQ,
		OP_PADDB,
		OP_PADDW,
		OP_PADDD,
		OP_PABSD, // intel says this is both MMX and SSSE3!
		OP_PADDSB,
		OP_PADDSW,
		OP_PADDUSB,
		OP_PADDUSW,
		OP_PSUBB,
		OP_PSUBW,
		OP_PSUBD,
		OP_PSUBSB,
		OP_PSUBSW,
		OP_PSUBUSB,
		OP_PSUBUSW,
		OP_PMULHW,
		OP_PMULLW,
		OP_PMADDWD,
		OP_PCMPEQB,
		OP_PCMPEQW,
		OP_PCMPEQD,
		OP_PCMPGTB,
		OP_PCMPGTW,
		OP_PCMPGTD,
		OP_PAND,
		OP_PANDN,
		OP_POR,
		OP_PXOR,
		OP_PSLLW,
		OP_PSLLD,
		OP_PSLLQ,
		OP_PSRLW,
		OP_PSRLD,
		OP_PSRLQ,
		OP_PSRAW,
		OP_PSRAD,
		OP_EMMS,

		// SSE
		OP_MOVAPS,
		OP_MOVUPS,
		OP_MOVHPS,
		// TODO: MOVHLPS? where is it?
		OP_MOVLPS,
		// TODO: MOVLHPS? where is it?
		OP_MOVMSKPS,
		OP_MOVSS,
		OP_ADDPS,
		OP_ADDSS,
		OP_SUBPS,
		OP_SUBSS,
		OP_MULPS,
		OP_MULSS,
		OP_DIVPS,
		OP_DIVSS,
		OP_RCPPS,
		OP_RCPSS,
		OP_SQRTPS,
		OP_SQRTSS,
		OP_RSQRTPS,
		OP_RSQRTSS,
		OP_MAXPS,
		OP_MAXSS,
		OP_MINPS,
		OP_MINSS,
		OP_CMPPS,
		OP_CMPSS,
		OP_COMISS,
		OP_UCOMISS,
		OP_ANDPS,
		OP_ANDNPS,
		OP_ORPS,
		OP_XORPS,
		OP_SHUFPS,
		OP_UNPCKHPS,
		OP_UNPCKLPS,
		OP_CVTPI2PS,
		OP_CVTSI2SS,
		OP_CVTPS2PI,
		OP_CVTTPS2PI,
		OP_CVTSS2SI,
		OP_CVTTSS2SI,
		OP_LDMXCSR,
		OP_STMXCSR,
		OP_PAVGB,
		OP_PAVGW,
		OP_PEXTRW,
		OP_PINSRW,
		OP_PMAXUB,
		OP_PMAXSW,
		OP_PMINUB,
		OP_PMINSW,
		OP_PMOVMSKB,
		OP_PMULHUW,
		OP_PSADBW,
		OP_PSHUFW,
		OP_MASKMOVQ,
		OP_MOVNTQ,
		OP_MOVNTPS,
		OP_PREFETCHH,
		OP_SFENCE,

		// SSE2
		OP_MOVAPD,
		OP_MOVUPD,
		OP_MOVHPD,
		OP_MOVLPD,
		OP_MOVMSKPD,
		OP_MOVSD,
		OP_ADDPD,
		OP_ADDSD,
		OP_SUBPD,
		OP_SUBSD,
		OP_MULPD,
		OP_MULSD,
		OP_DIVPD,
		OP_DIVSD,
		OP_SQRTPD,
		OP_SQRTSD,
		OP_MAXPD,
		OP_MAXSD,
		OP_MINPD,
		OP_MINSD,
		OP_ANDPD,
		OP_ANDNPD,
		OP_ORPD,
		OP_XORPD,
		OP_CMPPD,
		OP_CMPSD,
		OP_COMISD,
		OP_UCOMISD,
		OP_SHUFPD,
		OP_UNPCKHPD,
		OP_UNPCKLPD,
		OP_CVTPD2PI,
		OP_CVTTPD2PI,
		OP_CVTPI2PD,
		OP_CVTPD2DQ,
		OP_CVTTPD2DQ,
		OP_CVTDQ2PD,
		OP_CVTPS2PD,
		OP_CVTPD2PS,
		OP_CVTSS2SD,
		OP_CVTSD2SS,
		OP_CVTSD2SI,
		OP_CVTTSD2SI,
		OP_CVTSI2SD,
		OP_CVTDQ2PS,
		OP_CVTPS2DQ,
		OP_CVTTPS2DQ,
		OP_MOVDQA,
		OP_MOVDQU,
		OP_MOVQ2DQ,
		OP_MOVDQ2Q,
		OP_PMULUDQ,
		OP_PADDQ,
		OP_PSUBQ,
		OP_PSHUFLW,
		OP_PSHUFHW,
		OP_PSHUFD,
		OP_PSLLDQ,
		OP_PSRLDQ,
		OP_PUNPCKHQDQ,
		OP_PUNPCKLQDQ,
		OP_CLFLUSH,
		OP_LFENCE,
		OP_MFENCE,
		OP_PAUSE,
		OP_MASKMOVDQU,
		OP_MOVNTPD,
		OP_MOVNTDQ,
		OP_MOVNTI,

		// SSE3
		OP_FISTTP,
		OP_LDDQU,
		OP_ADDSUBPS,
		OP_ADDSUBPD,
		OP_HADDPS,
		OP_HSUBPS,
		OP_HADDPD,
		OP_HSUBPD,
		OP_MOVSHDUP,
		OP_MOVSLDUP,
		OP_MOVDDUP,
		OP_MONITOR,
		OP_MWAIT,

		// SSSE3
		OP_PHADDW,
		OP_PHADDSW,
		OP_PHADDD,
		OP_PHSUBW,
		OP_PHSUBSW,
		OP_PHSUBD,
		OP_PABSB,
		OP_PABSW,
		OP_PMADDUBSW,
		OP_PMULHRSW,
		OP_PSHUFB,
		OP_PSIGNB,
		OP_PSIGNW,
		OP_PSIGND,
		OP_PALIGNR,

		// FPU ops
		OP_F2XM1,
		OP_FABS,
		OP_FADD,
		OP_FADDP,
		OP_FBLD,
		OP_FBSTP,
		OP_FCHS,
		OP_FCLEX,
		OP_FCMOVB,
		OP_FCMOVBE,
		OP_FCMOVE,
		OP_FCMOVNB,
		OP_FCMOVNBE,
		OP_FCMOVNE,
		OP_FCMOVNU,
		OP_FCMOVU,
		OP_FCOM,
		OP_FCOMI,
		OP_FCOMIP,
		OP_FCOMP,
		OP_FCOMPP,
		OP_FCOS,
		OP_FDECSTP,
		OP_FDISI,
		OP_FDIV,
		OP_FDIVP,
		OP_FDIVR,
		OP_FDIVRP,
		OP_FENI,
		OP_FFREE,
		OP_FIADD,
		OP_FICOM,
		OP_FICOMP,
		OP_FIDIV,
		OP_FIDIVR,
		OP_FILD,
		OP_FIMUL,
		OP_FINCSTP,
		OP_FINIT,
		OP_FIST,
		OP_FISTP,
		OP_FISUB,
		OP_FISUBR,
		OP_FLD,
		OP_FLD1,
		OP_FLDCW,
		OP_FLDENV,
		OP_FLDL2E,
		OP_FLDL2T,
		OP_FLDLG2,
		OP_FLDLN2,
		OP_FLDPI,
		OP_FLDZ,
		OP_FMUL,
		OP_FMULP,
		OP_FNCLEX,
		OP_FNDISI,
		OP_FNENI,
		OP_FNINIT,
		OP_FNOP,
		OP_FPATAN,
		OP_FPREM,
		OP_FPREM1,
		OP_FPTAN,
		OP_FRNDINT,
		OP_FRSTOR,
		OP_FSAVE,
		OP_FSCALE,
		OP_FSIN,
		OP_FSINCOS,
		OP_FSQRT,
		OP_FST,
		OP_FSTCW,
		OP_FSTENV,
		OP_FSTP,
		OP_FSTSW,
		OP_FSUB,
		OP_FSUBP,
		OP_FSUBR,
		OP_FSUBRP,
		OP_FTST,
		OP_FUCOM,
		OP_FUCOMI,
		OP_FUCOMIP,
		OP_FUCOMP,
		OP_FUCOMPP,
		OP_FXAM,
		OP_FXCH,
		OP_FXRSTOR,
		OP_FXSAVE,
		OP_FXTRACT,
		OP_FYL2X,
		OP_FYL2XP1,
		OP_WAIT,

		// AMD extensions
		OP_VMRUN,
		OP_VMMCALL,
		OP_VMLOAD,
		OP_VMSAVE,
		OP_STGI,
		OP_CLGI,
		OP_SKINIT,
		OP_INVLPGA,
		OP_RDTSCP,
		
		// undocumented
		OP_SALC,

		// these aren't really valid, should never been seen in any external API
		OP_GROUP1,
		OP_GROUP2,
		OP_GROUP3,
		OP_GROUP4,
		OP_GROUP5,
		OP_GROUP6,
		OP_GROUP7,
		OP_GROUP8,
		OP_GROUP9,
		OP_GROUP10,
		OP_GROUP11,
		OP_GROUP12,
		OP_GROUP13,
		OP_GROUP14,
		OP_GROUP15,
		OP_GROUP16,
		OP_GROUP17,
		OP_PREFIX,
		OP_2BYTE,
		OP_3BYTE,
		OP_FPU
	};

private:
	void process_prefixes(const uint8_t *&buf, std::size_t size);
	operand_t &next_operand();

private:
	// convenience binders,
	template <decoder_t F1, decoder_t F2, decoder_t F3>
	void decode3(const uint8_t *buf) {
		(this->*F1)(buf);
		(this->*F2)(buf);
		(this->*F3)(buf);
	}

	template <decoder_t F1, decoder_t F2>
	void decode2(const uint8_t *buf) {
		(this->*F1)(buf);
		(this->*F2)(buf);
	}

	template <decoder_t F1>
	void decode1(const uint8_t *buf) {
		(this->*F1)(buf);
	}

	void decode0(const uint8_t *buf) {
		UNUSED(buf);
	}

private:

	// tests if the new size is allowed
	void bounds_check(std::size_t new_size) {
		if(new_size > buffer_size_) {
			throw edisassm::instruction_too_big(instruction_t::size());
		}
	}

	template <class T>
	void decode_size_sensitive(const uint8_t *buf, const T(&opcodes)[3]) {
		switch(operand_size()) {
		case 16: opcode_ = &opcodes[0]; break;
		case 32: opcode_ = &opcodes[1]; break;
		case 64: opcode_ = &opcodes[2]; break;
		}
		(this->*(opcode_->decoder))(buf);
	}
	
	// special FPU variants..
	void wait_feni_fdisi_finit_fclex(const uint8_t *buf);

	// special cased names
	void decode_cwd_cdq_cqo(const uint8_t *buf);
	void decode_stosw_stosd_stosq(const uint8_t *buf);
	void decode_lodsw_lodsd_lodsq(const uint8_t *buf);
	void decode_cbw_cwde_cdqe(const uint8_t *buf);
	void decode_scasw_scasd_scasq(const uint8_t *buf);
	void decode_iretw_iret_iretq(const uint8_t *buf);
	void decode_pushfw_pushfd_pushfq(const uint8_t *buf);
	void decode_popfw_popfd_popfq(const uint8_t *buf);
	void decode_movsw_movsd_movsq(const uint8_t *buf);

	void decode_invalid_cmpxchg8b_cmpxchg16b(const uint8_t *buf);


	// no 64-bit variant
	void decode_cmpsw_cmpsd_cmpsq(const uint8_t *buf);
	void decode_outsw_outsd_invalid(const uint8_t *buf);
	void decode_insw_insd_invalid(const uint8_t *buf);
	void decode_pushaw_pushad_invalid(const uint8_t *buf);
	void decode_popaw_popad_invalid(const uint8_t *buf);

private:
	// groups
	void decode_group1(const uint8_t *buf);
	void decode_group2(const uint8_t *buf);
	void decode_group2D(const uint8_t *buf);
	void decode_group3(const uint8_t *buf);
	void decode_group4(const uint8_t *buf);
	void decode_group5(const uint8_t *buf);
	void decode_group6(const uint8_t *buf);
	void decode_group7(const uint8_t *buf);
	void decode_group8(const uint8_t *buf);
	void decode_group9(const uint8_t *buf);
	void decode_group10(const uint8_t *buf);
	void decode_group11(const uint8_t *buf);
	void decode_group12(const uint8_t *buf);
	void decode_group13(const uint8_t *buf);
	void decode_group14(const uint8_t *buf);
	void decode_group15(const uint8_t *buf);
	void decode_group16(const uint8_t *buf);
	void decode_group17(const uint8_t *buf);

private:
	// invalid handler
	void decode_invalid(const uint8_t *buf);

private:
	// multibyte encodings
	void decode_x87(const uint8_t *buf);
	void decode_2byte(const uint8_t *buf);
	void decode_3byte_38(const uint8_t *buf);
	void decode_3byte_3A(const uint8_t *buf);

private:
	ModRM get_modrm(const uint8_t *buf);
	
	SIB get_sib(const uint8_t *buf);

	template <typename operand_t::Register REG>
	void decode_Reg(const uint8_t *buf);

	template <int64_t IMM>
	void decode_const_Iq(const uint8_t *buf);

	template <int32_t IMM>
	void decode_const_Id(const uint8_t *buf);

	template <int16_t IMM>
	void decode_const_Iw(const uint8_t *buf);

	template <int8_t IMM>
	void decode_const_Ib(const uint8_t *buf);

	template <class T>
	T get_immediate(const uint8_t *buf);

	template <class T>
	T get_displacement(const uint8_t *buf);

	template <int index>
	void decode_STi(const uint8_t *buf);

	template <typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_Gx(const uint8_t *buf);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_0_16(const uint8_t *buf, const ModRM &rm, operand_t &operand);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_1_16(const uint8_t *buf, const ModRM &rm, operand_t &operand);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_2_16(const uint8_t *buf, const ModRM &rm, operand_t &operand);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_0_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit = true);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_1_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit = true);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_2_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit = true);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_3_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit = true);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_ModRM_Invalid(const uint8_t *buf, const ModRM &rm, operand_t &operand);

	template <typename operand_t::Type TYPE, typename operand_t::Register (*REG_DECODE)(uint8_t)>
	void decode_Ex(const uint8_t *buf);

private:
	// ModRM Reg or Memory
	void decode_Eb(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION8, &Instruction<M>::index_to_reg_8>(buf); }
	void decode_Ew(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION16, &Instruction<M>::index_to_reg_16>(buf); }
	void decode_Ed(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_32>(buf); }
	void decode_Eq(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction<M>::index_to_reg_64>(buf); }
	void decode_Ev(const uint8_t *buf);
	void decode_Qd(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_mmx>(buf); }
	void decode_Qq(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction<M>::index_to_reg_mmx>(buf); }
	void decode_Qo(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION128, &Instruction<M>::index_to_reg_mmx>(buf); }
	void decode_Ww(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION16, &Instruction<M>::index_to_reg_xmmx>(buf); }
	void decode_Wd(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_xmmx>(buf); }
	void decode_Wq(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction<M>::index_to_reg_xmmx>(buf); }
	void decode_Wo(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION128, &Instruction<M>::index_to_reg_xmmx>(buf); }


	// special cases for things like SMSW Rv/Mw
	template <decoder_t F1, decoder_t F2>
	void decode_Reg_Mem(const uint8_t *buf) {
		const ModRM rm = get_modrm(buf);

		if(rm.mod() == 0x03) {
			(this->*F1)(buf);
		} else {
			(this->*F2)(buf);
		}
	}

	void decode_Rv_Mw(const uint8_t *buf) { decode_Reg_Mem<&instruction_t::decode_Rv, &instruction_t::decode_Mw>(buf); }
	void decode_Rq_Mw(const uint8_t *buf) { decode_Reg_Mem<&instruction_t::decode_Rq, &instruction_t::decode_Mw>(buf); }
	void decode_Rd_Mw(const uint8_t *buf) { decode_Reg_Mem<&instruction_t::decode_Rd, &instruction_t::decode_Mw>(buf); }
	void decode_Rd_Mb(const uint8_t *buf) { decode_Reg_Mem<&instruction_t::decode_Rd, &instruction_t::decode_Mb>(buf); }
	void decode_Uo_Mw(const uint8_t *buf) { decode_Reg_Mem<&instruction_t::decode_Uo, &instruction_t::decode_Mw>(buf); }
	void decode_Uo_Md(const uint8_t *buf) { decode_Reg_Mem<&instruction_t::decode_Uo, &instruction_t::decode_Md>(buf); }
	void decode_Uo_Mq(const uint8_t *buf) { decode_Reg_Mem<&instruction_t::decode_Uo, &instruction_t::decode_Mq>(buf); }

	// TODO: throw an error if any of these return REG_INVALID
	// ModRM Memory Only
	void decode_Mo(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION128, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_M(const uint8_t *buf)    { decode_Ex<operand_t::TYPE_EXPRESSION, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_Ms(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_M108(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION, &Instruction<M>::index_to_reg_invalid>(buf); }  // Note this can be 108 or 98, but we just say "expression"
	void decode_M512(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION, &Instruction<M>::index_to_reg_invalid>(buf); }  // Note this can be 512 or 256, but we just say "expression"
	void decode_M28(const uint8_t *buf)  { decode_Ex<operand_t::TYPE_EXPRESSION, &Instruction<M>::index_to_reg_invalid>(buf); }  // Note this can be 28 or 14, but we just say "expression"
	void decode_Mw(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION16, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_Mp(const uint8_t *buf)   { decode_Ep(buf);}
	void decode_Mdq(const uint8_t *buf)  { decode_Ex<operand_t::TYPE_EXPRESSION128, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_Mq(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_Md(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_Mb(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION8, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_Ma(const uint8_t *buf)   { if(prefix_ & PREFIX_OPERAND) { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_invalid>(buf); } else { decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction<M>::index_to_reg_invalid>(buf); } }
	void decode_Mv(const uint8_t *buf);
	void decode_Ep(const uint8_t *buf)   { if(prefix_ & PREFIX_OPERAND) { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_invalid>(buf); } else { decode_Ex<operand_t::TYPE_EXPRESSION48, &Instruction<M>::index_to_reg_invalid>(buf); } }


	// FPU memory only
	void decode_WordInteger(const uint8_t *buf)  { decode_Ex<operand_t::TYPE_EXPRESSION16, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_SingleReal(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_ShortInteger(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_DoubleReal(const uint8_t *buf)   { decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_LongInteger(const uint8_t *buf)  { decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_ExtendedReal(const uint8_t *buf) { decode_Ex<operand_t::TYPE_EXPRESSION80, &Instruction<M>::index_to_reg_invalid>(buf); }
	void decode_PackedBCD(const uint8_t *buf)    { decode_Ex<operand_t::TYPE_EXPRESSION80, &Instruction<M>::index_to_reg_invalid>(buf); }

	// ModRM selects a register only (mod must be 0x3)
	// TODO: throw an error if any of these return TYPE_INVALID
	void decode_Rv(const uint8_t *buf);
	void decode_Rw(const uint8_t *buf) { decode_Ex<operand_t::TYPE_INVALID, &Instruction<M>::index_to_reg_16>(buf); }
	void decode_Rd(const uint8_t *buf) { decode_Ex<operand_t::TYPE_INVALID, &Instruction<M>::index_to_reg_32>(buf); }
	void decode_Rq(const uint8_t *buf) { decode_Ex<operand_t::TYPE_INVALID, &Instruction<M>::index_to_reg_64>(buf); }
	void decode_Nq(const uint8_t *buf) { decode_Ex<operand_t::TYPE_INVALID, &Instruction<M>::index_to_reg_mmx>(buf); }
	void decode_Uo(const uint8_t *buf) { decode_Ex<operand_t::TYPE_INVALID, &Instruction<M>::index_to_reg_xmmx>(buf); }
	void decode_Uq(const uint8_t *buf) { decode_Ex<operand_t::TYPE_INVALID, &Instruction<M>::index_to_reg_xmmx>(buf); }

	// ModRM Reg Field
	void decode_Gv(const uint8_t *buf);
	void decode_Gb(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_8>(buf); }
	void decode_Gw(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_16>(buf); }
	void decode_Gd(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_32>(buf); }
	void decode_Gq(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_64>(buf); }
	void decode_Gz(const uint8_t *buf) { if(prefix_ & PREFIX_OPERAND) { decode_Gx<&Instruction<M>::index_to_reg_16>(buf); } else { decode_Gx<&Instruction<M>::index_to_reg_32>(buf); } }
	void decode_Vo(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_xmmx>(buf); }
	void decode_Vd(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_xmmx>(buf); }
	void decode_Vq(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_xmmx>(buf); }
	void decode_Pq(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_mmx>(buf); }
	void decode_Pd(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_mmx>(buf); }

	// Decode [C/D/T]Rx from ModRM reg
	void decode_Sw(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_seg>(buf); }
	void decode_Cd(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_cr>(buf); }
	void decode_Dd(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_dr>(buf); }
	void decode_Td(const uint8_t *buf) { decode_Gx<&Instruction<M>::index_to_reg_tr>(buf); }

	// Immediate
	void decode_Ib(const uint8_t *buf);
	void decode_Iw(const uint8_t *buf);
	void decode_Id(const uint8_t *buf);
	void decode_Iq(const uint8_t *buf);
	void decode_Iv(const uint8_t *buf);
	void decode_Iz(const uint8_t *buf) { if(prefix_ & PREFIX_OPERAND) { decode_Iw(buf); } else { decode_Id(buf); } }


	// EIP relative
	void decode_Jb(const uint8_t *buf);
	void decode_Jw(const uint8_t *buf);
	void decode_Jd(const uint8_t *buf);
	void decode_Jq(const uint8_t *buf);
	void decode_Jz(const uint8_t *buf) { if(prefix_ & PREFIX_OPERAND) { decode_Jw(buf); } else { decode_Jd(buf); } }

	// Direct offset
	void decode_Ob(const uint8_t *buf);
	void decode_Ow(const uint8_t *buf);
	void decode_Od(const uint8_t *buf);
	void decode_Ov(const uint8_t *buf);

	// Absolute address
	void decode_Ap(const uint8_t *buf);

	// Decode Specific Register
	void decode_eAX(const uint8_t *buf);
	void decode_rAX(const uint8_t *buf);
	void decode_rAX_NOREX(const uint8_t *buf);
	void decode_rCX(const uint8_t *buf);
	void decode_rDX(const uint8_t *buf);
	void decode_rBX(const uint8_t *buf);
	void decode_rSP(const uint8_t *buf);
	void decode_rBP(const uint8_t *buf);
	void decode_rSI(const uint8_t *buf);
	void decode_rDI(const uint8_t *buf);
	void decode_AL(const uint8_t *buf);
	void decode_CL(const uint8_t *buf);
	void decode_DL(const uint8_t *buf);
	void decode_BL(const uint8_t *buf);
	void decode_AH(const uint8_t *buf);
	void decode_CH(const uint8_t *buf);
	void decode_DH(const uint8_t *buf);
	void decode_BH(const uint8_t *buf);
	void decode_DX(const uint8_t *buf) { decode_Reg<operand_t::REG_DX>(buf); }

	// simple wrappers around some of the these for cleaner tables
	void decode_SegCS(const uint8_t *buf)    { decode_Reg<operand_t::REG_CS>(buf); }
	void decode_SegDS(const uint8_t *buf)    { decode_Reg<operand_t::REG_DS>(buf); }
	void decode_SegES(const uint8_t *buf)    { decode_Reg<operand_t::REG_ES>(buf); }
	void decode_SegFS(const uint8_t *buf)    { decode_Reg<operand_t::REG_FS>(buf); }
	void decode_SegGS(const uint8_t *buf)    { decode_Reg<operand_t::REG_GS>(buf); }
	void decode_SegSS(const uint8_t *buf)    { decode_Reg<operand_t::REG_SS>(buf); }
	void decode_Ev_Gv_CL(const uint8_t *buf) { decode_Ev_Gv_Reg<operand_t::REG_CL>(buf); }

private:
	// 2 operand modes
	template <int index>
	void decode_ST_STi(const uint8_t *buf) { decode2<&instruction_t::template decode_Reg<operand_t::REG_ST>, &instruction_t::template decode_STi<index> >(buf); }

	template <int index>
	void decode_STi_ST(const uint8_t *buf) { decode2<&instruction_t::template decode_STi<index>, &instruction_t::template decode_Reg<operand_t::REG_ST> >(buf); }


	// macro for easier maintenance of these modes
	// only works for "simple" ones though (no template usage in params)
	#define DECODE2(op1, op2)                                                         \
	void decode_ ## op1 ## _ ## op2(const uint8_t *buf) {                             \
		decode2<&instruction_t::decode_ ## op1, &instruction_t::decode_ ## op2>(buf); \
	}

	DECODE2(Gq, Mo)
	DECODE2(Pd, Eq)
	DECODE2(Pd, Ed)
	DECODE2(Nq, Gq)
	DECODE2(Gq, Wo)
	DECODE2(Gd, Mo)
	DECODE2(Vo, Wd)
	DECODE2(Vo, Ww)
	DECODE2(Gq, Eb)
	DECODE2(Gq, Uo)
	DECODE2(Gq, Eq)
	DECODE2(Gd, Ev)
	DECODE2(Eq, Pd)
	DECODE2(Vd, Eq)
	DECODE2(Vq, Eq)
	DECODE2(Eq, Vo)
	DECODE2(Gq, Wd)
	DECODE2(Gq, Wq)
	DECODE2(Pq, Eq)
	DECODE2(Eq, Gq)
	DECODE2(Vo, Eq)
	DECODE2(Mv, Gv)
	DECODE2(Gv, Mv)
	DECODE2(Gd, Eb)
	DECODE2(Ev, Gv)
	DECODE2(Eb, Gb)
	DECODE2(Eb, Ib)
	DECODE2(Cd, Rd)
	DECODE2(Ed, Gd)
	DECODE2(Ed, Pd)
	DECODE2(Ed, Vo)
	DECODE2(Ev, Ib)
	DECODE2(Ev, Iz)
	DECODE2(Ev, Sw)
	DECODE2(Ew, Gw)
	DECODE2(Gb, Eb)
	DECODE2(Gd, Ed)
	DECODE2(Gd, Nq)
	DECODE2(Gd, Uo)
	DECODE2(Gd, Wd)
	DECODE2(Gd, Wq)
	DECODE2(Gv, Eb)
	DECODE2(Gv, Ev)
	DECODE2(Gv, Ew)
	DECODE2(Gv, M)
	DECODE2(Gv, Ma)
	DECODE2(Gv, Mp)
	DECODE2(Gz, Mp)
	DECODE2(Dd, Rd)
	DECODE2(Ib, rAX)
	DECODE2(Iw, Ib)
	DECODE2(Md, Gd)
	DECODE2(Mo, Vo)
	DECODE2(Mq, Pq)
	DECODE2(Mq, Vq)
	DECODE2(rAX, Ib)
	DECODE2(rAX, Iv)
	DECODE2(rAX, Iz)
	DECODE2(rAX, Ov)
	DECODE2(rBP, Iv)
	DECODE2(rBP, rAX)
	DECODE2(rBX, Iv)
	DECODE2(rBX, rAX)
	DECODE2(rCX, Iv)
	DECODE2(rCX, rAX)
	DECODE2(rDI, Iv)
	DECODE2(rDI, rAX)
	DECODE2(rDX, Iv)
	DECODE2(rDX, rAX)
	DECODE2(rAX, rAX)
	DECODE2(rSI, Iv)
	DECODE2(rSI, rAX)
	DECODE2(rSP, Iv)
	DECODE2(rSP, rAX)
	DECODE2(Ov, rAX)
	DECODE2(Nq, Ib)
	DECODE2(Pq, Ed)
	DECODE2(Pq, Nq)
	DECODE2(Pq, Qd)
	DECODE2(Pq, Qq)
	DECODE2(Pq, Uq)
	DECODE2(Pq, Wo)
	DECODE2(Pq, Wq)
	DECODE2(Qq, Pq)
	DECODE2(Rd, Cd)
	DECODE2(Rd, Dd)
	DECODE2(Sw, Ew)
	DECODE2(Uo, Ib)
	DECODE2(Vd, Ed)
	DECODE2(Vd, Wd)
	DECODE2(Vd, Wq)
	DECODE2(Vo, Ed)
	DECODE2(Vo, Mo)
	DECODE2(Vo, Mq)
	DECODE2(Vo, Nq)
	DECODE2(Vo, Qq)
	DECODE2(Vo, Uo)
	DECODE2(Vo, Wo)
	DECODE2(Gd, Wo)
	DECODE2(Vo, Wq)
	DECODE2(Vq, Ed)
	DECODE2(Vq, Mq)
	DECODE2(Vq, Wq)
	DECODE2(Wd, Vd)
	DECODE2(Wo, Vo)
	DECODE2(Wq, Vq)
	DECODE2(Qq, Wo)

	DECODE2(rCX, rAX_NOREX)
	DECODE2(rDX, rAX_NOREX)
	DECODE2(rBX, rAX_NOREX)
	DECODE2(rSP, rAX_NOREX)
	DECODE2(rBP, rAX_NOREX)
	DECODE2(rSI, rAX_NOREX)
	DECODE2(rDI, rAX_NOREX)
	DECODE2(eAX, Ib)
	DECODE2(Ib, eAX)
	DECODE2(eAX, DX)
	DECODE2(DX, eAX)

	DECODE2(Vo, Uo_Mq)
	DECODE2(Vo, Uo_Mw)
	DECODE2(Vo, Uo_Md)

	#undef DECODE2

	void decode_rAX_rAX_NOREX(const uint8_t *buf);

	void decode_AL_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_AL, &instruction_t::decode_Ib>(buf); }
	void decode_CL_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_CL, &instruction_t::decode_Ib>(buf); }
	void decode_DL_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_DL, &instruction_t::decode_Ib>(buf); }
	void decode_BL_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_BL, &instruction_t::decode_Ib>(buf); }
	void decode_AH_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_AH, &instruction_t::decode_Ib>(buf); }
	void decode_CH_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_CH, &instruction_t::decode_Ib>(buf); }
	void decode_DH_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_DH, &instruction_t::decode_Ib>(buf); }
	void decode_BH_Ib(const uint8_t *buf) { decode2<&instruction_t::decode_BH, &instruction_t::decode_Ib>(buf); }

	void decode_AL_DX(const uint8_t *buf)  { decode2<&instruction_t::template decode_Reg<operand_t::REG_AL>, &instruction_t::template decode_Reg<operand_t::REG_DX> >(buf); }
	void decode_AL_Ob(const uint8_t *buf)  { decode2<&instruction_t::template decode_Reg<operand_t::REG_AL>, &instruction_t::decode_Ob>(buf); }
	void decode_DX_AL(const uint8_t *buf)  { decode2<&instruction_t::template decode_Reg<operand_t::REG_DX>, &instruction_t::template decode_Reg<operand_t::REG_AL> >(buf); }
	void decode_DX_rAX(const uint8_t *buf) { decode2<&instruction_t::template decode_Reg<operand_t::REG_DX>, &instruction_t::decode_rAX >(buf); }
	void decode_Eb_1(const uint8_t *buf)   { decode2<&instruction_t::decode_Eb, &instruction_t::template decode_const_Ib<1> >(buf); }
	void decode_Eb_CL(const uint8_t *buf)  { decode2<&instruction_t::decode_Eb, &instruction_t::template decode_Reg<operand_t::REG_CL> >(buf); }
	void decode_Ev_1(const uint8_t *buf)   { decode2<&instruction_t::decode_Ev, &instruction_t::template decode_const_Id<1> >(buf); }
	void decode_Ev_CL(const uint8_t *buf)  { decode2<&instruction_t::decode_Ev, &instruction_t::template decode_Reg<operand_t::REG_CL> >(buf); }
	void decode_Ib_AL(const uint8_t *buf)  { decode2<&instruction_t::decode_Ib, &instruction_t::template decode_Reg<operand_t::REG_AL> >(buf); }
	void decode_Ob_AL(const uint8_t *buf)  { decode2<&instruction_t::decode_Ob, &instruction_t::template decode_Reg<operand_t::REG_AL> >(buf); }
	void decode_rAX_DX(const uint8_t *buf) { decode2<&instruction_t::decode_rAX, &instruction_t::template decode_Reg<operand_t::REG_DX> >(buf); }


private:

	// macro for easier maintenance of these modes
	// only works for "simple" ones though (no template usage in params)
	#define DECODE3(op1, op2, op3)                                                                                    \
	void decode_ ## op1 ## _ ## op2 ## _ ## op3(const uint8_t *buf) {                                                 \
		decode3<&instruction_t::decode_ ## op1, &instruction_t::decode_ ## op2, &instruction_t::decode_ ## op3>(buf); \
	}

	// 3 operand modes
	DECODE3(Ed, Vo, Ib)
	DECODE3(Eq, Vo, Ib)
	DECODE3(Ev, Gv, Ib)
	DECODE3(Gd, Nq, Ib)
	DECODE3(Gd, Uo, Ib)
	DECODE3(Gv, Ev, Ib)
	DECODE3(Gv, Ev, Iz)
	DECODE3(Pq, Qq, Ib)
	DECODE3(Vo, Ed, Ib)
	DECODE3(Vo, Eq, Ib)
	DECODE3(Vo, Mb, Ib)
	DECODE3(Vo, Md, Ib)
	DECODE3(Vo, Wo, Ib)

	DECODE3(Ev, Vo, Ib)

	DECODE3(Pq, Rd_Mw, Ib)
	DECODE3(Pq, Rq_Mw, Ib)
	DECODE3(Vo, Rd_Mw, Ib)
	DECODE3(Vo, Rq_Mw, Ib)

	DECODE3(Md_Gd, Vo, Ib)
	DECODE3(Rd_Mb, Vo, Ib)
	DECODE3(Rd_Mw, Vo, Ib)
	DECODE3(Rq_Mw, Vo, Ib)
	#undef DECODE3

	template <typename operand_t::Register REG>
	void decode_Ev_Gv_Reg(const uint8_t *buf) { decode3<&instruction_t::decode_Ev, &instruction_t::decode_Gv, &instruction_t::template decode_Reg<REG> >(buf); }

private:
	static typename operand_t::Register index_to_reg_invalid(uint8_t)    { return operand_t::REG_INVALID; }
	static typename operand_t::Register index_to_reg_8(uint8_t index)    { return static_cast<typename operand_t::Register>(operand_t::REG_AL + index); }
	static typename operand_t::Register index_to_reg_16(uint8_t index)   { return static_cast<typename operand_t::Register>(operand_t::REG_AX + index); }
	static typename operand_t::Register index_to_reg_32(uint8_t index)   { return static_cast<typename operand_t::Register>(operand_t::REG_EAX + index); }
	static typename operand_t::Register index_to_reg_64(uint8_t index)   { return static_cast<typename operand_t::Register>(operand_t::REG_RAX + index); }
	static typename operand_t::Register index_to_reg_seg(uint8_t index)  { return static_cast<typename operand_t::Register>(operand_t::REG_ES + index); }
	static typename operand_t::Register index_to_reg_mmx(uint8_t index)  { return static_cast<typename operand_t::Register>(operand_t::REG_MM0 + index); }
	static typename operand_t::Register index_to_reg_xmmx(uint8_t index) { return static_cast<typename operand_t::Register>(operand_t::REG_XMM0 + index); }
	static typename operand_t::Register index_to_reg_fpu(uint8_t index)  { return static_cast<typename operand_t::Register>(operand_t::REG_ST0 + index); }
	static typename operand_t::Register index_to_reg_cr(uint8_t index)   { return static_cast<typename operand_t::Register>(operand_t::REG_CR0 + index); }
	static typename operand_t::Register index_to_reg_dr(uint8_t index)   { return static_cast<typename operand_t::Register>(operand_t::REG_DR0 + index); }
	static typename operand_t::Register index_to_reg_tr(uint8_t index)   { return static_cast<typename operand_t::Register>(operand_t::REG_TR0 + index); }

public:
	Type type() const                                 { return opcode_->type; }
	address_t rva() const                             { return rva_; }
	bool valid() const                                { return type() != OP_INVALID; }
	const operand_t &operand(std::size_t index) const { return operands_[index]; }
	const uint8_t *buffer() const                     { return buffer_; }
	operator void *() const                           { return reinterpret_cast<void *>(valid()); }
	std::string mnemonic() const                      { return opcode_->mnemonic; }
	uint32_t prefix() const                           { return prefix_; }
	uint32_t mandatory_prefix() const                 { return mandatory_prefix_; }
	unsigned int operand_count() const                { return operand_count_; }
	unsigned int prefix_size() const                  { return prefix_size_; }
	unsigned int size() const                         { return prefix_size_ + rex_size_ + opcode_size_ + modrm_size_ + sib_size_ + disp_size_ + immediate_size_; }

private:
	int operand_size() const;

private:
	struct opcode_entry {
		const char  *mnemonic;
		decoder_t    decoder;
		Type         type;
		unsigned int flags;
		int          operand_count;
	};

private:
	static const opcode_entry Opcodes_x87_Lo[64];
	static const opcode_entry Opcodes_x87_Hi[0x200];

	static const opcode_entry Opcodes[0x100];
	static const opcode_entry Opcodes_2Byte_NA[0x100];
	static const opcode_entry Opcodes_2Byte_66[0x100];
	static const opcode_entry Opcodes_2Byte_F2[0x100];
	static const opcode_entry Opcodes_2Byte_F3[0x100];
	static const opcode_entry Opcodes_3Byte_38_NA[0x100];
	static const opcode_entry Opcodes_3Byte_38_66[0x100];
	static const opcode_entry Opcodes_3Byte_38_F2[0x100];
	static const opcode_entry Opcodes_3Byte_3A_NA[0x100];
	static const opcode_entry Opcodes_3Byte_3A_66[0x100];

private:
	static const opcode_entry Opcodes_Group1[32];
	static const opcode_entry Opcodes_Group2[16];
	static const opcode_entry Opcodes_Group2D[32];
	static const opcode_entry Opcodes_Group3[16];
	static const opcode_entry Opcodes_Group4[8];
	static const opcode_entry Opcodes_Group5[8];
	static const opcode_entry Opcodes_Group6[8];
	static const opcode_entry Opcodes_Group7[8];
	static const opcode_entry Opcodes_Group7A[64];
	static const opcode_entry Opcodes_Group8[8];
	static const opcode_entry Opcodes_Group9[8];
	static const opcode_entry Opcodes_Group9_66[8];
	static const opcode_entry Opcodes_Group9_F3[8];
	static const opcode_entry Opcodes_Group10[8];
	static const opcode_entry Opcodes_Group11[8];
	static const opcode_entry Opcodes_Group12[16];
	static const opcode_entry Opcodes_Group13[8];
	static const opcode_entry Opcodes_Group13_66[8];
	static const opcode_entry Opcodes_Group14[8];
	static const opcode_entry Opcodes_Group14_66[8];
	static const opcode_entry Opcodes_Group15[8];
	static const opcode_entry Opcodes_Group15_66[8];
	static const opcode_entry Opcodes_Group16_Reg[8];
	static const opcode_entry Opcodes_Group16_Mem[8];
	static const opcode_entry Opcodes_Group17[64];

private:
	// other
	static const opcode_entry Opcodes_nop_pause_xchg[3];
	static const opcode_entry Opcodes_cbw_cwde_cdqe[3];
	static const opcode_entry Opcodes_cwd_cdq_cqo[3];
	static const opcode_entry Opcodes_stosw_stosd_stosq[3];
	static const opcode_entry Opcodes_lodsw_lodsd_lodsq[3];
	static const opcode_entry Opcodes_scasw_scasd_scasq[3];
	static const opcode_entry Opcodes_iretw_iret_iretq[3];
	static const opcode_entry Opcodes_movsw_movsd_movsq[3];
	static const opcode_entry Opcodes_popfw_popfd_popfq[3];
	static const opcode_entry Opcodes_pushfw_pushfd_pushfq[3];
	static const opcode_entry Opcodes_invalid_cmpxchg8b_cmpxchg16b[3];
	static const opcode_entry Opcodes_insw_insd_invalid[3];
	static const opcode_entry Opcodes_outsw_outsd_invalid[3];
	static const opcode_entry Opcodes_cmpsw_cmpsd_cmpsq[3];
	static const opcode_entry Opcodes_pushaw_pushad_invalid[3];
	static const opcode_entry Opcodes_popaw_popad_invalid[3];

private:
	static const opcode_entry Opcode_invalid;

private:
	operand_t           operands_[MAX_OPERANDS];
	address_t           rva_;
	const uint8_t      *buffer_;
	std::size_t         buffer_size_;
	const opcode_entry *opcode_;

	uint32_t            prefix_;
	uint32_t            mandatory_prefix_;
	uint8_t             operand_count_;
	uint8_t             modrm_size_;
	uint8_t             sib_size_;
	uint8_t             disp_size_;
	uint8_t             prefix_size_;
	uint8_t             immediate_size_;
	uint8_t             opcode_size_;
	REX                 rex_byte_;
	uint8_t             rex_size_;
	bool                private_buffer_;
};

#endif
