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

#include "QCategoryList.h"
#include "RegisterViewDelegate.h"
#include <QHeaderView>
#include <QTreeView>
#include <QMouseEvent>

//------------------------------------------------------------------------------
// Name: QCategoryList(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QCategoryList::QCategoryList(QWidget *parent) : QTreeWidget(parent) {

	// set the delegate
	setItemDelegate(new RegisterViewDelegate(this, this));

	// hide the exapander, since we provide our own
	setRootIsDecorated(false);

	setColumnCount(1);
	header()->hide();
	header()->setResizeMode(QHeaderView::Stretch);

	connect(this, SIGNAL(itemPressed(QTreeWidgetItem *, int)), SLOT(handleMousePress(QTreeWidgetItem*)));
}

//------------------------------------------------------------------------------
// Name: ~QCategoryList()
// Desc:
//------------------------------------------------------------------------------
QCategoryList::~QCategoryList() {

}

//------------------------------------------------------------------------------
// Name: handleMousePress(QTreeWidgetItem *item)
// Desc:
//------------------------------------------------------------------------------
void QCategoryList::handleMousePress(QTreeWidgetItem *item) {
	if(isCategory(item)) {
	    setItemExpanded(item, !isItemExpanded(item));
	}
}

//------------------------------------------------------------------------------
// Name: mouseDoubleClickEvent(QMouseEvent * event)
// Desc:
//------------------------------------------------------------------------------
void QCategoryList::mouseDoubleClickEvent(QMouseEvent * event) {
	if(QTreeWidgetItem *const p = itemAt(event->pos())) {
		emit itemDoubleClicked(p, 0);
	}
}

//------------------------------------------------------------------------------
// Name: addCategory(const QString &name)
// Desc:
//------------------------------------------------------------------------------
QTreeWidgetItem *QCategoryList::addCategory(const QString &name) {
	QTreeWidgetItem *const cat = new QTreeWidgetItem(this);
	cat->setText(0, name);
	setItemExpanded(cat, true);
	return cat;
}

//------------------------------------------------------------------------------
// Name: addItem(QTreeWidgetItem *category, const QString &value)
// Desc:
//------------------------------------------------------------------------------
QTreeWidgetItem *QCategoryList::addItem(QTreeWidgetItem *category, const QString &value) {
	QTreeWidgetItem *const item = new QTreeWidgetItem(category);
	item->setText(0, value);
	return item;
}

//------------------------------------------------------------------------------
// Name: isCategory(QTreeWidgetItem *item) const
// Desc:
//------------------------------------------------------------------------------
bool QCategoryList::isCategory(QTreeWidgetItem *item) const {
	return item != 0 && item->parent() == 0;
}
