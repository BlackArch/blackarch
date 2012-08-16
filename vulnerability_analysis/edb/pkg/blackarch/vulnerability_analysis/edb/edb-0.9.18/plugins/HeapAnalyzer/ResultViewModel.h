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

#ifndef RESULTVIEWMODEL_20070419_H_
#define RESULTVIEWMODEL_20070419_H_

#include <QAbstractItemModel>
#include <QVector>
#include "Types.h"

struct Result {
	Result() : block(0), size(0) {
	}

	Result(edb::address_t block, edb::address_t size, const QString &type, const QString &data = QString()) : block(block), size(size), type(type), data(data) {
	}

	edb::address_t        block;
	edb::address_t        size;
	QString               type;
	QString               data;
	QList<edb::address_t> points_to;
};

class ResultViewModel : public QAbstractItemModel {
	Q_OBJECT
public:
	ResultViewModel(QObject *parent = 0);

public:
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &index) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual void sort (int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	void addResult(const Result &r);
	void clearResults();
	void update();
	void setUpdatesEnabled(bool value);
	bool updatesEnabled() const;

public:
	QVector<Result> &results() { return results_; }

private:
	QVector<Result> results_;
	bool            updates_enabled_;
};

#endif

