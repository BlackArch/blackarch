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

#ifndef BOOKMARKS_20061122_H_
#define BOOKMARKS_20061122_H_

#include "DebuggerPluginInterface.h"
#include "Types.h"
#include <QSet>
#include <QVariantList>

class QSignalMapper;
class BookmarkWidget;

class Bookmarks : public QObject, public DebuggerPluginInterface {
	Q_OBJECT
	Q_INTERFACES(DebuggerPluginInterface)
	Q_CLASSINFO("author", "Evan Teran")
	Q_CLASSINFO("url", "http://www.codef00.com")
	
	Q_PROPERTY(QVariantList addresses READ addresses)


public:
	Bookmarks();

public:
	virtual QMenu *menu(QWidget *parent = 0);
	virtual QList<QAction *> cpu_context_menu();

public Q_SLOTS:
	void add_bookmark_menu();

public:
	QVariantList addresses() const;

private:
	QMenu *              menu_;
	QSignalMapper *      signal_mapper_;
	BookmarkWidget *     bookmark_widget_;
};

#endif
