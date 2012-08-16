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

#ifndef SESSIONFILEINTERFACE_20090212_H_
#define SESSIONFILEINTERFACE_20090212_H_

class QString;

class SessionFileInterface {
public:
	virtual ~SessionFileInterface() {}

public:
	virtual void save_session(const QString &filename, const QString &executable) = 0;
	virtual void load_session(const QString &filename, const QString &executable) = 0;
};

#endif
