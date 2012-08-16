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

#ifndef DEBUGEVENT_20060720_H_
#define DEBUGEVENT_20060720_H_

#include "Types.h"
#include <QString>
#include <QCoreApplication>
#include <signal.h> // for the SIG* definitions
#include "API.h"

class EDB_EXPORT DebugEvent {
	Q_DECLARE_TR_FUNCTIONS(DebugEvent)

public:
	enum REASON {
		EVENT_EXITED,   // exited normally
		EVENT_SIGNALED, // application received a signal
		EVENT_STOPPED,  // we received an event
		EVENT_UNKNOWN
	};

	enum TRAP_REASON {
		TRAP_STEPPING,
		TRAP_BREAKPOINT
	};

	struct Message {
		Message(const QString &c, const QString &m) : caption(c), message(m) {
		}

		Message() {
		}

		QString caption;
		QString message;
	};

private:
	int        status;
	edb::pid_t pid;
	edb::tid_t tid;
	void *     fault_address_;
	long       fault_code_;

public:
	DebugEvent();
	DebugEvent(int s, edb::pid_t pid, edb::tid_t tid);
	DebugEvent(const DebugEvent &other);
	DebugEvent &operator=(const DebugEvent &rhs);

	int exit_code() const;
	int signal_code() const;
	int stop_code() const;
	REASON reason() const;
	TRAP_REASON trap_reason() const;
	edb::tid_t thread() const;
	edb::pid_t process() const;

	bool exited() const;
	bool signaled() const;
	bool stopped() const;

	// stops
	static const int sigstop = SIGSTOP;
	static const int sigtrap = SIGTRAP;

	static const int sigkill = SIGKILL;

	bool is_error() const;
	Message error_description() const;
};

#endif

