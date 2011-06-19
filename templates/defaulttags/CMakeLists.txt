project(grantlee_defaulttags)

include_directories(
  ${PROJECT_BINARY_DIR}
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
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)


