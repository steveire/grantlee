project(grantlee_scriptabletags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)


set (grantlee_scriptabletags_library_SRCS
  scriptabletags.cpp
  scriptablenode.cpp
  scriptablecontext.cpp
  scriptableparser.cpp
  scriptablevariable.cpp
  scriptablefilter.cpp
  scriptablefilterexpression.cpp
  scriptabletemplate.cpp
)

set(GRANTLEE_SCRIPTABLETAGS_LIB_SOVERSION 0)

automoc4_add_library(grantlee_scriptabletags_library SHARED
    ${grantlee_scriptabletags_library_SRCS}
)

target_link_libraries(grantlee_scriptabletags_library
  grantlee_core_library
  ${QT_QTSCRIPT_LIBRARY}
)

set_target_properties(grantlee_scriptabletags_library PROPERTIES
  VERSION    ${GRANTLEE_LIB_MAJOR_VERSION}.${GRANTLEE_LIB_MINOR_VERSION}.${GRANTLEE_LIB_PATCH_VERSION}
  SOVERSION  ${GRANTLEE_SCRIPTABLETAGS_LIB_SOVERSION}
)

install(TARGETS grantlee_scriptabletags_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(TARGETS grantlee_scriptabletags_library
         LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
)

install(FILES scripteddefaults.qs DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING} )
