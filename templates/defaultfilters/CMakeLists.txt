project(grantlee_defaultfilters)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
)

grantlee_add_plugin(grantlee_defaultfilters
  defaultfilters

  FILTERS
    datetime
    integers
    lists
    logic
    misc
    strings
)

install(TARGETS grantlee_defaultfilters
         RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)
