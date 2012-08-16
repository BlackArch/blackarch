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

#include "CommentServer.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "Configuration.h"
#include "Instruction.h"

#include <QString>

//------------------------------------------------------------------------------
// Name: ~CommentServer()
// Desc:
//------------------------------------------------------------------------------
CommentServer::~CommentServer() {
}

//------------------------------------------------------------------------------
// Name: set_comment(QHexView::address_t address,  const QString &comment)
// Desc:
//------------------------------------------------------------------------------
void CommentServer::set_comment(QHexView::address_t address, const QString &comment) {
	custom_comments_[address] = comment;
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc:
//------------------------------------------------------------------------------
void CommentServer::clear() {
	custom_comments_.clear();
}

// a call can be anywhere from 2 to 7 bytes long depends on if there is a Mod/RM byte
// or a SIB byte, etc
// this is ignoring prefixes, fortunately, no calls have mandatory prefixes
// TODO: this is a arch specific concept
#define CALL_MAX_SIZE 7
#define CALL_MIN_SIZE 2

//------------------------------------------------------------------------------
// Name: resolve_function_call(QHexView::address_t address) const
// Desc:
//------------------------------------------------------------------------------
QString CommentServer::resolve_function_call(QHexView::address_t address, bool &ok) const {
	QString ret;

	ok = false;

	// ok, we now want to locate the instruction before this one
	// so we need to look back a few bytes
	quint8 buffer[edb::Instruction::MAX_SIZE];

	// TODO: portability warning, makes assumptions on the size of a call
	if(edb::v1::debugger_core->read_bytes(address - CALL_MAX_SIZE, buffer, sizeof(buffer))) {
		for(int i = (CALL_MAX_SIZE - CALL_MIN_SIZE); i >= 0; --i) {
			edb::Instruction insn(buffer + i, sizeof(buffer) - i, 0, std::nothrow);
			if(insn.valid() && insn.type() == edb::Instruction::OP_CALL) {
				const QString symname = edb::v1::find_function_symbol(address);
				if(!symname.isEmpty()) {
					ret = tr("return to %1 <%2>").arg(edb::v1::format_pointer(address)).arg(symname);
				} else {
					ret = tr("return to %1").arg(edb::v1::format_pointer(address));
				}
				ok = true;
				break;
			}
		}
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: resolve_string(QHexView::address_t address) const
// Desc:
//------------------------------------------------------------------------------
QString CommentServer::resolve_string(QHexView::address_t address, bool &ok) const {

	const int min_string_length = edb::v1::config().min_string_length;
	QString ret;
	ok = false;

	int stringLen;
	QString temp;
	if((ok = edb::v1::get_ascii_string_at_address(address, temp, min_string_length, 256, stringLen))) {
		ret = tr("ASCII \"%1\"").arg(temp);
	} else if((ok = edb::v1::get_utf16_string_at_address(address, temp, min_string_length, 256, stringLen))) {
		ret = tr("UTF16 \"%1\"").arg(temp);
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: comment(QHexView::address_t address, int size) const
// Desc:
//------------------------------------------------------------------------------
QString CommentServer::comment(QHexView::address_t address, int size) const {

	QString ret;
	

	// if the view is currently looking at words which are a pointer in size
	// then see if it points to anything...
	if(size == edb::v1::pointer_size()) {
		edb::address_t value;
		if(edb::v1::debugger_core->read_bytes(address, &value, sizeof(value))) {

			QHash<quint64, QString>::const_iterator it = custom_comments_.find(value);
			if(it != custom_comments_.end()) {
				ret = it.value();
			} else {
				bool ok;
				ret = resolve_function_call(value, ok);
				if(!ok) {
					ret = resolve_string(value, ok);
				}
			}
		}
	}

	return ret;
}
