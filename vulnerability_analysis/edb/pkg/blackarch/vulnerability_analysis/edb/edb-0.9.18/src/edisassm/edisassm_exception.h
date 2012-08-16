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

#ifndef EDISASSM_EXCEPTION_20080414_H_
#define EDISASSM_EXCEPTION_20080414_H_

namespace edisassm {

struct invalid_instruction : public std::exception {
public:
	invalid_instruction(std::size_t size) : size_(size) {
	}

	std::size_t size() const { return size_; }

private:
	std::size_t size_;
};

struct instruction_too_big    : invalid_instruction { instruction_too_big(std::size_t size)    : invalid_instruction(size) {} };
struct invalid_operand        : invalid_instruction { invalid_operand(std::size_t size)        : invalid_instruction(size) {} };
struct too_many_operands      : invalid_instruction { too_many_operands(std::size_t size)      : invalid_instruction(size) {} };
struct multiple_displacements : invalid_instruction { multiple_displacements(std::size_t size) : invalid_instruction(size) {} };

}

#endif
