project(grantlee_scriptabletags)

add_library(grantlee_scriptabletags MODULE
  scriptablecontext.cpp
  scriptablefilterexpression.cpp
  scriptablenode.cpp
  scriptableparser.cpp
  scriptablesafestring.cpp
  scriptabletags.cpp
  scriptabletemplate.cpp
  scriptablevariable.cpp
  scriptablefilter.cpp
)
grantlee_adjust_plugin_name(grantlee_scriptabletags)

target_link_libraries(grantlee_scriptabletags
  grantlee_core
  Qt4::QtScript
)

install(TARGETS grantlee_scriptabletags
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
