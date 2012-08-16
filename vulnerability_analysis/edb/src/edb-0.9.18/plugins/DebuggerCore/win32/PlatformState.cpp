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

#include "PlatformState.h"
#include "Debugger.h"
#include <limits>
#include <cmath>

namespace {

// little-endian!
double read_float80(const uint8_t buffer[10]) {
	//80 bit floating point value according to IEEE-754:
	//1 bit sign, 15 bit exponent, 64 bit mantissa

	const uint16_t SIGNBIT    = 1 << 15;
	const uint16_t EXP_BIAS   = (1 << 14) - 1; // 2^(n-1) - 1 = 16383
	const uint16_t SPECIALEXP = (1 << 15) - 1; // all bits set
	const uint64_t HIGHBIT    = (uint64_t)1 << 63;
	const uint64_t QUIETBIT   = (uint64_t)1 << 62;

	// Extract sign, exponent and mantissa
	uint16_t exponent = *((uint16_t*)&buffer[8]);
	uint64_t mantissa = *((uint64_t*)&buffer[0]);

	double sign = (exponent & SIGNBIT) ? -1.0 : 1.0;
	exponent   &= ~SIGNBIT;

	// Check for undefined values
	if((!exponent && (mantissa & HIGHBIT)) || (exponent && !(mantissa & HIGHBIT))) {
		return std::numeric_limits<double>::quiet_NaN();
	}

	// Check for special values (infinity, NaN)
	if(exponent == 0) {
		if(mantissa == 0) {
			return sign * 0.0;
		} else {
			// denormalized
		}
	} else if(exponent == SPECIALEXP) {
		if(!(mantissa & ~HIGHBIT)) {
			return sign * std::numeric_limits<double>::infinity();
		} else {
			if(mantissa & QUIETBIT) {
				return std::numeric_limits<double>::quiet_NaN();
			} else {
				return std::numeric_limits<double>::signaling_NaN();
			}
		}
	}

	//value = (-1)^s * (m / 2^63) * 2^(e - 16383)
	double significand = ((double)mantissa / ((uint64_t)1 << 63));
	return sign * ldexp(significand, exponent - EXP_BIAS);
}
}


//------------------------------------------------------------------------------
// Name: PlatformState()
// Desc:
//------------------------------------------------------------------------------
PlatformState::PlatformState() {
	memset(&context_, 0, sizeof(context_));
	fs_base_ = 0;
	gs_base_ = 0;
}

//------------------------------------------------------------------------------
// Name: PlatformState::copy() const
// Desc: makes a copy of the state object
//------------------------------------------------------------------------------
StateInterface *PlatformState::copy() const {
	return new PlatformState(*this);
}

//------------------------------------------------------------------------------
// Name: flags_to_string(edb::reg_t flags) const
// Desc: returns the flags in a string form appropriate for this platform
//------------------------------------------------------------------------------
QString PlatformState::flags_to_string(edb::reg_t flags) const {
	char buf[14];
	qsnprintf(
		buf,
		sizeof(buf),
		"%c %c %c %c %c %c %c",
		((flags & 0x001) ? 'C' : 'c'),
		((flags & 0x004) ? 'P' : 'p'),
		((flags & 0x010) ? 'A' : 'a'),
		((flags & 0x040) ? 'Z' : 'z'),
		((flags & 0x080) ? 'S' : 's'),
		((flags & 0x400) ? 'D' : 'd'),
		((flags & 0x800) ? 'O' : 'o'));

	return buf;
}

//------------------------------------------------------------------------------
// Name: flags_to_string() const
// Desc: returns the flags in a string form appropriate for this platform
//------------------------------------------------------------------------------
QString PlatformState::flags_to_string() const {
	return flags_to_string(flags());
}

//------------------------------------------------------------------------------
// Name: value(const QString &reg) const
// Desc: returns a Register object which represents the register with the name
//       supplied
//------------------------------------------------------------------------------
Register PlatformState::value(const QString &reg) const {
	const QString lreg = reg.toLower();

#if defined(EDB_X86)
	if(lreg == "eax")			return Register("eax", context_.Eax, Register::TYPE_GPR);
	else if(lreg == "ebx")		return Register("ebx", context_.Ebx, Register::TYPE_GPR);
	else if(lreg == "ecx")		return Register("ecx", context_.Ecx, Register::TYPE_GPR);
	else if(lreg == "edx")		return Register("edx", context_.Edx, Register::TYPE_GPR);
	else if(lreg == "ebp")		return Register("ebp", context_.Ebp, Register::TYPE_GPR);
	else if(lreg == "esp")		return Register("esp", context_.Esp, Register::TYPE_GPR);
	else if(lreg == "esi")		return Register("esi", context_.Esi, Register::TYPE_GPR);
	else if(lreg == "edi")		return Register("edi", context_.Edi, Register::TYPE_GPR);
	else if(lreg == "eip")		return Register("eip", context_.Eip, Register::TYPE_IP);
	else if(lreg == "ax")		return Register("ax", context_.Eax & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bx")		return Register("bx", context_.Ebx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "cx")		return Register("cx", context_.Ecx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "dx")		return Register("dx", context_.Edx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bp")		return Register("bp", context_.Ebp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "sp")		return Register("sp", context_.Esp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "si")		return Register("si", context_.Esi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "di")		return Register("di", context_.Edi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "al")		return Register("al", context_.Eax & 0xff, Register::TYPE_GPR);
	else if(lreg == "bl")		return Register("bl", context_.Ebx & 0xff, Register::TYPE_GPR);
	else if(lreg == "cl")		return Register("cl", context_.Ecx & 0xff, Register::TYPE_GPR);
	else if(lreg == "dl")		return Register("dl", context_.Edx & 0xff, Register::TYPE_GPR);
	else if(lreg == "ah")		return Register("ah", (context_.Eax >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "bh")		return Register("bh", (context_.Ebx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "ch")		return Register("ch", (context_.Ecx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "dh")		return Register("dh", (context_.Edx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "cs")		return Register("cs", context_.SegCs, Register::TYPE_SEG, 0);
	else if(lreg == "ds")		return Register("ds", context_.SegDs, Register::TYPE_SEG, 0);
	else if(lreg == "es")		return Register("es", context_.SegEs, Register::TYPE_SEG, 0);
	else if(lreg == "fs")		return Register("fs", context_.SegFs, Register::TYPE_SEG, fs_base_);
	else if(lreg == "gs")		return Register("gs", context_.SegGs, Register::TYPE_SEG, gs_base_);
	else if(lreg == "ss") 		return Register("ss", context_.SegSs, Register::TYPE_SEG, 0);
	else if(lreg == "eflags") 	return Register("eflags", context_.EFlags, Register::TYPE_COND);
#elif defined(EDB_X86_64)
	if(lreg == "rax")			return Register("rax", context_.Rax, Register::TYPE_GPR);
	else if(lreg == "rbx")		return Register("rbx", context_.Rbx, Register::TYPE_GPR);
	else if(lreg == "rcx")		return Register("rcx", context_.Rcx, Register::TYPE_GPR);
	else if(lreg == "rdx")		return Register("rdx", context_.Rdx, Register::TYPE_GPR);
	else if(lreg == "rbp")		return Register("rbp", context_.Rbp, Register::TYPE_GPR);
	else if(lreg == "rsp")		return Register("rsp", context_.Rsp, Register::TYPE_GPR);
	else if(lreg == "rsi")		return Register("rsi", context_.Rsi, Register::TYPE_GPR);
	else if(lreg == "rdi")		return Register("rdi", context_.Rdi, Register::TYPE_GPR);
	else if(lreg == "rip")		return Register("rip", context_.Rip, Register::TYPE_IP);
	else if(lreg == "r8")		return Register("r8", context_.R8, Register::TYPE_GPR);
	else if(lreg == "r9")		return Register("r9", context_.R9, Register::TYPE_GPR);
	else if(lreg == "r10")		return Register("r10", context_.R10, Register::TYPE_GPR);
	else if(lreg == "r11")		return Register("r11", context_.R11, Register::TYPE_GPR);
	else if(lreg == "r12")		return Register("r12", context_.R12, Register::TYPE_GPR);
	else if(lreg == "r13")		return Register("r13", context_.R13, Register::TYPE_GPR);
	else if(lreg == "r14")		return Register("r14", context_.R14, Register::TYPE_GPR);
	else if(lreg == "r15")		return Register("r15", context_.R15, Register::TYPE_GPR);
	else if(lreg == "eax")		return Register("eax", context_.Rax & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ebx")		return Register("ebx", context_.Rbx & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ecx")		return Register("ecx", context_.Rcx & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "edx")		return Register("edx", context_.Rdx & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ebp")		return Register("ebp", context_.Rbp & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "esp")		return Register("esp", context_.Rsp & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "esi")		return Register("esi", context_.Rsi & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "edi")		return Register("edi", context_.Rdi & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r8d")		return Register("r8d", context_.R8 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r9d")		return Register("r9d", context_.R9 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r10d") 	return Register("r10d", context_.R10 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r11d") 	return Register("r11d", context_.R11 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r12d") 	return Register("r12d", context_.R12 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r13d") 	return Register("r13d", context_.R13 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r14d") 	return Register("r14d", context_.R14 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r15d")		return Register("r15d", context_.R15 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ax")		return Register("ax", context_.Rax & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bx")		return Register("bx", context_.Rbx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "cx")		return Register("cx", context_.Rcx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "dx")		return Register("dx", context_.Rdx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bp")		return Register("bp", context_.Rbp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "sp")		return Register("sp", context_.Rsp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "si")		return Register("si", context_.Rsi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "di")		return Register("di", context_.Rdi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r8w")		return Register("r8w", context_.R8 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r9w")		return Register("r9w", context_.R9 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r10w")		return Register("r10w", context_.R10 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r11w")		return Register("r11w", context_.R11 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r12w")		return Register("r12w", context_.R12 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r13w")		return Register("r13w", context_.R13 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r14w")		return Register("r14w", context_.R14 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r15w")		return Register("r15w", context_.R15 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "al")		return Register("al", context_.Rax & 0xff, Register::TYPE_GPR);
	else if(lreg == "bl")		return Register("bl", context_.Rbx & 0xff, Register::TYPE_GPR);
	else if(lreg == "cl")		return Register("cl", context_.Rcx & 0xff, Register::TYPE_GPR);
	else if(lreg == "dl")		return Register("dl", context_.Rdx & 0xff, Register::TYPE_GPR);
	else if(lreg == "ah")		return Register("ah", (context_.Rax >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "bh")		return Register("bh", (context_.Rbx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "ch")		return Register("ch", (context_.Rcx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "dh")		return Register("dh", (context_.Rdx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "spl")		return Register("spl", (context_.Rsp >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "bpl")		return Register("bpl", (context_.Rbp >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "sil")		return Register("sil", (context_.Rsi >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "dil")		return Register("dil", (context_.Rdi >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "r8b")		return Register("r8b", context_.R8 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r9b")		return Register("r9b", context_.R9 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r10b")		return Register("r10b", context_.R10 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r11b")		return Register("r11b", context_.R11 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r12b")		return Register("r12b", context_.R12 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r13b")		return Register("r13b", context_.R13 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r14b")		return Register("r14b", context_.R14 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r15b")		return Register("r15b", context_.R15 & 0xff, Register::TYPE_GPR);
	else if(lreg == "cs")		return Register("cs", context_.SegCs, Register::TYPE_SEG, 0);
	else if(lreg == "ds")		return Register("ds", context_.SegDs, Register::TYPE_SEG, 0);
	else if(lreg == "es")		return Register("es", context_.SegEs, Register::TYPE_SEG, 0);
	else if(lreg == "fs")		return Register("fs", context_.SegFs, Register::TYPE_SEG, fs_base_);
	else if(lreg == "gs")		return Register("gs", context_.SegGs, Register::TYPE_SEG, gs_base_);
	else if(lreg == "ss") 		return Register("ss", context_.SegSs, Register::TYPE_SEG, 0);
	else if(lreg == "rflags") 	return Register("rflags", context_.EFlags, Register::TYPE_COND);
#endif

	return Register();
}

//------------------------------------------------------------------------------
// Name: frame_pointer() const
// Desc: returns what is conceptually the frame pointer for this platform
//------------------------------------------------------------------------------
edb::address_t PlatformState::frame_pointer() const {
#if defined(EDB_X86)
	return context_.Ebp;
#elif defined(EDB_X86_64)
	return context_.Rbp;
#endif
}

//------------------------------------------------------------------------------
// Name: instruction_pointer() const
// Desc: returns the instruction pointer for this platform
//------------------------------------------------------------------------------
edb::address_t PlatformState::instruction_pointer() const {
#if defined(EDB_X86)
	return context_.Eip;
#elif defined(EDB_X86_64)
	return context_.Rip;
#endif
}

//------------------------------------------------------------------------------
// Name: stack_pointer() const
// Desc: returns the stack pointer for this platform
//------------------------------------------------------------------------------
edb::address_t PlatformState::stack_pointer() const {
#if defined(EDB_X86)
	return context_.Esp;
#elif defined(EDB_X86_64)
	return context_.Rsp;
#endif
}

//------------------------------------------------------------------------------
// Name: debug_register(int n) const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t PlatformState::debug_register(int n) const {

	switch(n) {
	case 0: return context_.Dr0;
	case 1: return context_.Dr1;
	case 2: return context_.Dr2;
	case 3: return context_.Dr3;
	case 6: return context_.Dr6;
	case 7: return context_.Dr7;
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: flags() const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t PlatformState::flags() const {
#if defined(EDB_X86)
	return context_.EFlags;
#elif defined(EDB_X86_64)
	return context_.EFlags;
#endif
}

//------------------------------------------------------------------------------
// Name: fpu_register(int n) const
// Desc:
//------------------------------------------------------------------------------
long double PlatformState::fpu_register(int n) const {
double ret = 0.0;

	if(n >= 0 && n <= 7) {
#if defined(EDB_X86)
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&context_.FloatSave.RegisterArea[n*10]);
		if(sizeof(long double) == 10) { // can we check this at compile time?
			ret = *(reinterpret_cast<const long double*>(p));
		} else {
			ret = read_float80(p);
		}
#elif defined(EDB_X86_64)
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&context_.FltSave.FloatRegisters[n]);
		if(sizeof(long double) == 10) {
			ret = *(reinterpret_cast<const long double*>(p));
		} else {
			ret = read_float80(p);
		}
#endif
	}
	return ret;
}

quint64 PlatformState::mmx_register(int n) const {
	quint64 ret = 0;

	// TODO: actually check again for this again
	if(true) {
		if(n >= 0 && n <= 7) {
#if defined(EDB_X86)
			// MMX registers are an alias to the lower 64-bits of the FPU regs
			const quint64* p = reinterpret_cast<const quint64*>(&context_.FloatSave.RegisterArea[n*10]);
			ret = *p; // little endian!
#elif defined(EDB_X86_64)
			const quint64* p = reinterpret_cast<const quint64*>(&context_.FltSave.FloatRegisters[n]);
			ret = *p;
#endif
		}
	}
	return ret;
}

QByteArray PlatformState::xmm_register(int n) const {
	QByteArray ret(16, 0);

	// TODO: actually check again for this again
	if(true) {
#if defined(EDB_X86)
		if(n >= 0 && n <= 7) {
			const char* p = reinterpret_cast<const char*>(&context_.ExtendedRegisters[(10+n)*16]);
			ret = QByteArray(p, 16);
			std::reverse(ret.begin(), ret.end()); //little endian!
		}
#elif defined(EDB_X86_64)
		if(n >= 0 && n <= 15) {
			const char* p = reinterpret_cast<const char*>(&context_.FltSave.XmmRegisters[n]);
			ret = QByteArray(p, sizeof(M128A));
			std::reverse(ret.begin(), ret.end());
		}
#endif
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: adjust_stack(int bytes)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::adjust_stack(int bytes) {
#if defined(EDB_X86)
	context_.Esp += bytes;
#elif defined(EDB_X86_64)
	context_.Rsp += bytes;
#endif
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc:
//------------------------------------------------------------------------------
void PlatformState::clear() {
	memset(&context_, 0, sizeof(context_));
	fs_base_ = 0;
	gs_base_ = 0;
}

//------------------------------------------------------------------------------
// Name: set_debug_register(int n, edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_debug_register(int n, edb::reg_t value) {
	switch(n) {
	case 0: context_.Dr0 = value; break;
	case 1: context_.Dr1 = value; break;
	case 2: context_.Dr2 = value; break;
	case 3: context_.Dr3 = value; break;
	case 6: context_.Dr6 = value; break;
	case 7: context_.Dr7 = value; break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
// Name: set_flags(edb::reg_t flags)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_flags(edb::reg_t flags) {
#if defined(EDB_X86)
	context_.EFlags = flags;
#elif defined(EDB_X86_64)
	context_.EFlags = flags;
#endif
}

//------------------------------------------------------------------------------
// Name: set_instruction_pointer(edb::address_t value)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_instruction_pointer(edb::address_t value) {
#if defined(EDB_X86)
	context_.Eip = value;
#elif defined(EDB_X86_64)
	context_.Rip = value;
#endif
}

//------------------------------------------------------------------------------
// Name: set_register(const QString &name, edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_register(const QString &name, edb::reg_t value) {

	const QString lreg = name.toLower();
#if defined(EDB_X86)
	if(lreg == "eax") { context_.Eax = value; }
	else if(lreg == "ebx") { context_.Ebx = value; }
	else if(lreg == "ecx") { context_.Ecx = value; }
	else if(lreg == "edx") { context_.Edx = value; }
	else if(lreg == "ebp") { context_.Ebp = value; }
	else if(lreg == "esp") { context_.Esp = value; }
	else if(lreg == "esi") { context_.Esi = value; }
	else if(lreg == "edi") { context_.Edi = value; }
	else if(lreg == "eip") { context_.Eip = value; }
	else if(lreg == "cs") { context_.SegCs = value; }
	else if(lreg == "ds") { context_.SegDs = value; }
	else if(lreg == "es") { context_.SegEs = value; }
	else if(lreg == "fs") { context_.SegFs = value; }
	else if(lreg == "gs") { context_.SegGs = value; }
	else if(lreg == "ss") { context_.SegSs = value; }
	else if(lreg == "eflags") { context_.EFlags = value; }
#elif defined(EDB_X86_64)
	if(lreg == "rax") { context_.Rax = value; }
	else if(lreg == "rbx") { context_.Rbx = value; }
	else if(lreg == "rcx") { context_.Rcx = value; }
	else if(lreg == "rdx") { context_.Rdx = value; }
	else if(lreg == "rbp") { context_.Rbp = value; }
	else if(lreg == "rsp") { context_.Rsp = value; }
	else if(lreg == "rsi") { context_.Rsi = value; }
	else if(lreg == "rdi") { context_.Rdi = value; }
	else if(lreg == "r8") { context_.R8 = value; }
	else if(lreg == "r9") { context_.R9 = value; }
	else if(lreg == "r10") { context_.R10 = value; }
	else if(lreg == "r11") { context_.R11 = value; }
	else if(lreg == "r12") { context_.R12 = value; }
	else if(lreg == "r13") { context_.R13 = value; }
	else if(lreg == "r14") { context_.R14 = value; }
	else if(lreg == "r15") { context_.R15 = value; }
	else if(lreg == "rip") { context_.Rip = value; }
	else if(lreg == "cs") { context_.SegCs = value; }
	else if(lreg == "ds") { context_.SegDs = value; }
	else if(lreg == "es") { context_.SegEs = value; }
	else if(lreg == "fs") { context_.SegFs = value; }
	else if(lreg == "gs") { context_.SegGs = value; }
	else if(lreg == "ss") { context_.SegSs = value; }
	else if(lreg == "rflags") { context_.EFlags = value; }
#endif
}
