project(grantlee_loadertags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

grantlee_add_plugin(grantlee_loadertags_library
  loadertags

  block
  extends
  include
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