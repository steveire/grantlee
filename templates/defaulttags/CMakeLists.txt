project(grantlee_defaulttags)

add_library(grantlee_defaulttags MODULE
  defaulttags.cpp
  autoescape.cpp
  comment.cpp
  cycle.cpp
  debug.cpp
  filtertag.cpp
  firstof.cpp
  for.cpp
  if.cpp
  ifchanged.cpp
  ifequal.cpp
  load.cpp
  mediafinder.cpp
  now.cpp
  range.cpp
  regroup.cpp
  spaceless.cpp
  templatetag.cpp
  widthratio.cpp
  with.cpp
)
target_link_libraries(grantlee_defaulttags Grantlee::Templates)
grantlee_adjust_plugin_name(grantlee_defaulttags)

install(TARGETS grantlee_defaulttags EXPORT grantlee_targets
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Templates
)
