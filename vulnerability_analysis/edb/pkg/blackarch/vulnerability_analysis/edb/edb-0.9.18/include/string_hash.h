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

#ifndef STRING_HASH_20110823_H_
#define STRING_HASH_20110823_H_

#include <QtGlobal>

namespace edb {
	template <char Ch1, char Ch2 = '\0', char Ch3 = '\0', char Ch4 = '\0', char Ch5 = '\0', char Ch6 = '\0', char Ch7 = '\0', char Ch8 = '\0'>
	struct string_hash {
		static const quint64 value = 
			(static_cast<quint64>(Ch1) << 56) | 
			(static_cast<quint64>(Ch2) << 48) | 
			(static_cast<quint64>(Ch3) << 40) | 
			(static_cast<quint64>(Ch4) << 32) | 
			(static_cast<quint64>(Ch5) << 24) | 
			(static_cast<quint64>(Ch6) << 16) | 
			(static_cast<quint64>(Ch7) << 8)  | 
			(Ch8);
	};
}

#endif
