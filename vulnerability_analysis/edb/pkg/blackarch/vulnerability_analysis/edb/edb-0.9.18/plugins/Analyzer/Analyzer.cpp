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

#include "Analyzer.h"
#include "AnalyzerOptionsPage.h"
#include "AnalyzerWidget.h"
#include "ArchProcessorInterface.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "DialogSpecifiedFunctions.h"
#include "Instruction.h"
#include "MemoryRegions.h"
#include "State.h"
#include "SymbolManagerInterface.h"
#include "Util.h"
#include "BinaryInfo.h"

#include <QMainWindow>
#include <QToolBar>
#include <QCoreApplication>
#include <QHash>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QStack>
#include <QTime>
#include <QtDebug>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#ifdef USE_QT_CONCURRENT
#include <QtConcurrentMap>
#endif

#include <cstring>

#define MIN_REFCOUNT 2

namespace {
#if defined(EDB_X86)
	const edb::Operand::Register STACK_REG = edb::Operand::REG_ESP;
	const edb::Operand::Register FRAME_REG = edb::Operand::REG_EBP;
#elif defined(EDB_X86_64)
	const edb::Operand::Register STACK_REG = edb::Operand::REG_RSP;
	const edb::Operand::Register FRAME_REG = edb::Operand::REG_RBP;
#endif
}

//------------------------------------------------------------------------------
// Name: Analyzer(
// Desc:
//------------------------------------------------------------------------------
Analyzer::Analyzer() : menu_(0), analyzer_widget_(0) {
}

//------------------------------------------------------------------------------
// Name: options_page()
// Desc:
//------------------------------------------------------------------------------
QWidget *Analyzer::options_page() {
	return new AnalyzerOptionsPage;
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *Analyzer::menu(QWidget *parent) {
	if(menu_ == 0) {
		menu_ = new QMenu(tr("Analyzer"), parent);
		menu_->addAction(tr("Show &Specified Functions"), this, SLOT(show_specified()));
#if defined(EDB_X86)
		menu_->addAction(tr("&Analyze EIP's Region"), this, SLOT(do_ip_analysis()), QKeySequence(tr("Ctrl+A")));
#elif defined(EDB_X86_64)
		menu_->addAction(tr("&Analyze RIP's Region"), this, SLOT(do_ip_analysis()), QKeySequence(tr("Ctrl+A")));
#endif
		menu_->addAction(tr("&Analyze Viewed Region"), this, SLOT(do_view_analysis()), QKeySequence(tr("Ctrl+Shift+A")));

		// if we are dealing with a main window (and we are...)
		// add the dock object
		if(QMainWindow *const main_window = qobject_cast<QMainWindow *>(parent)) {
			analyzer_widget_ = new AnalyzerWidget;

			// make the toolbar widget and _name_ it, it is important to name it so
			// that it's state is saved in the GUI info
			QToolBar *const toolbar = new QToolBar(tr("Region Analysis"), parent);
			toolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
			toolbar->setObjectName(QString::fromUtf8("Region Analysis"));
			toolbar->addWidget(analyzer_widget_);

			// add it to the dock
			main_window->addToolBar(Qt::TopToolBarArea, toolbar);

			// make the menu and add the show/hide toggle for the widget
			menu_->addAction(toolbar->toggleViewAction());
		}
	}

	return menu_;
}

//------------------------------------------------------------------------------
// Name: private_init()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::private_init() {
	edb::v1::set_analyzer(this);
}

//------------------------------------------------------------------------------
// Name: show_specified()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::show_specified() {
	static QDialog *dialog = new DialogSpecifiedFunctions(edb::v1::debugger_ui);
	dialog->show();
}

//------------------------------------------------------------------------------
// Name: do_ip_analysis()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::do_ip_analysis() {
	MemRegion region;
	
	State state;
	edb::v1::debugger_core->get_state(state);
	
	const edb::address_t eip = state.instruction_pointer();
	if(edb::v1::memory_regions().find_region(eip, region)) {
		do_analysis(region);
	}
}

//------------------------------------------------------------------------------
// Name: do_view_analysis()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::do_view_analysis() {
	do_analysis(edb::v1::current_cpu_view_region());
}

//------------------------------------------------------------------------------
// Name: mark_function_start()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::mark_function_start() {
	const edb::address_t address = edb::v1::cpu_selected_address();
	MemRegion region;
	if(edb::v1::memory_regions().find_region(address, region)) {
		qDebug("Added %p to the list of known functions", reinterpret_cast<void *>(address));
		specified_functions_.insert(address);
		invalidate_dynamic_analysis(region);
	}
}

//------------------------------------------------------------------------------
// Name: goto_function_start()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::goto_function_start() {

	const edb::address_t address = edb::v1::cpu_selected_address();

	Function function;
	if(find_containing_function(address, function)) {
		edb::v1::jump_to_address(function.entry_address);
		return;
	}

	QMessageBox::information(
		0,
		tr("Goto Function Start"),
		tr("The selected instruction is not inside of a known function. Have you run an analysis of this region?"));
}

//------------------------------------------------------------------------------
// Name: goto_function_end()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::goto_function_end() {

	const edb::address_t address = edb::v1::cpu_selected_address();

	Function function;
	if(find_containing_function(address, function)) {
		edb::v1::jump_to_address(function.last_instruction);
		return;
	}

	QMessageBox::information(
		0,
		tr("Goto Function End"),
		tr("The selected instruction is not inside of a known function. Have you run an analysis of this region?"));
}

//------------------------------------------------------------------------------
// Name: cpu_context_menu()
// Desc:
//------------------------------------------------------------------------------
QList<QAction *> Analyzer::cpu_context_menu() {

	QList<QAction *> ret;

	QAction *const action_find                = new QAction(tr("Analyze Here"), this);
	QAction *const action_goto_function_start = new QAction(tr("Goto Function Start"), this);
	QAction *const action_goto_function_end   = new QAction(tr("Goto Function End"), this);
	QAction *const action_mark_function_start = new QAction(tr("Mark As Function Start"), this);

	connect(action_find, SIGNAL(triggered()), this, SLOT(do_view_analysis()));
	connect(action_goto_function_start, SIGNAL(triggered()), this, SLOT(goto_function_start()));
	connect(action_goto_function_end, SIGNAL(triggered()),   this, SLOT(goto_function_end()));
	connect(action_mark_function_start, SIGNAL(triggered()), this, SLOT(mark_function_start()));
	ret << action_find << action_goto_function_start << action_goto_function_end << action_mark_function_start;

	return ret;
}

//------------------------------------------------------------------------------
// Name: do_analysis(const MemRegion &region)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::do_analysis(const MemRegion &region) {
	if(region.size() != 0) {
		QProgressDialog progress(tr("Performing Analysis"), 0, 0, 100, edb::v1::debugger_ui);
		connect(this, SIGNAL(update_progress(int)), &progress, SLOT(setValue(int)));
		progress.show();
		progress.setValue(0);
		analyze(region);
		edb::v1::repaint_cpu_view();
	}
}

//------------------------------------------------------------------------------
// Name: find_function_calls(const MemRegion &region, FunctionMap &found_functions)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::find_function_calls(const MemRegion &region, FunctionMap &found_functions) {
	static const edb::address_t page_size = edb::v1::debugger_core->page_size();

	const edb::address_t size_in_pages = region.size() / page_size;

	try {
		QVector<quint8> pages(size_in_pages * page_size);

		if(edb::v1::debugger_core->read_pages(region.start, &pages[0], size_in_pages)) {
			for(edb::address_t i = 0; i < size_in_pages * page_size; ++i) {

				const edb::Instruction insn(&pages[i], region.size() - i, region.start + i, std::nothrow);

				if(insn.valid() && insn.type() == edb::Instruction::OP_CALL) {

					const edb::address_t ip = region.start + i;
					const edb::Operand &op = insn.operand(0);

					if(op.general_type() == edb::Operand::TYPE_REL) {
						const edb::address_t ea = op.relative_target();

						// skip over ones which are : call <label>; label:
						if(ea != ip + insn.size()) {
							if(region.contains(ea)) {
								// avoid calls which land in the middle of a function...
								// this may or may not be the best approach
								if(!is_inside_known(region, ea)) {
									found_functions[ea].entry_address = ea;
									found_functions[ea].end_address   = ea;
									found_functions[ea].reference_count++;
								}
							}
						}
					}

					emit update_progress(util::percentage(6, 10, i, region.size()));
				}
			}
		}
	} catch(const std::bad_alloc &) {
		QMessageBox::information(0, tr("Memroy Allocation Error"),
			tr("Unable to satisfy memory allocation request for requested region."));
	}
}

//------------------------------------------------------------------------------
// Name: is_stack_frame(edb::address_t addr)
// Desc:
//------------------------------------------------------------------------------
bool Analyzer::is_stack_frame(edb::address_t addr) const {

	quint8 buf[edb::Instruction::MAX_SIZE];

	unsigned int i = 0;

	while(i < 2) {
		// gets the bytes for the instruction
		int buf_size = sizeof(buf);
		if(!edb::v1::get_instruction_bytes(addr, buf, buf_size)) {
			break;
		}

		// decode it
		const edb::Instruction insn(buf, buf_size, addr, std::nothrow);
		if(!insn.valid()) {
			break;;
		}

		// which part of the sequence are we looking at?
		switch(i++) {
		case 0:
			// are we looking at a 'push rBP' ?
			if(insn.type() == edb::Instruction::OP_PUSH) {
				Q_ASSERT(insn.operand_count() == 1);
				const edb::Operand &op = insn.operand(0);
				if(op.complete_type() == edb::Operand::TYPE_REGISTER) {
					if(op.reg() == FRAME_REG) {
						break;
					}
				}
			// if it is an 'enter 0,0' instruction, then it's a stack frame right away
			} else if(insn.type() == edb::Instruction::OP_ENTER) {
				Q_ASSERT(insn.operand_count() == 2);
				const edb::Operand &op0 = insn.operand(0);
				const edb::Operand &op1 = insn.operand(1);
				if(op0.immediate() == 0 && op1.immediate() == 0) {
					return true;
				}
			}
			++i;
			break;
		case 1:
			// are we looking at a 'mov rBP, rSP' ?
			if(insn.type() == edb::Instruction::OP_MOV) {
				Q_ASSERT(insn.operand_count() == 2);
				const edb::Operand &op0 = insn.operand(0);
				const edb::Operand &op1 = insn.operand(1);
				if(op0.complete_type() == edb::Operand::TYPE_REGISTER && op1.complete_type() == edb::Operand::TYPE_REGISTER) {
					if(op0.reg() == FRAME_REG && op1.reg() == STACK_REG) {
						return true;
					}
				}
			}
			break;
		default:
			break;
		}

		addr += insn.size();
	}

	return false;
}

//------------------------------------------------------------------------------
// Name: bonus_stack_frames_helper(Function &info) const
// Desc:
//------------------------------------------------------------------------------
void Analyzer::bonus_stack_frames_helper(Function &info) const {
	if(is_stack_frame(info.entry_address)) {
		info.reference_count++;
	}
}

//------------------------------------------------------------------------------
// Name: bonus_stack_frames(FunctionMap &results)
// Desc: give bonus if we see a "push ebp; mov ebp, esp;"
//------------------------------------------------------------------------------
void Analyzer::bonus_stack_frames(FunctionMap &results) {
#ifdef USE_QT_CONCURRENT
	QtConcurrent::blockingMap(
		results,
		boost::bind(&Analyzer::bonus_stack_frames_helper, this, _1));
#else
	std::for_each(
		results.begin(),
		results.end(),
		boost::bind(&Analyzer::bonus_stack_frames_helper, this, _1));
#endif
}

//------------------------------------------------------------------------------
// Name: update_results_entry(FunctionMap &results, edb::address_t address) const
// Desc:
//------------------------------------------------------------------------------
void Analyzer::update_results_entry(FunctionMap &results, edb::address_t address) const {
	results[address].entry_address = address;
	results[address].end_address   = address;
	
	if(results[address].reference_count == 0) {
		results[address].reference_count = MIN_REFCOUNT;
	} else {
		results[address].reference_count++;
	}
}

//------------------------------------------------------------------------------
// Name: bonus_main(const MemRegion &region, FunctionMap &results)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::bonus_main(const MemRegion &region, FunctionMap &results) const {
	const QString s = edb::v1::get_process_exe();
	if(!s.isEmpty()) {
		const edb::address_t main = edb::v1::locate_main_function();

		if(main != 0 && region.contains(main)) {
			// make sure we have an entry for this function
			update_results_entry(results, main);
		}
	}
}

//------------------------------------------------------------------------------
// Name: bonus_symbols_helper(const MemRegion &region, FunctionMap &results, const Symbol::pointer &sym)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::bonus_symbols_helper(const MemRegion &region, FunctionMap &results, const Symbol::pointer &sym) {
	const edb::address_t addr = sym->address;

	if(region.contains(addr) && sym->is_code()) {
		qDebug("[Analyzer] adding: %s <%p>", qPrintable(sym->name), reinterpret_cast<void *>(addr));

		// make sure we have an entry for this function
		update_results_entry(results, addr);
	}
}


//------------------------------------------------------------------------------
// Name: bonus_symbols(const MemRegion &region, FunctionMap &results)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::bonus_symbols(const MemRegion &region, FunctionMap &results) {

	// give bonus if we have a symbol for the address
	const QList<Symbol::pointer> symbols = edb::v1::symbol_manager().symbols();

	std::for_each(
		symbols.begin(),
		symbols.end(),
		boost::bind(&Analyzer::bonus_symbols_helper, this, region, boost::ref(results), _1));
}

//------------------------------------------------------------------------------
// Name: bonus_marked_functions(const MemRegion &region, FunctionMap &results)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::bonus_marked_functions(const MemRegion &region, FunctionMap &results) {

	Q_FOREACH(edb::address_t addr, specified_functions_) {
		if(region.contains(addr)) {

			qDebug("[Analyzer] adding: <%p>", reinterpret_cast<void *>(addr));

			// make sure we have an entry for this function
			update_results_entry(results, addr);
		}
	}
}

//------------------------------------------------------------------------------
// Name: walk_all_functions(FunctionMap &results, const MemRegion &region, QSet<edb::address_t> &walked_functions)
// Desc:
//------------------------------------------------------------------------------
int Analyzer::walk_all_functions(FunctionMap &results, const MemRegion &region, QSet<edb::address_t> &walked_functions) {
	int updates = 0;

	QSet<edb::address_t> found_functions;

	FunctionMap::iterator it = results.begin();
	while(it != results.end()) {
		Function &function = it.value();

		const FunctionMap::iterator next = ++it;

		if(function.reference_count >= MIN_REFCOUNT) {
			if(!walked_functions.contains(function.entry_address)) {

				// the function's upper bound is either the entry point of the next function
				// or the region's end which is the absolute max end this function can have
				const edb::address_t next_entry = (next != results.end()) ? next.value().entry_address : region.end;

				// walk the function and collect some results

				find_function_end(function, next_entry, found_functions, results);
				walked_functions.insert(function.entry_address);

				// if the very last instruction happens to be a jmp, then this may
				// be a call/ret -> jmp optimization. This isn't always the case
				// but often enough that it's probably right
				quint8 buf[edb::Instruction::MAX_SIZE];
				int buf_size = sizeof(buf);
				if(edb::v1::get_instruction_bytes(function.last_instruction, buf, buf_size)) {
					const edb::Instruction insn(buf, buf_size, function.last_instruction, std::nothrow);
					if(insn.valid() && insn.type() == edb::Instruction::OP_JMP) {

						Q_ASSERT(insn.operand_count() == 1);
						const edb::Operand &op = insn.operand(0);

						if(op.general_type() == edb::Operand::TYPE_REL) {
							const edb::address_t target = op.relative_target();

							Function func;
							if(!find_containing_function(target, func)) {
								found_functions.insert(target);
							}
						}
					}
				}
			}
		}

		it = next;
	}

	// add the newly found functions to the list and report the number of "updates"
	Q_FOREACH(edb::address_t func, found_functions) {
		if(!results.contains(func)) {
			results[func].entry_address   = func;
			results[func].end_address     = func;
			results[func].reference_count = MIN_REFCOUNT;
			++updates;
		}
	}

	return updates;
}

//------------------------------------------------------------------------------
// Name: fix_overlaps(FunctionMap &function_map)
// Desc: ensures that no function overlaps another
//------------------------------------------------------------------------------
void Analyzer::fix_overlaps(FunctionMap &function_map) {
	for(FunctionMap::iterator it = function_map.begin(); it != function_map.end(); ) {
		Function &func = *it++;
		if(it != function_map.end()) {
			const Function &next_func = *it;
			if(next_func.entry_address <= func.end_address) {
				func.end_address = next_func.entry_address - 1;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: find_function_end(Function &function, edb::address_t end_address, QSet<edb::address_t> &found_functions, const FunctionMap &results)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::find_function_end(Function &function, edb::address_t end_address, QSet<edb::address_t> &found_functions, const FunctionMap &results) {

	QStack<edb::address_t>		jump_targets;
	QHash<edb::address_t, int>	visited_addresses;

	// we start with the entry point of the function
	jump_targets.push(function.entry_address);

	// while no more jump targets... (includes entry point)
	while(!jump_targets.empty()) {
	
		edb::address_t addr = jump_targets.pop();

		// for certain forward jump scenarioes this is possible.
		if(visited_addresses.contains(addr)) {
			continue;
		}

		// keep going until we go out of bounds
		while(addr >= function.entry_address && addr < end_address) {

			quint8 buf[edb::Instruction::MAX_SIZE];
			int buf_size = sizeof(buf);
			if(!edb::v1::get_instruction_bytes(addr, buf, buf_size)) {
				break;
			}

			// an invalid instruction ends this "block"
			const edb::Instruction insn(buf, buf_size, addr, std::nothrow);
			if(!insn.valid()) {
				break;
			}

			// ok, it was a valid instruction, let's add it to the
			// list of possible 'code addresses'
			visited_addresses.insert(addr, insn.size());

			const edb::Instruction::Type type = insn.type();

			if(type == edb::Instruction::OP_RET || type == edb::Instruction::OP_HLT) {
				// instructions that clearly terminate the current block...
				break;
				
			} else if(type == edb::Instruction::OP_JCC) {

				// note if neccessary the Jcc target and move on, yes this can be fooled by "conditional"
				// jumps which are always true or false, not much we can do about it at this level.
				const edb::Operand &op = insn.operand(0);
				if(op.general_type() == edb::Operand::TYPE_REL) {
					const edb::address_t ea = op.relative_target();

					if(!visited_addresses.contains(ea) && !jump_targets.contains(ea)) {
						jump_targets.push(ea);
					}
				}
			} else if(type == edb::Instruction::OP_CALL) {


				// similar to above, note the destination and move on
				// we special case simple things for speed.
				// also this is an opportunity to find call tables.
				const edb::Operand &op = insn.operand(0);
				if(op.general_type() == edb::Operand::TYPE_REL) {
					const edb::address_t ea = op.relative_target();

					// skip over ones which are: "call <label>; label:"
					if(ea != addr + insn.size()) {
						found_functions.insert(ea);
					}
				} else if(op.general_type() == edb::Operand::TYPE_EXPRESSION) {
					// looks like: "call [...]", if it is of the form, call [C + REG]
					// then it may be a jump table using REG as an offset
				}
			} else if(type == edb::Instruction::OP_JMP) {

				const edb::Operand &op = insn.operand(0);
				if(op.general_type() == edb::Operand::TYPE_REL) {
					const edb::address_t ea = op.relative_target();


					// an absolute jump within this function
					if(ea >= function.entry_address && ea < addr) {
						addr += insn.size();
						continue;
					}


					// is it a jump to another function's entry point?
					// if so, this is a dead end, resume from other branches
					// but give the target a bonus reference
					FunctionMap::const_iterator it = results.find(ea);
					if(it != results.end()) {
						found_functions.insert(ea);
						break;
					}

#if 0
					// ok, it is a jmp to an address we haven't seen yet
					// so just continue processing from there
					if(!visited_addresses.contains(ea)) {
						addr = ea;
						continue;
					} else {
						break;
					}
#else
					break;
#endif
				}
			}

			addr += insn.size();
		}
	}

	function.last_instruction = function.entry_address;
	function.end_address      = function.entry_address;

	// get the last instruction and the last byte of the function
	for(QHash<edb::address_t, int>::const_iterator it = visited_addresses.begin(); it != visited_addresses.end(); ++it) {
		function.end_address      = qMax(function.end_address,      it.key() + it.value() - 1);
		function.last_instruction = qMax(function.last_instruction, it.key());
	}
}

//------------------------------------------------------------------------------
// Name: is_thunk(edb::address_t address)
// Desc: basically returns true if the first instruction of the function is a
//       jmp
//------------------------------------------------------------------------------
bool Analyzer::is_thunk(edb::address_t address) const {
	quint8 buf[edb::Instruction::MAX_SIZE];
	int buf_size = sizeof(buf);
	if(edb::v1::get_instruction_bytes(address, buf, buf_size)) {
		const edb::Instruction insn(buf, buf_size, address, std::nothrow);
		return insn.valid() && insn.type() == edb::Instruction::OP_JMP;
	}

	return false;
}

//------------------------------------------------------------------------------
// Name: set_function_types_helper(Function &info) const
// Desc:
//------------------------------------------------------------------------------
void Analyzer::set_function_types_helper(Function &info) const {

	if(is_thunk(info.entry_address)) {
		info.type = Function::FUNCTION_THUNK;
	} else {
		info.type = Function::FUNCTION_STANDARD;
	}
}

//------------------------------------------------------------------------------
// Name: set_function_types(FunctionMap &results)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::set_function_types(FunctionMap &results) {
	// give bonus if we have a symbol for the address
#ifdef USE_QT_CONCURRENT
	QtConcurrent::blockingMap(
		results,
		boost::bind(&Analyzer::set_function_types_helper, this, _1));
#else
	std::for_each(
		results.begin(),
		results.end(),
		boost::bind(&Analyzer::set_function_types_helper, this, _1));
#endif
}

//------------------------------------------------------------------------------
// Name: is_inside_known(const MemRegion &region, edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
bool Analyzer::is_inside_known(const MemRegion &region, edb::address_t address) {

	const FunctionMap &funcs = functions(region);
	Q_FOREACH(const Function &func, funcs) {
		if(address >= func.entry_address && address <= func.end_address) {
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------------
// Name: find_calls_from_known(const MemRegion &region, FunctionMap &results, QSet<edb::address_t> &walked_functions)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::find_calls_from_known(const MemRegion &region, FunctionMap &results, QSet<edb::address_t> &walked_functions) {
	int updates;
	do {
		updates = walk_all_functions(results, region, walked_functions);
		qDebug() << "[Analyzer] got" << updates << "updates";
	} while(updates != 0);
}

//------------------------------------------------------------------------------
// Name: collect_high_ref_results(FunctionMap &function_map, FunctionMap &found_functions) const
// Desc:
//------------------------------------------------------------------------------
void Analyzer::collect_high_ref_results(FunctionMap &function_map, FunctionMap &found_functions) const {
	for(FunctionMap::iterator it = found_functions.begin(); it != found_functions.end(); ) {
		if(it->reference_count >= MIN_REFCOUNT) {
			function_map[it->entry_address] = *it;
			found_functions.erase(it++);
		} else {
			++it;
		}
	}
}

//------------------------------------------------------------------------------
// Name: 
// Desc:
//------------------------------------------------------------------------------
void Analyzer::collect_low_ref_results(const MemRegion &region, FunctionMap &function_map, FunctionMap &found_functions) {
	// promote weak symbols...
	SymbolManagerInterface &syms = edb::v1::symbol_manager();
	Q_FOREACH(const Function &func, found_functions) {
		if(!is_inside_known(region, func.entry_address)) {
			if(!function_map.contains(func.entry_address)) {
				function_map[func.entry_address] = func;

				const Symbol::pointer s = syms.find(func.entry_address);
				if(s && s->is_weak()) {
					function_map[func.entry_address].reference_count++;
				}

			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: 
// Desc:
//------------------------------------------------------------------------------
void Analyzer::indent_header() {

}

//------------------------------------------------------------------------------
// Name: analyze(const MemRegion &region)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::analyze(const MemRegion &region_ref) {

	QTime t;
	t.start();

	// NOTE: through a series of craziness,
	// bonus_main, calls
	// edb::v1::locate_main_function, which calls
	// edb::v1::primary_code_region, which calls
	// MemoryRegions::sync()
	// which happens to invalidate the references to regions we are potentially passed..
	// so just to be sure, we make a copy! wow, that was an annoying bug!
	MemRegion region(region_ref);

	RegionInfo &region_info = analysis_info_[region];

	QSettings settings;
	const bool fuzzy          = settings.value("Analyzer/fuzzy_logic_functions.enabled", true).toBool();
	const QByteArray md5      = md5_region(region);
	const QByteArray prev_md5 = region_info.md5;
	
	if(md5 != prev_md5 || fuzzy != region_info.fuzzy) {
		FunctionMap &function_map = region_info.analysis;
		function_map.clear();
		
		QSet<edb::address_t> walked_functions;
		FunctionMap          found_functions;
		
		const struct {
			const char             *message;
			boost::function<void()> function;
		} analysis_steps[] = {
			{ "identifying executable headers...",                       boost::bind(&Analyzer::indent_header,          this) },
			{ "adding entry points to the list...", 					 boost::bind(&Analyzer::bonus_entry_point,      this, boost::cref(region), boost::ref(function_map)) },
			{ "attempting to add 'main' to the list...",				 boost::bind(&Analyzer::bonus_main,             this, boost::cref(region), boost::ref(function_map)) },
			{ "attempting to add marked functions to the list...",  	 boost::bind(&Analyzer::bonus_marked_functions, this, boost::cref(region), boost::ref(function_map)) },
			{ "attempting to add functions with symbols to the list...", boost::bind(&Analyzer::bonus_symbols,          this, boost::cref(region), boost::ref(function_map)) },
			{ "calculating function bounds... (pass 1)",                 boost::bind(&Analyzer::find_calls_from_known,  this, boost::cref(region), boost::ref(function_map), boost::ref(walked_functions)) },
		};		
		
		const struct {
			const char             *message;
			boost::function<void()> function;
		} fuzzy_analysis_steps[] = {
			{ "finding possible function calls...",      boost::bind(&Analyzer::find_function_calls,      this, boost::cref(region), boost::ref(found_functions)) },
			{ "bonusing stack frames...",                boost::bind(&Analyzer::bonus_stack_frames,       this, boost::ref(found_functions)) },
			{ "collecting high reference answers...",    boost::bind(&Analyzer::collect_high_ref_results, this, boost::ref(function_map), boost::ref(found_functions)) },
			{ "calculating function bounds... (pass 2)", boost::bind(&Analyzer::find_calls_from_known,    this, boost::cref(region), boost::ref(function_map), boost::ref(walked_functions)) },
			{ "collecting low reference answers...",     boost::bind(&Analyzer::collect_low_ref_results,  this, boost::cref(region), boost::ref(function_map), boost::ref(found_functions)) },
			{ "calculating function bounds... (pass 3)", boost::bind(&Analyzer::find_calls_from_known,    this, boost::cref(region), boost::ref(function_map), boost::ref(walked_functions)) },
		};
		
		const int analysis_steps_count       = sizeof(analysis_steps) / sizeof(analysis_steps[0]);
		const int fuzzy_analysis_steps_count = sizeof(fuzzy_analysis_steps) / sizeof(fuzzy_analysis_steps[0]);
		const int total_steps = analysis_steps_count + fuzzy_analysis_steps_count + 1;
		
		emit update_progress(util::percentage(0, total_steps));
		for(int i = 0; i < analysis_steps_count; ++i) {
			qDebug("[Analyzer] %s", analysis_steps[i].message);
			analysis_steps[i].function();
			emit update_progress(util::percentage(i + 1, total_steps));
		}
		
		fix_overlaps(function_map);

		// ok, at this point, we've done the best we can with knowns
		// we should have a full analysis of all functions which are
		// reachable from known entry points in the code
		// let's start looking for unknowns
		if(fuzzy) {
			for(int i = 0; i < fuzzy_analysis_steps_count; ++i) {
				qDebug("[Analyzer] %s", fuzzy_analysis_steps[i].message);
				fuzzy_analysis_steps[i].function();
				emit update_progress(util::percentage(analysis_steps_count + i + 1, total_steps));
			}
		}

		qDebug("[Analyzer] determining function types...");
		set_function_types(function_map);

		qDebug("[Analyzer] complete");
		emit update_progress(100);
		
		if(analyzer_widget_) {
			analyzer_widget_->repaint();
		}

		region_info.md5   = md5;
		region_info.fuzzy = fuzzy;
	} else {
		qDebug("[Analyzer] region unchanged, using previous analysis");
	}

	qDebug("[Analyzer] elapsed: %d ms", t.elapsed());
}

//------------------------------------------------------------------------------
// Name: category(edb::address_t address) const
// Desc:
//------------------------------------------------------------------------------
AnalyzerInterface::AddressCategory Analyzer::category(edb::address_t address) const {

	Function func;
	if(find_containing_function(address, func)) {
		if(address == func.entry_address) {
			return ADDRESS_FUNC_START;
		} else if(address == func.end_address) {
			return ADDRESS_FUNC_END;
		} else {
			return ADDRESS_FUNC_BODY;
		}
	}
	return ADDRESS_FUNC_UNKNOWN;
}

//------------------------------------------------------------------------------
// Name: functions(const MemRegion &region) const
// Desc:
//------------------------------------------------------------------------------
AnalyzerInterface::FunctionMap Analyzer::functions(const MemRegion &region) const {
	return analysis_info_[region].analysis;
}

//------------------------------------------------------------------------------
// Name: find_containing_function(edb::address_t address, AnalyzerInterface::Function &function) const
// Desc:
//------------------------------------------------------------------------------
bool Analyzer::find_containing_function(edb::address_t address, AnalyzerInterface::Function &function) const {

	MemRegion region;
	if(edb::v1::memory_regions().find_region(address, region)) {
		const FunctionMap &funcs = functions(region);
		Q_FOREACH(const Function &f, funcs) {
			if(address >= f.entry_address && address <= f.end_address) {
				function = f;
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: md5_region(const MemRegion &region) const
// Desc: returns a byte array representing the MD5 of a region
//------------------------------------------------------------------------------
QByteArray Analyzer::md5_region(const MemRegion &region) const{

	static const edb::address_t page_size = edb::v1::debugger_core->page_size();

	const edb::address_t size_in_pages = region.size() / page_size;
	try {
		QVector<quint8> pages(size_in_pages * page_size);

		if(edb::v1::debugger_core->read_pages(region.start, &pages[0], size_in_pages)) {
			return edb::v1::get_md5(&pages[0], size_in_pages * page_size);
		}

	} catch(const std::bad_alloc &) {
		QMessageBox::information(0, tr("Memroy Allocation Error"),
			tr("Unable to satisfy memory allocation request for requested region."));
	}

	return QByteArray();
}

//------------------------------------------------------------------------------
// Name: module_entry_point(const MemRegion &region) const
// Desc:
//------------------------------------------------------------------------------
edb::address_t Analyzer::module_entry_point(const MemRegion &region) const {

	edb::address_t entry = 0;
	if(BinaryInfo *const binary_info = edb::v1::get_binary_info(region)) {
		entry = binary_info->entry_point();
		delete binary_info;
	}

	return entry;
}


//------------------------------------------------------------------------------
// Name: bonus_entry_point(const MemRegion &region, FunctionMap &results) const
// Desc:
//------------------------------------------------------------------------------
void Analyzer::bonus_entry_point(const MemRegion &region, FunctionMap &results) const {

	if(edb::address_t entry = module_entry_point(region)) {

		// if the entry seems like a relative one (like for a library)
		// then add the base of its image
		if(entry < region.start) {
			entry += region.start;
		}
		
		qDebug("[Analyzer] found entry point: %p", reinterpret_cast<void*>(entry));

		// make sure we have an entry for this function
		if(region.contains(entry)) {
			update_results_entry(results, entry);
		}
	}
}

//------------------------------------------------------------------------------
// Name: invalidate_analysis(const MemRegion &region)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::invalidate_analysis(const MemRegion &region) {
	invalidate_dynamic_analysis(region);
	Q_FOREACH(edb::address_t addr, specified_functions_) {
		if(addr >= region.start && addr < region.end) {
			specified_functions_.remove(addr);
		}
	}
}

//------------------------------------------------------------------------------
// Name: invalidate_dynamic_analysis(const MemRegion &region)
// Desc:
//------------------------------------------------------------------------------
void Analyzer::invalidate_dynamic_analysis(const MemRegion &region) {
	analysis_info_[region] = RegionInfo();
}

//------------------------------------------------------------------------------
// Name: invalidate_analysis()
// Desc:
//------------------------------------------------------------------------------
void Analyzer::invalidate_analysis() {
	analysis_info_.clear();
	specified_functions_.clear();
}

Q_EXPORT_PLUGIN2(Analyzer, Analyzer)
