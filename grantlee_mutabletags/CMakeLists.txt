project(grantlee_mutabletags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

set (grantlee_mutabletags_SRCS
  mutabletags.cpp
  raw.cpp
  repeater.cpp
)

automoc4_add_library(grantlee_mutabletags_library SHARED
  ${grantlee_mutabletags_SRCS}
)

target_link_libraries(grantlee_mutabletags_library
  grantlee_core_library
)

install(TARGETS grantlee_mutabletags_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(TARGETS grantlee_mutabletags_library
         LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
)
