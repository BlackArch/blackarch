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

#include "Configuration.h"
#include "DebuggerCoreInterface.h"
#include "DebuggerPluginInterface.h"
#include "DebuggerMain.h"
#include "Debugger.h"
#include "DebuggerInternal.h"
#include "symbols.h"
#include "version.h"

#include <QApplication>
#include <QDir>
#include <QLibrary>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QPluginLoader>
#include <QTranslator>
#include <QtDebug>

#include <ctime>
#include <iostream>

namespace {
	//--------------------------------------------------------------------------
	// Name: load_plugins(const QString &directory)
	// Desc: attempts to load all plugins in a given directory
	//--------------------------------------------------------------------------
	void load_plugins(const QString &directory) {

		QDir plugins_dir(qApp->applicationDirPath());

		// TODO: attempt to detect the same plugin being loaded twice

		plugins_dir.cd(directory);

		Q_FOREACH(const QString &file_name, plugins_dir.entryList(QDir::Files)) {
			if(QLibrary::isLibrary(file_name)) {
				const QString full_path = plugins_dir.absoluteFilePath(file_name);
				QPluginLoader loader(full_path);
				loader.setLoadHints(QLibrary::ExportExternalSymbolsHint);

				if(QObject *const plugin = loader.instance()) {

					// TODO: handle the case where we find more than one core plugin...
					if(DebuggerCoreInterface *const core_plugin = qobject_cast<DebuggerCoreInterface *>(plugin)) {
						if(edb::v1::debugger_core == 0) {
							qDebug("[load_plugins] Loading Core Plugin: %-25s : %p", qPrintable(file_name), static_cast<void *>(plugin));
							edb::v1::debugger_core = core_plugin;
						}
					}

					if(DebuggerPluginInterface *const generic_plugin = qobject_cast<DebuggerPluginInterface *>(plugin)) {
						if(edb::internal::register_plugin(full_path, plugin)) {
							// we have a general purpose plugin
							qDebug("[load_plugins] Loading Plugin: %-30s : %p", qPrintable(file_name), static_cast<void *>(plugin));
						}
					}
				} else {
					qDebug() << "[load_plugins]" << qPrintable(loader.errorString());
				}
			}
		}
	}

	//--------------------------------------------------------------------------
	// Name: start_debugger(edb::pid_t attach_pid)
	// Desc: starts the main debugger code
	//--------------------------------------------------------------------------
	int start_debugger(edb::pid_t attach_pid, const QString &program, const QStringList &programArgs) {

		qDebug() << "Starting edb version:" << edb::version;
		qDebug("Please Report Bugs & Requests At: http://bugs.codef00.com/");

		// look for some plugins..
		load_plugins(QDir().absolutePath());

		const QString plugin_path = edb::v1::config().plugin_path;
		if(QDir().absolutePath() != plugin_path) {
			load_plugins(plugin_path);
		}
		
		edb::internal::load_function_db();

		// create the main window object
		DebuggerMain debugger;

		// let the plugins setup their menus
		debugger.finish_plugin_setup(edb::v1::plugin_list());

		// ok things are initialized to a reasonable degree, let's show the main window
		debugger.show();

		// have we been asked to attach to a given program?
		if(attach_pid != 0) {
			debugger.attach(attach_pid);
		} else if(!program.isEmpty()) {
			debugger.execute(program, programArgs);
		}

		if(edb::v1::debugger_core == 0) {
			QMessageBox::warning(
				0,
				QT_TRANSLATE_NOOP("edb", "edb Failed To Load A Necessary Plugin"),
				QT_TRANSLATE_NOOP("edb",
					"Failed to successfully load the debugger core plugin. Please make sure it exists and that the plugin path is correctly configured.\n"
					"This is normal if edb has not been previously run or the configuration file has been removed."));

			edb::v1::dialog_options()->exec();

			QMessageBox::warning(
				0,
				QT_TRANSLATE_NOOP("edb", "edb"),
				QT_TRANSLATE_NOOP("edb", "edb will now close. If you were successful in specifying the location of the debugger core plugin, please run edb again.")
				);

			// TODO: detect if they corrected the issue and try again
			return -1;
		} else {
	    	return qApp->exec();
		}
	}

	void load_translations() {
		// load some translations
		QTranslator qtTranslator;
		qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
		qApp->installTranslator(&qtTranslator);

		QTranslator myappTranslator;
		myappTranslator.load("edb_" + QLocale::system().name());
		qApp->installTranslator(&myappTranslator);
	}
}


//------------------------------------------------------------------------------
// Name: main(int argc, char *argv[])
// Desc: entry point
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

	QT_REQUIRE_VERSION(argc, argv, "4.5.0");

	QApplication app(argc, argv);
	QApplication::setWindowIcon(QIcon(":/debugger/images/edb48-logo.png"));

	qsrand(std::time(0));

	// setup organization info so settings go in right place
	QApplication::setOrganizationName("codef00.com");
	QApplication::setOrganizationDomain("codef00.com");
	QApplication::setApplicationName("edb");

	load_translations();

	const QStringList args = app.arguments();
	edb::pid_t  attach_pid = 0;
	QStringList run_args;
	QString     run_app;

	if(args.size() > 1) {
		if(args.size() == 3 && args[1] == "--attach") {
			attach_pid = args[2].toUInt();
		} else if(args.size() >= 3 && args[1] == "--run") {
			run_app = args[2];

			for(int i = 3; i < args.size(); ++i) {
				run_args.push_back(args[i]);
			}
		} else if(args.size() == 3 && args[1] == "--symbols") {
			symbols::generate_symbols(args[2]);
			return 0;
		} else if(args.size() == 2 && args[1] == "--version") {
			std::cout << "edb version: " << edb::version << std::endl;
			return 0;
		} else {
			std::cerr << "usage: " << qPrintable(args[0]) << " [--symbols <filename>] [ --attach <pid> ] [ --run <program> (args...) ] [ --version ]" << std::endl;
			return -1;
		}
	}

	return start_debugger(attach_pid, run_app, run_args);
}
