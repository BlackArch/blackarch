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

//------------------------------------------------------------------------------
// Name: PlatformState()
// Desc:
//------------------------------------------------------------------------------
PlatformState::PlatformState() {
	memset(&regs_, 0, sizeof(regs_));
	memset(&fpregs_, 0, sizeof(fpregs_));
	memset(&dr_, 0, sizeof(dr_));
#if defined(EDB_X86)
	fs_base = 0;
	gs_base = 0;
#endif
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
	if(lreg == "eax")			return Register("eax", regs_.eax, Register::TYPE_GPR);
	else if(lreg == "ebx")		return Register("ebx", regs_.ebx, Register::TYPE_GPR);
	else if(lreg == "ecx")		return Register("ecx", regs_.ecx, Register::TYPE_GPR);
	else if(lreg == "edx")		return Register("edx", regs_.edx, Register::TYPE_GPR);
	else if(lreg == "ebp")		return Register("ebp", regs_.ebp, Register::TYPE_GPR);
	else if(lreg == "esp")		return Register("esp", regs_.esp, Register::TYPE_GPR);
	else if(lreg == "esi")		return Register("esi", regs_.esi, Register::TYPE_GPR);
	else if(lreg == "edi")		return Register("edi", regs_.edi, Register::TYPE_GPR);
	else if(lreg == "eip")		return Register("eip", regs_.eip, Register::TYPE_IP);
	else if(lreg == "ax")		return Register("ax", regs_.eax & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bx")		return Register("bx", regs_.ebx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "cx")		return Register("cx", regs_.ecx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "dx")		return Register("dx", regs_.edx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bp")		return Register("bp", regs_.ebp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "sp")		return Register("sp", regs_.esp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "si")		return Register("si", regs_.esi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "di")		return Register("di", regs_.edi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "al")		return Register("al", regs_.eax & 0xff, Register::TYPE_GPR);
	else if(lreg == "bl")		return Register("bl", regs_.ebx & 0xff, Register::TYPE_GPR);
	else if(lreg == "cl")		return Register("cl", regs_.ecx & 0xff, Register::TYPE_GPR);
	else if(lreg == "dl")		return Register("dl", regs_.edx & 0xff, Register::TYPE_GPR);
	else if(lreg == "ah")		return Register("ah", (regs_.eax >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "bh")		return Register("bh", (regs_.ebx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "ch")		return Register("ch", (regs_.ecx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "dh")		return Register("dh", (regs_.edx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "cs")		return Register("cs", regs_.xcs, Register::TYPE_SEG, 0);
	else if(lreg == "ds")		return Register("ds", regs_.xds, Register::TYPE_SEG, 0);
	else if(lreg == "es")		return Register("es", regs_.xes, Register::TYPE_SEG, 0);
	else if(lreg == "fs")		return Register("fs", regs_.xfs, Register::TYPE_SEG, fs_base);
	else if(lreg == "gs")		return Register("gs", regs_.xgs, Register::TYPE_SEG, gs_base);
	else if(lreg == "ss") 		return Register("ss", regs_.xss, Register::TYPE_SEG, 0);
	else if(lreg == "eflags") 	return Register("eflags", regs_.eflags, Register::TYPE_COND);
#elif defined(EDB_X86_64)
	if(lreg == "rax")			return Register("rax", regs_.rax, Register::TYPE_GPR);
	else if(lreg == "rbx")		return Register("rbx", regs_.rbx, Register::TYPE_GPR);
	else if(lreg == "rcx")		return Register("rcx", regs_.rcx, Register::TYPE_GPR);
	else if(lreg == "rdx")		return Register("rdx", regs_.rdx, Register::TYPE_GPR);
	else if(lreg == "rbp")		return Register("rbp", regs_.rbp, Register::TYPE_GPR);
	else if(lreg == "rsp")		return Register("rsp", regs_.rsp, Register::TYPE_GPR);
	else if(lreg == "rsi")		return Register("rsi", regs_.rsi, Register::TYPE_GPR);
	else if(lreg == "rdi")		return Register("rdi", regs_.rdi, Register::TYPE_GPR);
	else if(lreg == "rip")		return Register("rip", regs_.rip, Register::TYPE_IP);
	else if(lreg == "r8")		return Register("r8", regs_.r8, Register::TYPE_GPR);
	else if(lreg == "r9")		return Register("r9", regs_.r9, Register::TYPE_GPR);
	else if(lreg == "r10")		return Register("r10", regs_.r10, Register::TYPE_GPR);
	else if(lreg == "r11")		return Register("r11", regs_.r11, Register::TYPE_GPR);
	else if(lreg == "r12")		return Register("r12", regs_.r12, Register::TYPE_GPR);
	else if(lreg == "r13")		return Register("r13", regs_.r13, Register::TYPE_GPR);
	else if(lreg == "r14")		return Register("r14", regs_.r14, Register::TYPE_GPR);
	else if(lreg == "r15")		return Register("r15", regs_.r15, Register::TYPE_GPR);
	else if(lreg == "eax")		return Register("eax", regs_.rax & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ebx")		return Register("ebx", regs_.rbx & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ecx")		return Register("ecx", regs_.rcx & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "edx")		return Register("edx", regs_.rdx & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ebp")		return Register("ebp", regs_.rbp & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "esp")		return Register("esp", regs_.rsp & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "esi")		return Register("esi", regs_.rsi & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "edi")		return Register("edi", regs_.rdi & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r8d")		return Register("r8d", regs_.r8 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r9d")		return Register("r9d", regs_.r9 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r10d") 	return Register("r10d", regs_.r10 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r11d") 	return Register("r11d", regs_.r11 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r12d") 	return Register("r12d", regs_.r12 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r13d") 	return Register("r13d", regs_.r13 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r14d") 	return Register("r14d", regs_.r14 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "r15d")		return Register("r15d", regs_.r15 & 0xffffffff, Register::TYPE_GPR);
	else if(lreg == "ax")		return Register("ax", regs_.rax & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bx")		return Register("bx", regs_.rbx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "cx")		return Register("cx", regs_.rcx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "dx")		return Register("dx", regs_.rdx & 0xffff, Register::TYPE_GPR);
	else if(lreg == "bp")		return Register("bp", regs_.rbp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "sp")		return Register("sp", regs_.rsp & 0xffff, Register::TYPE_GPR);
	else if(lreg == "si")		return Register("si", regs_.rsi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "di")		return Register("di", regs_.rdi & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r8w")		return Register("r8w", regs_.r8 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r9w")		return Register("r9w", regs_.r9 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r10w")		return Register("r10w", regs_.r10 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r11w")		return Register("r11w", regs_.r11 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r12w")		return Register("r12w", regs_.r12 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r13w")		return Register("r13w", regs_.r13 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r14w")		return Register("r14w", regs_.r14 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "r15w")		return Register("r15w", regs_.r15 & 0xffff, Register::TYPE_GPR);
	else if(lreg == "al")		return Register("al", regs_.rax & 0xff, Register::TYPE_GPR);
	else if(lreg == "bl")		return Register("bl", regs_.rbx & 0xff, Register::TYPE_GPR);
	else if(lreg == "cl")		return Register("cl", regs_.rcx & 0xff, Register::TYPE_GPR);
	else if(lreg == "dl")		return Register("dl", regs_.rdx & 0xff, Register::TYPE_GPR);
	else if(lreg == "ah")		return Register("ah", (regs_.rax >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "bh")		return Register("bh", (regs_.rbx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "ch")		return Register("ch", (regs_.rcx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "dh")		return Register("dh", (regs_.rdx >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "spl")		return Register("spl", (regs_.rsp >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "bpl")		return Register("bpl", (regs_.rbp >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "sil")		return Register("sil", (regs_.rsi >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "dil")		return Register("dil", (regs_.rdi >> 8) & 0xff, Register::TYPE_GPR);
	else if(lreg == "r8b")		return Register("r8b", regs_.r8 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r9b")		return Register("r9b", regs_.r9 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r10b")		return Register("r10b", regs_.r10 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r11b")		return Register("r11b", regs_.r11 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r12b")		return Register("r12b", regs_.r12 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r13b")		return Register("r13b", regs_.r13 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r14b")		return Register("r14b", regs_.r14 & 0xff, Register::TYPE_GPR);
	else if(lreg == "r15b")		return Register("r15b", regs_.r15 & 0xff, Register::TYPE_GPR);
	else if(lreg == "cs")		return Register("cs", regs_.cs, Register::TYPE_SEG, 0);
	else if(lreg == "ds")		return Register("ds", regs_.ds, Register::TYPE_SEG, 0);
	else if(lreg == "es")		return Register("es", regs_.es, Register::TYPE_SEG, 0);
	else if(lreg == "fs")		return Register("fs", regs_.fs, Register::TYPE_SEG, regs_.fs_base);
	else if(lreg == "gs")		return Register("gs", regs_.gs, Register::TYPE_SEG, regs_.gs_base);
	else if(lreg == "ss") 		return Register("ss", regs_.ss, Register::TYPE_SEG, 0);
	else if(lreg == "rflags") 	return Register("rflags", regs_.eflags, Register::TYPE_COND);
#endif

	return Register();
}

//------------------------------------------------------------------------------
// Name: frame_pointer() const
// Desc: returns what is conceptually the frame pointer for this platform
//------------------------------------------------------------------------------
edb::address_t PlatformState::frame_pointer() const {
#if defined(EDB_X86)
	return regs_.ebp;
#elif defined(EDB_X86_64)
	return regs_.rbp;
#endif
}

//------------------------------------------------------------------------------
// Name: instruction_pointer() const
// Desc: returns the instruction pointer for this platform
//------------------------------------------------------------------------------
edb::address_t PlatformState::instruction_pointer() const {
#if defined(EDB_X86)
	return regs_.eip;
#elif defined(EDB_X86_64)
	return regs_.rip;
#endif
}

//------------------------------------------------------------------------------
// Name: stack_pointer() const
// Desc: returns the stack pointer for this platform
//------------------------------------------------------------------------------
edb::address_t PlatformState::stack_pointer() const {
#if defined(EDB_X86)
	return regs_.esp;
#elif defined(EDB_X86_64)
	return regs_.rsp;
#endif
}

//------------------------------------------------------------------------------
// Name: debug_register(int n) const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t PlatformState::debug_register(int n) const {
	return dr_[n];
}

//------------------------------------------------------------------------------
// Name: flags() const
// Desc:
//------------------------------------------------------------------------------
edb::reg_t PlatformState::flags() const {
#if defined(EDB_X86)
	return regs_.eflags;
#elif defined(EDB_X86_64)
	return regs_.eflags;
#endif
}

//------------------------------------------------------------------------------
// Name: fpu_register(int n) const
// Desc:
//------------------------------------------------------------------------------
long double PlatformState::fpu_register(int n) const {

	if(sizeof(long double) == 16) {
		// st_space is an array of 128 bytes, 16 bytes for each of 8 FPU registers
		const long double *const p = reinterpret_cast<const long double *>(fpregs_.st_space);
		return p[n];
	}
	return 0.0;
}

//------------------------------------------------------------------------------
// Name: adjust_stack(int bytes)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::adjust_stack(int bytes) {
#if defined(EDB_X86)
	regs_.esp += bytes;
#elif defined(EDB_X86_64)
	regs_.rsp += bytes;
#endif
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc:
//------------------------------------------------------------------------------
void PlatformState::clear() {
	memset(&regs_, 0, sizeof(regs_));
	memset(&fpregs_, 0, sizeof(fpregs_));
	memset(&dr_, 0, sizeof(dr_));
#if defined(EDB_X86)
	fs_base = 0;
	gs_base = 0;
#endif
}

//------------------------------------------------------------------------------
// Name: set_debug_register(int n, edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_debug_register(int n, edb::reg_t value) {
	dr_[n] = value;
}

//------------------------------------------------------------------------------
// Name: set_flags(edb::reg_t flags)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_flags(edb::reg_t flags) {
#if defined(EDB_X86)
	regs_.eflags = flags;
#elif defined(EDB_X86_64)
	regs_.eflags = flags;
#endif
}

//------------------------------------------------------------------------------
// Name: set_instruction_pointer(edb::address_t value)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_instruction_pointer(edb::address_t value) {
#if defined(EDB_X86)
	regs_.eip = value;
	regs_.orig_eax = -1;
#elif defined(EDB_X86_64)
	regs_.rip = value;
	regs_.orig_rax = -1;
#endif
}

//------------------------------------------------------------------------------
// Name: set_register(const QString &name, edb::reg_t value)
// Desc:
//------------------------------------------------------------------------------
void PlatformState::set_register(const QString &name, edb::reg_t value) {

	const QString lreg = name.toLower();
#if defined(EDB_X86)
	if(lreg == "eax") { regs_.eax = value; }
	else if(lreg == "ebx") { regs_.ebx = value; }
	else if(lreg == "ecx") { regs_.ecx = value; }
	else if(lreg == "edx") { regs_.edx = value; }
	else if(lreg == "ebp") { regs_.ebp = value; }
	else if(lreg == "esp") { regs_.esp = value; }
	else if(lreg == "esi") { regs_.esi = value; }
	else if(lreg == "edi") { regs_.edi = value; }
	else if(lreg == "eip") { regs_.eip = value; regs_.orig_eax = -1; }
	else if(lreg == "cs") { regs_.xcs = value; }
	else if(lreg == "ds") { regs_.xds = value; }
	else if(lreg == "es") { regs_.xes = value; }
	else if(lreg == "fs") { regs_.xfs = value; }
	else if(lreg == "gs") { regs_.xgs = value; }
	else if(lreg == "ss") { regs_.xss = value; }
	else if(lreg == "eflags") { regs_.eflags = value; }
#elif defined(EDB_X86_64)
	if(lreg == "rax") { regs_.rax = value; }
	else if(lreg == "rbx") { regs_.rbx = value; }
	else if(lreg == "rcx") { regs_.rcx = value; }
	else if(lreg == "rdx") { regs_.rdx = value; }
	else if(lreg == "rbp") { regs_.rbp = value; }
	else if(lreg == "rsp") { regs_.rsp = value; }
	else if(lreg == "rsi") { regs_.rsi = value; }
	else if(lreg == "rdi") { regs_.rdi = value; }
	else if(lreg == "r8") { regs_.r8 = value; }
	else if(lreg == "r9") { regs_.r9 = value; }
	else if(lreg == "r10") { regs_.r10 = value; }
	else if(lreg == "r11") { regs_.r11 = value; }
	else if(lreg == "r12") { regs_.r12 = value; }
	else if(lreg == "r13") { regs_.r13 = value; }
	else if(lreg == "r14") { regs_.r14 = value; }
	else if(lreg == "r15") { regs_.r15 = value; }
	else if(lreg == "rip") { regs_.rip = value; regs_.orig_rax = -1; }
	else if(lreg == "cs") { regs_.cs = value; }
	else if(lreg == "ds") { regs_.ds = value; }
	else if(lreg == "es") { regs_.es = value; }
	else if(lreg == "fs") { regs_.fs = value; }
	else if(lreg == "gs") { regs_.gs = value; }
	else if(lreg == "ss") { regs_.ss = value; }
	else if(lreg == "rflags") { regs_.eflags = value; }
#endif
}

//------------------------------------------------------------------------------
// Name: 
// Desc:
//------------------------------------------------------------------------------
quint64 PlatformState::mmx_register(int n) const {
	Q_UNUSED(n);
	return 0;
}

//------------------------------------------------------------------------------
// Name: 
// Desc:
//------------------------------------------------------------------------------
QByteArray PlatformState::xmm_register(int n) const {
	Q_UNUSED(n);
	return QByteArray();
}
