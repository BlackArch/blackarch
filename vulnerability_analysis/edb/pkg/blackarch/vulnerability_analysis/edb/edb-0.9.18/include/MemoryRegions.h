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

#ifndef MEMORY_REGIONS_20060501_H_
#define MEMORY_REGIONS_20060501_H_

#include "Types.h"
#include "API.h"
#include "MemRegion.h"

#include <QAbstractItemModel>
#include <QList>

class EDB_EXPORT MemoryRegions : public QAbstractItemModel {
	Q_OBJECT

public:
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &index) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:
	MemoryRegions();
	virtual ~MemoryRegions();

	void set_pid(edb::pid_t pid);
	void clear();

	const QList<MemRegion> &regions() const { return regions_; }

	void sync();

	bool find_region(edb::address_t address, MemRegion &region) const;
	bool find_region(edb::address_t address) const;

private:
	edb::pid_t       pid_;
	QList<MemRegion> regions_;
};

// provide a reasonable hash function for the region
inline uint qHash(const MemRegion &region) {
	return qHash(region.start);
}

#endif
