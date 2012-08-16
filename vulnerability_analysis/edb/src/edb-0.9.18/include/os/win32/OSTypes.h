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

#ifndef OSTYPES_20070116_H_
#define OSTYPES_20070116_H_

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

namespace edb {
	typedef DWORD pid_t;
	typedef DWORD uid_t;
	typedef DWORD tid_t;
}

#endif
