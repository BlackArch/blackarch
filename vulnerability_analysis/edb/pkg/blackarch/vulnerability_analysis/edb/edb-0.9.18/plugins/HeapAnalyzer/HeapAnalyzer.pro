
include(../plugins.pri)

# Input
HEADERS += HeapAnalyzer.h DialogHeap.h ResultViewModel.h
FORMS += dialogheap.ui
SOURCES += HeapAnalyzer.cpp DialogHeap.cpp ResultViewModel.cpp

graph {
	DEFINES += ENABLE_GRAPH
}
