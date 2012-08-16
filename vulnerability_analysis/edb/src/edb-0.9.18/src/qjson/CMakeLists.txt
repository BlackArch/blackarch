include_directories (./)

# add_custom_command (OUTPUT ${qjson_SOURCE_DIR}/lib/json_parser.cc
#                    PRE_BUILD
#                    COMMAND bison -t -o json_parser.cc -d json_parser.yy
#                    DEPENDS json_parser.yy
#                    WORKING_DIRECTORY ${qjson_SOURCE_DIR}/lib/
#                    )

install(FILES
        parser.h
        parserrunnable.h
				qobjecthelper.h
        serializer.h
        serializerrunnable.h
        qjson_export.h
        DESTINATION ${INCLUDE_INSTALL_DIR}/qjson COMPONENT Devel
)

set(qjson_MOC_HDRS
  parserrunnable.h
  serializerrunnable.h
)

qt4_wrap_cpp(qjson_MOC_SRCS ${qjson_MOC_HDRS})

set (qjson_SRCS parser.cpp qobjecthelper.cpp json_scanner.cpp json_parser.cc parserrunnable.cpp serializer.cpp serializerrunnable.cpp)

add_library (qjson SHARED ${qjson_SRCS} ${qjson_MOC_SRCS})
target_link_libraries( qjson ${QT_LIBRARIES})

set_target_properties(qjson PROPERTIES
                      VERSION ${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}
                      SOVERSION ${QJSON_LIB_MAJOR_VERSION}
                      DEFINE_SYMBOL QJSON_MAKEDLL
                      )

INSTALL(TARGETS qjson
   LIBRARY DESTINATION ${LIB_INSTALL_DIR}
   RUNTIME DESTINATION bin
   ARCHIVE DESTINATION  ${LIB_INSTALL_DIR}
)
