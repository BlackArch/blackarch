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

#ifndef INSTRUCTION_20080314_TCC_
#define INSTRUCTION_20080314_TCC_

#include <locale>
#include <iostream>
#include <cassert>

#include "OPTable_FPU.tcc"
#include "OPTable_1byte.tcc"
#include "OPTable_2byte.tcc"
#include "OPTable_3byte.tcc"
#include "OPTable_Group1.tcc"
#include "OPTable_Group2.tcc"
#include "OPTable_Group3.tcc"
#include "OPTable_Group4.tcc"
#include "OPTable_Group5.tcc"
#include "OPTable_Group6.tcc"
#include "OPTable_Group7.tcc"
#include "OPTable_Group8.tcc"
#include "OPTable_Group9.tcc"
#include "OPTable_Group10.tcc"
#include "OPTable_Group11.tcc"
#include "OPTable_Group12.tcc"
#include "OPTable_Group13.tcc"
#include "OPTable_Group14.tcc"
#include "OPTable_Group15.tcc"
#include "OPTable_Group16.tcc"
#include "OPTable_Group17.tcc"
#include "OPTable_Other.tcc"
#include "ModRM.h"
#include "SIB.h"
#include "REX.h"
#include "Operand.h"

//------------------------------------------------------------------------------
// Name: decode_Gx(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_Gx(const uint8_t *buf) {

	const ModRM rm = get_modrm(buf);

	int reg_index = rm.reg();
	if(BITS == 64 && rex_byte_.is_rex()) {
		reg_index |= (rex_byte_.r() << 3);

		if(REG_DECODE == &index_to_reg_8) {
			if(reg_index > 3 && reg_index < 8) {
				reg_index += 12;
			}
		}
	}

	operand_t &operand = next_operand();
	operand.u.reg      = (*REG_DECODE)(reg_index);
	operand.type_      = operand_t::TYPE_REGISTER;
}

//------------------------------------------------------------------------------
// Name: decode_STi(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <int index>
void Instruction<M>::decode_STi(const uint8_t *buf) {
	UNUSED(buf);

	operand_t &operand = next_operand();
	operand.u.reg      = index_to_reg_fpu(index);
	operand.type_      = operand_t::TYPE_REGISTER;
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_0_16(const uint8_t *buf, const ModRM &rm, operand_t &operand)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_0_16(const uint8_t *buf, const ModRM &rm, operand_t &operand) {

	operand.type_                          = TYPE;
	operand.u.expression.scale             = 1;
	operand.u.expression.displacement_type = operand_t::DISP_NONE;

	switch(rm.rm()) {
	case 0x00:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	case 0x01:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	case 0x02:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x03:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x04:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_NULL;
		break;
	case 0x05:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_NULL;
		break;
	case 0x06:
		operand.u.expression.index             = operand_t::REG_NULL;
		operand.u.expression.base              = operand_t::REG_NULL;
		operand.u.expression.s_disp16          = get_displacement<int16_t>(buf);
		operand.u.expression.displacement_type = operand_t::DISP_S16;
		break;
	case 0x07:
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_1_16(const uint8_t *buf, const ModRM &rm, operand_t &operand)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_1_16(const uint8_t *buf, const ModRM &rm, operand_t &operand) {

	operand.type_                          = TYPE;
	operand.u.expression.scale             = 1;
	operand.u.expression.s_disp8           = get_displacement<int8_t>(buf);
	operand.u.expression.displacement_type = operand_t::DISP_S8;

	switch(rm.rm()) {
	case 0x00:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	case 0x01:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	case 0x02:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x03:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x04:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_NULL;
		break;
	case 0x05:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_NULL;
		break;
	case 0x06:
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x07:
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_2_16(const uint8_t *buf, const ModRM &rm, operand_t &operand)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_2_16(const uint8_t *buf, const ModRM &rm, operand_t &operand) {

	operand.type_                          = TYPE;
	operand.u.expression.scale             = 1;
	operand.u.expression.s_disp16          = get_displacement<int16_t>(buf);
	operand.u.expression.displacement_type = operand_t::DISP_S16;

	switch(rm.rm()) {
	case 0x00:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	case 0x01:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	case 0x02:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x03:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x04:
		operand.u.expression.index = operand_t::REG_SI;
		operand.u.expression.base  = operand_t::REG_NULL;
		break;
	case 0x05:
		operand.u.expression.index = operand_t::REG_DI;
		operand.u.expression.base  = operand_t::REG_NULL;
		break;
	case 0x06:
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.base  = operand_t::REG_BP;
		break;
	case 0x07:
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.base  = operand_t::REG_BX;
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_Reg(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Register REG>
void Instruction<M>::decode_Reg(const uint8_t *buf) {
	UNUSED(buf);

	operand_t &operand = next_operand();

	operand.u.reg = REG;
	operand.type_ = operand_t::TYPE_REGISTER;
}

//------------------------------------------------------------------------------
// Name: decode_const_Iq(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <int64_t IMM>
void Instruction<M>::decode_const_Iq(const uint8_t *buf) {
	UNUSED(buf);

	operand_t &operand = next_operand();

	operand.u.sqword = IMM;
	operand.type_    = operand_t::TYPE_IMMEDIATE64;

	// NOTE: so far this is only used for implicit constants, other
	// ones may need a size!
	//immediate_size_ += sizeof(int64_t);
}

//------------------------------------------------------------------------------
// Name: decode_const_Id(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <int32_t IMM>
void Instruction<M>::decode_const_Id(const uint8_t *buf) {
	UNUSED(buf);

	operand_t &operand = next_operand();

	operand.u.sdword = IMM;
	operand.type_    = operand_t::TYPE_IMMEDIATE32;

	// NOTE: so far this is only used for implicit constants, other
	// ones may need a size!
	//immediate_size_ += sizeof(int32_t);
}

//------------------------------------------------------------------------------
// Name: decode_const_Iw(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <int16_t IMM>
void Instruction<M>::decode_const_Iw(const uint8_t *buf) {
	UNUSED(buf);
	UNUSED(size);

	operand_t &operand = next_operand();

	operand.u.sword = IMM;
	operand.type_   = operand_t::TYPE_IMMEDIATE16;

	// NOTE: so far this is only used for implicit constants, other
	// ones may need a size!
	//immediate_size_ += sizeof(int16_t);
}

//------------------------------------------------------------------------------
// Name: decode_const_Ib(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <int8_t IMM>
void Instruction<M>::decode_const_Ib(const uint8_t *buf) {
	UNUSED(buf);

	operand_t &operand = next_operand();

	operand.u.sbyte = IMM;
	operand.type_   = operand_t::TYPE_IMMEDIATE8;

	// NOTE: so far this is only used for implicit constants, other
	// ones may need a size!
	//immediate_size_ += sizeof(int8_t);
}

//------------------------------------------------------------------------------
// Name: get_modrm(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
ModRM Instruction<M>::get_modrm(const uint8_t *buf) {
	// we got enough room for the ModRM byte?
	if(modrm_size_ == 0) {
		bounds_check(size() + 1);
	}

	modrm_size_ = 1;
	return ModRM(buf[opcode_size_]);
}


//------------------------------------------------------------------------------
// Name: get_sib(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
SIB Instruction<M>::get_sib(const uint8_t *buf) {
	// we got enough room for the SIB byte?
	if(sib_size_ == 0) {
		bounds_check(size() + 1);
	}

	sib_size_ = 1;
	return SIB(buf[opcode_size_ + modrm_size_]);
}

//------------------------------------------------------------------------------
// Name: get_immediate(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <class T>
T Instruction<M>::get_immediate(const uint8_t *buf) {
	bounds_check(size() + sizeof(T));

	const T ret = *reinterpret_cast<const T *>(&buf[opcode_size_ + modrm_size_ + sib_size_ + disp_size_ + immediate_size_]);
	immediate_size_ += sizeof(T);
	return ret;
}

//------------------------------------------------------------------------------
// Name: get_displacement(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <class T>
T Instruction<M>::get_displacement(const uint8_t *buf) {

	bounds_check(size() + sizeof(T));

	// there should only every be one displacement value!
	if(disp_size_ != 0) {
		throw edisassm::multiple_displacements(size());
	}

	const T ret = *reinterpret_cast<const T *>(&buf[opcode_size_ + modrm_size_ + sib_size_ + disp_size_]);
	disp_size_ = sizeof(T);
	return ret;
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_0_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_0_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit) {

	UNUSED(enable_64_bit);

	operand.type_ = TYPE;

	if(rm.rm() == 0x04) {
		const SIB sib = get_sib(buf);

		int sib_index = sib.index();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			sib_index |= (rex_byte_.x() << 3);
		}

		if(sib_index != 0x04) {

			if(BITS == 64 && enable_64_bit) {
				operand.u.expression.index = index_to_reg_64(sib_index);
			} else {
				operand.u.expression.index = index_to_reg_32(sib_index);
			}


			operand.u.expression.scale = 1 << sib.scale();
		} else {
			operand.u.expression.index = operand_t::REG_NULL;
			operand.u.expression.scale = 1;
		}

		if(sib.base() == 0x05) {

			// we could only get here if modrm.mod == 0x00
			// so it is always sdword
			operand.u.expression.base              = operand_t::REG_NULL;
			operand.u.expression.s_disp32          = get_displacement<int32_t>(buf);
			operand.u.expression.displacement_type = operand_t::DISP_S32;

		} else {

			int sibbase = sib.base();

			if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
				sibbase |= (rex_byte_.b() << 3);
			}

			if(BITS == 64 && enable_64_bit) {
				operand.u.expression.base = index_to_reg_64(sibbase);
			} else {
				operand.u.expression.base = index_to_reg_32(sibbase);
			}
			operand.u.expression.displacement_type = operand_t::DISP_NONE;
		}
	} else if(rm.rm() == 0x05) {
		if(BITS == 64 && enable_64_bit) {
			operand.u.expression.index             = operand_t::REG_NULL;
			operand.u.expression.scale             = 1;
			operand.u.expression.base              = operand_t::REG_RIP;
			operand.u.expression.s_disp32          = get_displacement<int32_t>(buf);
			operand.u.expression.displacement_type = operand_t::DISP_S32;
		} else {
			operand.u.expression.index             = operand_t::REG_NULL;
			operand.u.expression.scale             = 1;
			operand.u.expression.base              = operand_t::REG_NULL;
			operand.u.expression.s_disp32          = get_displacement<int32_t>(buf);
			operand.u.expression.displacement_type = operand_t::DISP_S32;
		}
	} else {
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.scale = 1;

		int rmbase = rm.rm();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			rmbase |= (rex_byte_.b() << 3);
		}

		if(BITS == 64 && enable_64_bit) {
			operand.u.expression.base = index_to_reg_64(rmbase);
		} else {
			operand.u.expression.base = index_to_reg_32(rmbase);
		}
		operand.u.expression.s_disp8           = 0;
		operand.u.expression.displacement_type = operand_t::DISP_NONE;
	}
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_1_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_1_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit) {

	UNUSED(enable_64_bit);

	if(rm.rm() == 0x04) {

		const SIB sib = get_sib(buf);

		int sibbase = sib.base();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			sibbase |= (rex_byte_.b() << 3);
		}

		if(BITS == 64 && enable_64_bit) {
			operand.u.expression.base = index_to_reg_64(sibbase);
		} else {
			operand.u.expression.base = index_to_reg_32(sibbase);
		}
		operand.u.expression.scale = 1 << sib.scale();

		int sib_index = sib.index();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			sib_index |= (rex_byte_.x() << 3);
		}

		if(sib_index != 0x04) {
			if(BITS == 64 && enable_64_bit) {
				operand.u.expression.index = index_to_reg_64(sib_index);
			} else {
				operand.u.expression.index = index_to_reg_32(sib_index);
			}
		} else {
			operand.u.expression.index = operand_t::REG_NULL;
		}
	} else {
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.scale = 1;

		int rmbase = rm.rm();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			rmbase |= (rex_byte_.b() << 3);
		}

		if(BITS == 64 && enable_64_bit) {
			operand.u.expression.base = index_to_reg_64(rmbase);
		} else {
			operand.u.expression.base = index_to_reg_32(rmbase);
		}

	}

	operand.type_                          = TYPE;
	operand.u.expression.s_disp8           = get_displacement<int8_t>(buf);
	operand.u.expression.displacement_type = operand_t::DISP_S8;
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_2_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_2_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit) {

	UNUSED(enable_64_bit);

	if(rm.rm() == 0x04) {
		const SIB sib = get_sib(buf);

		int sibbase = sib.base();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			sibbase |= (rex_byte_.b() << 3);
		}

		if(BITS == 64 && enable_64_bit) {
			operand.u.expression.base = index_to_reg_64(sibbase);
		} else {
			operand.u.expression.base = index_to_reg_32(sibbase);
		}

		operand.u.expression.scale = 1 << sib.scale();

		int sib_index = sib.index();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			sib_index |= (rex_byte_.x() << 3);
		}

		if(sib_index != 0x04) {
			if(BITS == 64 && enable_64_bit) {
				operand.u.expression.index = index_to_reg_64(sib_index);
			} else {
				operand.u.expression.index = index_to_reg_32(sib_index);
			}

		} else {
			operand.u.expression.index = operand_t::REG_NULL;
		}
	} else {
		operand.u.expression.index = operand_t::REG_NULL;
		operand.u.expression.scale = 1;

		int rmbase = rm.rm();

		if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
			rmbase |= (rex_byte_.b() << 3);
		}

		if(BITS == 64 && enable_64_bit) {
			operand.u.expression.base = index_to_reg_64(rmbase);
		} else {
			operand.u.expression.base = index_to_reg_32(rmbase);
		}

	}

	operand.type_                          = TYPE;
	operand.u.expression.s_disp32          = get_displacement<int32_t>(buf);
	operand.u.expression.displacement_type = operand_t::DISP_S32;
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_3_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_3_32(const uint8_t *buf, const ModRM &rm, operand_t &operand, bool enable_64_bit) {
	UNUSED(buf);
	UNUSED(enable_64_bit);

	int rmbase = rm.rm();

	if(BITS == 64 && rex_byte_.is_rex() && enable_64_bit) {
		rmbase |= (rex_byte_.b() << 3);
		if(REG_DECODE == &index_to_reg_8) {
			if(rmbase > 3 && rmbase < 8) {
				rmbase += 12;
			}
		}
	}

	operand.u.reg = (*REG_DECODE)(rmbase);
	operand.type_ = operand_t::TYPE_REGISTER;
}

//------------------------------------------------------------------------------
// Name: decode_ModRM_Invalid(const uint8_t *buf, const ModRM &rm, operand_t &operand)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_ModRM_Invalid(const uint8_t *buf, const ModRM &rm, operand_t &operand) {
	UNUSED(rm);
	UNUSED(operand);
	UNUSED(buf);
	throw edisassm::invalid_operand(size());
}

//------------------------------------------------------------------------------
// Name: decode_Ex(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
template <typename Operand<M>::Type TYPE, typename Operand<M>::Register (*REG_DECODE)(uint8_t)>
void Instruction<M>::decode_Ex(const uint8_t *buf) {

	const ModRM rm = get_modrm(buf);

	operand_t &operand = next_operand();

	if(prefix_ & PREFIX_ADDRESS) {
		if(BITS == 64) {
			switch(rm.mod()) {
			case 0x00:
				decode_ModRM_0_32<operand_t::TYPE_EXPRESSION32, REG_DECODE>(buf, rm, operand, false);
				break;
			case 0x01:
				decode_ModRM_1_32<operand_t::TYPE_EXPRESSION32, REG_DECODE>(buf, rm, operand, false);
				break;
			case 0x02:
				decode_ModRM_2_32<operand_t::TYPE_EXPRESSION32, REG_DECODE>(buf, rm, operand, false);
				break;
			case 0x03:
				decode_ModRM_3_32<operand_t::TYPE_EXPRESSION32, REG_DECODE>(buf, rm, operand, false);
				break;
			}
		} else {
			switch(rm.mod()) {
			case 0x00:
				decode_ModRM_0_16<TYPE, REG_DECODE>(buf, rm, operand);
				break;
			case 0x01:
				decode_ModRM_1_16<TYPE, REG_DECODE>(buf, rm, operand);
				break;
			case 0x02:
				decode_ModRM_2_16<TYPE, REG_DECODE>(buf, rm, operand);
				break;
			case 0x03:
				// same as in 32-bit mode!
				decode_ModRM_3_32<TYPE, REG_DECODE>(buf, rm, operand);
				break;
			}
		}
	} else {
		switch(rm.mod()) {
		case 0x00:
			decode_ModRM_0_32<TYPE, REG_DECODE>(buf, rm, operand);
			break;
		case 0x01:
			decode_ModRM_1_32<TYPE, REG_DECODE>(buf, rm, operand);
			break;
		case 0x02:
			decode_ModRM_2_32<TYPE, REG_DECODE>(buf, rm, operand);
			break;
		case 0x03:
			decode_ModRM_3_32<TYPE, REG_DECODE>(buf, rm, operand);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: Instruction(const uint8_t *buf, std::size_t size, address_t rva, const std::nothrow_t&) throw()
//------------------------------------------------------------------------------
template <class M>
Instruction<M>::Instruction(const uint8_t *buf, std::size_t size, address_t rva, const std::nothrow_t&) throw()
		: rva_(rva), buffer_(buf), buffer_size_(size), opcode_(&Opcode_invalid),
		prefix_(0), mandatory_prefix_(0), operand_count_(0), modrm_size_(0),
		sib_size_(0), disp_size_(0), prefix_size_(0), immediate_size_(0),
		opcode_size_(0), rex_byte_(0), rex_size_(0), private_buffer_(false) {

	try {
		initialize(buf, size);
	} catch(const edisassm::invalid_instruction &) {
		opcode_ = &Opcode_invalid;
		//throw;
	}
}

//------------------------------------------------------------------------------
// Name: Instruction(const uint8_t *buf, std::size_t size, address_t rva)
//------------------------------------------------------------------------------
template <class M>
Instruction<M>::Instruction(const uint8_t *buf, std::size_t size, address_t rva)
		: rva_(rva), buffer_(buf), buffer_size_(size), opcode_(&Opcode_invalid),
		prefix_(0), mandatory_prefix_(0), operand_count_(0), modrm_size_(0),
		sib_size_(0), disp_size_(0), prefix_size_(0), immediate_size_(0),
		opcode_size_(0), rex_byte_(0), rex_size_(0), private_buffer_(false) {

	initialize(buf, size);
}

//------------------------------------------------------------------------------
// Name: ~Instruction()
//------------------------------------------------------------------------------
template <class M>
Instruction<M>::~Instruction() {
	if(private_buffer_) {
		delete [] buffer_;
	}
}

//------------------------------------------------------------------------------
// Name: initialize(const uint8_t *buf, std::size_t size)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::initialize(const uint8_t *buf, std::size_t n) {

	for(int i = 0; i < M::MAX_OPERANDS; ++i) {
		operands_[i].invalidate();
	}

	process_prefixes(buf, n);


	// is there any space left for the opcode/operands?
	bounds_check(size() + 1);
	

	// find the entry in the table
	opcode_      = &Opcodes[*buf];
	opcode_size_ = 1;

	// decode it
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: Instruction(const Instruction &other)
//------------------------------------------------------------------------------
template <class M>
Instruction<M>::Instruction(const Instruction &other) :
		rva_(other.rva_),
		opcode_(other.opcode_),
		prefix_(other.prefix_),
		mandatory_prefix_(other.mandatory_prefix_),
		operand_count_(other.operand_count_),
		modrm_size_(other.modrm_size_),
		sib_size_(other.sib_size_),
		disp_size_(other.disp_size_),
		prefix_size_(other.prefix_size_),
		immediate_size_(other.immediate_size_),
		opcode_size_(other.opcode_size_),
		rex_byte_(other.rex_byte_),
		rex_size_(other.rex_size_),
		private_buffer_(true) {

	using std::memcpy;

	for(int i = 0; i < M::MAX_OPERANDS; ++i) {
		operands_[i] = other.operands_[i];
		operands_[i].set_owner(this);
	}

	buffer_size_ = other.size();
	uint8_t *const buffer = new uint8_t[buffer_size_];
	memcpy(buffer, other.buffer_, buffer_size_);
	buffer_ = buffer;
}

//------------------------------------------------------------------------------
// Name: operator=(const Instruction &rhs)
//------------------------------------------------------------------------------
template <class M>
Instruction<M> &Instruction<M>::operator=(const Instruction &rhs) {
	if(this != &rhs) {
		Instruction(rhs).swap(*this);
	}
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap(Instruction &other)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::swap(Instruction &other) {
	using std::swap;

	for(int i = 0; i < MAX_OPERANDS; ++i) {
		operands_[i].swap(other.operands_[i]);
	}

	swap(rva_, other.rva_);
	swap(buffer_, other.buffer_);
	swap(buffer_size_, other.buffer_size_);
	swap(opcode_, other.opcode_);
	swap(prefix_, other.prefix_);
	swap(mandatory_prefix_, other.mandatory_prefix_);
	swap(operand_count_, other.operand_count_);
	swap(modrm_size_, other.modrm_size_);
	swap(sib_size_, other.sib_size_);
	swap(disp_size_, other.disp_size_);
	swap(prefix_size_, other.prefix_size_);
	swap(immediate_size_, other.immediate_size_);
	swap(opcode_size_, other.opcode_size_);
	swap(rex_byte_, other.rex_byte_);
	swap(rex_size_, other.rex_size_);
	swap(private_buffer_, other.private_buffer_);
}

//------------------------------------------------------------------------------
// Name: process_prefixes(const uint8_t *&buf, std::size_t size)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::process_prefixes(const uint8_t *&buf, std::size_t size) {

	bool done = false;

	// we only allow one prefix from each group to be set,
	// currently, the last one from a given group in the stream
	// will take precedence
	do {
		if(size == 0) {
			break;
		}

		switch(*buf) {
		// group1
		case 0xf0:
			prefix_ = (prefix_ & 0xffffff00) | PREFIX_LOCK;
			break;
		case 0xf2:
			prefix_ = (prefix_ & 0xffffff00) | PREFIX_REPNE;
			break;
		case 0xf3:
			prefix_ = (prefix_ & 0xffffff00) | PREFIX_REP;
			break;
		// group2
		case 0x2e:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_CS;
			break;
		case 0x36:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_SS;
			break;
		case 0x3e:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_DS;
			break;
		case 0x26:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_ES;
			break;
		case 0x64:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_FS;
			break;
		case 0x65:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_GS;
			break;
		#if 0
		case 0x2e:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_BRANCH_NOT_TAKEN;
			break;
		case 0x3e:
			prefix_ = (prefix_ & 0xffff00ff) | PREFIX_BRANCH_TAKEN;
			break;
		#endif
		// group3
		case 0x66:
			prefix_ = (prefix_ & 0xff00ffff) | PREFIX_OPERAND;
			break;
		// group4
		case 0x67:
			prefix_ = (prefix_ & 0x00ffffff) | PREFIX_ADDRESS;
			break;
		default:
			done = true;
			break;
		}

		// this if will get merged with the while's condition if the optimiser
		// is smart
		if(!done) {
			++buf;
			++prefix_size_;
			--size;
		}
	} while(!done);

	if(BITS == 64) {
		if(size != 0) {
			rex_byte_ = *buf;
			if(rex_byte_.is_rex()) {
				++buf;
				++rex_size_;
				--size;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: next_operand()
//------------------------------------------------------------------------------
template <class M>
typename Instruction<M>::operand_t &Instruction<M>::next_operand() {

	if(operand_count_ >= MAX_OPERANDS) {
		throw edisassm::too_many_operands(size());
	}

	operand_t &ret = operands_[operand_count_++];
	ret.set_owner(this);
	return ret;
}

//------------------------------------------------------------------------------
// Name: decode_invalid(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_invalid(const uint8_t *buf) {
	UNUSED(buf);
	throw edisassm::invalid_instruction(Instruction<M>::size());
}

template <class M> void Instruction<M>::decode_cbw_cwde_cdqe(const uint8_t *buf)                { decode_size_sensitive(buf, Opcodes_cbw_cwde_cdqe); }
template <class M> void Instruction<M>::decode_cwd_cdq_cqo(const uint8_t *buf)                  { decode_size_sensitive(buf, Opcodes_cwd_cdq_cqo);}
template <class M> void Instruction<M>::decode_stosw_stosd_stosq(const uint8_t *buf)            { decode_size_sensitive(buf, Opcodes_stosw_stosd_stosq);}
template <class M> void Instruction<M>::decode_lodsw_lodsd_lodsq(const uint8_t *buf)            { decode_size_sensitive(buf, Opcodes_lodsw_lodsd_lodsq);}
template <class M> void Instruction<M>::decode_scasw_scasd_scasq(const uint8_t *buf)            { decode_size_sensitive(buf, Opcodes_scasw_scasd_scasq);}
template <class M> void Instruction<M>::decode_iretw_iret_iretq(const uint8_t *buf)             { decode_size_sensitive(buf, Opcodes_iretw_iret_iretq);}
template <class M> void Instruction<M>::decode_movsw_movsd_movsq(const uint8_t *buf)            { decode_size_sensitive(buf, Opcodes_movsw_movsd_movsq);}
template <class M> void Instruction<M>::decode_popfw_popfd_popfq(const uint8_t *buf)            { decode_size_sensitive(buf, Opcodes_popfw_popfd_popfq);}
template <class M> void Instruction<M>::decode_pushfw_pushfd_pushfq(const uint8_t *buf)         { decode_size_sensitive(buf, Opcodes_pushfw_pushfd_pushfq);}
template <class M> void Instruction<M>::decode_invalid_cmpxchg8b_cmpxchg16b(const uint8_t *buf) { decode_size_sensitive(buf, Opcodes_invalid_cmpxchg8b_cmpxchg16b);}
template <class M> void Instruction<M>::decode_insw_insd_invalid(const uint8_t *buf)            { decode_size_sensitive(buf, Opcodes_insw_insd_invalid);}
template <class M> void Instruction<M>::decode_outsw_outsd_invalid(const uint8_t *buf)          { decode_size_sensitive(buf, Opcodes_outsw_outsd_invalid);}
template <class M> void Instruction<M>::decode_cmpsw_cmpsd_cmpsq(const uint8_t *buf)            { decode_size_sensitive(buf, Opcodes_cmpsw_cmpsd_cmpsq);}
template <class M> void Instruction<M>::decode_pushaw_pushad_invalid(const uint8_t *buf)        { decode_size_sensitive(buf, Opcodes_pushaw_pushad_invalid);}
template <class M> void Instruction<M>::decode_popaw_popad_invalid(const uint8_t *buf)          { decode_size_sensitive(buf, Opcodes_popaw_popad_invalid);}


//------------------------------------------------------------------------------
// Name: wait_feni_fdisi_finit_fclex(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::wait_feni_fdisi_finit_fclex(const uint8_t *buf) {
	
	
	static const opcode_entry Opcodes_wait_finit_fclex[5] = {
		{ "feni", &Instruction::decode0,  OP_FENI, FLAG_NONE, 0 },
		{ "fdisi", &Instruction::decode0, OP_FDISI, FLAG_NONE, 0 },
		{ "fclex", &Instruction::decode0, OP_FCLEX, FLAG_NONE, 0 },
		{ "finit", &Instruction::decode0, OP_FINIT, FLAG_NONE, 0 },
		{ "wait", &Instruction::decode0,  OP_WAIT, FLAG_NONE, 0 },
	};
		
	if(buf[0] != 0x9b) {
		throw edisassm::invalid_instruction(size());
	}
	
	opcode_ = &Opcodes_wait_finit_fclex[4];
	opcode_size_ = 1;
	
	if(size() + 2 <= buffer_size_) {	
		if(buf[1] == 0xdb) {
			switch(buf[2]) {
			case 0xe0:
				opcode_ = &Opcodes_wait_finit_fclex[0];
				opcode_size_ = 3;
				break;
			case 0xe1:
				opcode_ = &Opcodes_wait_finit_fclex[1];
				opcode_size_ = 3;
				break;
			case 0xe2:
				opcode_ = &Opcodes_wait_finit_fclex[2];
				opcode_size_ = 3;
				break;
			case 0xe3:
				opcode_ = &Opcodes_wait_finit_fclex[3];
				opcode_size_ = 3;
				break;
			}
		}
	}
}


//------------------------------------------------------------------------------
// Name: decode_x87(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_x87(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	const uint8_t esc_num = (buf[0] - 0xd8);
	const uint8_t byte2 = buf[opcode_size_];

	if((byte2 & 0xc0) != 0xc0) {
		opcode_ = &Opcodes_x87_Lo[modrm.reg() + esc_num * 8];
	} else {
		opcode_ = &Opcodes_x87_Hi[(byte2 & 0x3f) + esc_num * 64];
	}

	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_2byte(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_2byte(const uint8_t *buf) {

	// we got enough room for next byte?
	bounds_check(size() + 1);

	opcode_size_ = 2;

	const uint8_t index = buf[opcode_size_ - 1];

	if(prefix_ & PREFIX_OPERAND) {
		// 0x66
		mandatory_prefix_ |= PREFIX_OPERAND;
		opcode_ = &Opcodes_2Byte_66[index];
	} else if(prefix_ & PREFIX_REPNE) {
		// 0xf2
		mandatory_prefix_ |= PREFIX_REPNE;
		opcode_ = &Opcodes_2Byte_F2[index];
	} else if(prefix_ & PREFIX_REP) {
		// 0xf3
		mandatory_prefix_ |= PREFIX_REP;
		opcode_ = &Opcodes_2Byte_F3[index];
	} else {
		// N/A
		opcode_ = &Opcodes_2Byte_NA[index];
	}

	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_3byte_38(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_3byte_38(const uint8_t *buf) {

	// we got enough room for next byte?
	bounds_check(size() + 1);

	opcode_size_ = 3;

	const uint8_t index = buf[opcode_size_ - 1];

	if(prefix_ & PREFIX_OPERAND) {
		// 0x66
		mandatory_prefix_ |= PREFIX_OPERAND;
		opcode_ = &Opcodes_3Byte_38_66[index];
	} else if(prefix_ & PREFIX_REPNE) {
		// 0xf2
		mandatory_prefix_ |= PREFIX_REPNE;
		opcode_ = &Opcodes_3Byte_38_F2[index];
	} else {
		// N/A
		opcode_ = &Opcodes_3Byte_38_NA[index];
	}

	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_3byte_3A(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_3byte_3A(const uint8_t *buf) {

	// we got enough room for next byte?
	bounds_check(size() + 1);

	opcode_size_ = 3;

	const uint8_t index = buf[opcode_size_ - 1];

	if(prefix_ & PREFIX_OPERAND) {
		// 0x66
		mandatory_prefix_ |= PREFIX_OPERAND;
		opcode_ = &Opcodes_3Byte_3A_66[index];
	} else {
		// N/A
		opcode_ = &Opcodes_3Byte_3A_NA[index];
	}

	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group1(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group1(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group1[modrm.reg() + 8 * (buf[0] & 3)];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group2(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group2(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group2[modrm.reg() + 8 * (buf[0] & 1)];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group2D(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group2D(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group2D[modrm.reg() + 8 * (buf[0] & 3)];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group3(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group3(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group3[modrm.reg() + 8 * ((buf[0] - 6) & 1)];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group4(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group4(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group4[modrm.reg()];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group5(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group5(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group5[modrm.reg()];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group6(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group6(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group6[modrm.reg()];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group7(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group7(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	const uint8_t index = modrm.reg();

	if(modrm.mod() == 0x03) {
		opcode_ = &Opcodes_Group7A[(index << 3) | modrm.rm()];
	} else {
		opcode_ = &Opcodes_Group7[index];
	}
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group8(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group8(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group8[modrm.reg()];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group9(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group9(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	const uint8_t index = modrm.reg();

	if(prefix_ & PREFIX_OPERAND) {
		// 0x66
		opcode_ = &Opcodes_Group9_66[index];
	} else if(prefix_ & PREFIX_REP) {
		// 0xf3
		opcode_ = &Opcodes_Group9_F3[index];
	} else {
		// N/A
		opcode_ = &Opcodes_Group9[index];
	}

	(this->*(opcode_->decoder))(buf);
}


//------------------------------------------------------------------------------
// Name: decode_group10(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group10(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group10[modrm.reg()];
	(this->*(opcode_->decoder))(buf);
}


//------------------------------------------------------------------------------
// Name: decode_group11(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group11(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group11[modrm.reg()];
	(this->*(opcode_->decoder))(buf);
}


//------------------------------------------------------------------------------
// Name: decode_group12(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group12(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group12[modrm.reg() + 8 * ((buf[0] - 6) & 1)];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group13(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group13(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	const uint8_t index = modrm.reg();

	if(prefix_ & PREFIX_OPERAND) {
		// 0x66
		opcode_ = &Opcodes_Group13_66[index];
	} else {
		// N/A
		opcode_ = &Opcodes_Group13[index];
	}

	(this->*(opcode_->decoder))(buf);
}


//------------------------------------------------------------------------------
// Name: decode_group14(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group14(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	const uint8_t index = modrm.reg();

	if(prefix_ & PREFIX_OPERAND) {
		// 0x66
		opcode_ = &Opcodes_Group14_66[index];
	} else {
		// N/A
		opcode_ = &Opcodes_Group14[index];
	}

	(this->*(opcode_->decoder))(buf);
}


//------------------------------------------------------------------------------
// Name: decode_group15(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group15(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	const uint8_t index = modrm.reg();

	if(prefix_ & PREFIX_OPERAND) {
		// 0x66
		opcode_ = &Opcodes_Group15_66[index];
	} else {
		// N/A
		opcode_ = &Opcodes_Group15[index];
	}

	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group16(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group16(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	const uint8_t index = modrm.reg();

	if(modrm.mod() == 0x03) {
		opcode_ = &Opcodes_Group16_Reg[index];
	} else {
		opcode_ = &Opcodes_Group16_Mem[index];
	}

	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_group17(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_group17(const uint8_t *buf) {

	const ModRM modrm = get_modrm(buf);

	opcode_ = &Opcodes_Group17[modrm.reg() + 8 * (buf[1] - 0x18)];
	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: decode_Ap(const uint8_t *buf)
// Desc: absolute pointer (32 or 48 bit)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Ap(const uint8_t *buf) {

	operand_t &operand = next_operand();
	operand.type_      = operand_t::TYPE_ABSOLUTE;

	if(prefix_ & PREFIX_OPERAND) {
		operand.u.absolute.offset = get_immediate<uint16_t>(buf);
	} else {
		operand.u.absolute.offset = get_immediate<uint32_t>(buf);
	}

	operand.u.absolute.seg = get_immediate<uint16_t>(buf);
}

//------------------------------------------------------------------------------
// Name: decode_Ib(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Ib(const uint8_t *buf) {
	operand_t &operand = next_operand();

	operand.u.sbyte = get_immediate<int8_t>(buf);
	operand.type_   = operand_t::TYPE_IMMEDIATE8;
}

//------------------------------------------------------------------------------
// Name: decode_Iw(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Iw(const uint8_t *buf) {
	operand_t &operand = next_operand();

	operand.u.sword = get_immediate<int16_t>(buf);
	operand.type_   = operand_t::TYPE_IMMEDIATE16;
}

//------------------------------------------------------------------------------
// Name: decode_Id(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Id(const uint8_t *buf) {
	operand_t &operand = next_operand();

	operand.u.sdword = get_immediate<int32_t>(buf);
	operand.type_    = operand_t::TYPE_IMMEDIATE32;
}

//------------------------------------------------------------------------------
// Name: decode_Iq(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Iq(const uint8_t *buf) {
	operand_t &operand = next_operand();

	operand.u.sqword = get_immediate<int64_t>(buf);
	operand.type_    = operand_t::TYPE_IMMEDIATE64;
}

//------------------------------------------------------------------------------
// Name: decode_Jb(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Jb(const uint8_t *buf) {

	operand_t &operand = next_operand();

	operand.u.sbyte = get_immediate<int8_t>(buf);
	operand.type_   = operand_t::TYPE_REL8;
}

//------------------------------------------------------------------------------
// Name: decode_Jw(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Jw(const uint8_t *buf) {

	operand_t &operand = next_operand();

	operand.u.sword = get_immediate<int16_t>(buf);
	operand.type_   = operand_t::TYPE_REL16;
}

//------------------------------------------------------------------------------
// Name: decode_Jd(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Jd(const uint8_t *buf) {

	operand_t &operand = next_operand();

	operand.u.sdword = get_immediate<int32_t>(buf);
	operand.type_    = operand_t::TYPE_REL32;
}

//------------------------------------------------------------------------------
// Name: decode_Jq(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Jq(const uint8_t *buf) {

	operand_t &operand = next_operand();

	operand.u.sqword = get_immediate<int64_t>(buf);
	operand.type_    = operand_t::TYPE_REL64;
}

//------------------------------------------------------------------------------
// Name: decode_Ev(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Ev(const uint8_t *buf) {

	switch(operand_size()) {
	case 16: decode_Ex<operand_t::TYPE_EXPRESSION16, &Instruction::index_to_reg_16>(buf); break;
	case 32: decode_Ex<operand_t::TYPE_EXPRESSION32, &Instruction::index_to_reg_32>(buf); break;
	case 64: decode_Ex<operand_t::TYPE_EXPRESSION64, &Instruction::index_to_reg_64>(buf); break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_Rv(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Rv(const uint8_t *buf) {
	switch(operand_size()) {
	case 16: decode_Ex<operand_t::TYPE_INVALID, &Instruction::index_to_reg_16>(buf); break;
	case 32: decode_Ex<operand_t::TYPE_INVALID, &Instruction::index_to_reg_32>(buf); break;
	case 64: decode_Ex<operand_t::TYPE_INVALID, &Instruction::index_to_reg_64>(buf); break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_Gv(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Gv(const uint8_t *buf) {

	switch(operand_size()) {
	case 16: decode_Gx<&Instruction::index_to_reg_16>(buf); break;
	case 32: decode_Gx<&Instruction::index_to_reg_32>(buf); break;
	case 64: decode_Gx<&Instruction::index_to_reg_64>(buf); break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_Iv(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Iv(const uint8_t *buf) {
	switch(operand_size()) {
	case 16: decode_Iw(buf); break;
	case 32: decode_Id(buf); break;
	case 64: decode_Iq(buf); break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_Ob(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Ob(const uint8_t *buf) {
	operand_t &operand = next_operand();

	operand.type_                          = operand_t::TYPE_EXPRESSION8;
	operand.u.expression.displacement_type = operand_t::DISP_S32;
	operand.u.expression.index             = operand_t::REG_NULL;
	operand.u.expression.base              = operand_t::REG_NULL;
	operand.u.expression.scale             = 1;
	operand.u.expression.s_disp32          = get_displacement<int32_t>(buf);
}

//------------------------------------------------------------------------------
// Name: decode_Ow(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Ow(const uint8_t *buf) {
	operand_t &operand = next_operand();

	operand.type_                          = operand_t::TYPE_EXPRESSION16;
	operand.u.expression.displacement_type = operand_t::DISP_S32;
	operand.u.expression.index             = operand_t::REG_NULL;
	operand.u.expression.base              = operand_t::REG_NULL;
	operand.u.expression.scale             = 1;
	operand.u.expression.s_disp32          = get_displacement<int32_t>(buf);
}

//------------------------------------------------------------------------------
// Name: decode_Od(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Od(const uint8_t *buf) {
	operand_t &operand = next_operand();

	operand.type_                          = operand_t::TYPE_EXPRESSION32;
	operand.u.expression.displacement_type = operand_t::DISP_S32;
	operand.u.expression.index             = operand_t::REG_NULL;
	operand.u.expression.base              = operand_t::REG_NULL;
	operand.u.expression.scale             = 1;
	operand.u.expression.s_disp32          = get_displacement<int32_t>(buf);
}

//------------------------------------------------------------------------------
// Name: decode_Ov(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Ov(const uint8_t *buf) {
	switch(operand_size()) {
	case 16: decode_Ow(buf); break;
	case 32: decode_Od(buf); break;
	case 64: throw edisassm::invalid_instruction(size()); break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_Mv(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_Mv(const uint8_t *buf) {
	switch(operand_size()) {
	case 16: decode_Mw(buf); break;
	case 32: decode_Md(buf); break;
	case 64: decode_Mq(buf); break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_AL(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_AL(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R8B>(buf);
	} else {
		decode_Reg<operand_t::REG_AL>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_CL(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_CL(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R9B>(buf);
	} else {
		decode_Reg<operand_t::REG_CL>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_DL(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_DL(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R10B>(buf);
	} else {
		decode_Reg<operand_t::REG_DL>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_BL(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_BL(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R11B>(buf);
	} else {
		decode_Reg<operand_t::REG_BL>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_AH(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_AH(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R12B>(buf);
	} else {
		decode_Reg<operand_t::REG_AH>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_CH(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_CH(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R13B>(buf);
	} else {
		decode_Reg<operand_t::REG_CH>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_DH(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_DH(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R14B>(buf);
	} else {
		decode_Reg<operand_t::REG_DH>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_BH(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_BH(const uint8_t *buf) {
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		decode_Reg<operand_t::REG_R15B>(buf);
	} else {
		decode_Reg<operand_t::REG_BH>(buf);
	}
}

//------------------------------------------------------------------------------
// Name: decode_rAX_NOREX(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rAX_NOREX(const uint8_t *buf) {
	switch(operand_size()) {;
	case 16: decode_Reg<operand_t::REG_AX>(buf);  break;
	case 32: decode_Reg<operand_t::REG_EAX>(buf); break;
	case 64: decode_Reg<operand_t::REG_RAX>(buf); break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_eAX(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_eAX(const uint8_t *buf) {
	// TODO: is this correct, it seems to be the same
	// because eAX is only used for ops where REX is illegal
	decode_rAX_NOREX(buf);
}

//------------------------------------------------------------------------------
// Name: decode_rAX(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rAX(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R8W>(buf);
		} else {
			decode_Reg<operand_t::REG_AX>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R8D>(buf);
		} else {
			decode_Reg<operand_t::REG_EAX>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R8>(buf);
		} else {
			decode_Reg<operand_t::REG_RAX>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rCX(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rCX(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R9W>(buf);
		} else {
			decode_Reg<operand_t::REG_CX>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R9D>(buf);
		} else {
			decode_Reg<operand_t::REG_ECX>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R9>(buf);
		} else {
			decode_Reg<operand_t::REG_RCX>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rDX(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rDX(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R10W>(buf);
		} else {
			decode_Reg<operand_t::REG_DX>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R10D>(buf);
		} else {
			decode_Reg<operand_t::REG_EDX>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R10>(buf);
		} else {
			decode_Reg<operand_t::REG_RDX>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rBX(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rBX(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R11W>(buf);
		} else {
			decode_Reg<operand_t::REG_BX>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R11D>(buf);
		} else {
			decode_Reg<operand_t::REG_EBX>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R11>(buf);
		} else {
			decode_Reg<operand_t::REG_RBX>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rSP(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rSP(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R12W>(buf);
		} else {
			decode_Reg<operand_t::REG_SP>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R12D>(buf);
		} else {
			decode_Reg<operand_t::REG_ESP>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R12>(buf);
		} else {
			decode_Reg<operand_t::REG_RSP>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rBP(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rBP(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R13W>(buf);
		} else {
			decode_Reg<operand_t::REG_BP>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R13D>(buf);
		} else {
			decode_Reg<operand_t::REG_EBP>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R13>(buf);
		} else {
			decode_Reg<operand_t::REG_RBP>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rSI(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rSI(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R14W>(buf);
		} else {
			decode_Reg<operand_t::REG_SI>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R14D>(buf);
		} else {
			decode_Reg<operand_t::REG_ESI>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R14>(buf);
		} else {
			decode_Reg<operand_t::REG_RSI>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rDI(const uint8_t *buf)
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rDI(const uint8_t *buf) {
	switch(operand_size()) {
	case 16:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R15W>(buf);
		} else {
			decode_Reg<operand_t::REG_DI>(buf);
		}
		break;
	case 32:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R15D>(buf);
		} else {
			decode_Reg<operand_t::REG_EDI>(buf);
		}
		break;
	case 64:
		if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
			decode_Reg<operand_t::REG_R15>(buf);
		} else {
			decode_Reg<operand_t::REG_RDI>(buf);
		}
		break;
	}
}

//------------------------------------------------------------------------------
// Name: decode_rAX_rAX_NOREX(const uint8_t *buf
// NOTE: special case because this represents 3 possible ops!
//------------------------------------------------------------------------------
template <class M>
void Instruction<M>::decode_rAX_rAX_NOREX(const uint8_t *buf) {

	// TODO: does F3 or xchg r8, rAX take precedence
	if(BITS == 64 && rex_byte_.is_rex() && rex_byte_.b()) {
		opcode_ = &Opcodes_nop_pause_xchg[2];
	} else if(prefix_ & PREFIX_REP) {
		mandatory_prefix_ |= PREFIX_REP;
		opcode_ = &Opcodes_nop_pause_xchg[1]; /* with 0xf3 */
	} else {
		opcode_ = &Opcodes_nop_pause_xchg[0]; /* without 0xf3 */
	}

	(this->*(opcode_->decoder))(buf);
}

//------------------------------------------------------------------------------
// Name: operand_size() const
//------------------------------------------------------------------------------
template <class M>
int Instruction<M>::operand_size() const {
	int ret = 32;

	// we check if 16-bit mode is enabled
	if(prefix_ & PREFIX_OPERAND) {
		ret = 16;
	}

	// we check if 64-bit mode is enabled
	if(BITS == 64) {
		if(rex_byte_.is_rex() && rex_byte_.w()) {
			ret = 64;
		} else {
			const Type type = opcode_->type;
			// push/pop/jmp/call/jcc defaults to 64-bit even without prefix
			if(type == OP_PUSH || type == OP_POP || type == OP_PUSHF || type == OP_POPF || type == OP_CALL || type == OP_JMP || type == OP_JCC) {
				ret = 64;
			}
		}
	}

	return ret;
}

#endif

