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

#ifndef OPERAND_20080421_TCC_
#define OPERAND_20080421_TCC_

#include "Instruction.h"
#include <cstring>


//------------------------------------------------------------------------------
// Name: Operand()
//------------------------------------------------------------------------------
template <class M>
Operand<M>::Operand() : owner_(0), type_(TYPE_INVALID) {
	using std::memset;
	memset(&u, 0, sizeof(U));
}

//------------------------------------------------------------------------------
// Name: swap(Operand &other)
//------------------------------------------------------------------------------
template <class M>
void Operand<M>::swap(Operand &other) {
	using std::swap;

	swap(owner_, other.owner_);
	swap(type_, other.type_);
	swap(u, other.u);
}

//------------------------------------------------------------------------------
// Name: relative_target() const
//------------------------------------------------------------------------------
template <class M>
typename Operand<M>::address_t Operand<M>::relative_target() const {

	const address_t rva     = owner_->rva();
	const unsigned int size = owner_->size();
	const address_t offset  = rva + size;

	switch(type_) {
	case TYPE_REL8:
		return static_cast<address_t>(u.sbyte + offset);
	case TYPE_REL16:
		// NOTE: intel truncates EIP to 16-bit here
		return static_cast<address_t>((u.sword + offset) & 0xffff);
	case TYPE_REL32:
		return static_cast<address_t>(u.sdword + offset);
	case TYPE_REL64:
		return static_cast<address_t>(u.sqword + offset);
	default:
		return 0;
	}
}

//------------------------------------------------------------------------------
// Name: displacement() const
//------------------------------------------------------------------------------
template <class M>
int32_t Operand<M>::displacement() const {

	switch(u.expression.displacement_type) {
	case DISP_U8:  return static_cast<int32_t>(u.expression.u_disp8);
	case DISP_U16: return static_cast<int32_t>(u.expression.u_disp16);
	case DISP_U32: return static_cast<int32_t>(u.expression.u_disp32);
	case DISP_S8:  return static_cast<int32_t>(u.expression.s_disp8);
	case DISP_S16: return static_cast<int32_t>(u.expression.s_disp16);
	case DISP_S32: return static_cast<int32_t>(u.expression.s_disp32);
	default:
		return 0;
	}
}

//------------------------------------------------------------------------------
// Name: immediate() const
//------------------------------------------------------------------------------
template <class M>
int64_t Operand<M>::immediate() const {

	switch(type_) {
	case TYPE_IMMEDIATE8:  return static_cast<int64_t>(u.sbyte);
	case TYPE_IMMEDIATE16: return static_cast<int64_t>(u.sword);
	case TYPE_IMMEDIATE32: return static_cast<int64_t>(u.sdword);
	case TYPE_IMMEDIATE64: return static_cast<int64_t>(u.sqword);
	default:
		return 0;
	}
}

//------------------------------------------------------------------------------
// Name: general_type() const
//------------------------------------------------------------------------------
template <class M>
typename Operand<M>::Type Operand<M>::general_type() const {
	return static_cast<Type>(static_cast<unsigned int>(type_) & TYPE_MASK);
}

#endif
