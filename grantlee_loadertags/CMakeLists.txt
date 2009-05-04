project(grantlee_loadertags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)


set (grantlee_loadertags_library_SRCS
  loadertags.cpp
  include.cpp
  extends.cpp
  block.cpp
)

set(GRANTLEE_LOADERTAGS_LIB_SOVERSION 0)

automoc4_add_library(grantlee_loadertags_library SHARED
  ${grantlee_loadertags_library_SRCS}
)

target_link_libraries(grantlee_loadertags_library
  grantlee_core_library
)

set_target_properties(grantlee_loadertags_library PROPERTIES
  VERSION    ${GRANTLEE_LIB_MAJOR_VERSION}.${GRANTLEE_LIB_MINOR_VERSION}.${GRANTLEE_LIB_PATCH_VERSION}
  SOVERSION  ${GRANTLEE_LOADERTAGS_LIB_SOVERSION}
)

install(TARGETS grantlee_loadertags_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)
