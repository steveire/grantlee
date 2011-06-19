project(grantlee_scriptabletags)

include_directories(
  ${PROJECT_BINARY_DIR}
)

grantlee_add_plugin(grantlee_scriptabletags
  scriptablecontext
  scriptablefilterexpression
  scriptablenode
  scriptableparser
  scriptablesafestring
  scriptabletags
  scriptabletemplate
  scriptablevariable

  FILTERS
    scriptablefilter
)

target_link_libraries(grantlee_scriptabletags
  ${QT_QTSCRIPT_LIBRARY}
)

install(TARGETS grantlee_scriptabletags
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)

