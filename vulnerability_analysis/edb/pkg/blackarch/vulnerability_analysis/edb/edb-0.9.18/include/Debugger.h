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

#ifndef DEBUGGER_20061101_H_
#define DEBUGGER_20061101_H_

#include "Types.h"
#include "API.h"
#include "Breakpoint.h"

#include <QString>
#include <QStringList>
#include <QHash>

class AnalyzerInterface;
class ArchProcessorInterface;
class BinaryInfo;
class Configuration;
class DebugEventHandlerInterface;
class DebuggerCoreInterface;
class DebuggerPluginInterface;
class FunctionInfo;
class MemoryRegions;
class SessionFileInterface;
class State;
class SymbolManagerInterface;

class QByteArray;
class QDialog;
class QFileInfo;
class QWidget;

struct ExpressionError;

#include "MemRegion.h"
#include "BinaryInfo.h"

namespace edb {
	namespace v1 {

		// some useful objects
		EDB_EXPORT extern DebuggerCoreInterface *debugger_core;
		EDB_EXPORT extern QWidget *              debugger_ui;

		// the symbol mananger
		EDB_EXPORT SymbolManagerInterface &symbol_manager();

		// the memory region manager
		EDB_EXPORT MemoryRegions &memory_regions();

		// the current arch processor
		EDB_EXPORT ArchProcessorInterface &arch_processor();

		// widgets
		EDB_EXPORT QWidget *disassembly_widget();


		// breakpoint managment
		EDB_EXPORT Breakpoint::pointer find_breakpoint(edb::address_t address);
		EDB_EXPORT void remove_breakpoint(edb::address_t address);
		EDB_EXPORT void create_breakpoint(edb::address_t address);
		EDB_EXPORT void toggle_breakpoint(edb::address_t address);
		EDB_EXPORT edb::address_t enable_breakpoint(edb::address_t address);
		EDB_EXPORT edb::address_t disable_breakpoint(edb::address_t address);
		EDB_EXPORT void set_breakpoint_condition(edb::address_t address, const QString &condition);
		EDB_EXPORT QString get_breakpoint_condition(edb::address_t address);

		EDB_EXPORT edb::address_t current_data_view_address();

		// change what the various views show
		EDB_EXPORT bool dump_data_range(edb::address_t address, edb::address_t end_address, bool new_tab);
		EDB_EXPORT bool dump_data_range(edb::address_t address, edb::address_t end_address);
		EDB_EXPORT bool dump_data(edb::address_t address, bool new_tab);
		EDB_EXPORT bool dump_data(edb::address_t address);
		EDB_EXPORT bool dump_stack(edb::address_t address, bool scroll_to);
		EDB_EXPORT bool dump_stack(edb::address_t address);
		EDB_EXPORT bool jump_to_address(edb::address_t address);

		// ask the user for a value in an expression form
		EDB_EXPORT bool get_expression_from_user(const QString &title, const QString prompt, edb::address_t &value);
		EDB_EXPORT bool eval_expression(const QString &expression, edb::address_t &value);

		// ask the user for a value suitable for a register via an input box
		EDB_EXPORT bool get_value_from_user(edb::reg_t &value, const QString &title);
		EDB_EXPORT bool get_value_from_user(edb::reg_t &value);

		// ask the user for a binary string via an input box
		EDB_EXPORT bool get_binary_string_from_user(QByteArray &value, const QString &title, int max_length);
		EDB_EXPORT bool get_binary_string_from_user(QByteArray &value, const QString &title);

		// determine if the given address is the starting point of an string, if so, s will contain it
		// (formatted with C-style escape chars, so foundLength will have the original length of the string in chars).
		EDB_EXPORT bool get_ascii_string_at_address(edb::address_t address, QString &s, int min_length, int max_length, int &found_length);
		EDB_EXPORT bool get_utf16_string_at_address(edb::address_t address, QString &s, int min_length, int max_length, int &found_length);

		// list of loaded librarys
		EDB_EXPORT QStringList loaded_libraries();

		EDB_EXPORT MemRegion current_cpu_view_region();
		EDB_EXPORT MemRegion primary_code_region();
		EDB_EXPORT MemRegion primary_data_region();

		// configuration
		EDB_EXPORT QDialog *dialog_options();
		EDB_EXPORT Configuration &config();

		// a numeric version of the current version suitable for integer comparison
		EDB_EXPORT quint32 edb_version();
		EDB_EXPORT quint32 int_version(const QString &s);

		// symbol resolution
		EDB_EXPORT QString find_function_symbol(edb::address_t address);
		EDB_EXPORT QString find_function_symbol(edb::address_t address, const QString &default_value);
		EDB_EXPORT QString find_function_symbol(edb::address_t address, const QString &default_value, int *offset);

		// basic process stats
		EDB_EXPORT QString get_process_cwd();
		EDB_EXPORT QString get_process_exe();
		EDB_EXPORT QStringList get_process_args();

		// ask the user for either a value or a variable (register name and such)
		EDB_EXPORT edb::address_t get_value(edb::address_t address, bool &ok, ExpressionError &err);
		EDB_EXPORT edb::address_t get_variable(const QString &s, bool &ok, ExpressionError &err);

		// hook the debug event system
		EDB_EXPORT DebugEventHandlerInterface *set_debug_event_handler(DebugEventHandlerInterface *p);
		EDB_EXPORT DebugEventHandlerInterface *debug_event_handler();

		EDB_EXPORT AnalyzerInterface *set_analyzer(AnalyzerInterface *p);
		EDB_EXPORT AnalyzerInterface *analyzer();

		EDB_EXPORT SessionFileInterface *set_session_file_handler(SessionFileInterface *p);
		EDB_EXPORT SessionFileInterface *session_file_handler();

		// reads up to size bytes from address (stores how many it could read in size)
		EDB_EXPORT bool get_instruction_bytes(edb::address_t address, quint8 *buf, int &size);

		EDB_EXPORT BinaryInfo *get_binary_info(const MemRegion &region);
		EDB_EXPORT const FunctionInfo *get_function_info(const QString &function);

		EDB_EXPORT edb::address_t locate_main_function();

		EDB_EXPORT const QHash<QString, QObject *> &plugin_list();
		EDB_EXPORT DebuggerPluginInterface *find_plugin_by_name(const QString &name);

		EDB_EXPORT void reload_symbols();
		EDB_EXPORT void repaint_cpu_view();

		// these are here and not members of state because
		// they may require using the debugger core plugin and
		// we don't want to force a dependancy between the two
		EDB_EXPORT void pop_value(State &state);
		EDB_EXPORT void push_value(State &state, edb::reg_t value);

		EDB_EXPORT void register_binary_info(BinaryInfo::create_func_ptr_t fptr);

		EDB_EXPORT bool overwrite_check(edb::address_t address, unsigned int size);
		EDB_EXPORT void modify_bytes(edb::address_t address, unsigned int size, QByteArray &bytes, quint8 fill);

		EDB_EXPORT QByteArray get_file_md5(const QString &s);
		EDB_EXPORT QByteArray get_md5(const void *p, size_t n);

		EDB_EXPORT QString basename(const QString &s);
		EDB_EXPORT QString symlink_target(const QString &s);
		EDB_EXPORT QStringList parse_command_line(const QString &cmdline);
		EDB_EXPORT edb::address_t string_to_address(const QString &s, bool &ok);
		EDB_EXPORT QString format_bytes(const QByteArray &x);
		EDB_EXPORT QString format_pointer(edb::address_t p);

		EDB_EXPORT edb::address_t cpu_selected_address();

		EDB_EXPORT edb::pid_t get_parent_pid(edb::pid_t pid);

		EDB_EXPORT void set_status(const QString &message);

		EDB_EXPORT int pointer_size();
		
		EDB_EXPORT QByteArray serialize_object(const QObject *object);
	}
}
#endif
