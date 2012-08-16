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

#ifndef UTIL_20061126_H_
#define UTIL_20061126_H_

namespace util {

//------------------------------------------------------------------------------
// Name: percentage(int regions_finished, int regions_total, int bytes_done, int bytes_total)
// Desc: calculates how much of a multi-region byte search we have completed
//------------------------------------------------------------------------------
inline int percentage(int regions_finished, int regions_total, int bytes_done, int bytes_total) {

	// how much percent does each region account for?
	const double region_step      = 1.0 / static_cast<double>(regions_total) * 100;

	// how many regions are done?
	const double regions_complete = region_step * regions_finished;

	// how much of the current region is done?
	const double region_percent   = region_step * static_cast<float>(bytes_done) / static_cast<float>(bytes_total);

	return static_cast<int>(regions_complete + region_percent);
}

//------------------------------------------------------------------------------
// Name: percentage(int bytes_done, int bytes_total)
// Desc: calculates how much of a single-region byte search we have completed
//------------------------------------------------------------------------------
inline int percentage(int bytes_done, int bytes_total) {
	return percentage(0, 1, bytes_done, bytes_total);
}

}

#endif
