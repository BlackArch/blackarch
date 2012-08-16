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

#ifndef CONFIGURATION_20061031_H_
#define CONFIGURATION_20061031_H_

#include "API.h"
#include <QString>

class EDB_EXPORT Configuration {
public:
	Configuration();
	~Configuration();

public:
	enum Syntax {
		Intel,
		ATT
	};

	enum CloseBehavior {
		Detach,
		Terminate
	};

	enum InitialBreakpoint {
		EntryPoint,
		MainSymbol
	};

public:
	// general tab
	CloseBehavior     close_behavior;

	// appearance tab
	bool              show_address_separator;
	QString           stack_font;
	QString           registers_font;
	QString           disassembly_font;
	QString           data_font;
	bool              data_show_address;
	bool              data_show_hex;
	bool              data_show_ascii;
	bool              data_show_comments;
	int               data_word_width;
	int               data_row_width;

	// debugging tab
	InitialBreakpoint initial_breakpoint;
	bool              warn_on_no_exec_bp;
	bool              find_main;
	bool              tty_enabled;
	QString           tty_command;

	// disassembly tab
	Syntax            syntax;
	bool              zeros_are_filling;
	bool              uppercase_disassembly;

	// directories tab
	QString           symbol_path;
	QString           plugin_path;
	QString           session_path;

	int               min_string_length;

protected:
	void read_settings();
	void write_settings();
};

#endif

