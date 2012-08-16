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
*/\
#ifndef DIALOGPLUGINS_20080926_H_
#define DIALOGPLUGINS_20080926_H_

namespace Ui { class DialogPlugins; }

#include <QDialog>

class DialogPlugins : public QDialog {
	Q_OBJECT
public:
	DialogPlugins(QWidget *parent = 0, Qt::WindowFlags f = 0);
	virtual ~DialogPlugins();

public:
	void showEvent(QShowEvent *);

private:
	Ui::DialogPlugins *const ui;
};

#endif

