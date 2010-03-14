project(grantlee_scriptabletags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../corelib
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
         RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

