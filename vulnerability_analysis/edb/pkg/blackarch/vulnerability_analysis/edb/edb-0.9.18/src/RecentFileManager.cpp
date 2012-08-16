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

#include "RecentFileManager.h"

#include <QSettings>
#include <QAction>
#include <QMenu>

//------------------------------------------------------------------------------
// Name: RecentFileManager()
// Desc: constructor
//------------------------------------------------------------------------------
RecentFileManager::RecentFileManager(QWidget * parent, Qt::WindowFlags f) : QWidget(parent, f), menu_(0) {
	load_file_list();
}

//------------------------------------------------------------------------------
// Name: ~RecentFileManager()
// Desc: destructor
//------------------------------------------------------------------------------
RecentFileManager::~RecentFileManager() {
	save_file_list();
}

//------------------------------------------------------------------------------
// Name: save_file_list()
// Desc:
//------------------------------------------------------------------------------
void RecentFileManager::save_file_list() {
	QSettings settings;
	settings.beginGroup("Recent Files");

	for(int i = 0; i < max_recent_files; ++i) {
		QString s;
		if(i < file_list_.size()) {
			s = file_list_[i];
		}
		settings.setValue(QString("File%1").arg(i), s);
	}
	settings.endGroup();
}

//------------------------------------------------------------------------------
// Name: load_file_list()
// Desc:
//------------------------------------------------------------------------------
void RecentFileManager::load_file_list() {
	QSettings settings;
	settings.beginGroup("Recent Files");

	file_list_.clear();
	for(int i = 0; i < max_recent_files; ++i) {
		const QString s = settings.value(QString("File%1").arg(i)).value<QString>();
		if(!s.isEmpty()) {
			file_list_.push_back(s);
		}
	}
	settings.endGroup();
}

//------------------------------------------------------------------------------
// Name: clear_file_list()
// Desc:
//------------------------------------------------------------------------------
void RecentFileManager::clear_file_list() {
	file_list_.clear();
	if(menu_ != 0) {
		menu_->clear();
		menu_->addSeparator();
		menu_->addAction(tr("Clear &Menu"), this, SLOT(clear_file_list()));
	}
}

//------------------------------------------------------------------------------
// Name: create_menu()
// Desc:
//------------------------------------------------------------------------------
QMenu *RecentFileManager::create_menu() {

	if(menu_ == 0) {
		menu_ = new QMenu(this);
		update();
	}

	return menu_;
}

//------------------------------------------------------------------------------
// Name: create_menu()
// Desc:
//------------------------------------------------------------------------------
void RecentFileManager::update() {
	if(menu_ != 0) {
		menu_->clear();

		Q_FOREACH(const QString &s, file_list_) {
			if(QAction *const action = menu_->addAction(s, this, SLOT(item_selected()))) {
				action->setData(s);
			}
		}

		menu_->addSeparator();
		menu_->addAction(tr("Clear &Menu"), this, SLOT(clear_file_list()));
	}
}

//------------------------------------------------------------------------------
// Name: item_selected()
// Desc:
//------------------------------------------------------------------------------
void RecentFileManager::item_selected() {
	if(QAction *const action = qobject_cast<QAction *>(sender())) {
		const QString s = action->data().toString();
		emit file_selected(s);
	}
}

//------------------------------------------------------------------------------
// Name: add_file(const QString &file)
// Desc:
//------------------------------------------------------------------------------
void RecentFileManager::add_file(const QString &file) {
	// update recent file list
	if(!file_list_.contains(file)) {
		file_list_.push_front(file);

		// make sure we don't add more than the max
		while(file_list_.size() > max_recent_files) {
			file_list_.pop_back();
		}
		update();
	}
}
