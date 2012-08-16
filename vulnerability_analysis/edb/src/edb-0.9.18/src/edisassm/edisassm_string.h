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

#ifndef EDISASSM_STRING_20110422_H_
#define EDISASSM_STRING_20110422_H_

#include <string>

template <class M>
class Instruction;

template <class M>
class Operand;

namespace edisassm {

	struct lower_case {};
	struct upper_case {};
	struct syntax_intel_lcase : lower_case {};
	struct syntax_intel_ucase : upper_case {};
	
	typedef syntax_intel_lcase syntax_intel;
	
	//------------------------------------------------------------------------------
	// Name: to_string(const Instruction<M> &insn, const T &)
	// Desc: creates a std::string which represents the given instruction, in uppercase
	//------------------------------------------------------------------------------
	template<class M, class T>
	std::string to_string(const Instruction<M> &insn, const T &);

	//------------------------------------------------------------------------------
	// Name: to_string(const Operand<M> &operand, const T &)
	// Desc: creates a std::string which represents the given operand
	//------------------------------------------------------------------------------
	template<class M, class T>
	std::string to_string(const Operand<M> &operand, const T &);
	
	
	//------------------------------------------------------------------------------
	// Name: to_string(const Instruction<M> &insn)
	// Desc: creates a std::string which represents the given instruction in lowercase
	//------------------------------------------------------------------------------
	template <class M>
	std::string to_string(const Instruction<M> &insn) {
		return to_string(insn, syntax_intel());
	}

	//------------------------------------------------------------------------------
	// Name: to_string(const Operand<M> &operand)
	// Desc: creates a std::string which represents the given operand in uppercase
	//------------------------------------------------------------------------------
	template<class M>
	std::string to_string(const Operand<M> &operand) {
		return to_string(operand, syntax_intel());
	}

	//------------------------------------------------------------------------------
	// Name: to_byte_string(const Instruction<M> &insn, const T&)
	// Desc: creates a std::string which represents the given instruction
	//------------------------------------------------------------------------------
	template <class M, class T>
	std::string to_byte_string(const Instruction<M> &insn, const T&);
	
	//------------------------------------------------------------------------------
	// Name: to_byte_string(const Instruction<M> &insn)
	// Desc: creates a std::string which represents the given instruction
	//------------------------------------------------------------------------------
	template <class M>
	std::string to_byte_string(const Instruction<M> &insn) {
		return to_byte_string(insn, upper_case());
	}
	
	//------------------------------------------------------------------------------
	// Name: register_name(typename Operand<M>::Register reg, const upper_case&)
	// Desc: creates a std::string which represents the given register
	//------------------------------------------------------------------------------
	template <class M>
	std::string register_name(typename Operand<M>::Register reg, const upper_case&);
	
	//------------------------------------------------------------------------------
	// Name: register_name(typename Operand<M>::Register reg, const lower_case&)
	// Desc: creates a std::string which represents the given register
	//------------------------------------------------------------------------------
	template <class M>
	std::string register_name(typename Operand<M>::Register reg, const lower_case&);
	
	//------------------------------------------------------------------------------
	// Name: register_name(typename Operand<M>::Register reg)
	// Desc: creates a std::string which represents the given register
	//------------------------------------------------------------------------------
	template <class M>
	std::string register_name(typename Operand<M>::Register reg) {
		return register_name<M>(reg, lower_case());
	}
}

#include "edisassm_string.tcc"
#endif
