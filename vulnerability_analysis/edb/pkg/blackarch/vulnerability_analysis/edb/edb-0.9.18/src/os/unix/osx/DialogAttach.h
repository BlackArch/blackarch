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

#ifndef DIALOGATTACH_20061101_H_
#define DIALOGATTACH_20061101_H_

#include "DialogAttachUNIX.h"

class DialogAttach : public DialogAttachUNIX {
	Q_OBJECT

public:
	DialogAttach(QWidget *parent = 0);
	virtual ~DialogAttach() {}

protected:
	virtual QMap<edb::pid_t, ProcessInfo> find_processes();
};

#endif
