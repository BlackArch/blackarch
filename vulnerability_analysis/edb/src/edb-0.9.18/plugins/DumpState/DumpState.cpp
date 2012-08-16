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

#include "DumpState.h"
#include "Debugger.h"
#include "State.h"
#include "DebuggerCoreInterface.h"
#include "DumpStateOptionsPage.h"
#include "Util.h"
#include "Instruction.h"

#include <QMenu>
#include <QSettings>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

namespace {
	//------------------------------------------------------------------------------
	// Name: hex_string(T value)
	// Desc:
	//------------------------------------------------------------------------------
	template <class T>
	std::string hex_string(T value) {
		std::stringstream ss;
		ss << std::hex << std::setw(sizeof(T) * 2) << std::setfill('0') <<  static_cast<edb::reg_t>(value);
		return ss.str();
	}
}

//------------------------------------------------------------------------------
// Name: DumpState()
// Desc:
//------------------------------------------------------------------------------
DumpState::DumpState() : menu_(0) {
}

//------------------------------------------------------------------------------
// Name: ~DumpState()
// Desc:
//------------------------------------------------------------------------------
DumpState::~DumpState() {
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *DumpState::menu(QWidget *parent) {

	if(menu_ == 0) {
		menu_ = new QMenu(tr("DumpState"), parent);
		menu_->addAction (tr("&Dump Current State"), this, SLOT(show_menu()), QKeySequence(tr("Ctrl+D")));
	}

	return menu_;
}

//------------------------------------------------------------------------------
// Name: dump_code(const State &state)
// Desc:
//------------------------------------------------------------------------------
void DumpState::dump_code(const State &state) {

	QSettings settings;
	const int instructions_to_print = settings.value("DumpState/instructions_after_ip", 6).toInt();

	const edb::address_t ip = state.instruction_pointer();
	edb::address_t address = ip;

	for(int i = 0; i < instructions_to_print + 1; ++i) {
		quint8 buf[edb::Instruction::MAX_SIZE];
		int size = sizeof(buf);

		if(edb::v1::get_instruction_bytes(address, buf, size)) {
			edb::Instruction insn(buf, size, address, std::nothrow);
			if(insn.valid()) {
				std::cout << ((address == ip) ? "> " : "  ") << hex_string(address) << ": " << edisassm::to_string(insn) << "\n";
			} else {
				break;
			}
			address += insn.size();
		} else {
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: dump_registers(const State &state)
// Desc:
//------------------------------------------------------------------------------
void DumpState::dump_registers(const State &state) {
#if defined(EDB_X86)
	std::cout << "     eax:" << hex_string(*state["eax"]);
	std::cout << " ebx:" << hex_string(*state["ebx"]);
	std::cout << "  ecx:" << hex_string(*state["ecx"]);
	std::cout << "  edx:" << hex_string(*state["edx"]);
	std::cout << "     eflags:" << hex_string(*state["eflags"]);
	std::cout << "\n";
	std::cout << "     esi:" << hex_string(*state["esi"]);
	std::cout << " edi:" << hex_string(*state["edi"]);
	std::cout << "  esp:" << hex_string(*state["esp"]);
	std::cout << "  ebp:" << hex_string(*state["ebp"]);
	std::cout << "     eip:" << hex_string(state.instruction_pointer());
	std::cout << "\n";
	std::cout << "     cs:" << hex_string<quint16>(*state["cs"]);
	std::cout << "  ds:" << hex_string<quint16>(*state["ds"]);
	std::cout << "  es:" << hex_string<quint16>(*state["es"]);
	std::cout << "  fs:" << hex_string<quint16>(*state["fs"]);
	std::cout << "  gs:" << hex_string<quint16>(*state["gs"]);
	std::cout << "  ss:" << hex_string<quint16>(*state["ss"]);
	std::cout << "    ";
	std::cout << ((*state["eflags"] & (1 << 11)) != 0 ? 'O' : 'o') << ' ';
	std::cout << ((*state["eflags"] & (1 << 10)) != 0 ? 'D' : 'd') << ' ';
	std::cout << ((*state["eflags"] & (1 <<  9)) != 0 ? 'I' : 'i') << ' ';
	std::cout << ((*state["eflags"] & (1 <<  8)) != 0 ? 'T' : 't') << ' ';
	std::cout << ((*state["eflags"] & (1 <<  7)) != 0 ? 'S' : 's') << ' ';
	std::cout << ((*state["eflags"] & (1 <<  6)) != 0 ? 'Z' : 'z') << ' ';
	std::cout << ((*state["eflags"] & (1 <<  4)) != 0 ? 'A' : 'a') << ' ';
	std::cout << ((*state["eflags"] & (1 <<  2)) != 0 ? 'P' : 'p') << ' ';
	std::cout << ((*state["eflags"] & (1 <<  0)) != 0 ? 'C' : 'c');
	std::cout << "\n";
#elif defined(EDB_X86_64)
	std::cout << "     rax:" << hex_string(*state["rax"]);
	std::cout << " rbx:" << hex_string(*state["rbx"]);
	std::cout << "  rcx:" << hex_string(*state["rcx"]);
	std::cout << "  rdx:" << hex_string(*state["rdx"]);
	std::cout << "     rflags:" << hex_string(*state["rflags"]);
	std::cout << "\n";
	std::cout << "     rsi:" << hex_string(*state["rsi"]);
	std::cout << " rdi:" << hex_string(*state["rdi"]);
	std::cout << "  rsp:" << hex_string(*state["rsp"]);
	std::cout << "  rbp:" << hex_string(*state["rbp"]);
	std::cout << "        rip:" << hex_string(state.instruction_pointer());
	std::cout << "\n";
	std::cout << "      r8:" << hex_string(*state["r8"]);
	std::cout << "  r9:" << hex_string(*state["r9"]);
	std::cout << "  r10:" << hex_string(*state["r10"]);
	std::cout << "  r11:" << hex_string(*state["r11"]);
	std::cout << "           ";
	std::cout << ((*state["rflags"] & (1 << 11)) != 0 ? 'O' : 'o') << ' ';
	std::cout << ((*state["rflags"] & (1 << 10)) != 0 ? 'D' : 'd') << ' ';
	std::cout << ((*state["rflags"] & (1 <<  9)) != 0 ? 'I' : 'i') << ' ';
	std::cout << ((*state["rflags"] & (1 <<  8)) != 0 ? 'T' : 't') << ' ';
	std::cout << ((*state["rflags"] & (1 <<  7)) != 0 ? 'S' : 's') << ' ';
	std::cout << ((*state["rflags"] & (1 <<  6)) != 0 ? 'Z' : 'z') << ' ';
	std::cout << ((*state["rflags"] & (1 <<  4)) != 0 ? 'A' : 'a') << ' ';
	std::cout << ((*state["rflags"] & (1 <<  2)) != 0 ? 'P' : 'p') << ' ';
	std::cout << ((*state["rflags"] & (1 <<  0)) != 0 ? 'C' : 'c');
	std::cout << "\n";
	std::cout << "     r12:" << hex_string(*state["r12"]);
	std::cout << " r13:" << hex_string(*state["r13"]);
	std::cout << "  r14:" << hex_string(*state["r14"]);
	std::cout << "  r15:" << hex_string(*state["r15"]);
	std::cout << "\n";
	std::cout << "      cs:" << hex_string<quint16>(*state["cs"]);
	std::cout << "  ds:" << hex_string<quint16>(*state["ds"]);
	std::cout << "   es:" << hex_string<quint16>(*state["es"]);
	std::cout << "   fs:" << hex_string<quint16>(*state["fs"]);
	std::cout << "\n";
	std::cout << "      gs:" << hex_string<quint16>(*state["gs"]);
	std::cout << "  ss:" << hex_string<quint16>(*state["ss"]);
	std::cout << "\n";
#endif
}

//------------------------------------------------------------------------------
// Name: dump_lines(edb::address_t address, int lines)
// Desc:
//------------------------------------------------------------------------------
void DumpState::dump_lines(edb::address_t address, int lines) {
	for(int i = 0; i < lines; ++i) {
		quint8 buf[16];
		if(edb::v1::debugger_core->read_bytes(address, buf, sizeof(buf))) {
			std::cout << hex_string(address) << " : ";

			for(int j = 0x00; j < 0x04; ++j) std::cout << hex_string(buf[j]) << " ";
			std::cout << " ";
			for(int j = 0x04; j < 0x08; ++j) std::cout << hex_string(buf[j]) << " ";
			std::cout << "- ";
			for(int j = 0x08; j < 0x0c; ++j) std::cout << hex_string(buf[j]) << " ";
			std::cout << " ";
			for(int j = 0x0c; j < 0x10; ++j) std::cout << hex_string(buf[j]) << " ";

			for(int j = 0; j < 16; ++j) {
				const quint8 ch = buf[j];
				std::cout << (
					(
                                                std::isprint(ch) ||
                                                (std::isspace(ch) && (ch != '\f' && ch != '\t' && ch != '\r' && ch != '\n') && ch < 0x80)
					) ? static_cast<char>(ch) : '.');
			}

			std::cout << "\n";
		} else {
			break;
		}
		address += 16;
	}
}

//------------------------------------------------------------------------------
// Name: dump_stack(const State &state)
// Desc:
//------------------------------------------------------------------------------
void DumpState::dump_stack(const State &state) {
	dump_lines(state.stack_pointer(), 4);
}

//------------------------------------------------------------------------------
// Name: dump_data(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
void DumpState::dump_data(edb::address_t address) {
	dump_lines(address, 2);
}

//------------------------------------------------------------------------------
// Name: show_menu()
// Desc:
//------------------------------------------------------------------------------
void DumpState::show_menu() {

	State state;
	edb::v1::debugger_core->get_state(state);

	std::cout << "------------------------------------------------------------------------------\n";
	dump_registers(state);
	std::cout << "[" << hex_string<quint16>(*state["ss"]) << ":" << hex_string(state.stack_pointer()) << "]---------------------------------------------------------[stack]\n";
	dump_stack(state);

	const edb::address_t data_address = edb::v1::current_data_view_address();
	std::cout << "[" << hex_string<quint16>(*state["ds"]) << ":" << hex_string(data_address) << "]---------------------------------------------------------[ data]\n";
	dump_data(data_address);
	std::cout << "[" << hex_string<quint16>(*state["cs"]) << ":" << hex_string(state.instruction_pointer()) << "]---------------------------------------------------------[ code]\n";
	dump_code(state);
	std::cout << "------------------------------------------------------------------------------\n";
}

//------------------------------------------------------------------------------
// Name: options_page()
// Desc:
//------------------------------------------------------------------------------
QWidget *DumpState::options_page() {
	return new DumpStateOptionsPage;
}

Q_EXPORT_PLUGIN2(DumpState, DumpState)
