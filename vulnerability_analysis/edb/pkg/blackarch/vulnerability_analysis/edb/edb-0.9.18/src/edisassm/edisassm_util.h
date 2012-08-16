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

#ifndef EDISASSM_UTIL_20080314_H_
#define EDISASSM_UTIL_20080314_H_

#include <string>
#include <algorithm>
#include <functional>
#include <cctype>

#define UNUSED(x) (void)(x)

namespace edisassm {
	namespace util {
		inline std::string toupper_copy(std::string s) {
			std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::toupper));
			return s;
		}

		inline std::string &toupper(std::string &s) {
			std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::toupper));
			return s;
		}

		inline std::string &tolower(std::string &s) {
			std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::tolower));
			return s;
		}

		inline std::string &rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
		}

		inline std::string &ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
		}

		inline std::string &trim(std::string &s) {
			return ltrim(rtrim(s));
		}

		template <class T>
		bool is_small_num(T value) {
			return (value > -127 && value < 128);
		}
	}
}

#endif
