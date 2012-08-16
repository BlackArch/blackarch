
include(../plugins.pri)

unix {
	DEPENDPATH  += unix
	INCLUDEPATH += unix
	
	SOURCES += DebuggerCoreUNIX.cpp
	HEADERS += DebuggerCoreUNIX.h

	linux-* {
		DEPENDPATH  += unix/linux
		INCLUDEPATH += unix/linux
	}

	openbsd-* {
		DEPENDPATH  += unix/openbsd
		INCLUDEPATH += unix/openbsd
	}

	freebsd-*{
		DEPENDPATH  += unix/freebsd
		INCLUDEPATH += unix/freebsd
	}

	macx {
		DEPENDPATH  += unix/osx
		INCLUDEPATH += unix/osx
	}
}

win32 {
	DEPENDPATH  += win32 .
	INCLUDEPATH += win32 .
}

HEADERS += PlatformState.h   DebuggerCoreBase.h   DebuggerCore.h   X86Breakpoint.h
SOURCES += PlatformState.cpp DebuggerCoreBase.cpp DebuggerCore.cpp X86Breakpoint.cpp
