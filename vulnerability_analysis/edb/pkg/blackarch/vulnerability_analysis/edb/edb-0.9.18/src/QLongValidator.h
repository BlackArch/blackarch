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

#ifndef QLONGVALIDATOR_20071128_H_
#define QLONGVALIDATOR_20071128_H_

#include <QValidator>

class QLongValidator : public QValidator {
public:
	typedef long value_type;

public:
	explicit QLongValidator(QObject *parent = 0);
	QLongValidator(value_type minimum, value_type maximum, QObject *parent = 0);
	virtual ~QLongValidator() {}

	value_type bottom() const;
	void setBottom(value_type bottom);
	virtual void setRange(value_type bottom, value_type top);
	void setTop(value_type top);
	value_type top() const;
	virtual QValidator::State validate(QString &input, int &pos) const;

private:
	value_type minimum_;
	value_type maximum_;
};

#endif

