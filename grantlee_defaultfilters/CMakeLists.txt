project(grantlee_defaultfilters)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

grantlee_add_plugin(grantlee_defaultfilters_library
  defaultfilters

  datetime
  integers
  lists
  logic
  misc
  strings
)

target_link_libraries(grantlee_defaultfilters_library
  grantlee_core_library
)

install(TARGETS grantlee_defaultfilters_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)


install(TARGETS grantlee_defaultfilters_library
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
         LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
         ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/${GRANTLEE_MAJOR_MINOR_VERSION_STRING}
)

