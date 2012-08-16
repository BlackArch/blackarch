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

#include "SessionManager.h"
#include "Debugger.h"
#include <QtDebug>
#include <QMetaProperty>

//------------------------------------------------------------------------------
// Name: private_init()
// Desc:
//------------------------------------------------------------------------------
void SessionManager::private_init() {
	edb::v1::set_session_file_handler(this);
}

//------------------------------------------------------------------------------
// Name: save_session(const QString &filename, const QString &executable)
// Desc:
//------------------------------------------------------------------------------
void SessionManager::save_session(const QString &filename, const QString &executable) {
	qDebug() << "[SessionManager] saving session file:" << filename <<  "for:" << executable;
	
	QHash<QString, QObject *> plugins = edb::v1::plugin_list();
	for(QHash<QString, QObject *>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
		QObject *const o = it.value();
		if(DebuggerPluginInterface *const p = qobject_cast<DebuggerPluginInterface *>(o)) {
			//qDebug() << edb::v1::serialize_object(o);
		}
	}
}

//------------------------------------------------------------------------------
// Name: load_session(const QString &filename)
// Desc:
//------------------------------------------------------------------------------
void SessionManager::load_session(const QString &filename, const QString &executable) {
	qDebug() << "[SessionManager] loading session file:" << filename <<  "for:" << executable;
	
	QHash<QString, QObject *> plugins = edb::v1::plugin_list();
	for(QHash<QString, QObject *>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
		if(DebuggerPluginInterface *const p = qobject_cast<DebuggerPluginInterface *>(it.value())) {

		}
	}
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *SessionManager::menu(QWidget *parent) {
	Q_UNUSED(parent);
	return 0;
}

Q_EXPORT_PLUGIN2(SessionManager, SessionManager)
