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

#ifndef REGIONBUFFER_20101111_H_
#define REGIONBUFFER_20101111_H_

#include <QIODevice>
#include "MemRegion.h"

class RegionBuffer : public QIODevice {
	Q_OBJECT
public:
	RegionBuffer(const MemRegion &region);
	RegionBuffer(const MemRegion &region, QObject *parent);

public:
	void set_region(const MemRegion &region);

public:
	virtual qint64 readData(char * data, qint64 maxSize);
	virtual qint64 writeData(const char*, qint64);
	virtual qint64 size() const       { return region_.size(); }
	virtual bool isSequential() const { return false; }

private:
	MemRegion region_;
};

#endif

