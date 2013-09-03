project(grantlee_defaultfilters)

add_library(grantlee_defaultfilters MODULE
  defaultfilters.cpp
  datetime.cpp
  integers.cpp
  lists.cpp
  logic.cpp
  misc.cpp
  stringfilters.cpp
)
target_link_libraries(grantlee_defaultfilters Grantlee::Templates)
grantlee_adjust_plugin_name(grantlee_defaultfilters)

install(TARGETS grantlee_defaultfilters EXPORT grantlee_targets
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Templates
)
