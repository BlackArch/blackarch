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

#include "RegionBuffer.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"

//------------------------------------------------------------------------------
// Name: RegionBuffer(const MemRegion &region)
// Desc:
//------------------------------------------------------------------------------
RegionBuffer::RegionBuffer(const MemRegion &region) : QIODevice(), region_(region) {
	setOpenMode(QIODevice::ReadOnly);
}

//------------------------------------------------------------------------------
// Name: RegionBuffer(const MemRegion &region, QObject *parent)
// Desc:
//------------------------------------------------------------------------------
RegionBuffer::RegionBuffer(const MemRegion &region, QObject *parent) : QIODevice(parent), region_(region) {
	setOpenMode(QIODevice::ReadOnly);
}

//------------------------------------------------------------------------------
// Name: set_region(const MemRegion &region)
// Desc:
//------------------------------------------------------------------------------
void RegionBuffer::set_region(const MemRegion &region) {
	region_ = region;
	reset();
}

//------------------------------------------------------------------------------
// Name: readData(char *data, qint64 maxSize)
// Desc:
//------------------------------------------------------------------------------
qint64 RegionBuffer::readData(char *data, qint64 maxSize) {

	const edb::address_t start = region_.start + pos();
	const edb::address_t end   = region_.start + region_.size();

	if(start + maxSize > end) {
		maxSize = end - start;
	}

    if(maxSize == 0) {
        return 0;
    }

	if(edb::v1::debugger_core->read_bytes(start, data, maxSize)) {
		return maxSize;
	} else {
		return -1;
	}

}

//------------------------------------------------------------------------------
// Name: writeData(const char*, qint64)
// Desc:
//------------------------------------------------------------------------------
qint64 RegionBuffer::writeData(const char*, qint64) {
	return -1;
}
