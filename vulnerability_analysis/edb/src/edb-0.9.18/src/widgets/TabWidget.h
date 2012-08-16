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

#ifndef TABWIDGET_20080118_H_
#define TABWIDGET_20080118_H_

#include <QTabWidget>

class TabWidget : public QTabWidget {
	Q_OBJECT

public:
	TabWidget(QWidget *parent = 0);
	virtual ~TabWidget();

Q_SIGNALS:
	void customContextMenuRequested(int, const QPoint &);

public:
	void setData(int index, const QVariant &data);
	QVariant data(int index) const;

protected:
	virtual void mousePressEvent(QMouseEvent *event);
};

#endif

