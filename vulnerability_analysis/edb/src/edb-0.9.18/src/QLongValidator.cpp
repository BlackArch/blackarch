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

#include "QLongValidator.h"

//------------------------------------------------------------------------------
// Name: QLongValidator(QObject *parent)
// Desc:
//------------------------------------------------------------------------------
QLongValidator::QLongValidator(QObject *parent) : QValidator(parent), minimum_(0), maximum_(0) {
}

//------------------------------------------------------------------------------
// Name: QLongValidator(QLongValidator::value_type minimum, QLongValidator::value_type maximum, QObject *parent)
// Desc:
//------------------------------------------------------------------------------
QLongValidator::QLongValidator(QLongValidator::value_type minimum, QLongValidator::value_type maximum, QObject *parent) : QValidator(parent), minimum_(minimum), maximum_(maximum) {
}

//------------------------------------------------------------------------------
// Name: bottom() const
// Desc:
//------------------------------------------------------------------------------
QLongValidator::value_type QLongValidator::bottom() const {
	return minimum_;
}

//------------------------------------------------------------------------------
// Name: setBottom(QLongValidator::value_type bottom)
// Desc:
//------------------------------------------------------------------------------
void QLongValidator::setBottom(QLongValidator::value_type bottom) {
	minimum_ = bottom;
}

//------------------------------------------------------------------------------
// Name: setRange(QLongValidator::value_type bottom, QLongValidator::value_type top)
// Desc:
//------------------------------------------------------------------------------
void QLongValidator::setRange(QLongValidator::value_type bottom, QLongValidator::value_type top) {
	setBottom(bottom);
	setTop(top);
}

//------------------------------------------------------------------------------
// Name: setTop(QLongValidator::value_type top)
// Desc:
//------------------------------------------------------------------------------
void QLongValidator::setTop(QLongValidator::value_type top) {
	maximum_ = top;
}

//------------------------------------------------------------------------------
// Name: top() const
// Desc:
//------------------------------------------------------------------------------
QLongValidator::value_type QLongValidator::top() const {
	return maximum_;
}

//------------------------------------------------------------------------------
// Name: validate(QString &input, int &pos) const
// Desc:
//------------------------------------------------------------------------------
QValidator::State QLongValidator::validate(QString &input, int &pos) const {
	Q_UNUSED(pos);

	if(input.isEmpty()) {
		return QValidator::Acceptable;
	}

	if(input.length() == 1 && input == "-") {
		return QValidator::Intermediate;
	}

	bool ok;
	const value_type temp = input.toLong(&ok);
	if(!ok) {
		return QValidator::Invalid;
	}

	return (temp >= bottom() && temp <= top()) ? QValidator::Acceptable : QValidator::Intermediate;
}
