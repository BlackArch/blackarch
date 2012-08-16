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

#ifndef SYMBOL_20110401_H_
#define SYMBOL_20110401_H_

#include "API.h"
#include "Types.h"
#include <QString>
#include <QSharedPointer>

class EDB_EXPORT Symbol {
public:
	typedef QSharedPointer<Symbol> pointer;

public:
	QString        file;
	QString        name;
	QString        name_no_prefix;
	edb::address_t address;
	quint32        size;
	char           type;

	bool is_code() const { return type == 't' || type == 'T' || type == 'P'; }
	bool is_data() const { return !is_code(); }
	bool is_weak() const { return type == 'W'; }
};

#endif

