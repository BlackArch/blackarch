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

#include "HardwareBreakpoints.h"
#include "Debugger.h"
#include "DebugEvent.h"
#include "DebuggerCoreInterface.h"
#include "DialogHWBreakpoints.h"
#include "State.h"

#include <QMenu>
#include <QDialog>

#include "ui_dialoghwbreakpoints.h"

// TODO: at the moment, nearly this entire file is x86/x86-64 specific
//       we need to figure out a proper way to support (if at all) non
//       x86 arches

#if !(defined(EDB_X86_64) || defined(EDB_X86))
#error "Unsupported Platform"
#endif

//------------------------------------------------------------------------------
// Name: HardwareBreakpoints()
// Desc:
//------------------------------------------------------------------------------
HardwareBreakpoints::HardwareBreakpoints() : menu_(0), dialog_(0), old_event_handler_(0) {
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *HardwareBreakpoints::menu(QWidget *parent) {
	if(menu_ == 0) {
		menu_ = new QMenu(tr("Hardware BreakpointManager"), parent);
		menu_->addAction(tr("&Hardware Breakpoints"), this, SLOT(show_menu()), QKeySequence(tr("Ctrl+Shift+H")));
	}

	return menu_;
}

//------------------------------------------------------------------------------
// Name: setup_bp(State &state, int num, bool enabled, edb::address_t addr, int type, int size)
// Desc:
//------------------------------------------------------------------------------
void HardwareBreakpoints::setup_bp(State &state, int num, bool enabled, edb::address_t addr, int type, int size) {

	const int N1 = 16 + (num * 4);
	const int N2 = 18 + (num * 4);

	// default to disabled
	state.set_debug_register(7, (state.debug_register(7) & ~(0x01 << (num * 2))));

	if(enabled) {
		// set the address
		state.set_debug_register(num, addr);

		// enable this breakpoint
		state.set_debug_register(7, state.debug_register(7) | (0x01 << (num * 2)));

		// setup the type
		switch(type) {
		case 2:
			// read/write
			state.set_debug_register(7, (state.debug_register(7) & ~(0x03 << N1)) | (0x03 << N1));
			break;
		case 1:
			// write
			state.set_debug_register(7, (state.debug_register(7) & ~(0x03 << N1)) | (0x01 << N1));
			break;
		case 0:
			// execute
			state.set_debug_register(7, (state.debug_register(7) & ~(0x03 << N1)));
			break;
		}

		if(type != 0) {
			// setup the size
			switch(size) {
			case 2:
				// 4 bytes
				state.set_debug_register(7, (state.debug_register(7) & ~(0x03 << N2)) | (0x03 << N2));
				break;
			case 1:
				// 2 bytes
				state.set_debug_register(7, (state.debug_register(7) & ~(0x03 << N2)) | (0x01 << N2));
				break;
			case 0:
				// 1 byte
				state.set_debug_register(7, (state.debug_register(7) & ~(0x03 << N2)));
				break;
			}
		} else {
			state.set_debug_register(7, (state.debug_register(7) & ~(0x03 << N2)));
		}
	}
}

//------------------------------------------------------------------------------
// Name: setup_breakpoints()
// Desc:
//------------------------------------------------------------------------------
void HardwareBreakpoints::setup_breakpoints() {

	// TODO: assert that we are paused
	
	
	if(DialogHWBreakpoints *const p = qobject_cast<DialogHWBreakpoints *>(dialog_)) {

		const bool enabled =
			p->ui->chkBP1->isChecked() ||
			p->ui->chkBP2->isChecked() ||
			p->ui->chkBP3->isChecked() ||
			p->ui->chkBP4->isChecked();

		if(enabled) {
			// we want to be enabled, if we aren't already hooked,
			// hook it
			if(old_event_handler_ == 0) {
				old_event_handler_ = edb::v1::set_debug_event_handler(this);
			}

			State state;
			bool ok;
			edb::v1::debugger_core->get_state(state);

			edb::address_t addr;

			addr = edb::v1::string_to_address(p->ui->txtBP1->text(), ok);
			if(ok) {
				setup_bp(state, 0, p->ui->chkBP1->isChecked(), addr, p->ui->cmbType1->currentIndex(), p->ui->cmbSize1->currentIndex());
			}

			addr = edb::v1::string_to_address(p->ui->txtBP2->text(), ok);
			if(ok) {
				setup_bp(state, 1, p->ui->chkBP2->isChecked(), addr, p->ui->cmbType2->currentIndex(), p->ui->cmbSize2->currentIndex());
			}

			addr = edb::v1::string_to_address(p->ui->txtBP3->text(), ok);
			if(ok) {
				setup_bp(state, 2, p->ui->chkBP3->isChecked(), addr, p->ui->cmbType3->currentIndex(), p->ui->cmbSize3->currentIndex());
			}

			addr = edb::v1::string_to_address(p->ui->txtBP4->text(), ok);
			if(ok) {
				setup_bp(state, 3, p->ui->chkBP4->isChecked(), addr, p->ui->cmbType4->currentIndex(), p->ui->cmbSize4->currentIndex());
			}

			edb::v1::debugger_core->set_state(state);

		} else {

			State state;
			edb::v1::debugger_core->get_state(state);
			state.set_debug_register(7, 0);
			edb::v1::debugger_core->set_state(state);

			// we want to be disabled and we have hooked, so unhook
			if(old_event_handler_ != 0) {
				edb::v1::set_debug_event_handler(old_event_handler_);
				old_event_handler_ = 0;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: show_menu()
// Desc:
//------------------------------------------------------------------------------
void HardwareBreakpoints::show_menu() {

	if(dialog_ == 0) {
		dialog_ = new DialogHWBreakpoints(edb::v1::debugger_ui);
	}

	if(dialog_->exec() == QDialog::Accepted) {
		setup_breakpoints();
	}
}

//------------------------------------------------------------------------------
// Name: handle_event(const DebugEvent &event)
// Desc: this hooks the debug event handler so we can make the breakpoints
//       able to be resumed
//------------------------------------------------------------------------------
edb::EVENT_STATUS HardwareBreakpoints::handle_event(const DebugEvent &event) {

	if(event.reason() == DebugEvent::EVENT_STOPPED) {
		if(event.stop_code() == DebugEvent::sigtrap) {
			// check DR6 to see if it was a HW BP event
			// if so, set the resume flag
			State state;
			edb::v1::debugger_core->get_state(state);
			if((state.debug_register(6) & 0x0f) != 0x00) {
				state.set_flags(state.flags() | (1 << 16));
				edb::v1::debugger_core->set_state(state);
			}

		}
	}

	// pass the event down the stack
	return old_event_handler_->handle_event(event);
}


Q_EXPORT_PLUGIN2(HardwareBreakpoints, HardwareBreakpoints)
