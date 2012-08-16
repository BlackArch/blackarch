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

#ifndef DEBUGGEROPS_20090424_H_
#define DEBUGGEROPS_20090424_H_

#include "State.h"
#include "Debugger.h"
#include "Instruction.h"
#include "DebuggerCoreInterface.h"

namespace edb {
	namespace detail {

		//----------------------------------------------------------------------
		// Name: step_over(F1 run_func, F2 step_func)
		//----------------------------------------------------------------------
		template <class F1, class F2>
		void step_over(F1 run_func, F2 step_func) {
			
			State state;
			edb::v1::debugger_core->get_state(state);
			
			const edb::address_t ip = state.instruction_pointer();

			quint8 buffer[edb::Instruction::MAX_SIZE];
			int sz = sizeof(buffer);

			if(edb::v1::get_instruction_bytes(ip, buffer, sz)) {
				edb::Instruction insn(buffer, sz, 0, std::nothrow);
				if(insn.valid() && edb::v1::arch_processor().can_step_over(insn)) {

					// add a temporary breakpoint at the instruction just
					// after the call
					Breakpoint::pointer bp = edb::v1::debugger_core->add_breakpoint(ip + insn.size());
					bp->set_internal(true);
					bp->set_one_time(true);

					run_func();
					return;
				}
			}

			// if all else fails, it's a step into
			step_func();
		}
	}
}

#endif
