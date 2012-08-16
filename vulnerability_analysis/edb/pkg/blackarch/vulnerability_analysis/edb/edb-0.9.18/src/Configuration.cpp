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

#include "Configuration.h"

#include <QtDebug>
#include <QSettings>
#include <QDir>
#include <QFont>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

//------------------------------------------------------------------------------
// Name: Configuration()
// Desc: constructor
//------------------------------------------------------------------------------
Configuration::Configuration() {
	read_settings();
}

//------------------------------------------------------------------------------
// Name: ~Configuration()
// Desc: destructor
//------------------------------------------------------------------------------
Configuration::~Configuration() {
	write_settings();
}

//------------------------------------------------------------------------------
// Name: read_settings()
// Desc: read in the options from the file
//------------------------------------------------------------------------------
void Configuration::read_settings() {

#ifdef Q_OS_WIN32
	const QString default_font = QFont("Courier New", 8).toString();
#elif defined(Q_OS_MACX)
	const QString default_font = QFont("Courier New", 10).toString();
#else
	const QString default_font = QFont("Monospace", 8).toString();
#endif

#ifdef DEFAULT_SYMBOL_PATH
	const QString default_symbol_path = TOSTRING(DEFAULT_SYMBOL_PATH);
#else
	const QString default_symbol_path = QDir().absolutePath();
#endif

#ifdef DEFAULT_PLUGIN_PATH
	const QString default_plugin_path = TOSTRING(DEFAULT_PLUGIN_PATH);
#else
	const QString default_plugin_path = QDir().absolutePath();
#endif

#ifdef DEFAULT_SESSION_PATH
	const QString default_session_path = TOSTRING(DEFAULT_SESSION_PATH);
#else
	const QString default_session_path = QDir().absolutePath();
#endif

	QSettings settings;

	settings.beginGroup("General");
	close_behavior = static_cast<CloseBehavior>(settings.value("close_behavior").value<uint>());
	settings.endGroup();

	settings.beginGroup("Appearance");
	stack_font             = settings.value("appearance.stack.font", default_font).value<QString>();
	data_font              = settings.value("appearance.data.font", default_font).value<QString>();
	registers_font         = settings.value("appearance.registers.font", default_font).value<QString>();
	disassembly_font       = settings.value("appearance.disassembly.font", default_font).value<QString>();
	data_show_address      = settings.value("appearance.data.show_address.enabled", true).value<bool>();
	data_show_hex          = settings.value("appearance.data.show_hex.enabled", true).value<bool>();
	data_show_ascii        = settings.value("appearance.data.show_ascii.enabled", true).value<bool>();
	data_show_comments     = settings.value("appearance.data.show_comments.enabled", true).value<bool>();
	data_word_width        = settings.value("appearance.data.word_width", 1).value<int>();
	data_row_width         = settings.value("appearance.data.row_width", 16).value<int>();
	show_address_separator = settings.value("appearance.address_semicolon.enabled", true).value<bool>();
	settings.endGroup();

	settings.beginGroup("Debugging");
	initial_breakpoint = static_cast<InitialBreakpoint>(settings.value("debugger.initial_breakpoint.enabled", MainSymbol).value<uint>());
	warn_on_no_exec_bp = settings.value("debugger.BP_NX_warn.enabled", true).value<bool>();
	find_main          = settings.value("debugger.find_main.enabled", true).value<bool>();
	min_string_length  = settings.value("debugger.string_min", 4).value<uint>();
	tty_enabled        = settings.value("debugger.terminal.enabled", true).value<bool>();
	tty_command        = settings.value("debugger.terminal.command", "/usr/bin/xterm").value<QString>();
	settings.endGroup();

	settings.beginGroup("Disassembly");
	syntax                = static_cast<Syntax>(settings.value("disassembly.syntax", Intel).value<uint>());
	zeros_are_filling     = settings.value("disassembly.zeros_are_filling.enabled", true).value<bool>();
	uppercase_disassembly = settings.value("disassembly.uppercase.enabled", false).value<bool>();
	settings.endGroup();

	settings.beginGroup("Directories");
	symbol_path  = settings.value("directory.symbol.path", default_symbol_path).value<QString>();
	plugin_path  = settings.value("directory.plugin.path", default_plugin_path).value<QString>();
	session_path = settings.value("directory.session.path", default_session_path).value<QString>();
	settings.endGroup();

	// normalize values
	if(data_word_width != 1 && data_word_width != 2 && data_word_width != 4 && data_word_width != 8) {
		data_word_width = 1;
	}

	if(data_row_width != 1 && data_row_width != 2 && data_row_width != 4 && data_row_width != 8 && data_row_width != 16) {
		data_row_width = 16;
	}
}

//------------------------------------------------------------------------------
// Name: write_settings()
// Desc: writes the options to the file
//------------------------------------------------------------------------------
void Configuration::write_settings() {

	QSettings settings;

	settings.beginGroup("General");
	settings.setValue("close_behavior", close_behavior);
	settings.endGroup();

	settings.beginGroup("Appearance");
	settings.setValue("appearance.stack.font", stack_font);
	settings.setValue("appearance.data.font", data_font);
	settings.setValue("appearance.registers.font", registers_font);
	settings.setValue("appearance.disassembly.font", disassembly_font);
	settings.setValue("appearance.data.show_address.enabled", data_show_address);
	settings.setValue("appearance.data.show_hex.enabled", data_show_hex);
	settings.setValue("appearance.data.show_ascii.enabled", data_show_ascii);
	settings.setValue("appearance.data.show_comments.enabled", data_show_comments);
	settings.setValue("appearance.data.word_width", data_word_width);
	settings.setValue("appearance.data.row_width", data_row_width);
	settings.setValue("appearance.address_semicolon.enabled", show_address_separator);
	settings.endGroup();

	settings.beginGroup("Debugging");
	settings.setValue("debugger.BP_NX_warn.enabled", warn_on_no_exec_bp);
	settings.setValue("debugger.string_min", min_string_length);
	settings.setValue("debugger.initial_breakpoint.enabled", initial_breakpoint);
	settings.setValue("debugger.find_main.enabled", find_main);
	settings.setValue("debugger.terminal.enabled", tty_enabled);
	settings.setValue("debugger.terminal.command", tty_command);
	settings.endGroup();

	settings.beginGroup("Disassembly");
	settings.setValue("disassembly.syntax", syntax);
	settings.setValue("disassembly.zeros_are_filling.enabled", zeros_are_filling);
	settings.setValue("disassembly.uppercase.enabled", uppercase_disassembly);
	settings.endGroup();

	settings.beginGroup("Directories");
	settings.setValue("directory.symbol.path", symbol_path);
	settings.setValue("directory.plugin.path", plugin_path);
	settings.setValue("directory.session.path", session_path);
	settings.endGroup();
}
