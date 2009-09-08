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
  comment.cpp
  cycle.cpp
  debug.cpp
  filtertag.cpp
  firstof.cpp
  for.cpp
  if.cpp
  ifchanged.cpp
  ifequal.cpp
  load.cpp
  mediafinder.cpp
  now.cpp
  range.cpp
  regroup.cpp
  spaceless.cpp
  ssi.cpp
  templatetag.cpp
  widthratio.cpp
  with.cpp
)

set(GRANTLEE_DEFAULTTAGS_LIB_SOVERSION 0)

automoc4_add_library(grantlee_defaulttags_library SHARED
  ${grantlee_defaulttags_library_SRCS}
)

target_link_libraries(grantlee_defaulttags_library
  grantlee_core_library
)

install(TARGETS grantlee_defaulttags_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(TARGETS grantlee_defaulttags_library
         LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
)

