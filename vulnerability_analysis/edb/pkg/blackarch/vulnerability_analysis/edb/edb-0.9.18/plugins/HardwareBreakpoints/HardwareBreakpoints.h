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

#ifndef HARDWAREBREAKPOINTS_20080228_H_
#define HARDWAREBREAKPOINTS_20080228_H_

#include "DebuggerPluginInterface.h"
#include "DebugEventHandlerInterface.h"

class QDialog;
class QMenu;
class State;

class HardwareBreakpoints : public QObject, public DebuggerPluginInterface, public DebugEventHandlerInterface {
	Q_OBJECT
	Q_INTERFACES(DebuggerPluginInterface)
	Q_CLASSINFO("author", "Evan Teran")
	Q_CLASSINFO("url", "http://www.codef00.com")

public:
	HardwareBreakpoints();

public:
	virtual QMenu *menu(QWidget *parent = 0);
	virtual edb::EVENT_STATUS handle_event(const DebugEvent &event);

public Q_SLOTS:
	void show_menu();

private:
	void setup_breakpoints();
	void setup_bp(State &state, int num, bool enabled, edb::address_t addr, int type, int size);

private:
	QMenu *                      menu_;
	QDialog *                    dialog_;
	DebugEventHandlerInterface * old_event_handler_;
};

#endif
