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

#include "HeapAnalyzer.h"
#include "DialogHeap.h"
#include "Debugger.h"
#include <QMenu>

//------------------------------------------------------------------------------
// Name: HeapAnalyzer()
// Desc:
//------------------------------------------------------------------------------
HeapAnalyzer::HeapAnalyzer() : menu_(0), dialog_(0) {
}

//------------------------------------------------------------------------------
// Name: ~HeapAnalyzer()
// Desc:
//------------------------------------------------------------------------------
HeapAnalyzer::~HeapAnalyzer() {
	delete dialog_;
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *HeapAnalyzer::menu(QWidget *parent) {

	if(menu_ == 0) {
		menu_ = new QMenu(tr("HeapAnalyzer"), parent);
		menu_->addAction (tr("&Heap Analyzer"), this, SLOT(show_menu()), QKeySequence(tr("Ctrl+H")));
	}

	return menu_;
}

//------------------------------------------------------------------------------
// Name: mnuHeapAnalyzer()
// Desc:
//------------------------------------------------------------------------------
void HeapAnalyzer::show_menu() {

	if(dialog_ == 0) {
		dialog_ = new DialogHeap(edb::v1::debugger_ui);
	}

	dialog_->show();
}

Q_EXPORT_PLUGIN2(HeapAnalyzer, HeapAnalyzer)
