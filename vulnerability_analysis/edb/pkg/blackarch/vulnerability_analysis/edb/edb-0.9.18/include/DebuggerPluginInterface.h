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

#ifndef DEBUGGERPLUGININTERFACE_20061101_H_
#define DEBUGGERPLUGININTERFACE_20061101_H_

#include <QtPlugin>
#include <QList>

class QMenu;
class QAction;

class DebuggerPluginInterface {
public:
	virtual ~DebuggerPluginInterface() {}

public:
	void init() {
		private_init();
	}

public:
	virtual QMenu *menu(QWidget *parent = 0) = 0;

public:
	// optional, overload these to have there contents added to a view's context menu
	virtual QList<QAction *> cpu_context_menu()      { return QList<QAction *>(); }
	virtual QList<QAction *> register_context_menu() { return QList<QAction *>(); }
	virtual QList<QAction *> stack_context_menu()    { return QList<QAction *>(); }
	virtual QList<QAction *> data_context_menu()     { return QList<QAction *>(); }

	// optional, overload this to add a page to the options dialog
	virtual QWidget *options_page() { return 0; }

private:
	// optional init, overload this to have edb run it after loading the plugin
	virtual void private_init() {
	}
};

Q_DECLARE_INTERFACE(DebuggerPluginInterface, "EDB.DebuggerPluginInterface/1.0")

#endif
