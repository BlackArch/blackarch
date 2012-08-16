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

/*
 * NOTE: system specific functions may be implemented in
 * src/os/[*]/Debugger*.cpp this is to isolate the system specific code away
 * from the general code.
 */

#include "Debugger.h"
#include "ArchProcessor.h"
#include "BinaryString.h"
#include "ByteShiftArray.h"
#include "Configuration.h"
#include "DebuggerCoreInterface.h"
#include "DebuggerMain.h"
#include "DebuggerPluginInterface.h"
#include "DialogInputBinaryString.h"
#include "DialogInputValue.h"
#include "DialogOptions.h"
#include "Expression.h"
#include "FunctionInfo.h"
#include "MD5.h"
#include "MemoryRegions.h"
#include "QHexView"
#include "State.h"
#include "SymbolManager.h"
#include "version.h"
#include "serializer.h"
#include "qobjecthelper.h"


#include <QAction>
#include <QAtomicPointer>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include <QDomDocument>

#include <cctype>

#include "ui_debuggerui.h"

DebuggerCoreInterface *edb::v1::debugger_core = 0;
QWidget               *edb::v1::debugger_ui   = 0;

namespace {

	typedef QList<BinaryInfo::create_func_ptr_t> BinaryInfoList;

	QAtomicPointer<DebugEventHandlerInterface> g_DebugEventHandler = 0;
	QAtomicPointer<AnalyzerInterface>          g_Analyzer          = 0;
	QAtomicPointer<SessionFileInterface>       g_SessionHandler    = 0;
	QHash<QString, QObject *>                  g_GeneralPlugins;
	BinaryInfoList                             g_BinaryInfoList;
	
	QHash<QString, FunctionInfo>               g_FunctionDB;
	
	DebuggerMain *ui() {
		return qobject_cast<DebuggerMain *>(edb::v1::debugger_ui);
	}

	bool function_symbol_base(edb::address_t address, QString &value, int &offset) {
		bool ret = false;
		offset = 0;
		const Symbol::pointer s = edb::v1::symbol_manager().find_near_symbol(address);
		if(s) {
			value = s->name;
			offset = address - s->address;
			ret = true;
		}
		return ret;
	}


}

namespace edb {
namespace internal {

//------------------------------------------------------------------------------
// Name: register_plugin(const QString &filename, QObject *plugin)
// Desc:
//------------------------------------------------------------------------------
bool register_plugin(const QString &filename, QObject *plugin) {
	if(!g_GeneralPlugins.contains(filename)) {
		g_GeneralPlugins[filename] = plugin;
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
// Name: 
// Desc:
//------------------------------------------------------------------------------
void load_function_db() {
	QFile file(":/debugger/xml/functions.xml");
	QDomDocument doc;
		
	if(file.open(QIODevice::ReadOnly)) {	
		if(doc.setContent(&file)) {
		
			QDomElement root = doc.firstChildElement("functions");
			QDomElement function = root.firstChildElement("function");
			for (; !function.isNull(); function = function.nextSiblingElement("function")) {
			
			
				const QString function_name = function.attribute("name");
				
				FunctionInfo info;
						
				QDomElement argument = function.firstChildElement("argument");
				for (; !argument.isNull(); argument = argument.nextSiblingElement("argument")) {
					const QString argument_type = argument.attribute("type");
					info.params_.push_back(argument_type[0]);
				}
				
				g_FunctionDB[function_name] = info;	
			}
		}
	}
}

}
}

//------------------------------------------------------------------------------
// Name: cpu_selected_address()
// Desc:
//------------------------------------------------------------------------------
edb::address_t edb::v1::cpu_selected_address() {
	return ui()->ui->cpuView->selectedAddress();
}

//------------------------------------------------------------------------------
// Name: current_cpu_view_region()
// Desc:
//------------------------------------------------------------------------------
MemRegion edb::v1::current_cpu_view_region() {
	return ui()->ui->cpuView->region();
}

//------------------------------------------------------------------------------
// Name: repaint_cpu_view()
// Desc:
//------------------------------------------------------------------------------
void edb::v1::repaint_cpu_view() {
	DebuggerMain *const gui = ui();
	Q_CHECK_PTR(gui);
	gui->ui->cpuView->viewport()->repaint();
}

//------------------------------------------------------------------------------
// Name: symbol_manager()
// Desc:
//------------------------------------------------------------------------------
SymbolManagerInterface &edb::v1::symbol_manager() {
	static SymbolManager g_SymbolManager;
	return g_SymbolManager;
}

//------------------------------------------------------------------------------
// Name: memory_regions()
// Desc:
//------------------------------------------------------------------------------
MemoryRegions &edb::v1::memory_regions() {
	static MemoryRegions g_MemoryRegions;
	return g_MemoryRegions;
}

//------------------------------------------------------------------------------
// Name: arch_processor()
// Desc:
//------------------------------------------------------------------------------
ArchProcessorInterface &edb::v1::arch_processor() {
	static ArchProcessor g_ArchProcessor;
	return g_ArchProcessor;
}

//------------------------------------------------------------------------------
// Name: set_analyzer(AnalyzerInterface *p)
// Desc:
//------------------------------------------------------------------------------
AnalyzerInterface *edb::v1::set_analyzer(AnalyzerInterface *p) {
	Q_CHECK_PTR(p);
	return g_Analyzer.fetchAndStoreAcquire(p);
}

//------------------------------------------------------------------------------
// Name: analyzer()
// Desc:
//------------------------------------------------------------------------------
AnalyzerInterface *edb::v1::analyzer() {
	return g_Analyzer;
}

//------------------------------------------------------------------------------
// Name: set_session_file_handler(SessionFileInterface *p)
// Desc:
//------------------------------------------------------------------------------
SessionFileInterface *edb::v1::set_session_file_handler(SessionFileInterface *p) {
	Q_CHECK_PTR(p);
	return g_SessionHandler.fetchAndStoreAcquire(p);
}

//------------------------------------------------------------------------------
// Name: session_file_handler()
// Desc:
//------------------------------------------------------------------------------
SessionFileInterface *edb::v1::session_file_handler() {
	return g_SessionHandler;;
}

//------------------------------------------------------------------------------
// Name: set_debug_event_handler(DebugEventHandlerInterface *p)
// Desc:
//------------------------------------------------------------------------------
DebugEventHandlerInterface *edb::v1::set_debug_event_handler(DebugEventHandlerInterface *p) {
	Q_CHECK_PTR(p);
	return g_DebugEventHandler.fetchAndStoreAcquire(p);
}

//------------------------------------------------------------------------------
// Name: debug_event_handler();
// Desc:
//------------------------------------------------------------------------------
DebugEventHandlerInterface *edb::v1::debug_event_handler() {
	return g_DebugEventHandler;
}

//------------------------------------------------------------------------------
// Name: jump_to_address(edb::address_t address)
// Desc: sets the disassembly display to a given address, returning success
//       status
//------------------------------------------------------------------------------
bool edb::v1::jump_to_address(edb::address_t address) {
	DebuggerMain *const gui = ui();
	Q_CHECK_PTR(gui);
	return gui->jump_to_address(address);
}

//------------------------------------------------------------------------------
// Name: dump_data_range(edb::address_t address, edb::address_t end_address, bool newTab)
// Desc: shows a given address through a given end address in the data view,
//       optionally in a new tab
//------------------------------------------------------------------------------
bool edb::v1::dump_data_range(edb::address_t address, edb::address_t end_address, bool new_tab) {
	DebuggerMain *const gui = ui();
	Q_CHECK_PTR(gui);
	return gui->dump_data_range(address, end_address, new_tab);
}

//------------------------------------------------------------------------------
// Name: dump_data_range(edb::address_t address, edb::address_t end_address)
// Desc: shows a given address through a given end address in the data view
//------------------------------------------------------------------------------
bool edb::v1::dump_data_range(edb::address_t address, edb::address_t end_address) {
	return dump_data_range(address, end_address, false);
}

//------------------------------------------------------------------------------
// Name: dump_stack(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::dump_stack(edb::address_t address) {
	return dump_stack(address, true);
}

//------------------------------------------------------------------------------
// Name: dump_stack(edb::address_t address, bool scroll_to)
// Desc: shows a given address in the stack view
//------------------------------------------------------------------------------
bool edb::v1::dump_stack(edb::address_t address, bool scroll_to) {
	DebuggerMain *const gui = ui();
	Q_CHECK_PTR(gui);
	return gui->dump_stack(address, scroll_to);
}

//------------------------------------------------------------------------------
// Name: dump_data(edb::address_t address, bool new_tab)
// Desc: shows a given address in the data view, optionally in a new tab
//------------------------------------------------------------------------------
bool edb::v1::dump_data(edb::address_t address, bool new_tab) {
	DebuggerMain *const gui = ui();
	Q_CHECK_PTR(gui);
	return gui->dump_data(address, new_tab);
}

//------------------------------------------------------------------------------
// Name: dump_data(edb::address_t address)
// Desc: shows a given address in the data view
//------------------------------------------------------------------------------
bool edb::v1::dump_data(edb::address_t address) {
	return dump_data(address, false);
}

//------------------------------------------------------------------------------
// Name: set_breakpoint_condition(edb::address_t address, const QString &condition)
// Desc:
//------------------------------------------------------------------------------
void edb::v1::set_breakpoint_condition(edb::address_t address, const QString &condition) {
	Breakpoint::pointer bp = find_breakpoint(address);
	if(bp) {
		bp->condition = condition;
	}
}

//------------------------------------------------------------------------------
// Name: get_breakpoint_condition(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::get_breakpoint_condition(edb::address_t address) {
	QString ret;
	Breakpoint::pointer bp = find_breakpoint(address);
	if(bp) {
		ret = bp->condition;
	}

	return ret;
}


//------------------------------------------------------------------------------
// Name: create_breakpoint(edb::address_t address)
// Desc: adds a breakpoint at a given address
//------------------------------------------------------------------------------
void edb::v1::create_breakpoint(edb::address_t address) {

	MemRegion region;
	memory_regions().sync();
	if(memory_regions().find_region(address, region)) {
		int ret = QMessageBox::Yes;

		if(!region.executable() && config().warn_on_no_exec_bp) {
			ret = QMessageBox::question(
				0,
				QT_TRANSLATE_NOOP("edb", "Suspicious breakpoint"),
				QT_TRANSLATE_NOOP("edb",
					"You want to place a breakpoint in a non-executable region.\n"
					"An INT3 breakpoint set on data will not execute and may cause incorrect results or crashes.\n"
					"Do you really want to set a breakpoint here?"),
				QMessageBox::Yes, QMessageBox::No);
		} else {
			quint8 buffer[edb::Instruction::MAX_SIZE + 1];
			int size = sizeof(buffer);

			if(edb::v1::get_instruction_bytes(address, buffer, size)) {
				edb::Instruction insn(buffer, size, address, std::nothrow);
				if(!insn.valid()) {
					ret = QMessageBox::question(
						0,
						QT_TRANSLATE_NOOP("edb", "Suspicious breakpoint"),
						QT_TRANSLATE_NOOP("edb",
							"It looks like you may be setting an INT3 breakpoint on data.\n"
							"An INT3 breakpoint set on data will not execute and may cause incorrect results or crashes.\n"
							"Do you really want to set a breakpoint here?"),
						QMessageBox::Yes, QMessageBox::No);
				}
			}
		}

		if(ret == QMessageBox::Yes) {
			debugger_core->add_breakpoint(address);
			repaint_cpu_view();
		}


	} else {
		QMessageBox::information(
			0,
			QT_TRANSLATE_NOOP("edb", "Error Setting Breakpoint"),
			QT_TRANSLATE_NOOP("edb", "Sorry, but setting a breakpoint which is not in a valid region is not allowed."));
	}
}

//------------------------------------------------------------------------------
// Name: enable_breakpoint(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
edb::address_t edb::v1::enable_breakpoint(edb::address_t address) {
	if(address != 0) {
		Breakpoint::pointer bp = find_breakpoint(address);
		if(bp && bp->enable()) {
			return address;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: disable_breakpoint(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
edb::address_t edb::v1::disable_breakpoint(edb::address_t address) {
	if(address != 0) {
		Breakpoint::pointer bp = find_breakpoint(address);
		if(bp && bp->disable()) {
			return address;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: toggle_breakpoint(edb::address_t address)
// Desc: toggles the existence of a breakpoint at a given address
//------------------------------------------------------------------------------
void edb::v1::toggle_breakpoint(edb::address_t address) {
	if(find_breakpoint(address)) {
		remove_breakpoint(address);
	} else {
		create_breakpoint(address);
	}
}

//------------------------------------------------------------------------------
// Name: remove_breakpoint(edb::address_t address)
// Desc: removes a breakpoint
//------------------------------------------------------------------------------
void edb::v1::remove_breakpoint(edb::address_t address) {
	debugger_core->remove_breakpoint(address);
	repaint_cpu_view();
}

//------------------------------------------------------------------------------
// Name: eval_expression(const QString &expression, edb::address_t &value
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::eval_expression(const QString &expression, edb::address_t &value) {
	Expression<edb::address_t> expr(expression, get_variable, get_value);
	ExpressionError err;

	bool ok;
	const edb::address_t address = expr.evaluate_expression(ok, err);
	if(ok) {
		value = address;
		return true;
	} else {
		QMessageBox::information(debugger_ui, QT_TRANSLATE_NOOP("edb", "Error In Expression!"), err.what());
		return false;
	}
}

//------------------------------------------------------------------------------
// Name: get_expression_from_user(const QString &title, const QString prompt, edb::address_t &value)
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::get_expression_from_user(const QString &title, const QString prompt, edb::address_t &value) {
	bool ok;
    const QString text = QInputDialog::getText(debugger_ui, title, prompt, QLineEdit::Normal, QString(), &ok);

	if(ok && !text.isEmpty()) {
		return eval_expression(text, value);
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: get_value_from_user(edb::reg_t &value)
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::get_value_from_user(edb::reg_t &value) {
	return get_value_from_user(value, QT_TRANSLATE_NOOP("edb", "Input Value"));
}

//------------------------------------------------------------------------------
// Name: get_value_from_user(edb::reg_t &value, const QString &title)
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::get_value_from_user(edb::reg_t &value, const QString &title) {
	static DialogInputValue *const dlg = new DialogInputValue(debugger_ui);
	bool ret = false;

	dlg->setWindowTitle(title);
	dlg->set_value(value);
	if(dlg->exec() == QDialog::Accepted) {
		value = dlg->value();
		ret = true;
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: get_binary_string_from_user(QByteArray &value, const QString &title)
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::get_binary_string_from_user(QByteArray &value, const QString &title) {
	return get_binary_string_from_user(value, title, 10);
}

//------------------------------------------------------------------------------
// Name: get_binary_string_from_user(QByteArray &value, const QString &title, int max_length)
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::get_binary_string_from_user(QByteArray &value, const QString &title, int max_length) {
	static DialogInputBinaryString *const dlg = new DialogInputBinaryString(debugger_ui);

	bool ret = false;

	dlg->setWindowTitle(title);

	BinaryString *const bs = dlg->binary_string();

	// set the max length BEFORE the value! (or else we truncate incorrectly)
	if(value.length() <= max_length) {
		bs->setMaxLength(max_length);
	}

	bs->setValue(value);

	if(dlg->exec() == QDialog::Accepted) {
		value = bs->value();
		ret = true;
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: dialog_options()
// Desc: returns a pointer to the options dialog
//------------------------------------------------------------------------------
QDialog *edb::v1::dialog_options() {
	static QDialog *const dialog = new DialogOptions(debugger_ui);
	return dialog;
}

//------------------------------------------------------------------------------
// Name: config()
// Desc:
//------------------------------------------------------------------------------
Configuration &edb::v1::config() {
	static Configuration g_Configuration;
	return g_Configuration;
}

//------------------------------------------------------------------------------
// Name: get_ascii_string_at_address(edb::address_t address, QString &s, int min_length, int max_length, int &found_length)
// Desc: attempts to get a string at a given address whose length is >= min_length
//       and < max_length
// Note: strings are comprised of printable characters and whitespace.
// Note: found_length is needed because we replace characters which need an
//       escape char with the escape sequence (thus the resultant string may be
//       longer than the original). found_length is the original length.
//------------------------------------------------------------------------------
bool edb::v1::get_ascii_string_at_address(edb::address_t address, QString &s, int min_length, int max_length, int &found_length) {

	bool is_string = false;

	if(debugger_core != 0) {
		s.clear();

		if(min_length <= max_length) {
			while(max_length--) {
				char ch;
				if(!debugger_core->read_bytes(address++, &ch, sizeof(ch))) {
					break;
				}

				const int ascii_char = static_cast<unsigned char>(ch);
				if(ascii_char < 0x80 && (std::isprint(ascii_char) || std::isspace(ascii_char))) {
					s += ch;
				} else {
					break;
				}
			}
		}

		is_string = s.length() >= min_length;

		if(is_string) {
			found_length = s.length();
			s.replace("\r", "\\r");
			s.replace("\n", "\\n");
			s.replace("\t", "\\t");
			s.replace("\v", "\\v");
			s.replace("\"", "\\\"");
		}
	}

	return is_string;
}

//------------------------------------------------------------------------------
// Name: get_utf16_string_at_address(edb::address_t address, QString &s, int min_length, int max_length, int &found_length)
// Desc: attempts to get a string at a given address whose length os >= min_length
//       and < max_length
// Note: strings are comprised of printable characters and whitespace.
// Note: found_length is needed because we replace characters which need an
//       escape char with the escape sequence (thus the resultant string may be
//       longer than the original). found_length is the original length.
//------------------------------------------------------------------------------
bool edb::v1::get_utf16_string_at_address(edb::address_t address, QString &s, int min_length, int max_length, int &found_length) {
	bool is_string = false;
	if(debugger_core != 0) {
		s.clear();

		if(min_length <= max_length) {
			while(max_length--) {

				quint16 val;
				if(!debugger_core->read_bytes(address, &val, sizeof(val))) {
					break;
				}

				address += sizeof(val);

				QChar ch(val);

				// for now, we only acknowledge ASCII chars encoded as unicode
				const int ascii_char = ch.toAscii();
				if(ascii_char >= 0x20 && ascii_char < 0x80) {
					s += ch;
				} else {
					break;
				}
			}
		}

		is_string = s.length() >= min_length;

		if(is_string) {
			found_length = s.length();
			s.replace("\r", "\\r");
			s.replace("\n", "\\n");
			s.replace("\t", "\\t");
			s.replace("\v", "\\v");
			s.replace("\"", "\\\"");
		}
	}
	return is_string;
}

//------------------------------------------------------------------------------
// Name: find_function_symbol(edb::address_t address, const QString &default_value, int *offset)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::find_function_symbol(edb::address_t address, const QString &default_value, int *offset) {
	QString symname(default_value);
	int off;

	if(function_symbol_base(address, symname, off)) {
		symname = QString("%1+%2").arg(symname).arg(off, 0, 16);
		if(offset != 0) {
			*offset = off;
		}
	}

	return symname;
}

//------------------------------------------------------------------------------
// Name: find_function_symbol(edb::address_t address, const QString &default_value)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::find_function_symbol(edb::address_t address, const QString &default_value) {
	return find_function_symbol(address, default_value, 0);
}

//------------------------------------------------------------------------------
// Name: find_function_symbol(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::find_function_symbol(edb::address_t address) {
	return find_function_symbol(address, QString(), 0);
}

//------------------------------------------------------------------------------
// Name: get_variable(QString &s, bool &ok, ExpressionError &err)
// Desc:
//------------------------------------------------------------------------------
edb::address_t edb::v1::get_variable(const QString &s, bool &ok, ExpressionError &err) {

	Q_CHECK_PTR(debugger_core);


	State state;
	edb::v1::debugger_core->get_state(state);
	const Register reg = state.value(s);
	ok = reg;
	if(!ok) {
		err = ExpressionError(ExpressionError::UNKNOWN_VARIABLE);
	}

	if(reg.type() == Register::TYPE_SEG) {
		return reg.segment_base();
	}

	return *reg;
}

//------------------------------------------------------------------------------
// Name: get_value(edb::address_t address, bool &ok, ExpressionError &err)
// Desc:
//------------------------------------------------------------------------------
edb::address_t edb::v1::get_value(edb::address_t address, bool &ok, ExpressionError &err) {

	Q_CHECK_PTR(debugger_core);

	edb::address_t ret = 0;

	ok = debugger_core->read_bytes(address, &ret, sizeof(ret));

	if(!ok) {
		err = ExpressionError(ExpressionError::CANNOT_READ_MEMORY);
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: get_instruction_bytes(edb::address_t address, quint8 *buf, int &size)
// Desc: attempts to read at most size bytes, but will retry using smaller sizes as needed
//------------------------------------------------------------------------------
bool edb::v1::get_instruction_bytes(edb::address_t address, quint8 *buf, int &size) {

	Q_CHECK_PTR(debugger_core);
	Q_ASSERT(size >= 0);

	bool ok = debugger_core->read_bytes(address, buf, size);

	while(!ok && size) {
		ok = debugger_core->read_bytes(address, buf, --size);
	}

	return ok;
}

//------------------------------------------------------------------------------
// Name: get_binary_info(const MemRegion &region)
// Desc: gets an object which knows how to analyze the binary file provided
//       or NULL if none-found
//------------------------------------------------------------------------------
BinaryInfo *edb::v1::get_binary_info(const MemRegion &region) {
	Q_FOREACH(BinaryInfo::create_func_ptr_t f, g_BinaryInfoList) {
		BinaryInfo *const p = (*f)(region);

		if(p->validate_header()) {
			return p;
		}

		delete p;
	}

	return 0;
}

//------------------------------------------------------------------------------
// Name: locate_main_function()
// Desc:
// Note: this currently only works for glibc linked elf files
//------------------------------------------------------------------------------
edb::address_t edb::v1::locate_main_function() {

	const MemRegion region = primary_code_region();

	SCOPED_POINTER<BinaryInfo> binfo(get_binary_info(region));
	
	if(binfo) {
		const edb::address_t main_func = binfo->calculate_main();
		if(main_func != 0) {
			return main_func;
		} else {
			return binfo->entry_point();
		}
	}
	
	return 0;
}

//------------------------------------------------------------------------------
// Name: plugin_list()
// Desc:
//------------------------------------------------------------------------------
const QHash<QString, QObject *> &edb::v1::plugin_list() {
	return g_GeneralPlugins;
}

//------------------------------------------------------------------------------
// Name: find_plugin_by_name(const QString &name)
// Desc: gets a pointer to a plugin based on it's classname
//------------------------------------------------------------------------------
DebuggerPluginInterface *edb::v1::find_plugin_by_name(const QString &name) {
	Q_FOREACH(QObject *p, g_GeneralPlugins) {
		if(name == p->metaObject()->className()) {
			return qobject_cast<DebuggerPluginInterface *>(p);
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: reload_symbols()
// Desc:
//------------------------------------------------------------------------------
void edb::v1::reload_symbols() {
	symbol_manager().load_symbols(edb::v1::config().symbol_path);
}

//------------------------------------------------------------------------------
// Name: get_function_info(const QString &function)
// Desc:
//------------------------------------------------------------------------------
const FunctionInfo *edb::v1::get_function_info(const QString &function) {

	QHash<QString, FunctionInfo>::const_iterator it = g_FunctionDB.find(function);
	if(it != g_FunctionDB.end()) {
		return &(it.value());	
	}

	return 0;
}

//------------------------------------------------------------------------------
// Name: primary_data_region()
// Desc: returns the main .data section of the main executable module
// Note: make sure that memory regions has been sync'd first or you will like
//       get a null-region result
//------------------------------------------------------------------------------
MemRegion edb::v1::primary_data_region() {
	const QList<MemRegion> &l = edb::v1::memory_regions().regions();

	if(l.size() >= 2) {
		return l[1];
	} else {
		return MemRegion();
	}
}

//------------------------------------------------------------------------------
// Name: pop_value(State &state)
// Desc:
//------------------------------------------------------------------------------
void edb::v1::pop_value(State &state) {
	state.adjust_stack(sizeof(edb::reg_t));
}

//------------------------------------------------------------------------------
// Name: push_value(State &state, edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void edb::v1::push_value(State &state, edb::reg_t value) {
	state.adjust_stack(- static_cast<int>(sizeof(edb::reg_t)));
	edb::v1::debugger_core->write_bytes(state.stack_pointer(), &value, sizeof(edb::reg_t));
}

//------------------------------------------------------------------------------
// Name: register_binary_info(createFuncPtr fptr)
// Desc:
//------------------------------------------------------------------------------
void edb::v1::register_binary_info(BinaryInfo::create_func_ptr_t fptr) {
	if(!g_BinaryInfoList.contains(fptr)) {
		g_BinaryInfoList.push_back(fptr);
	}
}

//------------------------------------------------------------------------------
// Name: edb_version()
// Desc: returns an integer comparable version of our current version string
//------------------------------------------------------------------------------
quint32 edb::v1::edb_version() {
	return int_version(version);
}

//------------------------------------------------------------------------------
// Name: overwrite_check(edb::address_t address, unsigned int size)
// Desc:
//------------------------------------------------------------------------------
bool edb::v1::overwrite_check(edb::address_t address, unsigned int size) {
	bool firstConflict = true;
	for(edb::address_t addr = address; addr != (address + size); ++addr) {
		Breakpoint::pointer bp = find_breakpoint(addr);

		if(bp && bp->enabled()) {
			if(firstConflict) {
				const int ret = QMessageBox::question(
						0,
						QT_TRANSLATE_NOOP("edb", "Overwritting breakpoint"),
						QT_TRANSLATE_NOOP("edb", "You are attempting to modify bytes which overlap with a software breakpoint. Doing this will implicitly remove any breakpoints which are a conflict. Are you sure you want to do this?"),
						QMessageBox::Yes,
						QMessageBox::No);

				if(ret == QMessageBox::No) {
					return false;
				}
				firstConflict = false;
			}

			remove_breakpoint(addr);
		}
	}
	return true;
}

//------------------------------------------------------------------------------
// Name: modify_bytes(edb::address_t address, unsigned int size, QByteArray &bytes, quint8 fill
// Desc:
//------------------------------------------------------------------------------
void edb::v1::modify_bytes(edb::address_t address, unsigned int size, QByteArray &bytes, quint8 fill) {

	if(size != 0) {
		if(get_binary_string_from_user(bytes, QT_TRANSLATE_NOOP("edb", "Edit Binary String"), size)) {
			if(overwrite_check(address, size)) {

				// fill bytes
				while(bytes.size() < static_cast<int>(size)) {
					bytes.push_back(fill);
				}

				debugger_core->write_bytes(address, bytes.data(), size);

				// do a refresh, not full update
				DebuggerMain *const gui = ui();
				Q_CHECK_PTR(gui);
				gui->refresh_gui();
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: get_md5(const void *p, size_t n)
// Desc:
//------------------------------------------------------------------------------
QByteArray edb::v1::get_md5(const void *p, size_t n) {
	MD5 md5(p, n);

	// make a deep copy because MD5 is about to go out of scope
	return QByteArray(reinterpret_cast<const char *>(md5.digest()), 16);
}

//------------------------------------------------------------------------------
// Name: get_file_md5(const QString &s)
// Desc: returns a byte array representing the MD5 of a file
//------------------------------------------------------------------------------
QByteArray edb::v1::get_file_md5(const QString &s) {

	QFile file(s);
	file.open(QIODevice::ReadOnly);
	if(file.isOpen()) {
		const QByteArray file_bytes = file.readAll();
		return get_md5(file_bytes.data(), file_bytes.size());
	}

	return QByteArray();
}


//------------------------------------------------------------------------------
// Name: basename(const QString &s)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::basename(const QString &s) {

	const QFileInfo fileInfo(s);
	QString ret = fileInfo.baseName();

	if(!fileInfo.completeSuffix().isEmpty()) {
		ret += '.';
		ret += fileInfo.completeSuffix();
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: symlink_target(const QString &s)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::symlink_target(const QString &s) {
	return QFileInfo(s).symLinkTarget();
}

//------------------------------------------------------------------------------
// Name: int_version()
// Desc: returns an integer comparable version of a version string in x.y.z
//       format, or 0 if error
//------------------------------------------------------------------------------
quint32 edb::v1::int_version(const QString &s) {

	ulong ret = 0;
	const QStringList list = s.split(".");
	if(list.size() == 3) {
		bool ok[3];
		const unsigned int maj = list[0].toUInt(&ok[0]);
		const unsigned int min = list[1].toUInt(&ok[1]);
		const unsigned int rev = list[2].toUInt(&ok[2]);
		if(ok[0] && ok[1] && ok[2]) {
			ret = (maj << 12) | (min << 8) | (rev);
		}
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: parse_command_line(const QString &cmdline)
// Desc:
//------------------------------------------------------------------------------
QStringList edb::v1::parse_command_line(const QString &cmdline) {

	QStringList args;
	QString arg;

	int bcount = 0;
	bool in_quotes = false;

	QString::const_iterator s = cmdline.begin();

	while(s != cmdline.end()) {
		if(!in_quotes && s->isSpace()) {

			// Close the argument and copy it
			args << arg;
			arg.clear();

			// skip the remaining spaces
			do {
				++s;
			} while(s->isSpace());

			// Start with a new argument
			bcount = 0;
		} else if(*s == '\\') {

			// '\\'
			arg += *s++;
			++bcount;

		} else if(*s == '"') {

			// '"'
			if((bcount & 1) == 0) {
				/* Preceded by an even number of '\', this is half that
				 * number of '\', plus a quote which we erase.
				 */

				arg.chop(bcount / 2);
				in_quotes = !in_quotes;
			} else {
				/* Preceded by an odd number of '\', this is half that
				 * number of '\' followed by a '"'
				 */

				arg.chop(bcount / 2 + 1);
				arg += '"';
			}

			++s;
			bcount = 0;
		} else {
			arg += *s++;
			bcount = 0;
		}
	}

	if(!arg.isEmpty()) {
		args << arg;
	}

	return args;
}

//------------------------------------------------------------------------------
// Name: string_to_address(const QString &s, bool &ok)
// Desc:
//------------------------------------------------------------------------------
edb::address_t edb::v1::string_to_address(const QString &s, bool &ok) {
#if defined(EDB_X86)
	return s.left(8).toULongLong(&ok, 16);
#elif defined(EDB_X86_64)
	return s.left(16).toULongLong(&ok, 16);
#endif
}

//------------------------------------------------------------------------------
// Name: format_bytes(const QByteArray &x)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::format_bytes(const QByteArray &x) {
	QString bytes;

	if(x.size() != 0) {
		QString temp;
		temp.reserve(4);
		QByteArray::const_iterator it = x.begin();
		bytes += temp.sprintf("%02x", *it++ & 0xff);
		while(it != x.end()) {
			bytes += temp.sprintf(" %02x", *it++ & 0xff);
		}
	}

	return bytes;
}

//------------------------------------------------------------------------------
// Name: format_pointer(edb::address_t p)
// Desc:
//------------------------------------------------------------------------------
QString edb::v1::format_pointer(edb::address_t p) {
	return QString("%1").arg(p, EDB_MAX_HEX, 16, QChar('0'));
}

//------------------------------------------------------------------------------
// Name: current_data_view_address()
// Desc:
//------------------------------------------------------------------------------
edb::address_t edb::v1::current_data_view_address() {
	return qobject_cast<QHexView *>(ui()->ui->tabWidget->currentWidget())->firstVisibleAddress();
}

//------------------------------------------------------------------------------
// Name: set_status(const QString &message
// Desc:
//------------------------------------------------------------------------------
void edb::v1::set_status(const QString &message) {
	ui()->ui->statusbar->showMessage(message, 0);
}

//------------------------------------------------------------------------------
// Name: find_breakpoint(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
Breakpoint::pointer edb::v1::find_breakpoint(edb::address_t address) {
	if(edb::v1::debugger_core != 0) {
		return debugger_core->find_breakpoint(address);
	}
	return Breakpoint::pointer();
}

//------------------------------------------------------------------------------
// Name: pointer_size()
// Desc:
//------------------------------------------------------------------------------
int edb::v1::pointer_size() {
	
	if(edb::v1::debugger_core != 0) {
		return edb::v1::debugger_core->pointer_size();
	}
	
	return 0;
}

//------------------------------------------------------------------------------
// Name: pointer_size()
// Desc:
//------------------------------------------------------------------------------
QWidget *edb::v1::disassembly_widget() {
	return ui()->ui->cpuView;
}

//------------------------------------------------------------------------------
// Name: serialize_object(const QObject *object)
// Desc:
//------------------------------------------------------------------------------
QByteArray edb::v1::serialize_object(const QObject *object) {
	QVariantMap variant = QJson::QObjectHelper::qobject2qvariant(object);
	QJson::Serializer serializer;
	return serializer.serialize(variant);
}
