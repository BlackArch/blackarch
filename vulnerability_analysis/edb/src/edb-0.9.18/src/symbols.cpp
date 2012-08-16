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

#include "symbols.h"
#include "Debugger.h"
#include "Types.h"

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QFileInfo>
#include <QSet>
#include <iostream>

#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
#ifdef Q_OS_OPENBSD
#include <sys/exec_elf.h>
#else
#include <elf.h>
#endif
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace {

#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
	struct elf32_model {
		typedef quint32    address_t;

		typedef Elf32_Ehdr elf_header_t;
		typedef Elf32_Shdr elf_section_header_t;
		typedef Elf32_Sym  elf_symbol_t;
		typedef Elf32_Rela elf_relocation_a_t;
		typedef Elf32_Rel  elf_relocation_t;

		static const int plt_entry_size = 0x10;

		static quint32 elf_r_sym(quint32 x)               { return ELF32_R_SYM(x); }
		static quint32 elf_r_type(quint32 x)              { return ELF32_R_TYPE(x); }
		static unsigned char elf_st_type(unsigned char x) { return ELF32_ST_TYPE(x); }
		static unsigned char elf_st_bind(unsigned char x) { return ELF32_ST_BIND(x); }

		struct symbol {
			address_t address;
			size_t    size;
			QString   name;
			char      type;

			bool operator<(const symbol &rhs) const {
				return address < rhs.address || (address == rhs.address && name < rhs.name);
			}

			bool operator==(const symbol &rhs) const {
				return address == rhs.address && size == rhs.size && name == rhs.name && type == rhs.type;
			}

			QString to_string() const {
				char buf[256];
				qsnprintf(buf, sizeof(buf), "%08x %08x %c %s",
					address,
					size,
					type,
					qPrintable(name));
				return buf;
			}
		};
	};

	struct elf64_model {

		typedef quint64    address_t;

		typedef Elf64_Ehdr elf_header_t;
		typedef Elf64_Shdr elf_section_header_t;
		typedef Elf64_Sym  elf_symbol_t;
		typedef Elf64_Rela elf_relocation_a_t;
		typedef Elf64_Rel  elf_relocation_t;

		static const int plt_entry_size = 0x10;

		static quint64 elf_r_sym(quint64 x)               { return ELF64_R_SYM(x); }
		static quint64 elf_r_type(quint64 x)              { return ELF64_R_TYPE(x); }
		static unsigned char elf_st_type(unsigned char x) { return ELF64_ST_TYPE(x); }
		static unsigned char elf_st_bind(unsigned char x) { return ELF64_ST_BIND(x); }

		struct symbol {
			address_t address;
			size_t    size;
			QString   name;
			char      type;

			bool operator<(const symbol &rhs) const {
				return address < rhs.address || (address == rhs.address && name < rhs.name);
			}

			bool operator==(const symbol &rhs) const {
				return address == rhs.address && size == rhs.size && name == rhs.name && type == rhs.type;
			}

			QString to_string() const {
				char buf[256];
				qsnprintf(buf, sizeof(buf), "%016llx %08x %c %s",
					address,
					size,
					type,
					qPrintable(name));
				return buf;
			}
		};
	};


	bool is_elf32(const void *ptr) {
		const Elf32_Ehdr *const elf32_hdr = reinterpret_cast<const Elf32_Ehdr *>(ptr);
		if(std::memcmp(elf32_hdr->e_ident, ELFMAG, SELFMAG) == 0) {
			return elf32_hdr->e_ident[EI_CLASS] == ELFCLASS32;
		}
		return false;
	}

	bool is_elf64(const void *ptr) {
		const Elf64_Ehdr *const elf64_hdr = reinterpret_cast<const Elf64_Ehdr *>(ptr);
		if(std::memcmp(elf64_hdr->e_ident, ELFMAG, SELFMAG) == 0) {
			return elf64_hdr->e_ident[EI_CLASS] == ELFCLASS64;
		}
		return false;
	}

	/*
	 The  symbol  type.   At least the following types are used; others are, as well, depending on the object file format.  If lowercase,
	 the symbol is local; if uppercase, the symbol is global (external).

	 "A" The symbol's value is absolute, and will not be changed by further linking.

	 "B"
	 "b" The symbol is in the uninitialized data section (known as BSS).

	 "C" The symbol is common.  Common symbols are uninitialized data.  When linking, multiple common symbols may appear  with  the  same
	     name.  If the symbol is defined anywhere, the common symbols are treated as undefined references.

	 "D"
	 "d" The symbol is in the initialized data section.

	 "G"
	 "g" The  symbol is in an initialized data section for small objects.  Some object file formats permit more efficient access to small
	     data objects, such as a global int variable as opposed to a large global array.

	 "N" The symbol is a debugging symbol.

	 "p" The symbols is in a stack unwind section.

	 "R"
	 "r" The symbol is in a read only data section.

	 "S"
	 "s" The symbol is in an uninitialized data section for small objects.

	 "T"
	 "t" The symbol is in the text (code) section.

	 "U" The symbol is undefined.

	 "u" The  symbol  is  a unique global symbol.  This is a GNU extension to the standard set of ELF symbol bindings.  For such a symbol
	     the dynamic linker will make sure that in the entire process_symbols there is just one symbol with this name and type in use.

	 "V"
	 "v" The symbol is a weak object.  When a weak defined symbol is linked with a normal defined symbol, the normal  defined  symbol  is
	     used  with no error.  When a weak undefined symbol is linked and the symbol is not defined, the value of the weak symbol becomes
	     zero with no error.  On some systems, uppercase indicates that a default value has been specified.

	 "W"
	 "w" The symbol is a weak symbol that has not been specifically tagged as a weak object symbol.  When a weak defined symbol is linked
	     with  a  normal defined symbol, the normal defined symbol is used with no error.  When a weak undefined symbol is linked and the
	     symbol is not defined, the value of the symbol is determined in a system-specific manner without error.  On some systems, upper-
	     case indicates that a default value has been specified.

	 "-" The  symbol  is  a  stabs  symbol in an a.out object file.  In this case, the next values printed are the stabs other field, the
	     stabs desc field, and the stab type.  Stabs symbols are used to hold debugging information.

	 "?" The symbol type is unknown, or object file format specific.
	*/
	
	
	template <class M>
	QList<typename M::symbol> collect_symbols(const void *p, size_t size) {
		Q_UNUSED(size);

		typedef typename M::address_t            address_t;
		typedef typename M::elf_header_t         elf_header_t;
		typedef typename M::elf_section_header_t elf_section_header_t;
		typedef typename M::elf_symbol_t         elf_symbol_t;
		typedef typename M::elf_relocation_a_t   elf_relocation_a_t;
		typedef typename M::elf_relocation_t     elf_relocation_t;
		typedef typename M::symbol               symbol;

		const uintptr_t base = reinterpret_cast<uintptr_t>(p);

		const elf_header_t *const header                 = static_cast<const elf_header_t*>(p);
		const elf_section_header_t *const sections_begin = reinterpret_cast<elf_section_header_t*>(base + header->e_shoff);
		const elf_section_header_t *const sections_end   = sections_begin + header->e_shnum;
		const char *section_strings                    = reinterpret_cast<const char*>(base + sections_begin[header->e_shstrndx].sh_offset);


		address_t plt_address = 0;
		address_t got_address = 0;
		QSet<address_t> plt_addresses;
		QList<symbol>   symbols;

		// collect special section addresses
		for(const elf_section_header_t *section = sections_begin; section != sections_end; ++section) {
			if(strcmp(&section_strings[section->sh_name], ".plt") == 0) {
				plt_address = section->sh_addr;
			} else if(strcmp(&section_strings[section->sh_name], ".got") == 0) {
				got_address = section->sh_addr;
			}
		}

		// print out relocated symbols for special sections
		for(const elf_section_header_t *section = sections_begin; section != sections_end; ++section) {
			address_t base_address = 0;
			if(strcmp(&section_strings[section->sh_name], ".rela.plt") == 0) {
				base_address = plt_address;
			} else if(strcmp(&section_strings[section->sh_name], ".rel.plt") == 0) {
				base_address = plt_address;
			} else if(strcmp(&section_strings[section->sh_name], ".rela.got") == 0) {
				base_address = got_address;
			} else if(strcmp(&section_strings[section->sh_name], ".rel.got") == 0) {
				base_address = got_address;
			} else {
				continue;
			}

			switch(section->sh_type) {
			case SHT_RELA:
				{
					int n = 0;
					const elf_relocation_a_t *relocation = reinterpret_cast<elf_relocation_a_t*>(base + section->sh_offset);

					for(size_t i = 0; i < section->sh_size / section->sh_entsize; ++i) {

						const int sym_index              = M::elf_r_sym(relocation[i].r_info);
						const elf_section_header_t *linked = &sections_begin[section->sh_link];
						const elf_symbol_t *symbol_tab     = reinterpret_cast<elf_symbol_t*>(base + linked->sh_offset);
						const char *string_tab           = reinterpret_cast<const char*>(base + sections_begin[linked->sh_link].sh_offset);

						const address_t symbol_address = base_address + ++n * M::plt_entry_size;

						const char *sym_name = &section_strings[section->sh_name];
						if(strlen(sym_name) > (sizeof(".rela.") - 1) && memcmp(sym_name, ".rela.", (sizeof(".rela.") - 1)) == 0) {
							sym_name += 6;
						}

						plt_addresses.insert(symbol_address);

						symbol sym;
						sym.address = symbol_address;
						sym.size    = (symbol_tab[sym_index].st_size ? symbol_tab[sym_index].st_size : 0x10);
						sym.name    = &string_tab[symbol_tab[sym_index].st_name];
						sym.name    += "@";
						sym.name    += sym_name;
						sym.type    = 'P';
						symbols.push_back(sym);
					}

				}
				break;
			case SHT_REL:
				{
					int n = 0;
					const elf_relocation_t *relocation = reinterpret_cast<elf_relocation_t*>(base + section->sh_offset);

					for(size_t i = 0; i < section->sh_size / section->sh_entsize; ++i) {

						const int sym_index              = M::elf_r_sym(relocation[i].r_info);
						const elf_section_header_t *linked = &sections_begin[section->sh_link];
						const elf_symbol_t *symbol_tab     = reinterpret_cast<elf_symbol_t*>(base + linked->sh_offset);
						const char *string_tab           = reinterpret_cast<const char*>(base + sections_begin[linked->sh_link].sh_offset);

						const address_t symbol_address = base_address + ++n * M::plt_entry_size;

						const char *sym_name = &section_strings[section->sh_name];
						if(strlen(sym_name) > (sizeof(".rel.") - 1) && memcmp(sym_name, ".rel.", (sizeof(".rel.") - 1)) == 0) {
							sym_name += 5;
						}

						plt_addresses.insert(symbol_address);

						symbol sym;
						sym.address = symbol_address;
						sym.size    = (symbol_tab[sym_index].st_size ? symbol_tab[sym_index].st_size : 0x10);
						sym.name    = &string_tab[symbol_tab[sym_index].st_name];
						sym.name    += "@";
						sym.name    += sym_name;
						sym.type    = 'P';
						symbols.push_back(sym);
					}

				}
				break;
			}
		}

		// collect regular symbols
		for(const elf_section_header_t *section = sections_begin; section != sections_end; ++section) {

			switch(section->sh_type) {
			case SHT_SYMTAB:
			case SHT_DYNSYM:
				{
					elf_symbol_t *symbol_tab = reinterpret_cast<elf_symbol_t*>(base + section->sh_offset);
					const char *string_tab = reinterpret_cast<const char*>(base + sections_begin[section->sh_link].sh_offset);

					for(size_t i = 0; i < section->sh_size / section->sh_entsize; ++i) {

						const elf_section_header_t *related_section = 0;

						if(symbol_tab[i].st_shndx != SHN_UNDEF && symbol_tab[i].st_shndx < SHN_LORESERVE) {
							related_section = &sections_begin[symbol_tab[i].st_shndx];
						}

						Q_UNUSED(related_section);

						if(plt_addresses.find(symbol_tab[i].st_value) == plt_addresses.end()) {

							if(symbol_tab[i].st_value && strlen(&string_tab[symbol_tab[i].st_name]) > 0) {

								symbol sym;
								sym.address = symbol_tab[i].st_value;
								sym.size    = symbol_tab[i].st_size;
								sym.name    = &string_tab[symbol_tab[i].st_name];
								sym.type    = (M::elf_st_type(symbol_tab[i].st_info) == STT_FUNC ? 'T' : 'D');
								symbols.push_back(sym);
							}
						}
					}
				}
				break;
			}
		}

		// collect unnamed symbols
		for(const elf_section_header_t *section = sections_begin; section != sections_end; ++section) {

			switch(section->sh_type) {
			case SHT_SYMTAB:
			case SHT_DYNSYM:
				{
					elf_symbol_t *symbol_tab = reinterpret_cast<elf_symbol_t*>(base + section->sh_offset);
					const char *string_tab = reinterpret_cast<const char*>(base + sections_begin[section->sh_link].sh_offset);

					for(size_t i = 0; i < section->sh_size / section->sh_entsize; ++i) {

						const elf_section_header_t *related_section = 0;

						if(symbol_tab[i].st_shndx != SHN_UNDEF && symbol_tab[i].st_shndx < SHN_LORESERVE) {
							related_section = &sections_begin[symbol_tab[i].st_shndx];
						}
						
						Q_UNUSED(related_section);

						if(plt_addresses.find(symbol_tab[i].st_value) == plt_addresses.end()) {

							if(symbol_tab[i].st_value && strlen(&string_tab[symbol_tab[i].st_name]) == 0) {
								symbol sym;
								sym.address = symbol_tab[i].st_value;
								sym.size    = symbol_tab[i].st_size;
								sym.name    = QString("$sym_%1").arg(edb::v1::format_pointer(symbol_tab[i].st_value));
								sym.type    = (M::elf_st_type(symbol_tab[i].st_info) == STT_FUNC ? 'T' : 'D');
								symbols.push_back(sym);
							}
						}
					}
				}
				break;
			}
		}
		
		return symbols;
	}

	template <class M>
	void process_symbols(const void *p, size_t size) {
	
		typedef typename M::symbol symbol;
	
		QList<symbol> symbols = collect_symbols<M>(p, size);

		qSort(symbols.begin(), symbols.end());
		typename QList<symbol>::const_iterator new_end = std::unique(symbols.begin(), symbols.end());
		for(typename QList<symbol>::const_iterator it = symbols.begin(); it != new_end; ++it) {
			std::cout << qPrintable(it->to_string()) << '\n';
		}
	}
#endif
}

//--------------------------------------------------------------------------
// Name: generate_symbols(const QString &filename)
// Desc:
//--------------------------------------------------------------------------
void symbols::generate_symbols(const QString &filename) {
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly)) {
		std::cout << qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")) << '\n';

		const QByteArray md5 = edb::v1::get_file_md5(filename);
		std::cout << md5.toHex().data() << ' ' << qPrintable(QFileInfo(filename).absoluteFilePath()) << '\n';

		if(const void *const file_ptr = reinterpret_cast<void *>(file.map(0, file.size(), QFile::NoOptions))) {
			if(is_elf64(file_ptr)) {
				process_symbols<elf64_model>(file_ptr, file.size());
			} else if(is_elf32(file_ptr)) {
				process_symbols<elf32_model>(file_ptr, file.size());
			}
		}
	}
#else
	Q_UNUSED(filename);
	std::cerr << "generating symbols is not yet supported on non-unix targets" << std::endl;
#endif
}
