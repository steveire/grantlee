project(grantlee_defaultfilters)

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
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
