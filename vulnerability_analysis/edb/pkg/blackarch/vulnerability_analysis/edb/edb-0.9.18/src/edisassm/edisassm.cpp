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
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cstring>

enum DISPLAY_FLAGS {
	FLAG_NONE       = 0x00,
	FLAG_SHOW_BYTES = 0x01
};

//------------------------------------------------------------------------------
// Name: disassemble(const uint8_t *start_ptr, const uint8_t *end_ptr, typename Instruction<M>::address_t rva, unsigned int flags)
//------------------------------------------------------------------------------
template <class M>
void disassemble(const uint8_t *start_ptr, const uint8_t *end_ptr, typename Instruction<M>::address_t rva, unsigned int flags) {

	typedef Instruction<M> insn_t;

	const uint8_t *ptr = start_ptr;
	while(ptr < end_ptr) {
		insn_t instruction(ptr, end_ptr - ptr,  rva + (ptr - start_ptr), std::nothrow);
		if(instruction) {
			std::cout << std::hex << (rva + (ptr - start_ptr)) << ": ";
			if(flags & FLAG_SHOW_BYTES) {
				std::cout << edisassm::to_byte_string(instruction) << " ";
			}
			std::cout << edisassm::to_string(instruction) << '\n';
			ptr += instruction.size();
		} else {
			std::cout << "(bad)\n";
			ptr += 1;
		}
	}
}

//------------------------------------------------------------------------------
// Name: print_usage(const char *arg0)
//------------------------------------------------------------------------------
void print_usage(const char *arg0) {
	std::cerr << arg0 << " [-m32|-m64] [--rva <address>] [--show-bytes] <file>" << std::endl;
	exit(-1);
}

//------------------------------------------------------------------------------
// Name: main(int argc, char *argv[])
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

	if(argc == 1) {
		print_usage(argv[0]);
	}

	unsigned int flags    = FLAG_NONE;
	bool x86_64_mode      = false;
	uint64_t rva_address  = 0;
	std::string filename;

	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-m32") == 0) {
				x86_64_mode = false;
			} else if(strcmp(argv[i], "-m64") == 0) {
				x86_64_mode = true;
			} else if(strcmp(argv[i], "--rva") == 0) {
				++i;
				if(argv[i] == 0) {
					print_usage(argv[0]);
				}
				rva_address = strtoul(argv[i], 0, 0);
			} else if(strcmp(argv[i], "--show-bytes") == 0) {
				flags |= FLAG_SHOW_BYTES;
			} else {
				print_usage(argv[0]);
			}
		} else {
			filename = argv[i];
		}
	}

	std::ifstream file(filename.c_str(), std::ios::binary);
	if(file) {
		const std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		const uint8_t *const start_ptr = &data[0];
		const uint8_t *const end_ptr   = start_ptr + data.size();

		if(x86_64_mode) {
			disassemble<edisassm::x86_64>(start_ptr, end_ptr, rva_address, flags);
		} else {
			disassemble<edisassm::x86>(start_ptr, end_ptr, rva_address, flags);
		}
	} else {
		std::cerr << "could not open the file: " << filename << std::endl;
		return -1;
	}
}
