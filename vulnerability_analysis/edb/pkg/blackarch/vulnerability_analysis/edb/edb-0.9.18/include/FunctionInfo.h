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

#ifndef FUNCTIONINFO_20070320_H_
#define FUNCTIONINFO_20070320_H_

#include "API.h"
#include <QChar>
#include <QVector>

namespace edb {
namespace internal {
	void load_function_db();
}
}

class EDB_EXPORT FunctionInfo {
	friend void edb::internal::load_function_db();
public:
	FunctionInfo(const FunctionInfo &other) : params_(other.params_) {
	}

	FunctionInfo &operator=(const FunctionInfo &rhs) {
		params_ = rhs.params_;
		return *this;
	}

	FunctionInfo() {
	}


	~FunctionInfo() {}

public:
	const QVector<QChar> &params() const { return params_; }

private:
	QVector<QChar> params_;
};

#endif

