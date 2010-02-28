project(grantlee_defaulttags)

include(${Grantlee_USE_FILE})

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

grantlee_add_plugin ( grantlee_defaulttags
  defaulttags

  TAGS
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

install(TARGETS grantlee_defaulttags
        RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        COMPONENT Devel
)


