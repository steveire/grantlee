project(grantlee_defaulttags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)


set (grantlee_defaulttags_library_SRCS
  defaulttags.cpp
  autoescape.cpp
  regroup.cpp
  if.cpp
  for.cpp
  with.cpp
  cycle.cpp
  comment.cpp
  ifequal.cpp
  firstof.cpp
  templatetag.cpp
  widthratio.cpp
  filtertag.cpp
  load.cpp
  now.cpp
  ssi.cpp
  ifchanged.cpp
  spaceless.cpp
  debug.cpp
)

set(GRANTLEE_DEFAULTTAGS_LIB_SOVERSION 0)

automoc4_add_library(grantlee_defaulttags_library SHARED
  ${grantlee_defaulttags_library_SRCS}
)

target_link_libraries(grantlee_defaulttags_library
  grantlee_core_library
)

set_target_properties(grantlee_defaulttags_library PROPERTIES
  VERSION    ${GRANTLEE_LIB_MAJOR_VERSION}.${GRANTLEE_LIB_MINOR_VERSION}.${GRANTLEE_LIB_PATCH_VERSION}
  SOVERSION  ${GRANTLEE_DEFAULTTAGS_LIB_SOVERSION}
)

install(TARGETS grantlee_defaulttags_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

