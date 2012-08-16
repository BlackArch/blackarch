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

#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include <mach/mach.h>

namespace {
inline int resume_code(int status) {
	if(WIFSIGNALED(status)) {
		return WTERMSIG(status);
	} else if(WIFSTOPPED(status)) {
		return WSTOPSIG(status);
	}
	return 0;
}
}

//------------------------------------------------------------------------------
// Name: DebuggerCore()
// Desc: constructor
//------------------------------------------------------------------------------
DebuggerCore::DebuggerCore() {
	page_size_ = 0x1000;
}

//------------------------------------------------------------------------------
// Name: ~DebuggerCore()
// Desc:
//------------------------------------------------------------------------------
DebuggerCore::~DebuggerCore() {
	detach();
}

//------------------------------------------------------------------------------
// Name: 
// Desc: 
//------------------------------------------------------------------------------
bool DebuggerCore::has_extension(quint64 ext) const {
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
// Name: wait_debug_event(DebugEvent &event, bool &ok, int secs)
// Desc: waits for a debug event, secs is a timeout (but is not yet respected)
//       ok will be set to false if the timeout expires
//------------------------------------------------------------------------------
bool DebuggerCore::wait_debug_event(DebugEvent &event, int msecs) {
	if(attached()) {
		int status;
		bool timeout;

		const edb::tid_t tid = native::waitpid_timeout(pid(), &status, 0, msecs, timeout);
		if(!timeout) {
			if(tid > 0) {
				event = DebugEvent(status, pid(), tid);
				active_thread_       = event.thread();
				threads_[tid].status = status;
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: read_data(edb::address_t address, bool &ok)
// Desc:
//------------------------------------------------------------------------------
long DebuggerCore::read_data(edb::address_t address, bool &ok) {

        mach_port_t task;
        kern_return_t err = task_for_pid(mach_task_self(), pid(), &task);
        if(err != KERN_SUCCESS) {
            qDebug("task_for_pid() failed with %x [%d]", err, pid());
            ok = false;
            return -1;
        }

        long x;
        vm_size_t size;
        ok =  vm_read_overwrite(task, address, sizeof(long), (vm_address_t)&x, &size) == 0;
        return x;
#if 0
	errno = 0;
        const long v = ptrace(PT_READ_D, pid(), (char *)address, 0);
	SET_OK(ok, v);

        if(!ok) {
            qDebug("read_data: %d", errno);
        }

	return v;
#endif
}

//------------------------------------------------------------------------------
// Name: write_data(edb::address_t address, long value)
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCore::write_data(edb::address_t address, long value) {
	return ptrace(PT_WRITE_D, pid(), (char *)address, value) != -1;
}

//------------------------------------------------------------------------------
// Name: attach(edb::pid_t pid)
// Desc:
//------------------------------------------------------------------------------
bool DebuggerCore::attach(edb::pid_t pid) {
	detach();

	const long ret = ptrace(PT_ATTACH, pid, 0, 0);
	if(ret == 0) {
		pid_           = pid;
		active_thread_ = pid;
		threads_.clear();
		threads_.insert(pid, thread_info());

		// TODO: attach to all of the threads
	}

	return ret == 0;
}

//------------------------------------------------------------------------------
// Name: detach()
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::detach() {
	if(attached()) {
		clear_breakpoints();
		for(threadmap_t::const_iterator it = threads_.begin(); it != threads_.end(); ++it) {
			ptrace(PT_DETACH, it.key(), 0, 0);
		}

		pid_ = 0;
		threads_.clear();
	}
}

//------------------------------------------------------------------------------
// Name: kill()
// Desc:
//------------------------------------------------------------------------------
void DebuggerCore::kill() {
	if(attached()) {
		clear_breakpoints();
		ptrace(PT_KILL, pid(), 0, 0);
		native::waitpid(pid(), 0, WAIT_ANY);
		pid_ = 0;
		threads_.clear();
	}
}

//------------------------------------------------------------------------------
// Name: pause()
// Desc: stops *all* threads of a process
//------------------------------------------------------------------------------
void DebuggerCore::pause() {
	if(attached()) {
		for(threadmap_t::const_iterator it = threads_.begin(); it != threads_.end(); ++it) {
			::kill(it.key(), SIGSTOP);
		}
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
			ptrace(PT_CONTINUE, tid, reinterpret_cast<caddr_t>(1), code);
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
			ptrace(PT_STEP, tid, reinterpret_cast<caddr_t>(1), code);
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

		/* Get the mach task for the target process */
		mach_port_t task;
		kern_return_t err = task_for_pid(mach_task_self(), pid(), &task);
		if(err != KERN_SUCCESS) {
			qDebug("task_for_pid() failed with %x [%d]", err, pid());
			return;
		}

		/* Suspend the target process */
		err = task_suspend(task);
		if(err != KERN_SUCCESS) {
			qDebug("task_suspend() failed");
			return;
		}

		/* Get all threads in the specified task */
		thread_act_port_array_t thread_list;
		mach_msg_type_number_t  thread_count;

		err = task_threads(task, &thread_list, &thread_count);
		if(err != KERN_SUCCESS) {
			qDebug("task_threads() failed");
			err = task_resume(task);
			if(err != KERN_SUCCESS) {
				qDebug("task_resume() failed");
			}
		}

		Q_ASSERT(thread_count > 0);

        #ifdef EDB_X86
                mach_msg_type_number_t state_count           = x86_THREAD_STATE32_COUNT;
                const thread_state_flavor_t flavor           = x86_THREAD_STATE32;
                const thread_state_flavor_t debug_flavor     = x86_DEBUG_STATE32;
                const thread_state_flavor_t fpu_flavor       = x86_FLOAT_STATE32;
                const thread_state_flavor_t exception_flavor = x86_EXCEPTION_STATE32;
        #elif defined(EDB_X86_64)
                mach_msg_type_number_t state_count           = x86_THREAD_STATE64_COUNT;
                const thread_state_flavor_t flavor           = x86_THREAD_STATE64;
                const thread_state_flavor_t debug_flavor     = x86_DEBUG_STATE64;
                const thread_state_flavor_t fpu_flavor       = x86_FLOAT_STATE64;
                const thread_state_flavor_t exception_flavor = x86_EXCEPTION_STATE64;
        #endif
                // TODO Get all threads, not just the first one.
                err = thread_get_state(
                        thread_list[0],
                        flavor,
                        (thread_state_t)&state_impl->thread_state_,
                        &state_count);

                if(err != KERN_SUCCESS) {
                        qDebug("thread_get_state() failed with %.08x", err);
                        err = task_resume(task);
                        if(err != KERN_SUCCESS) {
                                qDebug("task_resume() failed");
                        }
                        return;
                }

                err = thread_get_state(
                        thread_list[0],
                        debug_flavor,
                        (thread_state_t)&state_impl->debug_state_,
                        &state_count);

                if(err != KERN_SUCCESS) {
                        qDebug("thread_get_state() failed with %.08x", err);
                        err = task_resume(task);
                        if(err != KERN_SUCCESS) {
                                qDebug("task_resume() failed");
                        }
                        return;
                }

                err = thread_get_state(
                        thread_list[0],
                        fpu_flavor,
                        (thread_state_t)&state_impl->float_state_,
                        &state_count);

                if(err != KERN_SUCCESS) {
                        qDebug("thread_get_state() failed with %.08x", err);
                        err = task_resume(task);
                        if(err != KERN_SUCCESS) {
                                qDebug("task_resume() failed");
                        }
                        return;
                }

                err = thread_get_state(
                        thread_list[0],
                        exception_flavor,
                        (thread_state_t)&state_impl->exception_state_,
                        &state_count);

                if(err != KERN_SUCCESS) {
                        qDebug("thread_get_state() failed with %.08x", err);
                        err = task_resume(task);
                        if(err != KERN_SUCCESS) {
                                qDebug("task_resume() failed");
                        }
                        return;
                }
	} else {
		state.clear();
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

		/* Get the mach task for the target process */
		mach_port_t task;
		kern_return_t err = task_for_pid(mach_task_self(), pid(), &task);
		if(err != KERN_SUCCESS) {
			qDebug("task_for_pid() failed with %x [%d]", err, pid());
			return;
		}

		/* Suspend the target process */
		err = task_suspend(task);
		if(err != KERN_SUCCESS) {
			qDebug("task_suspend() failed");
		}

		/* Get all threads in the specified task */
		thread_act_port_array_t thread_list;
		mach_msg_type_number_t  thread_count;
		err = task_threads(task, &thread_list, &thread_count);

		if(err != KERN_SUCCESS) {
			qDebug("task_threads() failed");
			err = task_resume(task);
			if(err != KERN_SUCCESS) {
				qDebug("task_resume() failed");
			}
		}

		Q_ASSERT(thread_count > 0);

        #ifdef EDB_X86
                mach_msg_type_number_t state_count           = x86_THREAD_STATE32_COUNT;
                const thread_state_flavor_t flavor           = x86_THREAD_STATE32;
                const thread_state_flavor_t debug_flavor     = x86_DEBUG_STATE32;
                //const thread_state_flavor_t fpu_flavor       = x86_FLOAT_STATE32;
                //const thread_state_flavor_t exception_flavor = x86_EXCEPTION_STATE32;
        #elif defined(EDB_X86_64)
                mach_msg_type_number_t state_count           = x86_THREAD_STATE64_COUNT;
                const thread_state_flavor_t flavor           = x86_THREAD_STATE64;
                const thread_state_flavor_t debug_flavor     = x86_DEBUG_STATE64;
                //const thread_state_flavor_t fpu_flavor       = x86_FLOAT_STATE64;
                //const thread_state_flavor_t exception_flavor = x86_EXCEPTION_STATE64;
        #endif

                // TODO Set for specific thread, not first one
                err = thread_set_state(
                        thread_list[0],
                        flavor,
                        (thread_state_t)&state_impl->thread_state_,
                        state_count);

		if(err != KERN_SUCCESS) {
			qDebug("thread_set_state() failed with %.08x", err);
			err = task_resume(task);
			if(err != KERN_SUCCESS) {
				qDebug("task_resume() failed");
			}
                        return;
		}

                err = thread_set_state(
                        thread_list[0],
                        debug_flavor,
                        (thread_state_t)&state_impl->debug_state_,
                        state_count);

                if(err != KERN_SUCCESS) {
                        qDebug("thread_set_state() failed with %.08x", err);
                        err = task_resume(task);
                        if(err != KERN_SUCCESS) {
                                qDebug("task_resume() failed");
                        }
                        return;
                }
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
		ptrace(PT_TRACE_ME, 0, 0, 0);

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
		// error!
		pid_ = 0;
		return false;
	default:
		// parent
		do {
			threads_.clear();

			int status;
			if(native::waitpid(pid, &status, 0) == -1) {
				return false;
			}

			// the very first event should be a STOP of type SIGTRAP
			if(!WIFSTOPPED(status) || WSTOPSIG(status) != SIGTRAP) {
				detach();
				return false;
			}

			// setup the first event data for the primary thread
			threads_.insert(pid, thread_info());
			pid_                 = pid;
			active_thread_       = pid;
			threads_[pid].status = status;
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
	Q_ASSERT(threads_.contains(tid));
	active_thread_ = tid;
}

//------------------------------------------------------------------------------
// Name: create_state() const
// Desc:
//------------------------------------------------------------------------------
StateInterface *DebuggerCore::create_state() const {
	return new PlatformState;
}

Q_EXPORT_PLUGIN2(DebuggerCore, DebuggerCore)
