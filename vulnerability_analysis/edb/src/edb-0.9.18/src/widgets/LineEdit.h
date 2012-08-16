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

#ifndef LINEEDIT_20110331_H_
#define LINEEDIT_20110331_H_

#include <QLineEdit>

class QToolButton;

class LineEdit : public QLineEdit {
	Q_OBJECT

public:
	LineEdit(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *);

private Q_SLOTS:
	void updateCloseButton(const QString &text);

private:
	QToolButton *const clear_button_;
};

#endif
