project(grantlee_mutabletags)

include(${Grantlee_USE_FILE})

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../corelib
)

grantlee_add_plugin(grantlee_mutabletags
  mutabletags

  TAGS
    raw
    repeater
)

install(TARGETS grantlee_mutabletags
         RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)
