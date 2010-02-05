project(grantlee_defaultfilters)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

grantlee_add_plugin(grantlee_defaultfilters
  defaultfilters

  datetime
  integers
  lists
  logic
  misc
  strings
)

target_link_libraries(grantlee_defaultfilters
  grantlee_core_library
)

install(TARGETS grantlee_defaultfilters
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)
