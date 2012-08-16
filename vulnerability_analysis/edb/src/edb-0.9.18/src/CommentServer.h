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

#ifndef COMMENTSERVER_20070427_H_
#define COMMENTSERVER_20070427_H_

#include "QHexView"
#include <QHash>
#include <QCoreApplication>

class CommentServer : public QHexView::CommentServerInterface {
	Q_DECLARE_TR_FUNCTIONS(CommentServer)

public:
	virtual ~CommentServer();

public:
	virtual void set_comment(QHexView::address_t address, const QString &comment);
	virtual QString comment(QHexView::address_t address, int size) const;
	virtual void clear();

private:
	QString resolve_function_call(QHexView::address_t address, bool &ok) const;
	QString resolve_string(QHexView::address_t address, bool &ok) const;

private:
        QHash<quint64, QString> custom_comments_;
};

#endif

