project(grantlee_defaulttags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

grantlee_add_plugin ( grantlee_defaulttags
  defaulttags

  autoescape
  comment
  cycle
  debug
  filtertag
  firstof
  for
  if
  ifchanged
  ifequal
  load
  mediafinder
  now
  range
  regroup
  spaceless
  ssi
  templatetag
  widthratio
  with
)

target_link_libraries(grantlee_defaulttags grantlee_core_library)

install(TARGETS grantlee_defaulttags
        RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        COMPONENT Devel
)


