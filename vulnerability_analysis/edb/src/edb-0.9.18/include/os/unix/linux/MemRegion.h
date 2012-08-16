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
#ifndef MEMREGION_20081209_H_
#define MEMREGION_20081209_H_

#include "Types.h"
#include "API.h"

#include <QString>
#include <QCoreApplication>

class MemoryRegions;

class EDB_EXPORT MemRegion {
	Q_DECLARE_TR_FUNCTIONS(MemRegion)
	friend class MemoryRegions;

public:
	typedef quint8 permissions_t;

public:
	MemRegion() : start(0), end(0), base(0), permissions_(0) {
	}

	virtual ~MemRegion();

public:
	MemRegion(const MemRegion &o) : start(o.start), end(o.end), base(o.base), name(o.name), permissions_(o.permissions_) {
	}

	MemRegion &operator=(const MemRegion &o) {
		if(&o != this) {
			MemRegion(o).swap(*this);
		}
		return *this;
	}

	bool operator==(const MemRegion &o) const {
		return
			start        == o.start &&
			end          == o.end &&
			base         == o.base &&
			name         == o.name &&
			permissions_ == o.permissions_;
	}

	bool operator!=(const MemRegion &o) const {
		return !(operator==(o));
	}

	bool operator<(const MemRegion &o) const {
		return start < o.start;
	}

	bool contains(edb::address_t address) const {
		return address >= start && address < end;
	}

	void swap(MemRegion &other);

public:
	bool accessible() const;
	bool readable() const;
	bool writable() const;
	bool executable() const;
	int size() const;

	void set_permissions(bool read, bool write, bool execute);

private:
	void set_permissions(bool read, bool write, bool execute, edb::address_t temp_address);

public:
	edb::address_t start;
	edb::address_t end;
	edb::address_t base;
	QString        name;

public:
	// TODO: eventually make this private, MemoryRegions has a non-member helper function
	// which writes to this, we can make this cleaner though..
	permissions_t  permissions_;
};

#endif
