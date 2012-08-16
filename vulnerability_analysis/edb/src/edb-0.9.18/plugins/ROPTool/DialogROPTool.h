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

#ifndef DIALOG_ROPTOOL_20100817_H_
#define DIALOG_ROPTOOL_20100817_H_

#include "Types.h"
#include "Instruction.h"

#include <QDialog>
#include <QSet>
#include <QList>
#include <QSortFilterProxyModel>

class QListWidgetItem;
class QModelIndex;
class QSortFilterProxyModel;
class QStandardItem;
class QStandardItemModel;

class ResultFilterProxy;

namespace Ui { class DialogROPTool; }

class ResultFilterProxy : public QSortFilterProxyModel {
public:
	ResultFilterProxy(QObject *parent = 0) : QSortFilterProxyModel(parent), mask_(0) {
	}

public:
	void set_mask_bit(quint32 mask, bool value) {
		if(value) {
			mask_ |= mask;
		} else {
			mask_ &= ~mask;
		}
		reset();
	}

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
		QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
		if(index.data(Qt::UserRole + 1).toUInt() & mask_) {
			return true;
		}
		return false;
	}

private:
	quint32 mask_;
};

class DialogROPTool : public QDialog {
	Q_OBJECT

public:
	DialogROPTool(QWidget *parent = 0);
	virtual ~DialogROPTool();

public Q_SLOTS:
	void on_btnFind_clicked();
	void on_listView_doubleClicked(const QModelIndex &index);
	void on_chkShowALU_stateChanged(int state);
	void on_chkShowStack_stateChanged(int state);
	void on_chkShowLogic_stateChanged(int state);
	void on_chkShowData_stateChanged(int state);
	void on_chkShowOther_stateChanged(int state);

private:
	void do_find();
	void add_gadget(edb::address_t start_address, QList<edb::Instruction> instructions, size_t size);
	void set_gadget_role(QStandardItem *item, const edb::Instruction &insn1);

private:
	virtual void showEvent(QShowEvent *event);

private:
	Ui::DialogROPTool *const ui;
	QSortFilterProxyModel *  filter_model_;
	QStandardItemModel *     result_model_;
	ResultFilterProxy *      result_filter_;
	QSet<QString>            unique_results_;
};

#endif
