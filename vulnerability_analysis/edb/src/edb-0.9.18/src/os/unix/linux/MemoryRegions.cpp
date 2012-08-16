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

#include "MemoryRegions.h"
#include "SymbolManager.h"
#include "DebuggerCoreInterface.h"
#include "Util.h"
#include "State.h"
#include "Debugger.h"

#include <sstream>
#include <string>
#include <cstring>

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QtGlobal>

#include <sys/mman.h>

//------------------------------------------------------------------------------
// Name: MemoryRegions()
// Desc: constructor
//------------------------------------------------------------------------------
MemoryRegions::MemoryRegions() : QAbstractItemModel(0), pid_(0) {
}

//------------------------------------------------------------------------------
// Name: ~MemoryRegions()
// Desc: destructor
//------------------------------------------------------------------------------
MemoryRegions::~MemoryRegions() {
}

//------------------------------------------------------------------------------
// Name: set_pid(edb::pid_t pid)
// Desc:
//------------------------------------------------------------------------------
void MemoryRegions::set_pid(edb::pid_t pid) {
	pid_ = pid;
	regions_.clear();
	sync();
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc:
//------------------------------------------------------------------------------
void MemoryRegions::clear() {
	pid_ = 0;
	regions_.clear();
}

namespace {
	//------------------------------------------------------------------------------
	// Name: process_map_line(const QString &line, MemRegion &region)
	// Desc: parses the data from a line of a memory map file
	//------------------------------------------------------------------------------
	bool process_map_line(const QString &line, MemRegion &region) {

		bool ret = false;
		const QStringList items = line.split(" ", QString::SkipEmptyParts);
		if(items.size() >= 3) {
			bool ok;
			const QStringList bounds = items[0].split("-");
			if(bounds.size() == 2) {
				region.start = bounds[0].toULongLong(&ok, 16);
				if(ok) {
					region.end = bounds[1].toULongLong(&ok, 16);
					if(ok) {
						region.base = items[2].toULongLong(&ok, 16);
						if(ok) {
							const QString perms = items[1];
							MemRegion::permissions_t permissions = 0;
							if(perms[0] == 'r') permissions |= PROT_READ;
							if(perms[1] == 'w') permissions |= PROT_WRITE;
							if(perms[2] == 'x') permissions |= PROT_EXEC;
							region.permissions_ = permissions;

							if(items.size() >= 6) {
								region.name = items[5];
							}

							// if the region has a name, is mapped starting
							// at the beginning of the file, and is executable, sounds
							// like a module mapping!
							if(!region.name.isEmpty()) {
								if(region.base == 0) {
									if(region.executable()) {
										edb::v1::symbol_manager().load_symbol_file(region.name, region.start);
									}
								}
							}

							ret = true;
						}
					}
				}
			}
		}
		return ret;
	}
}

//------------------------------------------------------------------------------
// Name: sync()
// Desc: reads a memory map file line by line
//------------------------------------------------------------------------------
void MemoryRegions::sync() {

	QList<MemRegion> regions;

	if(pid_ != 0) {
		const QString mapFile(QString("/proc/%1/maps").arg(pid_));

		QFile file(mapFile);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

			QTextStream in(&file);
			QString line = in.readLine();

			while(!line.isNull()) {
				MemRegion region;
				if(process_map_line(line, region)) {
					regions.push_back(region);
				}
				line = in.readLine();
			}
		}

		if(regions.isEmpty()) {
			qDebug() << "[MemoryRegions] warning: empty memory map";
		}
	}


	qSwap(regions_, regions);

	reset();
}

//------------------------------------------------------------------------------
// Name: find_region(edb::address_t address) const
// Desc:
//------------------------------------------------------------------------------
bool MemoryRegions::find_region(edb::address_t address) const {
	Q_FOREACH(const MemRegion &i, regions_) {
		if(i.contains(address)) {
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: find_region(edb::address_t address, MemRegion &region) const
// Desc:
//------------------------------------------------------------------------------
bool MemoryRegions::find_region(edb::address_t address, MemRegion &region) const {
	Q_FOREACH(const MemRegion &i, regions_) {
		if(i.contains(address)) {
			region = i;
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: data(const QModelIndex &index, int role) const
// Desc:
//------------------------------------------------------------------------------
QVariant MemoryRegions::data(const QModelIndex &index, int role) const {

	if(index.isValid() && role == Qt::DisplayRole) {

		const MemRegion &region = regions_[index.row()];

		switch(index.column()) {
		case 0: return edb::v1::format_pointer(region.start);
		case 1: return edb::v1::format_pointer(region.end);
		case 2: return QString("%1%2%3").arg(region.readable() ? 'r' : '-').arg(region.writable() ? 'w' : '-').arg(region.executable() ? 'x' : '-' );
		case 3: return region.name;
		}
	}

	return QVariant();
}

//------------------------------------------------------------------------------
// Name: index(int row, int column, const QModelIndex &parent) const
// Desc:
//------------------------------------------------------------------------------
QModelIndex MemoryRegions::index(int row, int column, const QModelIndex &parent) const {
	Q_UNUSED(parent);

	if(row >= rowCount(parent) || column >= columnCount(parent)) {
		return QModelIndex();
	}

	return createIndex(row, column, const_cast<MemRegion *>(&regions_[row]));
}

//------------------------------------------------------------------------------
// Name: parent(const QModelIndex &index) const
// Desc:
//------------------------------------------------------------------------------
QModelIndex MemoryRegions::parent(const QModelIndex &index) const {
	Q_UNUSED(index);
	return QModelIndex();
}

//------------------------------------------------------------------------------
// Name: rowCount(const QModelIndex &parent) const
// Desc:
//------------------------------------------------------------------------------
int MemoryRegions::rowCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return regions_.size();
}

//------------------------------------------------------------------------------
// Name: columnCount(const QModelIndex &parent) const
// Desc:
//------------------------------------------------------------------------------
int MemoryRegions::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return 4;
}

//------------------------------------------------------------------------------
// Name: headerData(int section, Qt::Orientation orientation, int role) const
// Desc:
//------------------------------------------------------------------------------
QVariant MemoryRegions::headerData(int section, Qt::Orientation orientation, int role) const {
	if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch(section) {
		case 0: return tr("Start Address");
		case 1: return tr("End Address");
		case 2: return tr("Permissions");
		case 3: return tr("Name");
		}
	}

	return QVariant();
}

