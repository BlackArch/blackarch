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

#include "RegisterViewDelegate.h"
#include <QHeaderView>
#include <QTreeView>

//------------------------------------------------------------------------------
// Name: RegisterViewDelegate(QTreeView *view, QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
RegisterViewDelegate::RegisterViewDelegate(QTreeView *view, QWidget *parent) : QItemDelegate(parent), view_(view) {
}

//------------------------------------------------------------------------------
// Name: ~RegisterViewDelegate()
// Desc:
//------------------------------------------------------------------------------
RegisterViewDelegate::~RegisterViewDelegate() {
}

//------------------------------------------------------------------------------
// Name: paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
// Desc:
//------------------------------------------------------------------------------
void RegisterViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

	const QAbstractItemModel *const model = index.model();
	Q_ASSERT(model);

	if(!model->parent(index).isValid()) {
		// this is a top-level item.
		QStyleOptionButton buttonOption;

		buttonOption.state = option.state;
	#ifdef Q_WS_MAC
		buttonOption.state |= QStyle::State_Raised;
	#endif
		buttonOption.state &= ~QStyle::State_HasFocus;

		buttonOption.rect     = option.rect;
		buttonOption.palette  = option.palette;
		buttonOption.features = QStyleOptionButton::None;
		view_->style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, view_);

		static const int i = 9; // ### hardcoded in qcommonstyle.cpp
		const QRect &r = option.rect;

		QStyleOption branchOption;
		branchOption.rect    = QRect(r.left() + i / 2, r.top() + (r.height() - i) / 2, i, i);
		branchOption.palette = option.palette;
		branchOption.state   = QStyle::State_Children;

		if(view_->isExpanded(index)) {
			branchOption.state |= QStyle::State_Open;
		}

		view_->style()->drawPrimitive(QStyle::PE_IndicatorBranch, &branchOption, painter, view_);

		// draw text
		const QRect textrect = QRect(r.left() + i * 2, r.top(), r.width() - ((5 * i) / 2), r.height());
		const QString text = elidedText(option.fontMetrics, textrect.width(), Qt::ElideMiddle, model->data(index, Qt::DisplayRole).toString());
		view_->style()->drawItemText(painter, textrect, Qt::AlignCenter, option.palette, view_->isEnabled(), text);

	} else {
		QItemDelegate::paint(painter, option, index);
	}
}

//------------------------------------------------------------------------------
// Name: sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const
// Desc:
//------------------------------------------------------------------------------
QSize RegisterViewDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const {
    return QItemDelegate::sizeHint(opt, index) + QSize(2, 2);
}
