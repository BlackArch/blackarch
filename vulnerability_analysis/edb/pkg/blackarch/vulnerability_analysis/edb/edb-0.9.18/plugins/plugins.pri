
EDB_ROOT = ../..

TEMPLATE = lib
CONFIG   += plugin
#CONFIG  += silent
DESTDIR  = $$EDB_ROOT
INSTALLS += target

DEFINES += EDB_PLUGIN

# ignore missing symbols, they'll be found when linked into edb
linux-g++*:QMAKE_LFLAGS -= $$QMAKE_LFLAGS_NOUNDEF
linux-g++*:QMAKE_LFLAGS -= "-Wl,--no-undefined"
macx-g++*:QMAKE_LFLAGS += "-undefined dynamic_lookup"

win32-g++*:QMAKE_LFLAGS += ""

unix {

	# generic unix include paths
	DEPENDPATH  += $$EDB_ROOT/include $$EDB_ROOT/include/os/unix
	INCLUDEPATH += $$EDB_ROOT/include $$EDB_ROOT/include/os/unix
	
	# OS include paths
	linux-* {
		DEPENDPATH  += $$EDB_ROOT/include/os/unix/linux
		INCLUDEPATH += $$EDB_ROOT/include/os/unix/linux
	}

	openbsd-* {
		DEPENDPATH  += $$EDB_ROOT/include/os/unix/openbsd
		INCLUDEPATH += $$EDB_ROOT/include/os/unix/openbsd /usr/local/include
	}

	freebsd-* {
		DEPENDPATH  += $$EDB_ROOT/include/os/unix/freebsd
		INCLUDEPATH += $$EDB_ROOT/include/os/unix/freebsd
	}

	macx-* {
		DEPENDPATH  += $$EDB_ROOT/include/os/unix/osx
		INCLUDEPATH += $$EDB_ROOT/include/os/unix/osx /opt/local/include
	}


	# arch includes paths
	macx {
		INCLUDEPATH += $$EDB_ROOT/include/arch/x86_64
		DEPENDPATH  += $$EDB_ROOT/include/arch/x86_64
		include(plugins-x86_64.pri)
	}

	!macx {
		INCLUDEPATH += $$EDB_ROOT/include/arch/$$QT_ARCH
		DEPENDPATH  += $$EDB_ROOT/include/arch/$$QT_ARCH
		include(plugins-$${QT_ARCH}.pri)
	}

    debug:OBJECTS_DIR   = $${OUT_PWD}/.obj/debug-shared
    release:OBJECTS_DIR = $${OUT_PWD}/.obj/release-shared

    debug:MOC_DIR   = $${OUT_PWD}/.moc/debug-shared
    release:MOC_DIR = $${OUT_PWD}/.moc/release-shared

    RCC_DIR = $${OUT_PWD}/.rcc
    UI_DIR  = $${OUT_PWD}/.uic
}

win32-msvc*:LIBS += $$EDB_ROOT/edb.lib
win64-msvc*:LIBS += $$EDB_ROOT/edb.lib

win32 {
	INCLUDEPATH += $$EDB_ROOT/include/os/win32 $$EDB_ROOT/include $$EDB_ROOT/include/arch/i386 "C:\Program Files\boost\boost_1_47"
	DEPENDPATH  += $$EDB_ROOT/include/os/win32 $$EDB_ROOT/include $$EDB_ROOT/include/arch/i386
}

win64 {
	INCLUDEPATH += $$EDB_ROOT/include/os/win64 $$EDB_ROOT/include $$EDB_ROOT/include/arch/x86_64 $$(BOOST)
	DEPENDPATH  += $$EDB_ROOT/include/os/win64 $$EDB_ROOT/include $$EDB_ROOT/include/arch/x86_64
}
