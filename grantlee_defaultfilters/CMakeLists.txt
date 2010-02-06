project(grantlee_defaultfilters)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

set(grantlee_defaultfilters_sources
  defaultfilters.cpp

  datetime.cpp
  integers.cpp
  lists.cpp
  logic.cpp
  misc.cpp
  strings.cpp
)

qt4_wrap_cpp(grantlee_defaultfilters_moc_sources defaultfilters.h)

add_library(grantlee_defaultfilters SHARED ${grantlee_defaultfilters_sources} ${grantlee_defaultfilters_moc_sources} )

set_target_properties(grantlee_defaultfilters PROPERTIES PREFIX "")


target_link_libraries(grantlee_defaultfilters
  grantlee_core_library
)

install(TARGETS grantlee_defaultfilters
         RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)
