project(grantlee_scriptabletags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

set (grantlee_scriptabletags_library_SRCS
  scriptablecontext.cpp
  scriptablefilter.cpp
  scriptablefilterexpression.cpp
  scriptablenode.cpp
  scriptableparser.cpp
  scriptablesafestring.cpp
  scriptabletags.cpp
  scriptabletemplate.cpp
  scriptablevariable.cpp
)

set(GRANTLEE_SCRIPTABLETAGS_LIB_SOVERSION 0)

automoc4_add_library(grantlee_scriptabletags_library SHARED
    ${grantlee_scriptabletags_library_SRCS}
)

target_link_libraries(grantlee_scriptabletags_library
  grantlee_core_library
  ${QT_QTSCRIPT_LIBRARY}
)

install(TARGETS grantlee_scriptabletags_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(TARGETS grantlee_scriptabletags_library
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
         LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
         ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
)

install(FILES scripteddefaults.qs DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING} )
