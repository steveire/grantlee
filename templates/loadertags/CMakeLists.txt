project(grantlee_loadertags)

set_source_files_properties(blockcontext.h PROPERTIES SKIP_AUTOMOC TRUE)

add_library(grantlee_loadertags MODULE
  loadertags.cpp
  blockcontext.cpp
  block.cpp
  extends.cpp
  include.cpp
)
target_link_libraries(grantlee_loadertags Grantlee::Templates)
grantlee_adjust_plugin_name(grantlee_loadertags)

install(TARGETS grantlee_loadertags EXPORT grantlee_targets
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Templates
)
