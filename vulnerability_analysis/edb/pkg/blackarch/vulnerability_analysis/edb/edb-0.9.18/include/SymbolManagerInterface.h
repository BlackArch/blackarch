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

#ifndef SYMBOLMANAGERINTERFACE_20110307_H_
#define SYMBOLMANAGERINTERFACE_20110307_H_

#include "API.h"
#include "Types.h"
#include "Symbol.h"
#include <QList>

class QString;

class EDB_EXPORT SymbolManagerInterface {
public:
	virtual ~SymbolManagerInterface() {}

public:
	virtual const QList<Symbol::pointer> symbols() const = 0;
	virtual const Symbol::pointer find(const QString &name) const = 0;
	virtual const Symbol::pointer find(edb::address_t address) const = 0;
	virtual const Symbol::pointer find_near_symbol(edb::address_t address) const = 0;
	virtual void clear() = 0;
	virtual void load_symbol_file(const QString &filename, edb::address_t base) = 0;
	virtual void load_symbols(const QString &symbol_directory) = 0;
	virtual void add_symbol(const Symbol::pointer &symbol) = 0;
};

#endif
