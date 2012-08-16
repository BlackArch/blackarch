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

#include "DebuggerCore.h"
#include "State.h"
#include "DebugEvent.h"
#include "PlatformState.h"

#include <QDebug>
#include <QDir>

#include <cerrno>
#include <cstring>
#include <csignal>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE        /* or _BSD_SOURCE or _SVID_SOURCE */
#endif

#include <asm/ldt.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#define tgkill(tgid, tid, sig) syscall(SYS_tgkill, (tgid), (tid), (sig));


// doesn't always seem to be defined in the headers
#ifndef PTRACE_GET_THREAD_AREA
#define PTRACE_GET_THREAD_AREA static_cast<__ptrace_request>(25)
#endif

#ifndef PTRACE_SET_THREAD_AREA
#define PTRACE_SET_THREAD_AREA static_cast<__ptrace_request>(26)
#endif

#define DEBUG_THREADS

namespace {

//------------------------------------------------------------------------------
// Name: resume_code(int status)
// Desc:
//------------------------------------------------------------------------------
int resume_code(int status) {

	if(WIFSTOPPED(status) && WSTOPSIG(status) == SIGSTOP) {
		return 0;
	}

	if(WIFSIGNALED(status)) {
		return WTERMSIG(status);
	}

	if(WIFSTOPPED(status)) {
		return WSTOPSIG(status);
	}

	return 0;
}

//------------------------------------------------------------------------------
// Name: is_clone_event(int status)
// Desc:
//------------------------------------------------------------------------------
bool is_clone_event(int status) {
	if(WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
		return (((status >> 16) & 0xffff) == PTRACE_EVENT_CLONE);
	}

	return false;
}
}

//------------------------------------------------------------------------------
// Name: DebuggerCore()
// Desc: constructor
//------------------------------------------------------------------------------
DebuggerCore::DebuggerCore() {
#if defined(_SC_PAGESIZE)
	page_size_ = sysconf(_SC_PAGESIZE);
#elif defined(_SC_PAGE_SIZE)
	page_size_ = sysconf(_SC_PAGE_SIZE);
#else
	page_size_ = PAGE_SIZE;
#endif
}

//------------------------------------------------------------------------------
// Name: 
// Desc: 
//------------------------------------------------------------------------------
bool DebuggerCore::has_extension(quint64 ext) const {
	Q_UNUSED(ext);
	return false;
}

//------------------------------------------------------------------------------
// Name: page_size() const
// Desc: returns the size of a page on this system
//------------------------------------------------------------------------------
edb::address_t DebuggerCore::page_size() const {
	return page_size_;
}

//------------------------------------------------------------------------------
// Name: ~DebuggerCore()
// Desc: destructor
//------------------------------------------------------------------------------
DebuggerCore::~DebuggerCore() {
	detach();
}

//------------------------------------------------------------------------------
// Name: ptrace_traceme()
// Desc:
//------------------------------------------------------------------------------
long DebuggerCore::ptrace_traceme() {
	return ptrace(PTRACE_TRACEME, 0, 0, 0);
}

//------------------------------------------------------------------------------
// Name: ptrace_continue(edb::tid_t tid, long status)
// Desc:
//------------------------------------------------------------------------------
long DebuggerCore::ptrace_continue(edb::tid_t tid, long status) {
	Q_ASSERT(waited_threads_.contains(tid));
	Q_ASSERT(tid != 0);
	waited_threads_.remove(tid);
	return ptrace(PTRACE_CONT, tid, 0, status);
}

//------------------------------------------------------------------------------
// Name: ptrace_step(edb::tid_t tid, long status)
// Desc:
//------------------------------------------------------------------------------
long DebuggerCore::ptrace_step(edb::tid_t tid, long status) {
	Q_ASSERT(waited_threads_.contains(tid));
	Q_ASSERT(tid != 0);
	waited_threads_.remove(tid);
	return ptrace(PTRACE_SINGLESTEP, tid, 0, status);
}

//------------------------------------------------------------------------------
// Name: ptrace_set_options(edb::tid_t tid, long options)
// Desc:
//------------------------------------------------------------------------------
long DebuggerCore::ptrace_set_options(edb::tid_t tid, long options) {
	Q_ASSERT(waited_threads_.contains(tid));
	Q_ASSERT(tid != 0);
	return ptrace(PTRACE_SETOPTIONS, tid, 0, options);
}

//------------------------------------------------------------------------------
// Name: ptrace_get_event_message(edb::tid_t tid, unsigned long *message)
// Desc:
//------------------------------------------------------------------------------
long DebuggerCore::ptrace_get_event_message(edb::tid_t tid, unsigned long *message) {
	Q_ASSERT(waited_threads_.contains(tid));
	Q_ASSERT(tid != 0);
	return ptrace(PTRACE_GETEVENTMSG, tid, 0, message);
}

//------------------------------------------------------------------------------
// Name: handle_event(DebugEvent &event, edb::tid_t tid, int status)
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCore::handle_event(DebugEvent &event, edb::tid_t tid, int status) {

	// note that we have waited on this thread
	waited_threads_.insert(tid);

#ifdef DEBUG_THREADS
	// was it a thread exit event?
	if(WIFEXITED(status)) {
		threads_.remove(tid);
		waited_threads_.remove(tid);

		// if this was the last thread, return true
		// so we report it to the user.
		// if this wasn't, then we should silently
		// procceed.
		return threads_.empty();
	}

	// was it a thread create event?
	if(is_clone_event(status)) {

		unsigned long new_tid;
		if(ptrace_get_event_message(tid, &new_tid) != -1) {

			threads_.insert(new_tid, thread_info(0));

			int thread_status = 0;
			if(!waited_threads_.contains(new_tid)) {
				if(native::waitpid(new_tid, &thread_status, __WALL) > 0) {
					waited_threads_.insert(new_tid);
				}
			}

			if(!WIFSTOPPED(thread_status) || WSTOPSIG(thread_status) != SIGSTOP) {
				qDebug("[warning] new thread [%d] received an event besides SIGSTOP", static_cast<int>(new_tid));
			}


			// TODO: what the heck do we do if this isn't a SIGSTOP?
			ptrace_continue(new_tid, resume_code(thread_status));
		}

		ptrace_continue(tid, 0);
		return false;
	}
#endif

	// normal event
	event                = DebugEvent(status, pid(), tid);
	active_thread_       = tid;
	event_thread_        = tid;
	threads_[tid].status = status;

	stop_threads();
	return true;
}

//------------------------------------------------------------------------------
// Name: stop_threads()
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::stop_threads() {
#ifdef DEBUG_THREADS
	for(threadmap_t::iterator it = threads_.begin(); it != threads_.end(); ++it) {
		if(!waited_threads_.contains(it.key())) {
			const edb::tid_t tid = it.key();

			tgkill(pid(), tid, SIGSTOP);

			int thread_status;
			if(native::waitpid(tid, &thread_status, __WALL) > 0) {
				waited_threads_.insert(tid);
				it->status = thread_status;

				if(!WIFSTOPPED(thread_status) || WSTOPSIG(thread_status) != SIGSTOP) {
					qDebug("[warning] paused thread [%d] received an event besides SIGSTOP", tid);
				}
			}
		}
	}
#endif
}

//------------------------------------------------------------------------------
// Name: wait_debug_event(DebugEvent &event, int msecs)
// Desc: waits for a debug event, msecs is a timeout
//      it will return false if an error or timeout occurs
//------------------------------------------------------------------------------
bool DebuggerCore::wait_debug_event(DebugEvent &event, int msecs) {

	if(attached()) {
		if(!native::wait_for_sigchld(msecs)) {
#ifdef DEBUG_THREADS
			Q_FOREACH(edb::tid_t thread, thread_ids()) {
				int status;
				const edb::tid_t tid = native::waitpid(thread, &status, __WALL | WNOHANG);
				if(tid > 0 && handle_event(event, tid, status)) {
					return true;
				}
			}
#else
			int status;
			const edb::tid_t tid = native::waitpid(pid(), &status, __WALL | WNOHANG);
			if(tid > 0 && handle_event(event, tid, status)) {
				return true;
			}
#endif
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: read_data(edb::address_t address, bool &ok)
// Desc:
// Note: this will fail on newer versions of linux if called from a
//       different thread than the one which attached to process
//------------------------------------------------------------------------------
long DebuggerCore::read_data(edb::address_t address, bool &ok) {
	errno = 0;
	const long v = ptrace(PTRACE_PEEKTEXT, pid(), address, 0);
	SET_OK(ok, v);
	return v;
}

//------------------------------------------------------------------------------
// Name: write_data(edb::address_t address, long value)
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCore::write_data(edb::address_t address, long value) {
	return ptrace(PTRACE_POKETEXT, pid(), address, value) != -1;
}

//------------------------------------------------------------------------------
// Name: attach_thread(edb::tid_t tid)
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCore::attach_thread(edb::tid_t tid) {
	if(ptrace(PTRACE_ATTACH, tid, 0, 0) == 0) {
		// I *think* that the PTRACE_O_TRACECLONE is only valid on
		// on stopped threads
		int status;
		if(native::waitpid(tid, &status, __WALL) > 0) {
			threads_[tid] = thread_info(status);
			waited_threads_.insert(tid);
	#ifdef DEBUG_THREADS
			if(ptrace_set_options(tid, PTRACE_O_TRACECLONE) == -1) {
				qDebug("[DebuggerCore] failed to set PTRACE_O_TRACECLONE: [%d] %s", tid, strerror(errno));
			}
	#endif
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: attach(edb::pid_t pid)
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCore::attach(edb::pid_t pid) {
	detach();

#ifdef DEBUG_THREADS
	bool attached;
	do {
		attached = false;
		QDir proc_directory(QString("/proc/%1/task/").arg(pid));
		Q_FOREACH(QString s, proc_directory.entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
			// this can get tricky if the threads decide to spawn new threads
			// when we are attaching. I wish that linux had an atomic way to do this
			// all in one shot
			const edb::tid_t tid = s.toUInt();
			if(!threads_.contains(tid) && attach_thread(tid)) {
				attached = true;
			}
		}
	} while(attached);
#else
	attach_thread(pid)
#endif

	if(!threads_.empty()) {
		pid_            = pid;
		active_thread_  = pid;
		event_thread_   = pid;
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
// Name: detach()
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::detach() {
	if(attached()) {
		clear_breakpoints();
#ifdef DEBUG_THREADS
		Q_FOREACH(edb::tid_t thread, thread_ids()) {
			if(ptrace(PTRACE_DETACH, thread, 0, 0) == 0) {
				native::waitpid(thread, 0, __WALL);
			}
		}
#else
		if(ptrace(PTRACE_DETACH, pid(), 0, 0) == 0) {
			native::waitpid(pid(), 0, __WALL);
		}
#endif

		reset();
	}
}

//------------------------------------------------------------------------------
// Name: kill()
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::kill() {
	if(attached()) {
		clear_breakpoints();

		ptrace(PTRACE_KILL, pid(), 0, 0);

		// TODO: do i need to actually do this wait?
		native::waitpid(pid(), 0, __WALL);

		reset();
	}
}

//------------------------------------------------------------------------------
// Name: pause()
// Desc: stops *all* threads of a process
//------------------------------------------------------------------------------
void DebuggerCore::pause() {
	if(attached()) {
		// belive it or not, I belive that this is sufficient for all threads
		// this is because in the debug event handler above, a SIGSTOP is sent
		// to all threads when any event arrives, so no need to explicitly do
		// it here. We just need any thread to stop. So we'll just target the
		// pid() which will send it to any one of the threads in the process.
		::kill(pid(), SIGSTOP);
	}
}

//------------------------------------------------------------------------------
// Name: resume(edb::EVENT_STATUS status)
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::resume(edb::EVENT_STATUS status) {
	// TODO: assert that we are paused

	if(attached()) {
		if(status != edb::DEBUG_STOP) {
			const edb::tid_t tid = active_thread();
			const int code = (status == edb::DEBUG_EXCEPTION_NOT_HANDLED) ? resume_code(threads_[tid].status) : 0;
			ptrace_continue(tid, code);


#ifdef DEBUG_THREADS
			// resume the other threads passing the signal they originally reported had
			for(threadmap_t::const_iterator it = threads_.begin(); it != threads_.end(); ++it) {
				if(waited_threads_.contains(it.key())) {
					ptrace_continue(it.key(), resume_code(it->status));
				}
			}
#endif
		}
	}
}

//------------------------------------------------------------------------------
// Name: step(edb::EVENT_STATUS status)
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::step(edb::EVENT_STATUS status) {
	// TODO: assert that we are paused

	if(attached()) {
		if(status != edb::DEBUG_STOP) {
			const edb::tid_t tid = active_thread();
			const int code = (status == edb::DEBUG_EXCEPTION_NOT_HANDLED) ? resume_code(threads_[tid].status) : 0;
			ptrace_step(tid, code);
		}
	}
}

//------------------------------------------------------------------------------
// Name: get_state(State &state)
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::get_state(State &state) {
	// TODO: assert that we are paused

	PlatformState *const state_impl = static_cast<PlatformState *>(state.impl_);

	if(attached()) {
		if(ptrace(PTRACE_GETREGS, active_thread(), 0, &state_impl->regs_) != -1) {
		#if defined(EDB_X86)
			struct user_desc desc;
			std::memset(&desc, 0, sizeof(desc));

			if(ptrace(PTRACE_GET_THREAD_AREA, active_thread(), (state_impl->regs_.xgs / LDT_ENTRY_SIZE), &desc) != -1) {
				state_impl->gs_base = desc.base_addr;
			} else {
				state_impl->gs_base = 0;
			}

			if(ptrace(PTRACE_GET_THREAD_AREA, active_thread(), (state_impl->regs_.xfs / LDT_ENTRY_SIZE), &desc) != -1) {
				state_impl->fs_base = desc.base_addr;
			} else {
				state_impl->fs_base = 0;
			}
		#elif defined(EDB_X86_64)
		#endif
		}

		// floating point registers
		if(ptrace(PTRACE_GETFPREGS, active_thread(), 0, &state_impl->fpregs_) != -1) {
		}

		// debug registers
		state_impl->dr_[0] = ptrace(PTRACE_PEEKUSER, active_thread(), offsetof(user, u_debugreg[0]), 0);
		state_impl->dr_[1] = ptrace(PTRACE_PEEKUSER, active_thread(), offsetof(user, u_debugreg[1]), 0);
		state_impl->dr_[2] = ptrace(PTRACE_PEEKUSER, active_thread(), offsetof(user, u_debugreg[2]), 0);
		state_impl->dr_[3] = ptrace(PTRACE_PEEKUSER, active_thread(), offsetof(user, u_debugreg[3]), 0);
		state_impl->dr_[4] = 0;
		state_impl->dr_[5] = 0;
		state_impl->dr_[6] = ptrace(PTRACE_PEEKUSER, active_thread(), offsetof(user, u_debugreg[6]), 0);
		state_impl->dr_[7] = ptrace(PTRACE_PEEKUSER, active_thread(), offsetof(user, u_debugreg[7]), 0);

	} else {
		state_impl->clear();
	}
}

//------------------------------------------------------------------------------
// Name: set_state(const State &state)
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::set_state(const State &state) {

	// TODO: assert that we are paused
	PlatformState *const state_impl = static_cast<PlatformState *>(state.impl_);

	if(attached()) {

		ptrace(PTRACE_SETREGS, active_thread(), 0, &state_impl->regs_);

		// debug registers
		ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[0]), state_impl->dr_[0]);
		ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[1]), state_impl->dr_[1]);
		ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[2]), state_impl->dr_[2]);
		ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[3]), state_impl->dr_[3]);
		//ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[4]), state_impl->dr_[4]);
		//ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[5]), state_impl->dr_[5]);
		ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[6]), state_impl->dr_[6]);
		ptrace(PTRACE_POKEUSER, active_thread(), offsetof(user, u_debugreg[7]), state_impl->dr_[7]);
	}
}

//------------------------------------------------------------------------------
// Name: open(const QString &path, const QString &cwd, const QStringList &args, const QString &tty)
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCore::open(const QString &path, const QString &cwd, const QStringList &args, const QString &tty) {
	detach();
	pid_t pid;

	switch(pid = fork()) {
	case 0:
		// we are in the child now...

		// set ourselves (the child proc) up to be traced
		ptrace_traceme();

		// redirect it's I/O
		if(!tty.isEmpty()) {
			FILE *const std_out = freopen(qPrintable(tty), "r+b", stdout);
			FILE *const std_in  = freopen(qPrintable(tty), "r+b", stdin);
			FILE *const std_err = freopen(qPrintable(tty), "r+b", stderr);

			Q_UNUSED(std_out);
			Q_UNUSED(std_in);
			Q_UNUSED(std_err);
		}

		// do the actual exec
		execute_process(path, cwd, args);

		// we should never get here!
		abort();
		break;
	case -1:
		// error! for some reason we couldn't fork
		reset();
		return false;
	default:
		// parent
		do {
			reset();

			int status;
			if(native::waitpid(pid, &status, __WALL) == -1) {
				return false;
			}

			// the very first event should be a STOP of type SIGTRAP
			if(!WIFSTOPPED(status) || WSTOPSIG(status) != SIGTRAP) {
				detach();
				return false;
			}

			waited_threads_.insert(pid);

#ifdef DEBUG_THREADS
			// enable following clones (threads)
			if(ptrace_set_options(pid, PTRACE_O_TRACECLONE) == -1) {
				qDebug("[DebuggerCore] failed to set PTRACE_SETOPTIONS: %s", strerror(errno));
				detach();
				return false;
			}
#endif

			// setup the first event data for the primary thread
			waited_threads_.insert(pid);
			threads_[pid]   = thread_info(status);
			pid_            = pid;
			active_thread_  = pid;
			event_thread_   = pid;

			return true;
		} while(0);
		break;
	}
}

//------------------------------------------------------------------------------
// Name: set_active_thread(edb::tid_t tid)
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::set_active_thread(edb::tid_t tid) {
	if(threads_.contains(tid)) {
#if 0
		active_thread_ = tid;
#else
		qDebug("[DebuggerCore::set_active_thread] not implemented yet");
#endif
	} else {
		qDebug("[DebuggerCore] warning, attempted to set invalid thread as active: %d", tid);
	}
}

//------------------------------------------------------------------------------
// Name: reset()
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::reset() {
	threads_.clear();
	waited_threads_.clear();
	active_thread_ = 0;
	pid_           = 0;
	event_thread_  = 0;
}

//------------------------------------------------------------------------------
// Name: create_state() const
// Desc:
//------------------------------------------------------------------------------
StateInterface *DebuggerCore::create_state() const {
	return new PlatformState;
}

Q_EXPORT_PLUGIN2(DebuggerCore, DebuggerCore)
