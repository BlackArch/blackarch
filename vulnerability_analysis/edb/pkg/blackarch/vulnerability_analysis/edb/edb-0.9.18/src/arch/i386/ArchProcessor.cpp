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

#include "ArchProcessor.h"
#include "Configuration.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "FunctionInfo.h"
#include "Instruction.h"
#include "QCategoryList.h"
#include "string_hash.h"
#include "State.h"
#include "Util.h"
#include <QApplication>
#include <QVector>
#include <climits>
#include <cmath>
#include <boost/math/special_functions/fpclassify.hpp>
#ifdef Q_OS_LINUX
#include <asm/unistd.h>
#endif

//------------------------------------------------------------------------------
// Name: ArchProcessor()
// Desc:
//------------------------------------------------------------------------------
ArchProcessor::ArchProcessor() : split_flags_(0) {
	has_mmx_ = edb::v1::debugger_core->has_extension(edb::string_hash<'M', 'M', 'X'>::value);
	has_xmm_ = edb::v1::debugger_core->has_extension(edb::string_hash<'X', 'M', 'M'>::value);
}

//------------------------------------------------------------------------------
// Name: setup_register_item(QCategoryList *category_list, QTreeWidgetItem *parent, const QString &name)
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::setup_register_item(QCategoryList *category_list, QTreeWidgetItem *parent, const QString &name) {

	Q_CHECK_PTR(category_list);

	QTreeWidgetItem *const p = category_list->addItem(parent, QString());

	Q_CHECK_PTR(p);

	p->setData(0, 1, name);
	register_view_items_.push_back(p);
}

//------------------------------------------------------------------------------
// Name: setup_register_view(QCategoryList *category_list)
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::setup_register_view(QCategoryList *category_list) {

	State state;
	edb::v1::debugger_core->get_state(state);

	Q_CHECK_PTR(category_list);

	// setup the register view
	QTreeWidgetItem *const gpr = category_list->addCategory(tr("General Purpose"));

	Q_CHECK_PTR(gpr);

	setup_register_item(category_list, gpr, "eax");
	setup_register_item(category_list, gpr, "ebx");
	setup_register_item(category_list, gpr, "ecx");
	setup_register_item(category_list, gpr, "edx");
	setup_register_item(category_list, gpr, "ebp");
	setup_register_item(category_list, gpr, "esp");
	setup_register_item(category_list, gpr, "esi");
	setup_register_item(category_list, gpr, "edi");
	setup_register_item(category_list, gpr, "eip");
	setup_register_item(category_list, gpr, "eflags");

	// split eflags view
	split_flags_ = new QTreeWidgetItem(register_view_items_[9]);
	split_flags_->setText(0, state.flags_to_string(0));

	QTreeWidgetItem *const segs = category_list->addCategory(tr("Segments"));

	Q_CHECK_PTR(segs);

	setup_register_item(category_list, segs, "cs");
	setup_register_item(category_list, segs, "ds");
	setup_register_item(category_list, segs, "es");
	setup_register_item(category_list, segs, "fs");
	setup_register_item(category_list, segs, "gs");
	setup_register_item(category_list, segs, "ss");

	QTreeWidgetItem *const fpu = category_list->addCategory(tr("FPU"));

	Q_CHECK_PTR(fpu);

	setup_register_item(category_list, fpu, "st0");
	setup_register_item(category_list, fpu, "st1");
	setup_register_item(category_list, fpu, "st2");
	setup_register_item(category_list, fpu, "st3");
	setup_register_item(category_list, fpu, "st4");
	setup_register_item(category_list, fpu, "st5");
	setup_register_item(category_list, fpu, "st6");
	setup_register_item(category_list, fpu, "st7");

	QTreeWidgetItem *const dbg = category_list->addCategory(tr("Debug"));

	Q_CHECK_PTR(dbg);

	setup_register_item(category_list, dbg, "dr0");
	setup_register_item(category_list, dbg, "dr1");
	setup_register_item(category_list, dbg, "dr2");
	setup_register_item(category_list, dbg, "dr3");
	setup_register_item(category_list, dbg, "dr4");
	setup_register_item(category_list, dbg, "dr5");
	setup_register_item(category_list, dbg, "dr6");
	setup_register_item(category_list, dbg, "dr7");

	if(has_mmx_) {
		QTreeWidgetItem *const mmx = category_list->addCategory(tr("MMX"));

		Q_CHECK_PTR(mmx);

		setup_register_item(category_list, mmx, "mm0");
		setup_register_item(category_list, mmx, "mm1");
		setup_register_item(category_list, mmx, "mm2");
		setup_register_item(category_list, mmx, "mm3");
		setup_register_item(category_list, mmx, "mm4");
		setup_register_item(category_list, mmx, "mm5");
		setup_register_item(category_list, mmx, "mm6");
		setup_register_item(category_list, mmx, "mm7");
	}

	if(has_xmm_) {
		QTreeWidgetItem *const xmm = category_list->addCategory(tr("XMM"));

		Q_CHECK_PTR(xmm);

		setup_register_item(category_list, xmm, "xmm0");
		setup_register_item(category_list, xmm, "xmm1");
		setup_register_item(category_list, xmm, "xmm2");
		setup_register_item(category_list, xmm, "xmm3");
		setup_register_item(category_list, xmm, "xmm4");
		setup_register_item(category_list, xmm, "xmm5");
		setup_register_item(category_list, xmm, "xmm6");
		setup_register_item(category_list, xmm, "xmm7");
#if defined(EDB_X86_64)
		setup_register_item(category_list, xmm, "xmm8");
		setup_register_item(category_list, xmm, "xmm9");
		setup_register_item(category_list, xmm, "xmm10");
		setup_register_item(category_list, xmm, "xmm11");
		setup_register_item(category_list, xmm, "xmm12");
		setup_register_item(category_list, xmm, "xmm13");
		setup_register_item(category_list, xmm, "xmm14");
		setup_register_item(category_list, xmm, "xmm15");
#endif
	}

	update_register_view(QString());
}

//------------------------------------------------------------------------------
// Name: value_from_item(const QTreeWidgetItem &item)
// Desc:
//------------------------------------------------------------------------------
Register ArchProcessor::value_from_item(const QTreeWidgetItem &item) {
	const QString &name = item.data(0, 1).toString();
	State state;
	edb::v1::debugger_core->get_state(state);
	return state[name];
}

//------------------------------------------------------------------------------
// Name: get_register_item(unsigned int index)
// Desc:
//------------------------------------------------------------------------------
QTreeWidgetItem *ArchProcessor::get_register_item(unsigned int index) {
	Q_ASSERT(index < static_cast<unsigned int>(register_view_items_.size()));
	return register_view_items_[index];
}

//------------------------------------------------------------------------------
// Name: update_register(QTreeWidgetItem *item, const QString &name, const Register &reg) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::update_register(QTreeWidgetItem *item, const QString &name, const Register &reg) const {

	Q_CHECK_PTR(item);

	QString reg_string;
	int string_length;
	const edb::reg_t value = *reg;

	if(edb::v1::get_ascii_string_at_address(value, reg_string, edb::v1::config().min_string_length, 256, string_length)) {
		item->setText(0, QString("%1: %2 ASCII \"%3\"").arg(name, edb::v1::format_pointer(value), reg_string));
	} else if(edb::v1::get_utf16_string_at_address(value, reg_string, edb::v1::config().min_string_length, 256, string_length)) {
		item->setText(0, QString("%1: %2 UTF16 \"%3\"").arg(name, edb::v1::format_pointer(value), reg_string));
	} else {
		item->setText(0, QString("%1: %2").arg(name, edb::v1::format_pointer(value)));
	}
}

//------------------------------------------------------------------------------
// Name: reset()
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::reset() {
	last_state_.clear();
	
	State state;
	edb::v1::debugger_core->get_state(state);
	
	split_flags_->setText(0, state.flags_to_string(0));
}

//------------------------------------------------------------------------------
// Name: update_register_view(const QString &default_region_name)
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::update_register_view(const QString &default_region_name) {

	State state;
	edb::v1::debugger_core->get_state(state);
	
	const QPalette palette = QApplication::palette();

	update_register(get_register_item(0), "EAX", state["eax"]);
	update_register(get_register_item(1), "EBX", state["ebx"]);
	update_register(get_register_item(2), "ECX", state["ecx"]);
	update_register(get_register_item(3), "EDX", state["edx"]);
	update_register(get_register_item(4), "EBP", state["ebp"]);
	update_register(get_register_item(5), "ESP", state["esp"]);
	update_register(get_register_item(6), "ESI", state["esi"]);
	update_register(get_register_item(7), "EDI", state["edi"]);

	const QString symname = edb::v1::find_function_symbol(state.instruction_pointer(), default_region_name);

	if(!symname.isEmpty()) {
		get_register_item(8)->setText(0, QString("EIP: %1 <%2>").arg(edb::v1::format_pointer(state.instruction_pointer())).arg(symname));
	} else {
		get_register_item(8)->setText(0, QString("EIP: %1").arg(edb::v1::format_pointer(state.instruction_pointer())));
	}
	get_register_item(9)->setText(0, QString("EFLAGS: %1").arg(edb::v1::format_pointer(state.flags())));

	get_register_item(10)->setText(0, QString("CS: %1").arg(*state["cs"] & 0xffff, 4, 16, QChar('0')));
	get_register_item(11)->setText(0, QString("DS: %1").arg(*state["ds"] & 0xffff, 4, 16, QChar('0')));
	get_register_item(12)->setText(0, QString("ES: %1").arg(*state["es"] & 0xffff, 4, 16, QChar('0')));
	get_register_item(13)->setText(0, QString("FS: %1 (%2)").arg(*state["fs"] & 0xffff, 4, 16, QChar('0')).arg(edb::v1::format_pointer(state["fs"].segment_base())));
	get_register_item(14)->setText(0, QString("GS: %1 (%2)").arg(*state["gs"] & 0xffff, 4, 16, QChar('0')).arg(edb::v1::format_pointer(state["gs"].segment_base())));
	get_register_item(15)->setText(0, QString("SS: %1").arg(*state["ss"] & 0xffff, 4, 16, QChar('0')));

	for(int i = 0; i < 8; ++i) {
		const long double current = state.fpu_register(i);
		const long double prev    = last_state_.fpu_register(i);
		get_register_item(16 + i)->setText(0, QString("ST%1: %2").arg(i).arg(current, 0, 'g', 16));
		get_register_item(16 + i)->setForeground(0, QBrush((current != prev && !(boost::math::isnan(prev) && boost::math::isnan(current))) ? Qt::red : palette.text()));
	}

	for(int i = 0; i < 8; ++i) {
		get_register_item(24 + i)->setText(0, QString("DR%1: %2").arg(i).arg(state.debug_register(i), 0, 16));
		get_register_item(24 + i)->setForeground(0, QBrush((state.debug_register(i) != last_state_.debug_register(i)) ? Qt::red : palette.text()));
	}

	if(has_mmx_) {
		for(int i = 0; i < 8; ++i) {
			const quint64 current = state.mmx_register(i);
			const quint64 prev    = last_state_.mmx_register(i);
			get_register_item(32 + i)->setText(0, QString("MM%1: %2").arg(i).arg(current, sizeof(quint64)*2, 16, QChar('0')));
			get_register_item(32 + i)->setForeground(0, QBrush((current != prev) ? Qt::red : palette.text()));
		}
	}

	if(has_xmm_) {
		for(int i = 0; i < 8; ++i) {
			const QByteArray current = state.xmm_register(i);
			const QByteArray prev    = last_state_.xmm_register(i);
			Q_ASSERT(current.size() == 16 || current.size() == 0);
			get_register_item(40 + i)->setText(0, QString("XMM%1: %2").arg(i).arg(current.toHex().constData()));
			get_register_item(40 + i)->setForeground(0, QBrush((current != prev) ? Qt::red : palette.text()));
		}
	}

	const bool flags_changed = state.flags() != last_state_.flags();
	if(flags_changed) {
		split_flags_->setText(0, state.flags_to_string());
	}

	// highlight any changed registers
	get_register_item(0)->setForeground(0, (state["eax"] != last_state_["eax"]) ? Qt::red : palette.text());
	get_register_item(1)->setForeground(0, (state["ebx"] != last_state_["ebx"]) ? Qt::red : palette.text());
	get_register_item(2)->setForeground(0, (state["ecx"] != last_state_["ecx"]) ? Qt::red : palette.text());
	get_register_item(3)->setForeground(0, (state["edx"] != last_state_["edx"]) ? Qt::red : palette.text());
	get_register_item(4)->setForeground(0, (state["ebp"] != last_state_["ebp"]) ? Qt::red : palette.text());
	get_register_item(5)->setForeground(0, (state["esp"] != last_state_["esp"]) ? Qt::red : palette.text());
	get_register_item(6)->setForeground(0, (state["esi"] != last_state_["esi"]) ? Qt::red : palette.text());
	get_register_item(7)->setForeground(0, (state["edi"] != last_state_["edi"]) ? Qt::red : palette.text());
	get_register_item(8)->setForeground(0, (state.instruction_pointer() != last_state_.instruction_pointer()) ? Qt::red : palette.text());
	get_register_item(9)->setForeground(0, flags_changed ? Qt::red : palette.text());

	last_state_ = state;
}

//------------------------------------------------------------------------------
// Name: resolve_function_parameters(const State &state, const QString &symname, int offset, QStringList &ret) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::resolve_function_parameters(const State &state, const QString &symname, int offset, QStringList &ret) const {

	// we will always be removing the last 2 chars '+0' from the string as well
	// as chopping the region prefix we like to prepend to symbols
	QString func_name;
	const int colon_index = symname.indexOf("::");

	if(colon_index != -1) {
		func_name = symname.left(symname.length() - 2).mid(colon_index + 2);
	}

	// safe not to check for -1, it means 'rest of string' for the mid function
	func_name = func_name.mid(0, func_name.indexOf("@"));

	if(const FunctionInfo *const info = edb::v1::get_function_info(func_name)) {

		QString function_call = func_name + "(";

		const QVector<QChar> params(info->params());
		int i = 0;
		Q_FOREACH(QChar ch, params) {

			edb::reg_t arg;
			edb::v1::debugger_core->read_bytes(state.stack_pointer() + i * sizeof(edb::reg_t) + offset, &arg, sizeof(arg));

			function_call += format_argument(ch, arg);

			if(i + 1 < params.size()) {
				function_call += ", ";
			}
			++i;
		}

		function_call += ")";
		ret << function_call;
	}
}

//------------------------------------------------------------------------------
// Name: is_jcc_taken(const State &state, quint8 instruction_byte) const
// Desc:
//------------------------------------------------------------------------------
bool ArchProcessor::is_jcc_taken(const State &state, quint8 instruction_byte) const {

	const edb::reg_t efl = state.flags();
	const bool cf = (efl & 0x0001) != 0;
	const bool pf = (efl & 0x0004) != 0;
	const bool zf = (efl & 0x0040) != 0;
	const bool sf = (efl & 0x0080) != 0;
	const bool of = (efl & 0x0800) != 0;

	bool taken = false;

	switch(instruction_byte & 0x0e) {
	case 0x00:
		taken = of;
		break;
	case 0x02:
		taken = cf;
		break;
	case 0x04:
		taken = zf;
		break;
	case 0x06:
		taken = cf || zf;
		break;
	case 0x08:
		taken = sf;
		break;
	case 0x0a:
		taken = pf;
		break;
	case 0x0c:
		taken = sf != of;
		break;
	case 0x0e:
		taken = zf || sf != of;
		break;
	}

	if(instruction_byte & 0x01) {
		taken = !taken;
	}

	return taken;
}

//------------------------------------------------------------------------------
// Name: analyze_cmov(const State &state, const edb::Instruction &insn, QStringList &ret) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::analyze_cmov(const State &state, const edb::Instruction &insn, QStringList &ret) const {

	const quint8 *const insn_buf = insn.buffer();

	const bool taken = is_jcc_taken(state, insn_buf[1 + insn.prefix_size()]);

	if(taken) {
		ret << tr("move performed");
	} else {
		ret << tr("move NOT performed");
	}
}

//------------------------------------------------------------------------------
// Name: analyze_jump(const State &state, const edb::Instruction &insn, QStringList &ret) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::analyze_jump(const State &state, const edb::Instruction &insn, QStringList &ret) const {

	bool taken = false;

	const quint8 *const insn_buf = insn.buffer();

	if(insn.type() == edb::Instruction::OP_JCC) {

		if(insn.size() - insn.prefix_size() == 2) {
			taken = is_jcc_taken(state, insn_buf[0 + insn.prefix_size()]);
		} else {
			taken = is_jcc_taken(state, insn_buf[1 + insn.prefix_size()]);
		}



	// TODO: this is not correct! 0xe3 IS an OP_JCC
	} else if(insn_buf[0] == 0xe3) {
		if(insn.prefix() & edb::Instruction::PREFIX_ADDRESS) {
			taken = (*state["ecx"] & 0xffff) == 0;
		} else {
			taken = *state["ecx"] == 0;
		}
	}

	if(taken) {
		ret << tr("jump taken");
	} else {
		ret << tr("jump NOT taken");
	}
}

//------------------------------------------------------------------------------
// Name: analyze_return(const State &state, const edb::Instruction &insn, QStringList &ret) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::analyze_return(const State &state, const edb::Instruction &insn, QStringList &ret) const {
	Q_UNUSED(insn);

	edb::address_t return_address;
	edb::v1::debugger_core->read_bytes(state.stack_pointer(), &return_address, sizeof(return_address));

	const QString symname = edb::v1::find_function_symbol(return_address);
	if(!symname.isEmpty()) {
		ret << tr("return to %1 <%2>").arg(edb::v1::format_pointer(return_address)).arg(symname);
	} else {
		ret << tr("return to %1").arg(edb::v1::format_pointer(return_address));
	}
}

//------------------------------------------------------------------------------
// Name: get_effective_address(const edb::Operand &op, const State &state) const
// Desc:
//------------------------------------------------------------------------------
edb::address_t ArchProcessor::get_effective_address(const edb::Operand &op, const State &state) const {
	edb::address_t ret = 0;

	// TODO: get registers by index, not string! too slow

	if(op.valid()) {
		switch(op.general_type()) {
		case edb::Operand::TYPE_REGISTER:
			ret = *state[QString::fromStdString(edisassm::register_name<edisassm::x86>(op.reg()))];
			break;
		case edb::Operand::TYPE_EXPRESSION:
			do {
				const edb::reg_t base  = *state[QString::fromStdString(edisassm::register_name<edisassm::x86>(op.expression().base))];
				const edb::reg_t index = *state[QString::fromStdString(edisassm::register_name<edisassm::x86>(op.expression().index))];
				ret                    = base + index * op.expression().scale + op.displacement();

				if(op.owner()->prefix() & edb::Instruction::PREFIX_GS) {
					ret += state["gs"].segment_base();
				}

				if(op.owner()->prefix() & edb::Instruction::PREFIX_FS) {
					ret += state["fs"].segment_base();
				}
			} while(0);
			break;
		case edb::Operand::TYPE_ABSOLUTE:
			ret = op.absolute().offset;
			if(op.owner()->prefix() & edb::Instruction::PREFIX_GS) {
				ret += state["gs"].segment_base();
			}

			if(op.owner()->prefix() & edb::Instruction::PREFIX_FS) {
				ret += state["fs"].segment_base();
			}
			break;
		case edb::Operand::TYPE_IMMEDIATE:
			break;
		case edb::Operand::TYPE_REL:
			ret = op.relative_target();
			break;
		default:
			break;
		}
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: analyze_call(const State &state, const edb::Instruction &insn, QStringList &ret) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::analyze_call(const State &state, const edb::Instruction &insn, QStringList &ret) const {

	const edb::Operand &operand = insn.operand(0);

	if(operand.valid()) {

		const edb::address_t effective_address = get_effective_address(operand, state);
		const QString temp_operand             = QString::fromStdString(edisassm::to_string(operand));
		QString temp;

		switch(operand.general_type()) {
		case edb::Operand::TYPE_REL:
		case edb::Operand::TYPE_REGISTER:
			do {
				int offset;
				const QString symname = edb::v1::find_function_symbol(effective_address, QString(), &offset);
				if(!symname.isEmpty()) {
					ret << temp.sprintf("%s = " EDB_FMT_PTR " <%s>", qPrintable(temp_operand), effective_address, qPrintable(symname));

					if(offset == 0) {
						if(insn.type() == edb::Instruction::OP_CALL) {
							resolve_function_parameters(state, symname, 0, ret);
						} else {
							resolve_function_parameters(state, symname, 4, ret);
						}
					}

				} else {
					ret << temp.sprintf("%s = " EDB_FMT_PTR, qPrintable(temp_operand), effective_address);
				}
			} while(0);
			break;

		case edb::Operand::TYPE_EXPRESSION:
		default:
			do {
				edb::address_t target;
				const bool ok = edb::v1::debugger_core->read_bytes(effective_address, &target, sizeof(target));

				if(ok) {
					int offset;
					const QString symname = edb::v1::find_function_symbol(target, QString(), &offset);
					if(!symname.isEmpty()) {
						ret << temp.sprintf("%s = [" EDB_FMT_PTR "] = " EDB_FMT_PTR " <%s>", qPrintable(temp_operand), effective_address, target, qPrintable(symname));

						if(offset == 0) {
							if(insn.type() == edb::Instruction::OP_CALL) {
								resolve_function_parameters(state, symname, 0, ret);
							} else {
								resolve_function_parameters(state, symname, 4, ret);
							}
						}

					} else {
						ret << temp.sprintf("%s = [" EDB_FMT_PTR "] = " EDB_FMT_PTR, qPrintable(temp_operand), effective_address, target);
					}
				} else {
					// could not read from the address
					ret << temp.sprintf("%s = [" EDB_FMT_PTR "] = ?", qPrintable(temp_operand), effective_address);
				}
			} while(0);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: analyze_operands(const State &state, const edb::Instruction &insn, QStringList &ret)
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::analyze_operands(const State &state, const edb::Instruction &insn, QStringList &ret) const {

	Q_UNUSED(insn);

	for(int j = 0; j < edb::Instruction::MAX_OPERANDS; ++j) {

		const edb::Operand &operand = insn.operand(j);

		if(operand.valid()) {

			const QString temp_operand = QString::fromStdString(edisassm::to_string(operand));

			switch(operand.general_type()) {
			case edb::Operand::TYPE_REL:
			case edb::Operand::TYPE_REGISTER:
				do {
					const edb::address_t effective_address = get_effective_address(operand, state);
					ret << QString("%1 = %2").arg(temp_operand).arg(edb::v1::format_pointer(effective_address));
				} while(0);
				break;
			case edb::Operand::TYPE_EXPRESSION:
				do {
					const edb::address_t effective_address = get_effective_address(operand, state);
					edb::address_t target;

					const bool ok = edb::v1::debugger_core->read_bytes(effective_address, &target, sizeof(target));

					if(ok) {
						switch(operand.complete_type()) {
						case edb::Operand::TYPE_EXPRESSION8:
							ret << QString("%1 = [%2] = 0x%3").arg(temp_operand).arg(edb::v1::format_pointer(effective_address)).arg(target & 0xff, 2, 16, QChar('0'));
							break;
						case edb::Operand::TYPE_EXPRESSION16:
							ret << QString("%1 = [%2] = 0x%3").arg(temp_operand).arg(edb::v1::format_pointer(effective_address)).arg(target & 0xffff, 4, 16, QChar('0'));
							break;
						case edb::Operand::TYPE_EXPRESSION32:
						default:
							ret << QString("%1 = [%2] = 0x%3").arg(temp_operand).arg(edb::v1::format_pointer(effective_address)).arg(target & 0xffffffff, 8, 16, QChar('0'));
							break;
						}
					} else {
						ret << QString("%1 = [%2] = ?").arg(temp_operand).arg(edb::v1::format_pointer(effective_address));
					}
				} while(0);
				break;
			default:
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: analyze_jump_targets(const edb::Instruction &insn, QStringList &ret) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::analyze_jump_targets(const edb::Instruction &insn, QStringList &ret) const {
	const edb::address_t address       = insn.rva();
	const edb::address_t start_address = address - 128;
	const edb::address_t end_address   = address + 127;

	quint8 buffer[edb::Instruction::MAX_SIZE];

	QString temp;

	for(edb::address_t addr = start_address; addr < end_address; ++addr) {
		int sz = sizeof(buffer);
		if(edb::v1::get_instruction_bytes(addr, buffer, sz)) {
			edb::Instruction insn(buffer, sz, addr, std::nothrow);
			if(insn.valid() && (insn.type() == edb::Instruction::OP_JCC || insn.type() == edb::Instruction::OP_JMP)) {
				const edb::Operand &operand = insn.operand(0);

				if(operand.general_type() == edb::Operand::TYPE_REL) {
					const edb::address_t target = operand.relative_target();

					if(target == address) {
						ret << tr("possible jump from 0x%1").arg(edb::v1::format_pointer(addr));
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: format_argument(QChar ch, edb::reg_t arg) const
// Desc:
//------------------------------------------------------------------------------
QString ArchProcessor::format_argument(QChar ch, edb::reg_t arg) const {
	QString param_text;

	switch(ch.toAscii()) {
	case 's':
	case 'f':
		do {
			QString string_param;
			int string_length;

			if(edb::v1::get_ascii_string_at_address(arg, string_param, edb::v1::config().min_string_length, 256, string_length)) {
				param_text = QString("<0x%1> \"%2\"").arg(edb::v1::format_pointer(arg)).arg(string_param);
			} else {
				char character;
				edb::v1::debugger_core->read_bytes(arg, &character, sizeof(character));
				if(character == '\0') {
					param_text = QString("<0x%1> \"\"").arg(edb::v1::format_pointer(arg));
				} else {
					param_text = QString("<0x%1>").arg(edb::v1::format_pointer(arg));
				}
			}
		} while(0);
		break;

	case 'p':
		param_text = QString("0x%1").arg(edb::v1::format_pointer(arg));
		break;
	case 'c':
		if(arg < 0x80 && (std::isprint(arg) || std::isspace(arg))) {
			param_text.sprintf("'%c'", static_cast<char>(arg));
		} else {
			param_text.sprintf("'\\x%02x'", static_cast<quint16>(arg));
		}
		break;
	case 'u':
		param_text.sprintf("%lu", static_cast<unsigned long>(arg));
		break;
	case 'i':
		param_text.sprintf("%ld", static_cast<long>(arg));
		break;
	case 'v':
		// for variadic, we don't quite handle that yet...
		break;
	}

	return param_text;
}

//------------------------------------------------------------------------------
// Name: analyze_syscall(const State &state, const edb::Instruction &insn, QStringList &ret) const
// Desc:
//------------------------------------------------------------------------------
void ArchProcessor::analyze_syscall(const State &state, const edb::Instruction &insn, QStringList &ret) const {
	Q_UNUSED(insn);
	Q_UNUSED(ret);
	Q_UNUSED(state);
	
#ifdef Q_OS_LINUX

	const edb::reg_t arg1 = *state["ebx"];
	const edb::reg_t arg2 = *state["ecx"];
	const edb::reg_t arg3 = *state["edx"];
	const edb::reg_t arg4 = *state["esi"];
	const edb::reg_t arg5 = *state["edi"];
	const edb::reg_t arg6 = *state["ebp"];

	switch(*state["eax"]) {
	#ifdef __NR_read
	case __NR_read:						ret << tr("SYSCALL: read(%1,%2,%3)").arg(format_argument('i', arg1)).arg(format_argument('p', arg2)).arg(format_argument('u', arg3)); break;
	#endif
	#ifdef __NR_write
	case __NR_write:					ret << tr("SYSCALL: write(%1,%2,%3)").arg(format_argument('i', arg1)).arg(format_argument('p', arg2)).arg(format_argument('u', arg3)); break;
	#endif
	#ifdef __NR_open
	case __NR_open:						ret << tr("SYSCALL: open(%1,%2,%3)").arg(format_argument('s', arg1)).arg(format_argument('i', arg2)).arg(format_argument('u', arg3)); break;
	#endif
	#ifdef __NR_close
	case __NR_close:					ret << tr("SYSCALL: close(%1)").arg(format_argument('i', arg1)); break;
	#endif
	#ifdef __NR_stat
	case __NR_stat:						ret << tr("SYSCALL: stat()"); break;
	#endif
	#ifdef __NR_fstat
	case __NR_fstat:					ret << tr("SYSCALL: fstat()"); break;
	#endif
	#ifdef __NR_lstat
	case __NR_lstat:					ret << tr("SYSCALL: lstat()"); break;
	#endif
	#ifdef __NR_poll
	case __NR_poll:						ret << tr("SYSCALL: poll()"); break;
	#endif
	#ifdef __NR_lseek
	case __NR_lseek:					ret << tr("SYSCALL: lseek()"); break;
	#endif
	#ifdef __NR_mmap
	case __NR_mmap:						ret << tr("SYSCALL: mmap(%1,%2,%3,%4,%5,%6)").arg(format_argument('p', arg1)).arg(format_argument('u', arg2)).arg(format_argument('i', arg3)).arg(format_argument('i', arg4)).arg(format_argument('i', arg5)).arg(format_argument('u', arg6)); break;
	#endif
	#ifdef __NR_mprotect
	case __NR_mprotect:					ret << tr("SYSCALL: mprotect(%1,%2,%3)").arg(format_argument('p', arg1)).arg(format_argument('u', arg2)).arg(format_argument('i', arg3)); break;
	#endif
	#ifdef __NR_munmap
	case __NR_munmap:					ret << tr("SYSCALL: munmap(%1,%2)").arg(format_argument('p', arg1)).arg(format_argument('u', arg2)); break;
	#endif
	#ifdef __NR_brk
	case __NR_brk:						ret << tr("SYSCALL: brk(%1)").arg(format_argument('p', arg1)); break;
	#endif
	#ifdef __NR_rt_sigaction
	case __NR_rt_sigaction:				ret << tr("SYSCALL: rt_sigaction()"); break;
	#endif
	#ifdef __NR_rt_sigprocmask
	case __NR_rt_sigprocmask:			ret << tr("SYSCALL: rt_sigprocmask()"); break;
	#endif
	#ifdef __NR_rt_sigreturn
	case __NR_rt_sigreturn:				ret << tr("SYSCALL: rt_sigreturn()"); break;
	#endif
	#ifdef __NR_ioctl
	case __NR_ioctl:					ret << tr("SYSCALL: ioctl()"); break;
	#endif
	#ifdef __NR_pread64
	case __NR_pread64:					ret << tr("SYSCALL: pread64()"); break;
	#endif
	#ifdef __NR_pwrite64
	case __NR_pwrite64:					ret << tr("SYSCALL: pwrite64()"); break;
	#endif
	#ifdef __NR_readv
	case __NR_readv:					ret << tr("SYSCALL: readv()"); break;
	#endif
	#ifdef __NR_writev
	case __NR_writev:					ret << tr("SYSCALL: writev()"); break;
	#endif
	#ifdef __NR_access
	case __NR_access:					ret << tr("SYSCALL: access()"); break;
	#endif
	#ifdef __NR_pipe
	case __NR_pipe:						ret << tr("SYSCALL: pipe()"); break;
	#endif
	#ifdef __NR_select
	case __NR_select:					ret << tr("SYSCALL: select()"); break;
	#endif
	#ifdef __NR_sched_yield
	case __NR_sched_yield:				ret << tr("SYSCALL: sched_yield()"); break;
	#endif
	#ifdef __NR_mremap
	case __NR_mremap:					ret << tr("SYSCALL: mremap()"); break;
	#endif
	#ifdef __NR_msync
	case __NR_msync:					ret << tr("SYSCALL: msync()"); break;
	#endif
	#ifdef __NR_mincore
	case __NR_mincore:					ret << tr("SYSCALL: mincore()"); break;
	#endif
	#ifdef __NR_madvise
	case __NR_madvise:					ret << tr("SYSCALL: madvise()"); break;
	#endif
	#ifdef __NR_shmget
	case __NR_shmget:					ret << tr("SYSCALL: shmget()"); break;
	#endif
	#ifdef __NR_shmat
	case __NR_shmat:					ret << tr("SYSCALL: shmat()"); break;
	#endif
	#ifdef __NR_shmctl
	case __NR_shmctl:					ret << tr("SYSCALL: shmctl()"); break;
	#endif
	#ifdef __NR_dup
	case __NR_dup:						ret << tr("SYSCALL: dup()"); break;
	#endif
	#ifdef __NR_dup2
	case __NR_dup2:						ret << tr("SYSCALL: dup2()"); break;
	#endif
	#ifdef __NR_pause
	case __NR_pause:					ret << tr("SYSCALL: pause()"); break;
	#endif
	#ifdef __NR_nanosleep
	case __NR_nanosleep:				ret << tr("SYSCALL: nanosleep()"); break;
	#endif
	#ifdef __NR_getitimer
	case __NR_getitimer:				ret << tr("SYSCALL: getitimer()"); break;
	#endif
	#ifdef __NR_alarm
	case __NR_alarm:					ret << tr("SYSCALL: alarm()"); break;
	#endif
	#ifdef __NR_setitimer
	case __NR_setitimer:				ret << tr("SYSCALL: setitimer()"); break;
	#endif
	#ifdef __NR_getpid
	case __NR_getpid:					ret << tr("SYSCALL: getpid()"); break;
	#endif
	#ifdef __NR_sendfile
	case __NR_sendfile:					ret << tr("SYSCALL: sendfile()"); break;
	#endif
	#ifdef __NR_socket
	case __NR_socket:					ret << tr("SYSCALL: socket()"); break;
	#endif
	#ifdef __NR_connect
	case __NR_connect:					ret << tr("SYSCALL: connect()"); break;
	#endif
	#ifdef __NR_accept
	case __NR_accept:					ret << tr("SYSCALL: accept()"); break;
	#endif
	#ifdef __NR_sendto
	case __NR_sendto:					ret << tr("SYSCALL: sendto()"); break;
	#endif
	#ifdef __NR_recvfrom
	case __NR_recvfrom:					ret << tr("SYSCALL: recvfrom()"); break;
	#endif
	#ifdef __NR_sendmsg
	case __NR_sendmsg:					ret << tr("SYSCALL: sendmsg()"); break;
	#endif
	#ifdef __NR_recvmsg
	case __NR_recvmsg:					ret << tr("SYSCALL: recvmsg()"); break;
	#endif
	#ifdef __NR_shutdown
	case __NR_shutdown:					ret << tr("SYSCALL: shutdown()"); break;
	#endif
	#ifdef __NR_bind
	case __NR_bind:						ret << tr("SYSCALL: bind()"); break;
	#endif
	#ifdef __NR_listen
	case __NR_listen:					ret << tr("SYSCALL: listen()"); break;
	#endif
	#ifdef __NR_getsockname
	case __NR_getsockname:				ret << tr("SYSCALL: getsockname()"); break;
	#endif
	#ifdef __NR_getpeername
	case __NR_getpeername:				ret << tr("SYSCALL: getpeername()"); break;
	#endif
	#ifdef __NR_socketpair
	case __NR_socketpair:				ret << tr("SYSCALL: socketpair()"); break;
	#endif
	#ifdef __NR_setsockopt
	case __NR_setsockopt:				ret << tr("SYSCALL: setsockopt()"); break;
	#endif
	#ifdef __NR_getsockopt
	case __NR_getsockopt:				ret << tr("SYSCALL: getsockopt()"); break;
	#endif
	#ifdef __NR_clone
	case __NR_clone:					ret << tr("SYSCALL: clone()"); break;
	#endif
	#ifdef __NR_fork
	case __NR_fork:						ret << tr("SYSCALL: fork()"); break;
	#endif
	#ifdef __NR_vfork
	case __NR_vfork:					ret << tr("SYSCALL: vfork()"); break;
	#endif
	#ifdef __NR_execve
	case __NR_execve:					ret << tr("SYSCALL: execve()"); break;
	#endif
	#ifdef __NR_exit
	case __NR_exit:						ret << tr("SYSCALL: exit()"); break;
	#endif
	#ifdef __NR_wait4
	case __NR_wait4:					ret << tr("SYSCALL: wait4()"); break;
	#endif
	#ifdef __NR_kill
	case __NR_kill:						ret << tr("SYSCALL: kill()"); break;
	#endif
	#ifdef __NR_uname
	case __NR_uname:					ret << tr("SYSCALL: uname()"); break;
	#endif
	#ifdef __NR_semget
	case __NR_semget:					ret << tr("SYSCALL: semget()"); break;
	#endif
	#ifdef __NR_semop
	case __NR_semop:					ret << tr("SYSCALL: semop()"); break;
	#endif
	#ifdef __NR_semctl
	case __NR_semctl:					ret << tr("SYSCALL: semctl()"); break;
	#endif
	#ifdef __NR_shmdt
	case __NR_shmdt:					ret << tr("SYSCALL: shmdt()"); break;
	#endif
	#ifdef __NR_msgget
	case __NR_msgget:					ret << tr("SYSCALL: msgget()"); break;
	#endif
	#ifdef __NR_msgsnd
	case __NR_msgsnd:					ret << tr("SYSCALL: msgsnd()"); break;
	#endif
	#ifdef __NR_msgrcv
	case __NR_msgrcv:					ret << tr("SYSCALL: msgrcv()"); break;
	#endif
	#ifdef __NR_msgctl
	case __NR_msgctl:					ret << tr("SYSCALL: msgctl()"); break;
	#endif
	#ifdef __NR_fcntl
	case __NR_fcntl:					ret << tr("SYSCALL: fcntl()"); break;
	#endif
	#ifdef __NR_flock
	case __NR_flock:					ret << tr("SYSCALL: flock()"); break;
	#endif
	#ifdef __NR_fsync
	case __NR_fsync:					ret << tr("SYSCALL: fsync()"); break;
	#endif
	#ifdef __NR_fdatasync
	case __NR_fdatasync:				ret << tr("SYSCALL: fdatasync()"); break;
	#endif
	#ifdef __NR_truncate
	case __NR_truncate:					ret << tr("SYSCALL: truncate()"); break;
	#endif
	#ifdef __NR_ftruncate
	case __NR_ftruncate:				ret << tr("SYSCALL: ftruncate()"); break;
	#endif
	#ifdef __NR_getdents
	case __NR_getdents:					ret << tr("SYSCALL: getdents()"); break;
	#endif
	#ifdef __NR_getcwd
	case __NR_getcwd:					ret << tr("SYSCALL: getcwd()"); break;
	#endif
	#ifdef __NR_chdir
	case __NR_chdir:					ret << tr("SYSCALL: chdir()"); break;
	#endif
	#ifdef __NR_fchdir
	case __NR_fchdir:					ret << tr("SYSCALL: fchdir()"); break;
	#endif
	#ifdef __NR_rename
	case __NR_rename:					ret << tr("SYSCALL: rename()"); break;
	#endif
	#ifdef __NR_mkdir
	case __NR_mkdir:					ret << tr("SYSCALL: mkdir()"); break;
	#endif
	#ifdef __NR_rmdir
	case __NR_rmdir:					ret << tr("SYSCALL: rmdir()"); break;
	#endif
	#ifdef __NR_creat
	case __NR_creat:					ret << tr("SYSCALL: creat()"); break;
	#endif
	#ifdef __NR_link
	case __NR_link:						ret << tr("SYSCALL: link()"); break;
	#endif
	#ifdef __NR_unlink
	case __NR_unlink:					ret << tr("SYSCALL: unlink()"); break;
	#endif
	#ifdef __NR_symlink
	case __NR_symlink:					ret << tr("SYSCALL: symlink()"); break;
	#endif
	#ifdef __NR_readlink
	case __NR_readlink:					ret << tr("SYSCALL: readlink(%1,%2,%3)").arg(format_argument('s', arg1)).arg(format_argument('p', arg2)).arg(format_argument('u', arg3)); break;
	#endif
	#ifdef __NR_chmod
	case __NR_chmod:					ret << tr("SYSCALL: chmod()"); break;
	#endif
	#ifdef __NR_fchmod
	case __NR_fchmod:					ret << tr("SYSCALL: fchmod()"); break;
	#endif
	#ifdef __NR_chown
	case __NR_chown:					ret << tr("SYSCALL: chown()"); break;
	#endif
	#ifdef __NR_fchown
	case __NR_fchown:					ret << tr("SYSCALL: fchown()"); break;
	#endif
	#ifdef __NR_lchown
	case __NR_lchown:					ret << tr("SYSCALL: lchown()"); break;
	#endif
	#ifdef __NR_umask
	case __NR_umask:					ret << tr("SYSCALL: umask()"); break;
	#endif
	#ifdef __NR_gettimeofday
	case __NR_gettimeofday:				ret << tr("SYSCALL: gettimeofday()"); break;
	#endif
	#ifdef __NR_getrlimit
	case __NR_getrlimit:				ret << tr("SYSCALL: getrlimit()"); break;
	#endif
	#ifdef __NR_getrusage
	case __NR_getrusage:				ret << tr("SYSCALL: getrusage()"); break;
	#endif
	#ifdef __NR_sysinfo
	case __NR_sysinfo:					ret << tr("SYSCALL: sysinfo()"); break;
	#endif
	#ifdef __NR_times
	case __NR_times:					ret << tr("SYSCALL: times()"); break;
	#endif
	#ifdef __NR_ptrace
	case __NR_ptrace:					ret << tr("SYSCALL: ptrace()"); break;
	#endif
	#ifdef __NR_getuid
	case __NR_getuid:					ret << tr("SYSCALL: getuid()"); break;
	#endif
	#ifdef __NR_syslog
	case __NR_syslog:					ret << tr("SYSCALL: syslog()"); break;
	#endif
	#ifdef __NR_getgid
	case __NR_getgid:					ret << tr("SYSCALL: getgid()"); break;
	#endif
	#ifdef __NR_setuid
	case __NR_setuid:					ret << tr("SYSCALL: setuid()"); break;
	#endif
	#ifdef __NR_setgid
	case __NR_setgid:					ret << tr("SYSCALL: setgid()"); break;
	#endif
	#ifdef __NR_geteuid
	case __NR_geteuid:					ret << tr("SYSCALL: geteuid()"); break;
	#endif
	#ifdef __NR_getegid
	case __NR_getegid:					ret << tr("SYSCALL: getegid()"); break;
	#endif
	#ifdef __NR_setpgid
	case __NR_setpgid:					ret << tr("SYSCALL: setpgid()"); break;
	#endif
	#ifdef __NR_getppid
	case __NR_getppid:					ret << tr("SYSCALL: getppid()"); break;
	#endif
	#ifdef __NR_getpgrp
	case __NR_getpgrp:					ret << tr("SYSCALL: getpgrp()"); break;
	#endif
	#ifdef __NR_setsid
	case __NR_setsid:					ret << tr("SYSCALL: setsid()"); break;
	#endif
	#ifdef __NR_setreuid
	case __NR_setreuid:					ret << tr("SYSCALL: setreuid()"); break;
	#endif
	#ifdef __NR_setregid
	case __NR_setregid:					ret << tr("SYSCALL: setregid()"); break;
	#endif
	#ifdef __NR_getgroups
	case __NR_getgroups:				ret << tr("SYSCALL: getgroups()"); break;
	#endif
	#ifdef __NR_setgroups
	case __NR_setgroups:				ret << tr("SYSCALL: setgroups()"); break;
	#endif
	#ifdef __NR_setresuid
	case __NR_setresuid:				ret << tr("SYSCALL: setresuid()"); break;
	#endif
	#ifdef __NR_getresuid
	case __NR_getresuid:				ret << tr("SYSCALL: getresuid()"); break;
	#endif
	#ifdef __NR_setresgid
	case __NR_setresgid:				ret << tr("SYSCALL: setresgid()"); break;
	#endif
	#ifdef __NR_getresgid
	case __NR_getresgid:				ret << tr("SYSCALL: getresgid()"); break;
	#endif
	#ifdef __NR_getpgid
	case __NR_getpgid:					ret << tr("SYSCALL: getpgid()"); break;
	#endif
	#ifdef __NR_setfsuid
	case __NR_setfsuid:					ret << tr("SYSCALL: setfsuid()"); break;
	#endif
	#ifdef __NR_setfsgid
	case __NR_setfsgid:					ret << tr("SYSCALL: setfsgid()"); break;
	#endif
	#ifdef __NR_getsid
	case __NR_getsid:					ret << tr("SYSCALL: getsid()"); break;
	#endif
	#ifdef __NR_capget
	case __NR_capget:					ret << tr("SYSCALL: capget()"); break;
	#endif
	#ifdef __NR_capset
	case __NR_capset:					ret << tr("SYSCALL: capset()"); break;
	#endif
	#ifdef __NR_rt_sigpending
	case __NR_rt_sigpending:			ret << tr("SYSCALL: rt_sigpending()"); break;
	#endif
	#ifdef __NR_rt_sigtimedwait
	case __NR_rt_sigtimedwait:			ret << tr("SYSCALL: rt_sigtimedwait()"); break;
	#endif
	#ifdef __NR_rt_sigqueueinfo
	case __NR_rt_sigqueueinfo:			ret << tr("SYSCALL: rt_sigqueueinfo()"); break;
	#endif
	#ifdef __NR_rt_sigsuspend
	case __NR_rt_sigsuspend:			ret << tr("SYSCALL: rt_sigsuspend()"); break;
	#endif
	#ifdef __NR_sigaltstack
	case __NR_sigaltstack:				ret << tr("SYSCALL: sigaltstack()"); break;
	#endif
	#ifdef __NR_utime
	case __NR_utime:					ret << tr("SYSCALL: utime()"); break;
	#endif
	#ifdef __NR_mknod
	case __NR_mknod:					ret << tr("SYSCALL: mknod()"); break;
	#endif
	#ifdef __NR_uselib
	case __NR_uselib:					ret << tr("SYSCALL: uselib()"); break;
	#endif
	#ifdef __NR_personality
	case __NR_personality:				ret << tr("SYSCALL: personality()"); break;
	#endif
	#ifdef __NR_ustat
	case __NR_ustat:					ret << tr("SYSCALL: ustat()"); break;
	#endif
	#ifdef __NR_statfs
	case __NR_statfs:					ret << tr("SYSCALL: statfs()"); break;
	#endif
	#ifdef __NR_fstatfs
	case __NR_fstatfs:					ret << tr("SYSCALL: fstatfs()"); break;
	#endif
	#ifdef __NR_sysfs
	case __NR_sysfs:					ret << tr("SYSCALL: sysfs()"); break;
	#endif
	#ifdef __NR_getpriority
	case __NR_getpriority:				ret << tr("SYSCALL: getpriority()"); break;
	#endif
	#ifdef __NR_setpriority
	case __NR_setpriority:				ret << tr("SYSCALL: setpriority()"); break;
	#endif
	#ifdef __NR_sched_setparam
	case __NR_sched_setparam:			ret << tr("SYSCALL: sched_setparam()"); break;
	#endif
	#ifdef __NR_sched_getparam
	case __NR_sched_getparam:			ret << tr("SYSCALL: sched_getparam()"); break;
	#endif
	#ifdef __NR_sched_setscheduler
	case __NR_sched_setscheduler:		ret << tr("SYSCALL: sched_setscheduler()"); break;
	#endif
	#ifdef __NR_sched_getscheduler
	case __NR_sched_getscheduler:		ret << tr("SYSCALL: sched_getscheduler()"); break;
	#endif
	#ifdef __NR_sched_get_priority_max
	case __NR_sched_get_priority_max:	ret << tr("SYSCALL: sched_get_priority_max()"); break;
	#endif
	#ifdef __NR_sched_get_priority_min
	case __NR_sched_get_priority_min:	ret << tr("SYSCALL: sched_get_priority_min()"); break;
	#endif
	#ifdef __NR_sched_rr_get_interval
	case __NR_sched_rr_get_interval:	ret << tr("SYSCALL: sched_rr_get_interval()"); break;
	#endif
	#ifdef __NR_mlock
	case __NR_mlock:			ret << tr("SYSCALL: mlock()"); break;
	#endif
	#ifdef __NR_munlock
	case __NR_munlock:			ret << tr("SYSCALL: munlock()"); break;
	#endif
	#ifdef __NR_mlockall
	case __NR_mlockall:			ret << tr("SYSCALL: mlockall()"); break;
	#endif
	#ifdef __NR_munlockall
	case __NR_munlockall:		ret << tr("SYSCALL: munlockall()"); break;
	#endif
	#ifdef __NR_vhangup
	case __NR_vhangup:			ret << tr("SYSCALL: vhangup()"); break;
	#endif
	#ifdef __NR_modify_ldt
	case __NR_modify_ldt:		ret << tr("SYSCALL: modify_ldt()"); break;
	#endif
	#ifdef __NR_pivot_root
	case __NR_pivot_root:		ret << tr("SYSCALL: pivot_root()"); break;
	#endif
	#ifdef __NR__sysctl
	case __NR__sysctl:			ret << tr("SYSCALL: _sysctl()"); break;
	#endif
	#ifdef __NR_prctl
	case __NR_prctl:			ret << tr("SYSCALL: prctl()"); break;
	#endif
	#ifdef __NR_arch_prctl
	case __NR_arch_prctl:		ret << tr("SYSCALL: arch_prctl()"); break;
	#endif
	#ifdef __NR_adjtimex
	case __NR_adjtimex:			ret << tr("SYSCALL: adjtimex()"); break;
	#endif
	#ifdef __NR_setrlimit
	case __NR_setrlimit:		ret << tr("SYSCALL: setrlimit()"); break;
	#endif
	#ifdef __NR_chroot
	case __NR_chroot:			ret << tr("SYSCALL: chroot()"); break;
	#endif
	#ifdef __NR_sync
	case __NR_sync:				ret << tr("SYSCALL: sync()"); break;
	#endif
	#ifdef __NR_acct
	case __NR_acct:				ret << tr("SYSCALL: acct()"); break;
	#endif
	#ifdef __NR_settimeofday
	case __NR_settimeofday:		ret << tr("SYSCALL: settimeofday()"); break;
	#endif
	#ifdef __NR_mount
	case __NR_mount:			ret << tr("SYSCALL: mount()"); break;
	#endif
	#ifdef __NR_umount2
	case __NR_umount2:			ret << tr("SYSCALL: umount2()"); break;
	#endif
	#ifdef __NR_swapon
	case __NR_swapon:			ret << tr("SYSCALL: swapon()"); break;
	#endif
	#ifdef __NR_swapoff
	case __NR_swapoff:			ret << tr("SYSCALL: swapoff()"); break;
	#endif
	#ifdef __NR_reboot
	case __NR_reboot:			ret << tr("SYSCALL: reboot()"); break;
	#endif
	#ifdef __NR_sethostname
	case __NR_sethostname:		ret << tr("SYSCALL: sethostname()"); break;
	#endif
	#ifdef __NR_setdomainname
	case __NR_setdomainname:	ret << tr("SYSCALL: setdomainname()"); break;
	#endif
	#ifdef __NR_iopl
	case __NR_iopl:				ret << tr("SYSCALL: iopl()"); break;
	#endif
	#ifdef __NR_ioperm
	case __NR_ioperm:			ret << tr("SYSCALL: ioperm()"); break;
	#endif
	#ifdef __NR_create_module
	case __NR_create_module:	ret << tr("SYSCALL: create_module()"); break;
	#endif
	#ifdef __NR_init_module
	case __NR_init_module:		ret << tr("SYSCALL: init_module()"); break;
	#endif
	#ifdef __NR_delete_module
	case __NR_delete_module:	ret << tr("SYSCALL: delete_module()"); break;
	#endif
	#ifdef __NR_get_kernel_syms
	case __NR_get_kernel_syms:	ret << tr("SYSCALL: get_kernel_syms()"); break;
	#endif
	#ifdef __NR_query_module
	case __NR_query_module:		ret << tr("SYSCALL: query_module()"); break;
	#endif
	#ifdef __NR_quotactl
	case __NR_quotactl:			ret << tr("SYSCALL: quotactl()"); break;
	#endif
	#ifdef __NR_nfsservctl
	case __NR_nfsservctl:		ret << tr("SYSCALL: nfsservctl()"); break;
	#endif
	#ifdef __NR_getpmsg
	case __NR_getpmsg:			ret << tr("SYSCALL: getpmsg()"); break;
	#endif
	#ifdef __NR_putpmsg
	case __NR_putpmsg:			ret << tr("SYSCALL: putpmsg()"); break;
	#endif
	#ifdef __NR_afs_syscall
	case __NR_afs_syscall:		ret << tr("SYSCALL: afs_syscall()"); break;
	#endif
	#ifdef __NR_tuxcall
	case __NR_tuxcall:			ret << tr("SYSCALL: tuxcall()"); break;
	#endif
	#ifdef __NR_security
	case __NR_security:			ret << tr("SYSCALL: security()"); break;
	#endif
	#ifdef __NR_gettid
	case __NR_gettid:			ret << tr("SYSCALL: gettid()"); break;
	#endif
	#ifdef __NR_readahead
	case __NR_readahead:		ret << tr("SYSCALL: readahead()"); break;
	#endif
	#ifdef __NR_setxattr
	case __NR_setxattr:			ret << tr("SYSCALL: setxattr()"); break;
	#endif
	#ifdef __NR_lsetxattr
	case __NR_lsetxattr:		ret << tr("SYSCALL: lsetxattr()"); break;
	#endif
	#ifdef __NR_fsetxattr
	case __NR_fsetxattr:		ret << tr("SYSCALL: fsetxattr()"); break;
	#endif
	#ifdef __NR_getxattr
	case __NR_getxattr:			ret << tr("SYSCALL: getxattr()"); break;
	#endif
	#ifdef __NR_lgetxattr
	case __NR_lgetxattr:		ret << tr("SYSCALL: lgetxattr()"); break;
	#endif
	#ifdef __NR_fgetxattr
	case __NR_fgetxattr:		ret << tr("SYSCALL: fgetxattr()"); break;
	#endif
	#ifdef __NR_listxattr
	case __NR_listxattr:		ret << tr("SYSCALL: listxattr()"); break;
	#endif
	#ifdef __NR_llistxattr
	case __NR_llistxattr:		ret << tr("SYSCALL: llistxattr()"); break;
	#endif
	#ifdef __NR_flistxattr
	case __NR_flistxattr:		ret << tr("SYSCALL: flistxattr()"); break;
	#endif
	#ifdef __NR_removexattr
	case __NR_removexattr:		ret << tr("SYSCALL: removexattr()"); break;
	#endif
	#ifdef __NR_lremovexattr
	case __NR_lremovexattr:		ret << tr("SYSCALL: lremovexattr()"); break;
	#endif
	#ifdef __NR_fremovexattr
	case __NR_fremovexattr:		ret << tr("SYSCALL: fremovexattr()"); break;
	#endif
	#ifdef __NR_tkill
	case __NR_tkill:			ret << tr("SYSCALL: tkill()"); break;
	#endif
	#ifdef __NR_time
	case __NR_time:				ret << tr("SYSCALL: time()"); break;
	#endif
	#ifdef __NR_futex
	case __NR_futex:			ret << tr("SYSCALL: futex()"); break;
	#endif
	#ifdef __NR_sched_setaffinity
	case __NR_sched_setaffinity:ret << tr("SYSCALL: sched_setaffinity()"); break;
	#endif
	#ifdef __NR_sched_getaffinity
	case __NR_sched_getaffinity:ret << tr("SYSCALL: sched_getaffinity()"); break;
	#endif
	#ifdef __NR_set_thread_area
	case __NR_set_thread_area:	ret << tr("SYSCALL: set_thread_area()"); break;
	#endif
	#ifdef __NR_io_setup
	case __NR_io_setup:			ret << tr("SYSCALL: io_setup()"); break;
	#endif
	#ifdef __NR_io_destroy
	case __NR_io_destroy:		ret << tr("SYSCALL: io_destroy()"); break;
	#endif
	#ifdef __NR_io_getevents
	case __NR_io_getevents:		ret << tr("SYSCALL: io_getevents()"); break;
	#endif
	#ifdef __NR_io_submit
	case __NR_io_submit:		ret << tr("SYSCALL: io_submit()"); break;
	#endif
	#ifdef __NR_io_cancel
	case __NR_io_cancel:		ret << tr("SYSCALL: io_cancel()"); break;
	#endif
	#ifdef __NR_get_thread_area
	case __NR_get_thread_area:	ret << tr("SYSCALL: get_thread_area()"); break;
	#endif
	#ifdef __NR_lookup_dcookie
	case __NR_lookup_dcookie:	ret << tr("SYSCALL: lookup_dcookie()"); break;
	#endif
	#ifdef __NR_epoll_create
	case __NR_epoll_create:		ret << tr("SYSCALL: epoll_create()"); break;
	#endif
	#ifdef __NR_epoll_ctl_old
	case __NR_epoll_ctl_old:	ret << tr("SYSCALL: epoll_ctl_old()"); break;
	#endif
	#ifdef __NR_epoll_wait_old
	case __NR_epoll_wait_old:	ret << tr("SYSCALL: epoll_wait_old()"); break;
	#endif
	#ifdef __NR_remap_file_pages
	case __NR_remap_file_pages:	ret << tr("SYSCALL: remap_file_pages()"); break;
	#endif
	#ifdef __NR_getdents64
	case __NR_getdents64:		ret << tr("SYSCALL: getdents64()"); break;
	#endif
	#ifdef __NR_set_tid_address
	case __NR_set_tid_address:	ret << tr("SYSCALL: set_tid_address()"); break;
	#endif
	#ifdef __NR_restart_syscall
	case __NR_restart_syscall:	ret << tr("SYSCALL: restart_syscall()"); break;
	#endif
	#ifdef __NR_semtimedop
	case __NR_semtimedop:		ret << tr("SYSCALL: semtimedop()"); break;
	#endif
	#ifdef __NR_fadvise64
	case __NR_fadvise64:		ret << tr("SYSCALL: fadvise64()"); break;
	#endif
	#ifdef __NR_timer_create
	case __NR_timer_create:		ret << tr("SYSCALL: timer_create()"); break;
	#endif
	#ifdef __NR_timer_settime
	case __NR_timer_settime:	ret << tr("SYSCALL: timer_settime()"); break;
	#endif
	#ifdef __NR_timer_gettime
	case __NR_timer_gettime:	ret << tr("SYSCALL: timer_gettime()"); break;
	#endif
	#ifdef __NR_timer_getoverrun
	case __NR_timer_getoverrun:	ret << tr("SYSCALL: timer_getoverrun()"); break;
	#endif
	#ifdef __NR_timer_delete
	case __NR_timer_delete:		ret << tr("SYSCALL: timer_delete()"); break;
	#endif
	#ifdef __NR_clock_settime
	case __NR_clock_settime:	ret << tr("SYSCALL: clock_settime()"); break;
	#endif
	#ifdef __NR_clock_gettime
	case __NR_clock_gettime:	ret << tr("SYSCALL: clock_gettime()"); break;
	#endif
	#ifdef __NR_clock_getres
	case __NR_clock_getres:		ret << tr("SYSCALL: clock_getres()"); break;
	#endif
	#ifdef __NR_clock_nanosleep
	case __NR_clock_nanosleep:	ret << tr("SYSCALL: clock_nanosleep()"); break;
	#endif
	#ifdef __NR_exit_group
	case __NR_exit_group:		ret << tr("SYSCALL: exit_group()"); break;
	#endif
	#ifdef __NR_epoll_wait
	case __NR_epoll_wait:		ret << tr("SYSCALL: epoll_wait()"); break;
	#endif
	#ifdef __NR_epoll_ctl
	case __NR_epoll_ctl:		ret << tr("SYSCALL: epoll_ctl()"); break;
	#endif
	#ifdef __NR_tgkill
	case __NR_tgkill:			ret << tr("SYSCALL: tgkill()"); break;
	#endif
	#ifdef __NR_utimes
	case __NR_utimes:			ret << tr("SYSCALL: utimes()"); break;
	#endif
	#ifdef __NR_vserver
	case __NR_vserver:			ret << tr("SYSCALL: vserver()"); break;
	#endif
	#ifdef __NR_mbind
	case __NR_mbind:			ret << tr("SYSCALL: mbind()"); break;
	#endif
	#ifdef __NR_set_mempolicy
	case __NR_set_mempolicy:	ret << tr("SYSCALL: set_mempolicy()"); break;
	#endif
	#ifdef __NR_get_mempolicy
	case __NR_get_mempolicy:	ret << tr("SYSCALL: get_mempolicy()"); break;
	#endif
	#ifdef __NR_mq_open
	case __NR_mq_open:			ret << tr("SYSCALL: mq_open()"); break;
	#endif
	#ifdef __NR_mq_unlink
	case __NR_mq_unlink:		ret << tr("SYSCALL: mq_unlink()"); break;
	#endif
	#ifdef __NR_mq_timedsend
	case __NR_mq_timedsend:		ret << tr("SYSCALL: mq_timedsend()"); break;
	#endif
	#ifdef __NR_mq_timedreceive
	case __NR_mq_timedreceive:	ret << tr("SYSCALL: mq_timedreceive()"); break;
	#endif
	#ifdef __NR_mq_notify
	case __NR_mq_notify:		ret << tr("SYSCALL: mq_notify()"); break;
	#endif
	#ifdef __NR_mq_getsetattr
	case __NR_mq_getsetattr:	ret << tr("SYSCALL: mq_getsetattr()"); break;
	#endif
	#ifdef __NR_kexec_load
	case __NR_kexec_load:		ret << tr("SYSCALL: kexec_load()"); break;
	#endif
	#ifdef __NR_waitid
	case __NR_waitid:			ret << tr("SYSCALL: waitid()"); break;
	#endif
	#ifdef __NR_add_key
	case __NR_add_key:			ret << tr("SYSCALL: add_key()"); break;
	#endif
	#ifdef __NR_request_key
	case __NR_request_key:		ret << tr("SYSCALL: request_key()"); break;
	#endif
	#ifdef __NR_keyctl
	case __NR_keyctl:			ret << tr("SYSCALL: keyctl()"); break;
	#endif
	#ifdef __NR_ioprio_set
	case __NR_ioprio_set:		ret << tr("SYSCALL: ioprio_set()"); break;
	#endif
	#ifdef __NR_ioprio_get
	case __NR_ioprio_get:		ret << tr("SYSCALL: ioprio_get()"); break;
	#endif
	#ifdef __NR_inotify_init
	case __NR_inotify_init:		ret << tr("SYSCALL: inotify_init()"); break;
	#endif
	#ifdef __NR_inotify_add_watch
	case __NR_inotify_add_watch:ret << tr("SYSCALL: inotify_add_watch()"); break;
	#endif
	#ifdef __NR_inotify_rm_watch
	case __NR_inotify_rm_watch:	ret << tr("SYSCALL: inotify_rm_watch()"); break;
	#endif
	#ifdef __NR_migrate_pages
	case __NR_migrate_pages:	ret << tr("SYSCALL: migrate_pages()"); break;
	#endif
	#ifdef __NR_openat
	case __NR_openat:			ret << tr("SYSCALL: openat()"); break;
	#endif
	#ifdef __NR_mkdirat
	case __NR_mkdirat:			ret << tr("SYSCALL: mkdirat()"); break;
	#endif
	#ifdef __NR_mknodat
	case __NR_mknodat:			ret << tr("SYSCALL: mknodat()"); break;
	#endif
	#ifdef __NR_fchownat
	case __NR_fchownat:			ret << tr("SYSCALL: fchownat()"); break;
	#endif
	#ifdef __NR_futimesat
	case __NR_futimesat:		ret << tr("SYSCALL: futimesat()"); break;
	#endif
	#ifdef __NR_newfstatat
	case __NR_newfstatat:		ret << tr("SYSCALL: newfstatat()"); break;
	#endif
	#ifdef __NR_unlinkat
	case __NR_unlinkat:			ret << tr("SYSCALL: unlinkat()"); break;
	#endif
	#ifdef __NR_renameat
	case __NR_renameat:			ret << tr("SYSCALL: renameat()"); break;
	#endif
	#ifdef __NR_linkat
	case __NR_linkat:			ret << tr("SYSCALL: linkat()"); break;
	#endif
	#ifdef __NR_symlinkat
	case __NR_symlinkat:		ret << tr("SYSCALL: symlinkat()"); break;
	#endif
	#ifdef __NR_readlinkat
	case __NR_readlinkat:		ret << tr("SYSCALL: readlinkat()"); break;
	#endif
	#ifdef __NR_fchmodat
	case __NR_fchmodat:			ret << tr("SYSCALL: fchmodat()"); break;
	#endif
	#ifdef __NR_faccessat
	case __NR_faccessat:		ret << tr("SYSCALL: faccessat()"); break;
	#endif
	#ifdef __NR_pselect6
	case __NR_pselect6:			ret << tr("SYSCALL: pselect6()"); break;
	#endif
	#ifdef __NR_ppoll
	case __NR_ppoll:			ret << tr("SYSCALL: ppoll()"); break;
	#endif
	#ifdef __NR_unshare
	case __NR_unshare:			ret << tr("SYSCALL: unshare()"); break;
	#endif
	#ifdef __NR_set_robust_list
	case __NR_set_robust_list:	ret << tr("SYSCALL: set_robust_list()"); break;
	#endif
	#ifdef __NR_get_robust_list
	case __NR_get_robust_list:	ret << tr("SYSCALL: get_robust_list()"); break;
	#endif
	#ifdef __NR_splice
	case __NR_splice:			ret << tr("SYSCALL: splice()"); break;
	#endif
	#ifdef __NR_tee
	case __NR_tee:				ret << tr("SYSCALL: tee()"); break;
	#endif
	#ifdef __NR_sync_file_range
	case __NR_sync_file_range:	ret << tr("SYSCALL: sync_file_range()"); break;
	#endif
	#ifdef __NR_vmsplice
	case __NR_vmsplice:			ret << tr("SYSCALL: vmsplice()"); break;
	#endif
	#ifdef __NR_move_pages
	case __NR_move_pages:		ret << tr("SYSCALL: move_pages()"); break;
	#endif
	#ifdef __NR_utimensat
	case __NR_utimensat:		ret << tr("SYSCALL: utimensat()"); break;
	#endif
	#ifdef __NR_epoll_pwait
	case __NR_epoll_pwait:		ret << tr("SYSCALL: epoll_pwait()"); break;
	#endif
	#ifdef __NR_signalfd
	case __NR_signalfd:			ret << tr("SYSCALL: signalfd()"); break;
	#endif
	#ifdef __NR_timerfd_create
	case __NR_timerfd_create:	ret << tr("SYSCALL: timerfd_create()"); break;
	#endif
	#ifdef __NR_eventfd
	case __NR_eventfd:			ret << tr("SYSCALL: eventfd()"); break;
	#endif
	#ifdef __NR_fallocate
	case __NR_fallocate:		ret << tr("SYSCALL: fallocate()"); break;
	#endif
	#ifdef __NR_timerfd_settime
	case __NR_timerfd_settime:	ret << tr("SYSCALL: timerfd_settime()"); break;
	#endif
	#ifdef __NR_timerfd_gettime
	case __NR_timerfd_gettime:	ret << tr("SYSCALL: timerfd_gettime()"); break;
        #endif
	#ifdef __NR_accept4
	case __NR_accept4:			ret << tr("SYSCALL: accept4()"); break;
	#endif
	#ifdef __NR_signalfd4
	case __NR_signalfd4:		ret << tr("SYSCALL: signalfd4()"); break;
	#endif
	#ifdef __NR_eventfd2
	case __NR_eventfd2:			ret << tr("SYSCALL: eventfd2()"); break;
	#endif
	#ifdef __NR_epoll_create1
	case __NR_epoll_create1:	ret << tr("SYSCALL: epoll_create1()"); break;
	#endif
	#ifdef __NR_dup3
	case __NR_dup3:				ret << tr("SYSCALL: dup3()"); break;
	#endif
	#ifdef __NR_pipe2
	case __NR_pipe2:			ret << tr("SYSCALL: pipe2()"); break;
	#endif
	#ifdef __NR_inotify_init1
	case __NR_inotify_init1:	ret << tr("SYSCALL: inotify_init1()"); break;
	#endif
	#ifdef __NR_preadv
	case __NR_preadv:			ret << tr("SYSCALL: preadv()"); break;
	#endif
	#ifdef __NR_pwritev
	case __NR_pwritev:			ret << tr("SYSCALL: pwritev()"); break;
	#endif
	default:
		break;
	}
#endif
}

//------------------------------------------------------------------------------
// Name: update_instruction_info(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
QStringList ArchProcessor::update_instruction_info(edb::address_t address) {

	QStringList ret;

	Q_CHECK_PTR(edb::v1::debugger_core);

	quint8 buffer[edb::Instruction::MAX_SIZE];

	const bool ok = edb::v1::debugger_core->read_bytes(address, buffer, sizeof(buffer));
	if(ok) {
		edb::Instruction insn(buffer, sizeof(buffer), address, std::nothrow);
		if(insn.valid()) {
		
			State state;
			edb::v1::debugger_core->get_state(state);
		
			// figure out the instruction type and display some information about it
			switch(insn.type()) {
			case edb::Instruction::OP_CMOVCC:
				analyze_cmov(state, insn, ret);
				break;
			case edb::Instruction::OP_RET:
				analyze_return(state, insn, ret);
				break;
			case edb::Instruction::OP_JCC:
				analyze_jump(state, insn, ret);
				// FALL THROUGH!
			case edb::Instruction::OP_JMP:
			case edb::Instruction::OP_CALL:
				analyze_call(state, insn, ret);
				break;
			#ifdef Q_OS_LINUX
			case edb::Instruction::OP_INT:
				if(insn.operand(0).complete_type() == edb::Operand::TYPE_IMMEDIATE8 && (insn.operand(0).immediate() & 0xff) == 0x80) {
					analyze_syscall(state, insn, ret);
				} else {
					analyze_operands(state, insn, ret);
				}
				break;
			#endif
			case edb::Instruction::OP_SYSCALL:
				analyze_syscall(state, insn, ret);
				break;
			default:
				analyze_operands(state, insn, ret);
				break;
			}

			analyze_jump_targets(insn, ret);

		}
	}

	// eliminate duplicates
	ret = QStringList::fromSet(ret.toSet());

	return ret;
}

//------------------------------------------------------------------------------
// Name: can_step_over(const edb::Instruction &insn) const
// Desc:
//------------------------------------------------------------------------------
bool ArchProcessor::can_step_over(const edb::Instruction &insn) const {
	return insn.valid() && (insn.type() == edb::Instruction::OP_CALL || (insn.prefix() & (edb::Instruction::PREFIX_REPNE | edb::Instruction::PREFIX_REP)));
}

//------------------------------------------------------------------------------
// Name: is_filling(const edb::Instruction &insn) const
// Desc:
//------------------------------------------------------------------------------
bool ArchProcessor::is_filling(const edb::Instruction &insn) const {
	bool ret = false;

	// fetch the operands
	if(insn.valid()) {
		const edb::Operand operands[edb::Instruction::MAX_OPERANDS] = {
			insn.operand(0),
			insn.operand(1),
			insn.operand(2)
		};

		switch(insn.type()) {
		case edb::Instruction::OP_NOP:
		case edb::Instruction::OP_INT3:
			ret = true;
			break;

		case edb::Instruction::OP_LEA:
			if(operands[0].valid() && operands[1].valid()) {
				if(operands[0].general_type() == edb::Operand::TYPE_REGISTER && operands[1].general_type() == edb::Operand::TYPE_EXPRESSION) {

					edb::Operand::Register reg1;
					edb::Operand::Register reg2;

					reg1 = operands[0].reg();

					if(operands[1].expression().scale == 1) {
						if(operands[1].expression().u_disp32 == 0) {

							if(operands[1].expression().base == edb::Operand::REG_NULL) {
								reg2 = operands[1].expression().index;
								ret = reg1 == reg2;
							} else if(operands[1].expression().index == edb::Operand::REG_NULL) {
								reg2 = operands[1].expression().base;
								ret = reg1 == reg2;
							}
						}
					}
				}
			}
			break;

		case edb::Instruction::OP_MOV:
			if(operands[0].valid() && operands[1].valid()) {
				if(operands[0].general_type() == edb::Operand::TYPE_REGISTER && operands[1].general_type() == edb::Operand::TYPE_REGISTER) {
					ret = operands[0].reg() == operands[1].reg();
				}
			}
			break;

		default:
			break;
		}

		if(!ret) {
			if(edb::v1::config().zeros_are_filling) {
				ret = (QByteArray::fromRawData(reinterpret_cast<const char *>(insn.buffer()), insn.size()) == QByteArray::fromRawData("\x00\x00", 2));
			}
		}
	} else {
		ret = (insn.size() == 1 && insn.buffer()[0] == 0x00);
	}

	return ret;
}
