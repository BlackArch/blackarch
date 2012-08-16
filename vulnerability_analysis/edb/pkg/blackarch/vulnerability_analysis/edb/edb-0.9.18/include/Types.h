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

#ifndef TYPES_20071127_H_
#define TYPES_20071127_H_

#include "ArchTypes.h"
#include "OSTypes.h"

namespace edb {
	enum EVENT_STATUS {
		DEBUG_STOP,                 // do nothing, the UI will instigate the next event
		DEBUG_CONTINUE,             // the event has been address, continue as normal
		DEBUG_CONTINUE_STEP,        // the event has been address, step as normal
		DEBUG_EXCEPTION_NOT_HANDLED // pass the event unmodified back thread and continue
	};
}

#endif
