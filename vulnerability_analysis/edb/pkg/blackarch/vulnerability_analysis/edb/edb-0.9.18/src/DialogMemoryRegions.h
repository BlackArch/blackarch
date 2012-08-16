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

#ifndef DIALOGMEMORYREGIONS_20061101_H_
#define DIALOGMEMORYREGIONS_20061101_H_

#include <QDialog>

class QSortFilterProxyModel;
class MemRegion;

namespace Ui { class DialogMemoryRegions; }

class DialogMemoryRegions : public QDialog {
	Q_OBJECT
public:
	DialogMemoryRegions(QWidget *parent = 0);
	virtual ~DialogMemoryRegions();

private:
	virtual void showEvent(QShowEvent *event);

private Q_SLOTS:
	void on_regions_table_customContextMenuRequested(const QPoint &pos);
	void set_access_none();
	void set_access_r();
	void set_access_w();
	void set_access_x();
	void set_access_rw();
	void set_access_rx();
	void set_access_wx();
	void set_access_rwx();
	void view_in_cpu();
	void view_in_stack();
	void view_in_dump();

public:
	MemRegion *selected_region() const;

private:
	void set_permissions(bool read, bool write, bool execute);

private:
	Ui::DialogMemoryRegions *const ui;
	QSortFilterProxyModel *        filter_model_;
};

#endif
