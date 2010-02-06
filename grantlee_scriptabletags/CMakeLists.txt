project(grantlee_scriptabletags)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

grantlee_add_plugin(grantlee_scriptabletags
  scriptablecontext
  scriptablefilter
  scriptablefilterexpression
  scriptablenode
  scriptableparser
  scriptablesafestring
  scriptabletags
  scriptabletemplate
  scriptablevariable
)

target_link_libraries(grantlee_scriptabletags
  grantlee_core_library
  ${QT_QTSCRIPT_LIBRARY}
)

install(TARGETS grantlee_scriptabletags
         RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

