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

#include "DebuggerUI.h"
#include "ArchProcessorInterface.h"
#include "Configuration.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "DebuggerPluginInterface.h"
#include "DialogOptions.h"
#include "Expression.h"
#include "QHexView"

#include <QDir>
#include <QInputDialog>
#include <QToolButton>
#include <QtDebug>

#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#if defined(Q_OS_LINUX) || defined(Q_OS_OPENBSD)
#include <unistd.h>
#endif

#include "ui_debuggerui.h"

//------------------------------------------------------------------------------
// Name: DebuggerUI(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DebuggerUI::DebuggerUI(QWidget *parent) : QMainWindow(parent),
		ui(new Ui::DebuggerUI),
		add_tab_(0),
		del_tab_(0),
		tty_proc_(new QProcess(this)),
		gui_state_(TERMINATED) {
}

//------------------------------------------------------------------------------
// Name: ~DebuggerUI()
// Desc:
//------------------------------------------------------------------------------
DebuggerUI::~DebuggerUI() {
	// kill our xterm and wait for it to die
	tty_proc_->kill();
	tty_proc_->waitForFinished(3000);
	delete ui;
}

//------------------------------------------------------------------------------
// Name: update_menu_state(GUI_STATE state)
// Desc:
//------------------------------------------------------------------------------
void DebuggerUI::update_menu_state(GUI_STATE state) {

	switch(state) {
	case PAUSED:
		ui->actionRun_Until_Return->setEnabled(true);
		ui->action_Restart->setEnabled(true);
		ui->action_Run->setEnabled(true);
		ui->action_Pause->setEnabled(false);
		ui->action_Step_Into->setEnabled(true);
		ui->action_Step_Over->setEnabled(true);
		ui->action_Step_Into_Pass_Signal_To_Application->setEnabled(true);
		ui->action_Step_Over_Pass_Signal_To_Application->setEnabled(true);
		ui->action_Run_Pass_Signal_To_Application->setEnabled(true);
		ui->action_Detach->setEnabled(true);
		ui->action_Kill->setEnabled(true);
		add_tab_->setEnabled(true);
		edb::v1::set_status(tr("paused"));
		break;
	case RUNNING:
		ui->actionRun_Until_Return->setEnabled(false);
		ui->action_Restart->setEnabled(false);
		ui->action_Run->setEnabled(false);
		ui->action_Pause->setEnabled(true);
		ui->action_Step_Into->setEnabled(false);
		ui->action_Step_Over->setEnabled(false);
		ui->action_Step_Into_Pass_Signal_To_Application->setEnabled(false);
		ui->action_Step_Over_Pass_Signal_To_Application->setEnabled(false);
		ui->action_Run_Pass_Signal_To_Application->setEnabled(false);
		ui->action_Detach->setEnabled(true);
		ui->action_Kill->setEnabled(true);
		add_tab_->setEnabled(true);
		edb::v1::set_status(tr("running"));
		break;
	case TERMINATED:
		ui->actionRun_Until_Return->setEnabled(false);
		ui->action_Restart->setEnabled(false);
		ui->action_Run->setEnabled(false);
		ui->action_Pause->setEnabled(false);
		ui->action_Step_Into->setEnabled(false);
		ui->action_Step_Over->setEnabled(false);
		ui->action_Step_Into_Pass_Signal_To_Application->setEnabled(false);
		ui->action_Step_Over_Pass_Signal_To_Application->setEnabled(false);
		ui->action_Run_Pass_Signal_To_Application->setEnabled(false);
		ui->action_Detach->setEnabled(false);
		ui->action_Kill->setEnabled(false);
		add_tab_->setEnabled(false);
		edb::v1::set_status(tr("terminated"));
		break;
	}

	gui_state_ = state;
}

//------------------------------------------------------------------------------
// Name: create_tty()
// Desc: creates a TTY object for our command line I/O
//------------------------------------------------------------------------------
QString DebuggerUI::create_tty() {
#if defined(Q_OS_LINUX) || defined(Q_OS_OPENBSD) || defined(Q_OS_FREEBSD)
	// we attempt to reuse an open output window
	if(edb::v1::config().tty_enabled && tty_proc_->state() != QProcess::Running) {
		const QString command = edb::v1::config().tty_command;

		if(!command.isEmpty()) {

			// ok, creating a new one...
			// first try to get a 'unique' filename, i would love to use a system
			// temp file API... but there doesn't seem to be one which will create
			// a pipe...only ordinary files!
			const QString temp_pipe = QString("%1/edb_temp_file_%2_%3").arg(QDir::tempPath()).arg(rand()).arg(getpid());

			// make sure it isn't already there, and then make the pipe
			::unlink(qPrintable(temp_pipe));
			::mkfifo(qPrintable(temp_pipe), S_IRUSR | S_IWUSR);

			// this is a basic shell script which will output the tty to a file (the pipe),
			// ignore kill sigs, close all standard IO, and then just hang
			const QString shell_script = QString(
		    	"tty > %1;"
				"trap \"\" INT QUIT TSTP;"
				"exec<&-; exec>&-;"
				"while :; do sleep 3600; done"
				).arg(temp_pipe);

			// parse up the command from the options, white space delimited
			QStringList proc_args = edb::v1::parse_command_line(command);
			const QString tty_command = proc_args.takeFirst().trimmed();

			// start constructing the arguments for the term

			if(tty_command.endsWith("/gnome-terminal")) {
				proc_args << "--hide-menubar" << "--title" << tr("edb output") << "-e" << QString("sh -c '%1'").arg(shell_script);
			} else if(tty_command.endsWith("/konsole")) {
				proc_args << "--nofork" << "--title" << tr("edb output") << "-e" << QString("sh -c '%1'").arg(shell_script);
			} else {
				proc_args << "-title" << tr("edb output") << "-e" << QString("sh -c '%1'").arg(shell_script);
			}

			// make the tty process object and connect it's death signal to our cleanup
			connect(tty_proc_, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(tty_proc_finished(int, QProcess::ExitStatus)));

			tty_proc_->start(tty_command, proc_args);

			if(tty_proc_->waitForStarted(3000)) {

				const Q_PID tty_pid = tty_proc_->pid();
				Q_UNUSED(tty_pid);

				// get the output, this should block until the xterm actually gets a chance to write it
				QFile file(temp_pipe);
				if(file.open(QIODevice::ReadOnly)) {
					tty_file_ = file.readLine().trimmed();
				}
			} else {
				qDebug().nospace() << "Could not launch the desired terminal [" << tty_command << "], please check that it exists and you have proper permissions.";
				tty_file_.clear();
			}

			// cleanup, god i wish there was an easier way than this!
			::unlink(qPrintable(temp_pipe));
		}
	}
#endif
	return tty_file_;
}

//------------------------------------------------------------------------------
// Name: tty_proc_finished(int exit_code, QProcess::ExitStatus exit_status)
// Desc: cleans up the data associated with a TTY when the terminal dies
//------------------------------------------------------------------------------
void DebuggerUI::tty_proc_finished(int exit_code, QProcess::ExitStatus exit_status) {
	Q_UNUSED(exit_code);
	Q_UNUSED(exit_status);

	tty_file_.clear();
}

//------------------------------------------------------------------------------
// Name: current_tab() const
// Desc:
//------------------------------------------------------------------------------
int DebuggerUI::current_tab() const {
	return ui->tabWidget->currentIndex();
}

//------------------------------------------------------------------------------
// Name: current_data_view_info() const
// Desc:
//------------------------------------------------------------------------------
DataViewInfo::pointer DebuggerUI::current_data_view_info() const {
	return data_regions_[current_tab()];
}

//------------------------------------------------------------------------------
// Name: set_debugger_caption(const QString &appname)
// Desc: sets the caption part to also show the application name and pid
//------------------------------------------------------------------------------
void DebuggerUI::set_debugger_caption(const QString &appname) {
	setWindowTitle(tr("edb - %1 [%2]").arg(appname).arg(edb::v1::debugger_core->pid()));
}

//------------------------------------------------------------------------------
// Name: delete_data_tab()
// Desc:
//------------------------------------------------------------------------------
void DebuggerUI::delete_data_tab() {
	const int current = current_tab();

	// get a pointer to the info we need (before removing it from the list!)
	// this seems redundant to current_data_view_info(), but we need the
	// index too, so may as well waste one line to avoid duplicate work
	DataViewInfo::pointer info = data_regions_[current];

	// remove it from the list
	data_regions_.remove(current);

	// remove the tab associated with it
	ui->tabWidget->removeTab(current);
}

//------------------------------------------------------------------------------
// Name: create_data_tab()
// Desc:
//------------------------------------------------------------------------------
void DebuggerUI::create_data_tab() {
	const int current = current_tab();

	// duplicate the current region
	DataViewInfo *const new_data_view = new DataViewInfo((current != -1) ? data_regions_[current]->region : MemRegion());

	QHexView *const hexview = new QHexView;

	new_data_view->view = QSharedPointer<QHexView>(hexview);

	// setup the context menu
	hexview->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(hexview, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(mnuDumpContextMenu(const QPoint &)));

	// show the initial data for this new view
	hexview->setAddressOffset(new_data_view->region.start);
	hexview->setData(new_data_view->stream);

	const Configuration &config = edb::v1::config();

	// set the default view options
	hexview->setShowAddress(config.data_show_address);
	hexview->setShowHexDump(config.data_show_hex);
	hexview->setShowAsciiDump(config.data_show_ascii);
	hexview->setShowComments(config.data_show_comments);
	hexview->setRowWidth(config.data_row_width);
	hexview->setWordWidth(config.data_word_width);

	// set the default font
	QFont dump_font;
	dump_font.fromString(config.data_font);
	hexview->setFont(dump_font);

	data_regions_.push_back(DataViewInfo::pointer(new_data_view));

	// create the tab!
	ui->tabWidget->addTab(hexview, tr("%1-%2").arg(
		edb::v1::format_pointer(new_data_view->region.start),
		edb::v1::format_pointer(new_data_view->region.end)));


	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

//------------------------------------------------------------------------------
// Name: finish_plugin_setup(const QHash<QString, QObject *> &plugins)
// Desc: finalizes plugin setup by adding each to the menu, we can do this now
//       that we have a GUI widget to attach it to
//------------------------------------------------------------------------------
void DebuggerUI::finish_plugin_setup(const QHash<QString, QObject *> &plugins) {

	// call the init function for each plugin
	Q_FOREACH(QObject *plugin, plugins) {
		if(DebuggerPluginInterface *const p = qobject_cast<DebuggerPluginInterface *>(plugin)) {
			p->init();
		}
	}

	// setup the menu for all plugins that which to do so
	DialogOptions *const options = qobject_cast<DialogOptions *>(edb::v1::dialog_options());
	Q_FOREACH(QObject *plugin, plugins) {
		if(DebuggerPluginInterface *const p = qobject_cast<DebuggerPluginInterface *>(plugin)) {
			if(QMenu *const menu = p->menu(this)) {
				ui->menu_Plugins->addMenu(menu);
			}

			if(QWidget *const options_page = p->options_page()) {
				options->addOptionsPage(options_page);
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: get_goto_expression(bool &ok)
// Desc:
//------------------------------------------------------------------------------
edb::address_t DebuggerUI::get_goto_expression(bool &ok) {

	edb::address_t address;
	ok = edb::v1::get_expression_from_user(tr("Goto Address"), tr("Address:"), address);
	return ok ? address : 0;
}

//------------------------------------------------------------------------------
// Name: get_follow_register(bool &ok) const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t DebuggerUI::get_follow_register(bool &ok) const {
	ok = false;
	if(const QTreeWidgetItem *const i = ui->registerList->currentItem()) {
		if(const Register reg = edb::v1::arch_processor().value_from_item(*i)) {
			if(reg.type() & (Register::TYPE_GPR | Register::TYPE_IP)) {
				ok = true;
				return *reg;
			}
		}
	}

	return 0;
}
