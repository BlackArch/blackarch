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
#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QStringList>


#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/vm_region.h>
#include <mach/vm_statistics.h>
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

//------------------------------------------------------------------------------
// Name: sync()
// Desc: reads a memory map file line by line
//------------------------------------------------------------------------------
void MemoryRegions::sync() {
#if 0
    static const char * inheritance_strings[] = {
		"SHARE", "COPY", "NONE", "DONATE_COPY",
	};

	static const char * behavior_strings[] = {
		"DEFAULT", "RANDOM", "SEQUENTIAL", "RESQNTL", "WILLNEED", "DONTNEED",
	};
#endif

	QList<MemRegion> regions;
	if(pid_ != 0) {
		task_t the_task;
		kern_return_t kr = task_for_pid(mach_task_self(), pid_, &the_task);
		if(kr != KERN_SUCCESS) {
			qDebug("task_for_pid failed");
			return;
		}

		vm_size_t vmsize;
		vm_address_t address;
		vm_region_basic_info_data_64_t info;
		mach_msg_type_number_t info_count;
		vm_region_flavor_t flavor;
		memory_object_name_t object;

		kr = KERN_SUCCESS;
		address = 0;

		do {
			flavor     = VM_REGION_BASIC_INFO_64;
			info_count = VM_REGION_BASIC_INFO_COUNT_64;
			kr = vm_region_64(the_task, &address, &vmsize, flavor, (vm_region_info_64_t)&info, &info_count, &object);
			if(kr == KERN_SUCCESS) {

				MemRegion region;
				region.start        = address;
				region.end          = address + vmsize;
				region.base         = address;
				region.name         = QString();
				region.permissions_ =
					((info.protection & VM_PROT_READ)    ? PROT_READ  : 0) |
					((info.protection & VM_PROT_WRITE)   ? PROT_WRITE : 0) |
					((info.protection & VM_PROT_EXECUTE) ? PROT_EXEC  : 0);

				regions.push_back(region);

				/*
				printf("%016llx-%016llx %8uK %c%c%c/%c%c%c %11s %6s %10s uwir=%hu sub=%u\n",
				address, (address + vmsize), (vmsize >> 10),
				(info.protection & VM_PROT_READ)        ? 'r' : '-',
				(info.protection & VM_PROT_WRITE)       ? 'w' : '-',
				(info.protection & VM_PROT_EXECUTE)     ? 'x' : '-',
				(info.max_protection & VM_PROT_READ)    ? 'r' : '-',
				(info.max_protection & VM_PROT_WRITE)   ? 'w' : '-',
				(info.max_protection & VM_PROT_EXECUTE) ? 'x' : '-',
				inheritance_strings[info.inheritance],
				(info.shared) ? "shared" : "-",
				behavior_strings[info.behavior],
				info.user_wired_count,
				info.reserved);
				*/

				address += vmsize;
			} else if(kr != KERN_INVALID_ADDRESS) {
				if(the_task != MACH_PORT_NULL) {
					mach_port_deallocate(mach_task_self(), the_task);
				}
				return;
			}
		} while(kr != KERN_INVALID_ADDRESS);

		if(the_task != MACH_PORT_NULL) {
			mach_port_deallocate(mach_task_self(), the_task);
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
		case 0:
			return edb::v1::format_pointer(region.start);
		case 1:
			return edb::v1::format_pointer(region.end);
		case 2:
			return QString("%1%2%3").arg(region.readable() ? 'r' : '-').arg(region.writable() ? 'w' : '-').arg(region.executable() ? 'x' : '-' );
		case 3:
			return region.name;
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
		case 0:
			return tr("Start Address");
		case 1:
			return tr("End Address");
		case 2:
			return tr("Permissions");
		case 3:
			return tr("Name");
		}
	}

	return QVariant();
}

