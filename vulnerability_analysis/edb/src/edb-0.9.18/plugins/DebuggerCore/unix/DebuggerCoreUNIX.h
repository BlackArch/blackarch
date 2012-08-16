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

#ifndef DEBUGGERCOREUNIX_20090529_H_
#define DEBUGGERCOREUNIX_20090529_H_

#include "DebuggerCoreBase.h"
#include <QList>

#define SET_OK(ok, v) do { (ok) = ((v) != -1) || (errno == 0); } while(0)
#define EDB_WORDSIZE  sizeof(long)

namespace native {
	int execvp(const char *file, char *const argv[]);
	int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
	int select_ex(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, quint64 msecs);
	pid_t waitpid(pid_t pid, int *status, int options);
	pid_t waitpid_timeout(pid_t pid, int *status, int options, int msecs, bool &timeout);
	ssize_t read(int fd, void *buf, size_t count);
	ssize_t write(int fd, const void *buf, size_t count);
	bool wait_for_sigchld(int msecs);
}

class DebuggerCoreUNIX : public DebuggerCoreBase {
public:
	DebuggerCoreUNIX();
	virtual ~DebuggerCoreUNIX() {}

protected:
	quint8 read_byte(edb::address_t address, bool &ok);
	quint8 read_byte_base(edb::address_t address, bool &ok);
	void execute_process(const QString &path, const QString &cwd, const QStringList &args);
	void write_byte(edb::address_t address, quint8 value, bool &ok);
	void write_byte_base(edb::address_t address, quint8 value, bool &ok);

public:
	virtual bool read_pages(edb::address_t address, void *buf, std::size_t count);
	virtual bool read_bytes(edb::address_t address, void *buf, std::size_t len);
	virtual bool write_bytes(edb::address_t address, const void *buf, std::size_t len);
	virtual int pointer_size() const;

protected:
	virtual long read_data(edb::address_t address, bool &ok) = 0;
	virtual bool write_data(edb::address_t address, long value) = 0;
};

#endif
