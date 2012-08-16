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

#ifndef ARCHPROCESSOR_20070312_H_
#define ARCHPROCESSOR_20070312_H_

#include <QVector>
#include <QStringList>
#include <QString>
#include <QCoreApplication>
#include "Types.h"
#include "State.h"
#include "ArchProcessorInterface.h"

class QTreeWidgetItem;

class ArchProcessor : public ArchProcessorInterface {
public:
	ArchProcessor();
	virtual ~ArchProcessor() {}

private:
	Q_DISABLE_COPY(ArchProcessor)
	Q_DECLARE_TR_FUNCTIONS(ArchProcessor)

public:
	virtual void setup_register_view(QCategoryList *category_list);
	virtual void update_register_view(const QString &default_region_name);
	virtual QStringList update_instruction_info(edb::address_t address);
	virtual Register value_from_item(const QTreeWidgetItem &item);
	virtual void reset();

public:
	virtual bool is_filling(const edb::Instruction &insn) const;
	virtual bool can_step_over(const edb::Instruction &insn) const;
	
private:
	edb::address_t get_effective_address(const edb::Operand &op, const State &state) const;

private:
	QTreeWidgetItem *get_register_item(unsigned int index);
	void update_register(QTreeWidgetItem *item, const QString &name, const Register &reg) const;
	void setup_register_item(QCategoryList *category_list, QTreeWidgetItem *parent, const QString &name);

private:
	void resolve_function_parameters(const State &state, const QString &symname, int offset, QStringList &ret) const;
	void analyze_call(const State &state, const edb::Instruction &insn, QStringList &ret) const;
	void analyze_jump(const State &state, const edb::Instruction &insn, QStringList &ret) const;
	void analyze_return(const State &state, const edb::Instruction &insn, QStringList &ret) const;
	void analyze_operands(const State &state, const edb::Instruction &insn, QStringList &ret) const;
	void analyze_cmov(const State &state, const edb::Instruction &insn, QStringList &ret) const;
	void analyze_syscall(const State &state, const edb::Instruction &insn, QStringList &ret) const;

	QString format_argument(QChar ch, edb::reg_t arg) const;

	void analyze_jump_targets(const edb::Instruction &insn, QStringList &ret) const;

	bool is_jcc_taken(const State &state, quint8 instruction_byte) const;

private:
	QVector<QTreeWidgetItem *> register_view_items_;
	QTreeWidgetItem *          split_flags_;
	State                      last_state_;
	
private:
	bool has_mmx_;
	bool has_xmm_;
};

#endif

