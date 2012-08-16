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

#include "OpcodeSearcher.h"
#include "DialogOpcodes.h"
#include "Debugger.h"
#include <QMenu>

//------------------------------------------------------------------------------
// Name: OpcodeSearcher()
// Desc:
//------------------------------------------------------------------------------
OpcodeSearcher::OpcodeSearcher() : menu_(0), dialog_(0) {
}

//------------------------------------------------------------------------------
// Name: ~OpcodeSearcher()
// Desc:
//------------------------------------------------------------------------------
OpcodeSearcher::~OpcodeSearcher() {
	delete dialog_;
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *OpcodeSearcher::menu(QWidget *parent) {

	if(menu_ == 0) {
		menu_ = new QMenu(tr("OpcodeSearcher"), parent);
		menu_->addAction(tr("&Opcode Search"), this, SLOT(show_menu()), QKeySequence(tr("Ctrl+O")));
	}

	return menu_;
}

//------------------------------------------------------------------------------
// Name: show_menu()
// Desc:
//------------------------------------------------------------------------------
void OpcodeSearcher::show_menu() {

	if(dialog_ == 0) {
		dialog_ = new DialogOpcodes(edb::v1::debugger_ui);
	}

	dialog_->show();
}


Q_EXPORT_PLUGIN2(OpcodeSearcher, OpcodeSearcher)
