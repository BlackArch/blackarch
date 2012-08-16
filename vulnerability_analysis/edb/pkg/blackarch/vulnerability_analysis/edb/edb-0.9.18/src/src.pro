TEMPLATE    = app
#CONFIG     += silent
TARGET      = edb
DEPENDPATH  += ./widgets ../include
INCLUDEPATH += ./widgets ../include
RESOURCES   = debugger.qrc
DESTDIR     = ../
target.path = /bin/
INSTALLS    += target 
QT          += xml

TRANSLATIONS += \
	edb_en.ts

HEADERS += \
	API.h \
	AnalyzerInterface.h \
	ArchProcessor.h \
	ArchProcessorInterface.h \
	ArchTypes.h \
	BinaryInfo.h \
	BinaryString.h \
	Breakpoint.h \
	ByteShiftArray.h \
	CommentServer.h \
	Configuration.h \
	DataViewInfo.h \
	DebugEvent.h \
	DebugEventHandlerInterface.h \
	Debugger.h \
	DebuggerCoreInterface.h \
	DebuggerMain.h \
	DebuggerOps.h \
	DebuggerPluginInterface.h \
	DebuggerUI.h \
	DialogArguments.h \
	DialogAttach.h \
	DialogInputBinaryString.h \
	DialogInputValue.h \
	DialogMemoryRegions.h \
	DialogOptions.h \
	DialogPlugins.h \
	DialogThreads.h \
	Expression.h \
	FunctionInfo.h \
	LineEdit.h \
	MD5.h \
	MemRegion.h \
	MemoryRegions.h \
	OSTypes.h \
	QCategoryList.h \
	QDisassemblyView.h \
	QLongValidator.h \
	QULongValidator.h \
	RecentFileManager.h \
	RegionBuffer.h \
	Register.h \
	RegisterViewDelegate.h \
	ScopedPointer.h \
	SessionFileInterface.h \
	State.h \
	symbols.h \
	SymbolManager.h \
	SyntaxHighlighter.h \
	TabWidget.h \
	Types.h \
	Util.h \
	version.h


FORMS += \
	binarystring.ui \
	debuggerui.ui \
	dialog_arguments.ui \
	dialog_attach.ui \
	dialog_inputbinarystring.ui \
	dialog_inputvalue.ui \
	dialog_memoryregions.ui \
	dialog_options.ui \
	dialog_plugins.ui \
	dialog_threads.ui 
	
SOURCES += \
	ArchProcessor.cpp \
	BinaryInfo.cpp \
	BinaryString.cpp \
	ByteShiftArray.cpp \
	CommentServer.cpp \
	Configuration.cpp \
	DataViewInfo.cpp \
	DebugEvent.cpp \
	Debugger.cpp \
	DebuggerMain.cpp \
	DebuggerOS.cpp \
	DebuggerUI.cpp \
	DialogArguments.cpp \
	DialogAttach.cpp \
	DialogInputBinaryString.cpp \
	DialogInputValue.cpp \
	DialogMemoryRegions.cpp \
	DialogOptions.cpp \
	DialogPlugins.cpp \
	DialogThreads.cpp \
	Instruction.cpp \
	LineEdit.cpp \
	MD5.cpp \
	MemRegion.cpp \
	MemoryRegions.cpp \
	QCategoryList.cpp \
	QDisassemblyView.cpp \
	QLongValidator.cpp \
	QULongValidator.cpp \
	RecentFileManager.cpp \
	RegionBuffer.cpp \
	Register.cpp \
	RegisterViewDelegate.cpp \
	State.cpp \
	symbols.cpp \
	SymbolManager.cpp \
	SyntaxHighlighter.cpp \
	TabWidget.cpp \
	main.cpp


DEPENDPATH  += ./qhexview
INCLUDEPATH += ./qhexview
SOURCES     += qhexview.cpp
HEADERS     += qhexview.h QHexView

# qjson stuff

DEPENDPATH  += ./qjson 
INCLUDEPATH  += ./qjson 

HEADERS += json_parser.hh \
           json_scanner.h \
           location.hh \
           parser.h \
           parser_p.h \
           parserrunnable.h \
           position.hh \
           qjson_debug.h \
           qjson_export.h \
           qobjecthelper.h \
           serializer.h \
           serializerrunnable.h \
           stack.hh
		   
SOURCES += json_parser.cc \
           json_scanner.cpp \
           parser.cpp \
           parserrunnable.cpp \
           qobjecthelper.cpp \
           serializer.cpp \
           serializerrunnable.cpp
	
win32 {
	DEPENDPATH  += os/win32 ../include/os/win32 arch/i386 ../include/arch/i386 edisassm
	INCLUDEPATH += os/win32 ../include/os/win32 arch/i386 ../include/arch/i386 edisassm "C:\\Program Files\\boost\\boost_1_47"
	DEFINES     += _CRT_SECURE_NO_WARNINGS
	LIBS			+= -lAdvapi32
	
	win32-g++*:	QMAKE_LFLAGS += ""
	RC_FILE = edb.rc
}

win64 {
	DEPENDPATH  += os/win64 ../include/os/win64 arch/x86_64 ../include/arch/x86_64 edisassm
	INCLUDEPATH += os/win64 ../include/os/win64 arch/x86_64 ../include/arch/x86_64 edisassm $$(BOOST)
	DEFINES     += _CRT_SECURE_NO_WARNINGS
	LIBS			+= -lAdvapi32
	
	win64-g++*:	QMAKE_LFLAGS += ""
	RC_FILE = edb.rc
}


unix {
	graph {
		DEPENDPATH  += ./graph
		INCLUDEPATH += ./graph
		HEADERS     += GraphEdge.h   GraphWidgetBase.h   GraphWidget.h   GraphNode.h
		SOURCES     += GraphEdge.cpp GraphWidgetBase.cpp GraphWidget.cpp GraphNode.cpp
		LIBS        += -lgvc
	}

	SOURCES += DialogAttachUNIX.cpp
	HEADERS += DialogAttachUNIX.h

	!isEmpty(DEFAULT_PLUGIN_PATH) {
		DEFINES += DEFAULT_PLUGIN_PATH=$$DEFAULT_PLUGIN_PATH
	}

	!isEmpty(DEFAULT_SYMBOL_PATH) {
		DEFINES += DEFAULT_SYMBOL_PATH=$$DEFAULT_SYMBOL_PATH
	}

	!isEmpty(DEFAULT_SESSION_PATH) {
		DEFINES += DEFAULT_SESSION_PATH=DEFAULT_SESSION_PATH
	}


	DEPENDPATH  += os/unix ../include/os/unix edisassm
	INCLUDEPATH += os/unix ../include/os/unix edisassm

	linux-* {
		DEPENDPATH  += os/unix/linux ../include/os/unix/linux
		INCLUDEPATH += os/unix/linux ../include/os/unix/linux
	}

	openbsd-* {
		DEPENDPATH  += os/unix/openbsd ../include/os/unix/openbsd
		INCLUDEPATH += os/unix/openbsd ../include/os/unix/openbsd /usr/local/include
	}

	freebsd-* {
		DEPENDPATH  += os/unix/freebsd ../include/os/unix/freebsd
		INCLUDEPATH += os/unix/freebsd ../include/os/unix/freebsd
	}

	macx-* {
		DEPENDPATH  += os/unix/osx ../include/os/unix/osx
		INCLUDEPATH += os/unix/osx ../include/os/unix/osx /opt/local/include
	}

	macx {
		DEPENDPATH  += arch/x86_64 ../include/arch/x86_64
		INCLUDEPATH += arch/x86_64 ../include/arch/x86_64
	}

	!macx {
		DEPENDPATH  += arch/$$QT_ARCH ../include/arch/$$QT_ARCH
		INCLUDEPATH += arch/$$QT_ARCH ../include/arch/$$QT_ARCH
	}

	*-g++* {
		QMAKE_CXXFLAGS += -ansi -pedantic -W -Wall -Wno-long-long -Wnon-virtual-dtor
	}

	linux-g++*:		QMAKE_CXXFLAGS += -fvisibility=hidden -Wstrict-null-sentinel

	linux-g++*:		QMAKE_LFLAGS += -rdynamic
	openbsd-g++*:	QMAKE_LFLAGS += -lkvm -Wl,--export-dynamic
	freebsd-g++*:	QMAKE_LFLAGS += -lkvm -Wl,--export-dynamic
	macx-g++*:		QMAKE_LFLAGS += -rdynamic


	debug {
		linux-g++*:		QMAKE_CXXFLAGS += -ggdb
	}

	debug:   OBJECTS_DIR = $${OUT_PWD}/.obj/debug-shared
	release: OBJECTS_DIR = $${OUT_PWD}/.obj/release-shared

	debug:   MOC_DIR = $${OUT_PWD}/.moc/debug-shared
	release: MOC_DIR = $${OUT_PWD}/.moc/release-shared

	RCC_DIR = $${OUT_PWD}/.rcc
	UI_DIR  = $${OUT_PWD}/.uic
}
