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

#ifndef DATAVIEWINFO_20100101_H_
#define DATAVIEWINFO_20100101_H_

#include "MemRegion.h"
#include "RegionBuffer.h"
#include <QSharedPointer>
#include "QHexView"

class DataViewInfo {
public:
	typedef QSharedPointer<DataViewInfo> pointer;
public:
	DataViewInfo(const MemRegion &r = MemRegion());
	~DataViewInfo();

private:
	Q_DISABLE_COPY(DataViewInfo)

public:
	MemRegion                    region;
	QSharedPointer<RegionBuffer> stream;
	QSharedPointer<QHexView>     view;

public:
	void update();
};

#endif

