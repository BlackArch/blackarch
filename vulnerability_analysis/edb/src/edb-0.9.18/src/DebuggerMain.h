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

#ifndef DEBUGGERMAIN_20060508_H_
#define DEBUGGERMAIN_20060508_H_

#include "DebugEventHandlerInterface.h"
#include "DebuggerUI.h"
#include "DebugEvent.h"
#include "ScopedPointer.h"
#include "QHexView"
#include <QDragEnterEvent>
#include <QDropEvent>

class BinaryInfo;
class Breakpoint;
class DebuggerPluginInterface;
class DialogArguments;
class RecentFileManager;

class QStringListModel;
class QTimer;
class QToolButton;
class QTreeWidgetItem;

class DebuggerMain : public DebuggerUI, public DebugEventHandlerInterface {
	Q_OBJECT

private:
	friend class RunUntilRet;

public:
	DebuggerMain(QWidget *parent = 0);
	virtual ~DebuggerMain();

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void showEvent(QShowEvent *event);

	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);

public:
	bool dump_data(edb::address_t address, bool new_tab);
	bool dump_data_range(edb::address_t address, edb::address_t end_address, bool new_tab);
	bool dump_stack(edb::address_t address, bool scroll_to);
	bool jump_to_address(edb::address_t address);
	void attach(edb::pid_t pid);
	void execute(const QString &s, const QStringList &args);
	void refresh_gui();
	void update_gui();

public Q_SLOTS:
	// the autoconnected slots
	void on_actionAbout_QT_triggered();
	void on_actionApplication_Arguments_triggered();
	void on_actionApplication_Working_Directory_triggered();
	void on_actionRun_Until_Return_triggered();
	void on_action_About_triggered();
	void on_action_Attach_triggered();
	void on_action_Configure_Debugger_triggered();
	void on_action_Detach_triggered();
	void on_action_Kill_triggered();
	void on_action_Memory_Regions_triggered();
	void on_action_Open_triggered();
	void on_action_Pause_triggered();
	void on_action_Plugins_triggered();
	void on_action_Restart_triggered();
	void on_action_Run_Pass_Signal_To_Application_triggered();
	void on_action_Run_triggered();
	void on_action_Step_Into_Pass_Signal_To_Application_triggered();
	void on_action_Step_Into_triggered();
	void on_action_Step_Over_Pass_Signal_To_Application_triggered();
	void on_action_Step_Over_triggered();
	void on_action_Threads_triggered();
	void on_cpuView_breakPointToggled(edb::address_t);
	void on_cpuView_customContextMenuRequested(const QPoint &);
	void on_registerList_customContextMenuRequested(const QPoint &);
	void on_registerList_itemDoubleClicked(QTreeWidgetItem *);

private Q_SLOTS:
	// the manually connected CPU slots
	void mnuCPUAddBreakpoint();
	void mnuCPUAddConditionalBreakpoint();
	void mnuCPUFillNop();
	void mnuCPUFillZero();
	void mnuCPUFollow();
	void mnuCPUFollowInDump();
	void mnuCPUFollowInStack();
	void mnuCPUJumpToAddress();
	void mnuCPUJumpToEIP();
	void mnuCPUModify();
	void mnuCPURemoveBreakpoint();
	void mnuCPUSetEIP();

private Q_SLOTS:
	// the manually connected Register slots
	void mnuRegisterFollowInDump()       { follow_register_in_dump(false); }
	void mnuRegisterFollowInDumpNewTab() { follow_register_in_dump(true); }
	void mnuRegisterFollowInStack();

private Q_SLOTS:
	// the manually connected Dump slots
	void mnuDumpContextMenu(const QPoint &pos);
	void mnuDumpCreateTab();
	void mnuDumpDeleteTab();
	void mnuDumpFollowInCPU();
	void mnuDumpFollowInDump();
	void mnuDumpFollowInStack();
	void mnuDumpGotoAddress();
	void mnuDumpModify();
	void mnuDumpSaveToFile();

private Q_SLOTS:
	// the manually connected Stack slots
	void mnuStackContextMenu(const QPoint &);
	void mnuStackFollowInCPU();
	void mnuStackFollowInDump();
	void mnuStackFollowInStack();
	void mnuStackGotoAddress();
	void mnuStackGotoEBP();
	void mnuStackGotoESP();
	void mnuStackModify();
	void mnuStackPop();
	void mnuStackPush();
	void mnuStackToggleLock(bool locked);

private Q_SLOTS:
	void goto_triggered();
	void next_debug_event();
	void open_file(const QString &s);
	void tab_context_menu(int index, const QPoint &pos);

public:
	virtual edb::EVENT_STATUS handle_event(const DebugEvent &event);

private:
	enum DEBUG_MODE {
		MODE_STEP,
		MODE_TRACE,
		MODE_RUN
	};

	enum EXCEPTION_RESUME {
		IGNORE_EXCEPTION,
		PASS_EXCEPTION
	};

	enum DETACH_ACTION {
		NO_KILL_ON_DETACH,
		KILL_ON_DETACH
	};

public:
	void update_data(const DataViewInfo::pointer &v);
	void clear_data(const DataViewInfo::pointer &v);

private:
	QString session_filename() const;
	bool breakpoint_condition_true(const QString &condition);
	bool common_open(const QString &s, const QStringList &args);
	bool current_instruction_is_return() const;
	edb::EVENT_STATUS debug_event_handler(const DebugEvent &event);
	edb::EVENT_STATUS handle_event_exited(const DebugEvent &event);
	edb::EVENT_STATUS handle_event_signaled(const DebugEvent &event);
	edb::EVENT_STATUS handle_event_stopped(const DebugEvent &event);
	edb::EVENT_STATUS handle_trap();
	edb::EVENT_STATUS resume_status(bool pass_exception);
	void apply_default_fonts();
	void apply_default_show_separator();
	void cleanup_debugger();
	void cpu_fill(quint8 byte);
	void detach_from_process(DETACH_ACTION kill);
	void do_jump_to_address(edb::address_t address, const MemRegion &r, bool scroll_to);
	void follow_register_in_dump(bool tabbed);
	void resume_execution(EXCEPTION_RESUME pass_exception, DEBUG_MODE mode);
	void set_initial_breakpoint(const QString &s);
	void set_initial_debugger_state();
	void setup_stack_view();
	void setup_tab_buttons();
	void setup_ui();
	void test_native_binary();
	void update_cpu_view(const State &state, MemRegion &region);
	void update_data_views();
	void update_disassembly(edb::address_t address, const MemRegion &r);
	void update_stack_view(const State &state);
	void update_tab_caption(const QSharedPointer<QHexView> &view, edb::address_t start, edb::address_t end);

private:

	template <class F>
	void follow_memory(edb::address_t address, F follow_func);

	template <class T>
	edb::address_t get_follow_address(const T &hv, bool &ok);
	
	template <class F, class T>
	void add_plugin_context_menu(const T &menu, const F &f);

	template <class T>
	void follow_in_cpu(const T &hv);

	template <class T>
	void follow_in_dump(const T &hv);

	template <class T>
	void follow_in_stack(const T &hv);

	template <class T>
	void modify_bytes(const T &hv);

private:
	QSharedPointer<QHexView>                         stack_view_;
	QStringListModel *                               list_model_;
	DialogArguments *                                arguments_dialog_;
	QTimer *                                         timer_;
	RecentFileManager *                              recent_file_manager_;

	QSharedPointer<QHexView::CommentServerInterface> stack_comment_server_;
	Breakpoint::pointer                              reenable_breakpoint_;
	SCOPED_POINTER<BinaryInfo>                       binary_info_;

	QString                                          last_open_directory_;
	QString                                          working_directory_;
	QString                                          program_executable_;
	bool                                             stack_view_locked_;
	bool                                             step_run_;
	DebugEvent                                       last_event_;
#ifdef Q_OS_UNIX
	edb::address_t                                   debug_pointer_;
#endif
};

#endif
