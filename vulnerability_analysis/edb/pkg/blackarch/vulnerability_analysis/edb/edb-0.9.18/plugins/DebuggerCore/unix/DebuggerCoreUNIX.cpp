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

// this code is common to all unix variants (linux/bsd/osx)

#include "DebuggerCoreUNIX.h"
#include "Debugger.h"

#include <QStringList>
#include <cerrno>
#include <climits>
#include <csignal>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace {
	int selfpipe[2];
	void (*old_sigchld_handler)(int sig, siginfo_t *, void *);

	//------------------------------------------------------------------------------
	// Name: sigchld_handler(int sig, siginfo_t *info, void *p)
	// Desc:
	//------------------------------------------------------------------------------
	void sigchld_handler(int sig, siginfo_t *info, void *p) {
		if(sig == SIGCHLD) {
			native::write(selfpipe[1], " ", sizeof(char));
		}

		if(old_sigchld_handler) {
			old_sigchld_handler(sig, info, p);
		}
	}
}

//------------------------------------------------------------------------------
// Name: execvp(const char *file, char *const argv[])
// Desc: execvp, but handles being interrupted
//------------------------------------------------------------------------------
int native::execvp(const char *file, char *const argv[]) {
	int ret;
	do {
		ret = ::execvp(file, argv);
	} while(ret == -1 && errno == EINTR);
	return ret;
}

//------------------------------------------------------------------------------
// Name: read(int fd, void *buf, size_t count)
// Desc: read, but handles being interrupted
//------------------------------------------------------------------------------
ssize_t native::read(int fd, void *buf, size_t count) {
	ssize_t ret;
	do {
		ret = ::read(fd, buf, count);
	} while(ret == -1 && errno == EINTR);
	return ret;
}

//------------------------------------------------------------------------------
// Name: write(int fd, const void *buf, size_t count)
// Desc: write, but handles being interrupted
//------------------------------------------------------------------------------
ssize_t native::write(int fd, const void *buf, size_t count) {
	ssize_t ret;
	do {
		ret = ::write(fd, buf, count);
	} while(ret == -1 && errno == EINTR);
	return ret;
}

//------------------------------------------------------------------------------
// Name: select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
// Desc: select but handles being interrupted
//------------------------------------------------------------------------------
int native::select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
	int ret;
	do {
		ret = ::select(nfds, readfds, writefds, exceptfds, timeout);
	} while(ret == -1 && errno == EINTR);
	return ret;
}

//------------------------------------------------------------------------------
// Name: waitpid(pid_t pid, int *status, int options)
// Desc: waitpid, but handles being interrupted
//------------------------------------------------------------------------------
pid_t native::waitpid(pid_t pid, int *status, int options) {
	pid_t ret;
	do {
		ret = ::waitpid(pid, status, options);
	} while(ret == -1 && errno == EINTR);
	return ret;
}

//------------------------------------------------------------------------------
// Name: select_ex(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, quint64 msecs)
// Desc: similar to select but has the timeout specified as an unsigned quantity of msecs
// Note: msecs == 0 means wait forever.
//------------------------------------------------------------------------------
int native::select_ex(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, quint64 msecs) {
	if(msecs != 0) {
		struct timeval tv;

		tv.tv_sec  = (msecs / 1000);
		tv.tv_usec = (msecs % 1000) * 1000;

		return native::select(nfds, readfds, writefds, exceptfds, &tv);
	} else {
		return native::select(nfds, readfds, writefds, exceptfds, NULL);
	}
}

//------------------------------------------------------------------------------
// Name: wait_for_sigchld(int msecs)
// Desc:
//------------------------------------------------------------------------------
bool native::wait_for_sigchld(int msecs) {

	fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(selfpipe[0], &rfds);

	if(native::select_ex(selfpipe[0] + 1, &rfds, 0, 0, msecs) == 0) {
		return true;
	}

	char ch;
	if(native::read(selfpipe[0], &ch, sizeof(char)) == -1) {
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
// Name: waitpid_timeout(pid_t pid, int *status, int options, int msecs, bool &timeout)
// Desc:
//------------------------------------------------------------------------------
pid_t native::waitpid_timeout(pid_t pid, int *status, int options, int msecs, bool &timeout) {

	Q_ASSERT(pid > 0);

	timeout = wait_for_sigchld(msecs);

	if(!timeout) {
		return native::waitpid(pid, status, options | WNOHANG);
	}

	return -1;
}

//------------------------------------------------------------------------------
// Name: DebuggerCoreUNIX()
// Desc:
//------------------------------------------------------------------------------
DebuggerCoreUNIX::DebuggerCoreUNIX() {

	// create a pipe and make it non-blocking
	int r = ::pipe(selfpipe);
	Q_UNUSED(r);

	::fcntl(selfpipe[0], F_SETFL, ::fcntl(selfpipe[0], F_GETFL) | O_NONBLOCK);
	::fcntl(selfpipe[1], F_SETFL, ::fcntl(selfpipe[1], F_GETFL) | O_NONBLOCK);

	// setup a signal handler
	struct sigaction new_action;
	struct sigaction old_action;

	std::memset(&new_action, 0, sizeof(new_action));
	std::memset(&old_action, 0, sizeof(old_action));

	new_action.sa_sigaction = sigchld_handler;
	new_action.sa_flags = SA_RESTART | SA_SIGINFO;

	sigaction(SIGCHLD, &new_action, &old_action);

	old_sigchld_handler = old_action.sa_sigaction;
}


//------------------------------------------------------------------------------
// Name: write_byte(edb::address_t address, quint8 value, bool &ok)
// Desc: writes a single byte at a given address
// Note: assumes the this will not trample any breakpoints, must be handled
//       in calling code!
//------------------------------------------------------------------------------
void DebuggerCoreUNIX::write_byte(edb::address_t address, quint8 value, bool &ok) {
	write_byte_base(address, value, ok);
}

//------------------------------------------------------------------------------
// Name: read_byte(edb::address_t address, bool &ok)
// Desc: reads a single bytes at a given address
//------------------------------------------------------------------------------
quint8 DebuggerCoreUNIX::read_byte(edb::address_t address, bool &ok) {

	// TODO: handle if breakponts have a size more than 1!
	const quint8 ret = read_byte_base(address, ok);

	if(ok) {
		if(const Breakpoint::pointer bp = find_breakpoint(address)) {
			return bp->original_bytes()[0];
		}
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: write_byte_base(edb::address_t address, quint8 value, bool &ok)
// Desc: the base implementation of writing a byte
//------------------------------------------------------------------------------
void DebuggerCoreUNIX::write_byte_base(edb::address_t address, quint8 value, bool &ok) {
	// TODO: assert that we are paused

	ok = false;
	if(attached()) {
		long v;
		long mask;
		// page_size() - 1 will always be 0xf* because pagesizes
		// are always 0x10*, so the masking works
		// range of a is [1..n] where n=pagesize, and we have to adjust
		// if a < wordsize
		const edb::address_t a = page_size() - (address & (page_size() - 1));

		v = value;
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		if(a < EDB_WORDSIZE) {
			address -= (EDB_WORDSIZE - a);                       // LE + BE
			mask = ~(0xffUL << (CHAR_BIT * (EDB_WORDSIZE - a))); // LE
			v <<= CHAR_BIT * (EDB_WORDSIZE - a);                 // LE
		} else {
			mask = ~0xffUL; // LE
		}
#else /* BIG ENDIAN */
		if(a < EDB_WORDSIZE) {
			address -= (EDB_WORDSIZE - a);            // LE + BE
			mask = ~(0xffUL << (CHAR_BIT * (a - 1))); // BE
			v <<= CHAR_BIT * (a - 1);                 // BE
		} else {
			mask = ~(0xffUL << (CHAR_BIT * (EDB_WORDSIZE - 1))); // BE
			v <<= CHAR_BIT * (EDB_WORDSIZE - 1);                 // BE
		}
#endif

		v |= (read_data(address, ok) & mask);
		if(ok) {
			ok = write_data(address, v);
		}
	}
}

//------------------------------------------------------------------------------
// Name: read_byte_base(edb::address_t address, bool &ok)
// Desc: the base implementation of reading a byte
//------------------------------------------------------------------------------
quint8 DebuggerCoreUNIX::read_byte_base(edb::address_t address, bool &ok) {
	// TODO: assert that we are paused

	ok = false;
	errno = -1;
	if(attached()) {
		// if this spot is unreadable, then just return 0xff, otherwise
		// continue as normal.

		// page_size() - 1 will always be 0xf* because pagesizes
		// are always 0x10*, so the masking works
		// range of a is [1..n] where n=pagesize, and we have to adjust
		// if a < wordsize
		const edb::address_t a = page_size() - (address & (page_size() - 1));

		if(a < EDB_WORDSIZE) {
			address -= (EDB_WORDSIZE - a); // LE + BE
		}

		long value = read_data(address, ok);

		if(ok) {
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
			if(a < EDB_WORDSIZE) {
				value >>= CHAR_BIT * (EDB_WORDSIZE - a); // LE
			}
#else
			if(a < EDB_WORDSIZE) {
				value >>= CHAR_BIT * (a - 1);            // BE
			} else {
				value >>= CHAR_BIT * (EDB_WORDSIZE - 1); // BE
			}
#endif
			return value & 0xff;
		}
	}

	return 0xff;
}

//------------------------------------------------------------------------------
// Name: read_pages(edb::address_t address, void *buf, std::size_t count)
// Desc: reads <count> pages from the process starting at <address>
// Note: buf's size must be >= count * page_size()
// Note: address should be page aligned.
//------------------------------------------------------------------------------
bool DebuggerCoreUNIX::read_pages(edb::address_t address, void *buf, std::size_t count) {

	Q_ASSERT(buf != 0);

	if(!attached()) {
		return false;
	}

	if((address & (page_size() - 1)) == 0) {
		const edb::address_t orig_address = address;
		long *ptr                         = reinterpret_cast<long *>(buf);
		quint8 *const orig_ptr            = reinterpret_cast<quint8 *>(buf);

		const edb::address_t end_address  = orig_address + page_size() * count;

		for(std::size_t c = 0; c < count; ++c) {
			for(edb::address_t i = 0; i < page_size(); i += EDB_WORDSIZE) {
				bool ok;
				const long v = read_data(address, ok);
				if(!ok) {
					return false;
				}

				*ptr++ = v;
				address += EDB_WORDSIZE;
			}
		}

		// TODO: handle if breakponts have a size more than 1!
		Q_FOREACH(const Breakpoint::pointer &bp, breakpoints_) {
			if(bp->address() >= orig_address && bp->address() < end_address) {
				// show the original bytes in the buffer..
				orig_ptr[bp->address() - orig_address] = bp->original_bytes()[0];
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------------
// Name: read_bytes(edb::address_t address, void *buf, std::size_t len)
// Desc: reads <len> bytes into <buf> starting at <address>
// Note: if the read failed, the part of the buffer that could not be read will
//       be filled with 0xff bytes
//------------------------------------------------------------------------------
bool DebuggerCoreUNIX::read_bytes(edb::address_t address, void *buf, std::size_t len) {

	Q_ASSERT(buf != 0);

	if(!attached()) {
		return false;
	}

	if(len != 0) {
		bool ok;
		quint8 *p = reinterpret_cast<quint8 *>(buf);
		quint8 ch = read_byte(address, ok);

		while(ok && len) {
			*p++ = ch;
			if(--len) {
				++address;
				ch = read_byte(address, ok);
			}
		}

		if(!ok) {
			while(len--) {
				*p++ = 0xff;
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------------
// Name: write_bytes(edb::address_t address, const void *buf, std::size_t len)
// Desc: writes <len> bytes from <buf> starting at <address>
//------------------------------------------------------------------------------
bool DebuggerCoreUNIX::write_bytes(edb::address_t address, const void *buf, std::size_t len) {

	Q_ASSERT(buf != 0);

	bool ok = false;
	if(attached()) {
		const quint8 *p = reinterpret_cast<const quint8 *>(buf);

		while(len--) {
			write_byte(address++, *p++, ok);
			if(!ok) {
				break;
			}
		}
	}
	return ok;
}

//------------------------------------------------------------------------------
// Name: execute_process(const QString &path, const QString &cwd, const QStringList &args)
// Desc:
//------------------------------------------------------------------------------
void DebuggerCoreUNIX::execute_process(const QString &path, const QString &cwd, const QStringList &args) {
	// change to the desired working directory
	if(::chdir(qPrintable(cwd)) == 0) {

		// allocate space for all the arguments
		char **const argv_pointers = new char *[args.count() + 2];

		char **p = argv_pointers;

		*p = new char[path.length() + 1];
		std::strcpy(*p, qPrintable(path));
		++p;

		for(int i = 0; i < args.count(); ++i) {
			const QString s(args[i]);
			*p = new char[s.length() + 1];
			std::strcpy(*p, qPrintable(s));
			++p;
		}

		*p = 0;

		const int ret = native::execvp(argv_pointers[0], argv_pointers);

		// should be no need to cleanup, the process which allocated all that
		// space no longer exists!
		// if we get here...execvp failed!
		if(ret == -1) {
			p = argv_pointers;
			while(*p != 0) {
				delete [] *p++;
			}
			delete [] argv_pointers;
		}
	}
}

//------------------------------------------------------------------------------
// Name: pointer_size() const
// Desc: returns the size of a pointer on this arch
//------------------------------------------------------------------------------
int DebuggerCoreUNIX::pointer_size() const {
	return sizeof(void *);
}
