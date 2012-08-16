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

#ifndef BREAKPOINT_20060720_H_
#define BREAKPOINT_20060720_H_

#include "Types.h"

#include <QByteArray>
#include <QString>
#include <QSharedPointer>

class Breakpoint {
public:
	typedef QSharedPointer<Breakpoint> pointer;
	
public:
	virtual ~Breakpoint() {}

public:
	virtual edb::address_t address() const = 0;
	virtual unsigned int hit_count() const = 0;
	virtual bool enabled() const = 0;
	virtual bool one_time() const = 0;
	virtual bool internal() const = 0;
	virtual QByteArray original_bytes() const = 0;

public:
	virtual bool enable() = 0;
	virtual bool disable() = 0;
	virtual void hit() = 0;
	virtual void set_one_time(bool value) = 0;
	virtual void set_internal(bool value) = 0;

public:
	QString condition;
};

#endif
