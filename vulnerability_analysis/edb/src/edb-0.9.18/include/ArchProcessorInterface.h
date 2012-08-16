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

#ifndef ARCHPROCESSORINTERFACE_20070717_H_
#define ARCHPROCESSORINTERFACE_20070717_H_

#include "Types.h"
#include "Register.h"
#include <QStringList>

class QCategoryList;
class QTreeWidgetItem;
class QByteArray;

class ArchProcessorInterface {
public:
	virtual ~ArchProcessorInterface() {}
public:
	virtual void setup_register_view(QCategoryList *categoryList) = 0;
	virtual void reset() = 0;
	virtual void update_register_view(const QString &default_region_name) = 0;
	virtual QStringList update_instruction_info(edb::address_t address) = 0;
	virtual Register value_from_item(const QTreeWidgetItem &item) = 0;

public:
	// instruction inspection
	virtual bool is_filling(const edb::Instruction &insn) const = 0;
	virtual bool can_step_over(const edb::Instruction &insn) const = 0;
};

#endif
