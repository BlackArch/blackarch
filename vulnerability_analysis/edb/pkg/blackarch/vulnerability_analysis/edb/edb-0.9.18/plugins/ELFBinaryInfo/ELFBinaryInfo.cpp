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

#include "ELFBinaryInfo.h"
#include "ELF32.h"
#include "ELF64.h"
#include "BinaryInfo.h"
#include "Debugger.h"

namespace {
	BinaryInfo *create_binary_info32(const MemRegion &region) {
		return new ELF32(region);
	}

	BinaryInfo *create_binary_info64(const MemRegion &region) {
		return new ELF64(region);
	}
}

//------------------------------------------------------------------------------
// Name: ELFBinaryInfo()
// Desc:
//------------------------------------------------------------------------------
ELFBinaryInfo::ELFBinaryInfo() {
}

//------------------------------------------------------------------------------
// Name: ELFBinaryInfo()
// Desc:
//------------------------------------------------------------------------------
void ELFBinaryInfo::private_init() {
	edb::v1::register_binary_info(create_binary_info32);
	edb::v1::register_binary_info(create_binary_info64);
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *ELFBinaryInfo::menu(QWidget *parent) {
	Q_UNUSED(parent);
	return 0;
}

Q_EXPORT_PLUGIN2(ELFBinaryInfo, ELFBinaryInfo)
