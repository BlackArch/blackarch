CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(edisassm)
SET(edisassm_SOURCES Instruction.cpp edisassm.cpp)
SET(edisassm_HEADERS Instruction.h ModRM.h Operand.h REX.h SIB.h edisassm_exception.h edisassm_types.h edisassm_util.h)
ADD_EXECUTABLE(edisassm ${edisassm_SOURCES})
ENABLE_TESTING()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-field-initializers -ansi -pedantic -W -Wall")


