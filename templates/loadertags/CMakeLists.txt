project(grantlee_loadertags)

set_source_files_properties(blockcontext.h PROPERTIES SKIP_AUTOMOC TRUE)

grantlee_add_plugin(grantlee_loadertags
  loadertags
  blockcontext

  TAGS
    block
    extends
    include
)

install(TARGETS grantlee_loadertags
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Templates
)
