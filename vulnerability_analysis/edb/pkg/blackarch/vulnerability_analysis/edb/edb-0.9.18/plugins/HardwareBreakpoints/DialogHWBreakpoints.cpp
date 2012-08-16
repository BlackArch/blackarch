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

#include "DialogHWBreakpoints.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "State.h"

#include <QRegExpValidator>

#include "ui_dialoghwbreakpoints.h"

#if defined(EDB_X86)
	#define MAX_HEX "8"
#elif defined(EDB_X86_64)
	#define MAX_HEX "16"
#endif

//------------------------------------------------------------------------------
// Name: DialogHWBreakpoints(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogHWBreakpoints::DialogHWBreakpoints(QWidget *parent) : QDialog(parent), ui(new Ui::DialogHWBreakpoints) {
	ui->setupUi(this);

	connect(ui->cmbType1, SIGNAL(currentIndexChanged(int)), this, SLOT(type1IndexChanged(int)));
	connect(ui->cmbType2, SIGNAL(currentIndexChanged(int)), this, SLOT(type2IndexChanged(int)));
	connect(ui->cmbType3, SIGNAL(currentIndexChanged(int)), this, SLOT(type3IndexChanged(int)));
	connect(ui->cmbType4, SIGNAL(currentIndexChanged(int)), this, SLOT(type4IndexChanged(int)));

	ui->txtBP1->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{0," MAX_HEX "}"), this));
	ui->txtBP2->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{0," MAX_HEX "}"), this));
	ui->txtBP3->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{0," MAX_HEX "}"), this));
	ui->txtBP4->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{0," MAX_HEX "}"), this));
}

//------------------------------------------------------------------------------
// Name: ~DialogHWBreakpoints()
// Desc:
//------------------------------------------------------------------------------
DialogHWBreakpoints::~DialogHWBreakpoints() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: type1IndexChanged(int index)
// Desc:
//------------------------------------------------------------------------------
void DialogHWBreakpoints::type1IndexChanged(int index) {
	ui->cmbSize1->setEnabled(index != 0);
}

//------------------------------------------------------------------------------
// Name: type2IndexChanged(int index)
// Desc:
//------------------------------------------------------------------------------
void DialogHWBreakpoints::type2IndexChanged(int index) {
	ui->cmbSize2->setEnabled(index != 0);
}

//------------------------------------------------------------------------------
// Name: type3IndexChanged(int index)
// Desc:
//------------------------------------------------------------------------------
void DialogHWBreakpoints::type3IndexChanged(int index) {
	ui->cmbSize3->setEnabled(index != 0);
}

//------------------------------------------------------------------------------
// Name: type4IndexChanged(int index)
// Desc:
//------------------------------------------------------------------------------
void DialogHWBreakpoints::type4IndexChanged(int index) {
	ui->cmbSize4->setEnabled(index != 0);
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *event)
// Desc:
//------------------------------------------------------------------------------
void DialogHWBreakpoints::showEvent(QShowEvent *event) {
	Q_UNUSED(event);

	State state;
	edb::v1::debugger_core->get_state(state);

	const bool bp1_enabled = (state.debug_register(7) & 0x00000001) != 0;
	const bool bp2_enabled = (state.debug_register(7) & 0x00000004) != 0;
	const bool bp3_enabled = (state.debug_register(7) & 0x00000010) != 0;
	const bool bp4_enabled = (state.debug_register(7) & 0x00000040) != 0;

	ui->chkBP1->setChecked(bp1_enabled);
	ui->chkBP2->setChecked(bp2_enabled);
	ui->chkBP3->setChecked(bp3_enabled);
	ui->chkBP4->setChecked(bp4_enabled);

	if(bp1_enabled) {
		ui->txtBP1->setText(edb::v1::format_pointer(state.debug_register(0)));
	}

	if(bp2_enabled) {
		ui->txtBP2->setText(edb::v1::format_pointer(state.debug_register(1)));
	}

	if(bp3_enabled) {
		ui->txtBP3->setText(edb::v1::format_pointer(state.debug_register(2)));
	}

	if(bp4_enabled) {
		ui->txtBP4->setText(edb::v1::format_pointer(state.debug_register(3)));
	}
}
