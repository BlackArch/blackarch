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

#ifndef RECENTFILEMANAGER_20070430_H_
#define RECENTFILEMANAGER_20070430_H_

#include <QStringList>
#include <QString>
#include <QWidget>

class QMenu;
class QWidget;

class RecentFileManager : public QWidget {
	Q_OBJECT
public:
	RecentFileManager(QWidget * parent = 0, Qt::WindowFlags f = 0);
	virtual ~RecentFileManager();

public:
	void add_file(const QString &file);
	QMenu *create_menu();

public Q_SLOTS:
	void clear_file_list();
	void item_selected();

signals:
	void file_selected(const QString &);

private:
	void save_file_list();
	void load_file_list();
	void update();

private:
	static const int max_recent_files = 4;

private:
	QStringList file_list_;
	QMenu *     menu_;
};

#endif

