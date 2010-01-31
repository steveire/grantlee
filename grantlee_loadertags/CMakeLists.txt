project(grantlee_loadertags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

set (grantlee_loadertags_library_SRCS
  loadertags.cpp
  block.cpp
  extends.cpp
  include.cpp
)

set(GRANTLEE_LOADERTAGS_LIB_SOVERSION 0)

automoc4_add_library(grantlee_loadertags_library SHARED
  ${grantlee_loadertags_library_SRCS}
)

target_link_libraries(grantlee_loadertags_library
  grantlee_core_library
)

install(TARGETS grantlee_loadertags_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(TARGETS grantlee_loadertags_library
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
         LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
         ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
)
