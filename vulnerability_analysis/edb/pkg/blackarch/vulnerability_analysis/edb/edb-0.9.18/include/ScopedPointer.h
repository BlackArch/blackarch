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

#ifndef SCOPED_POINTER_20090821_H_
#define SCOPED_POINTER_20090821_H_

#if QT_VERSION >= 0x040600
	#include <QScopedPointer>
	#define SCOPED_POINTER QScopedPointer
#else
	#include <boost/scoped_ptr.hpp>
	#define SCOPED_POINTER boost::scoped_ptr
#endif

#endif
