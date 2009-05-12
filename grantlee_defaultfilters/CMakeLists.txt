project(grantlee_defaultfilters)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)


set (grantlee_defaultfilters_library_SRCS
  defaultfilters.cpp
  upper.cpp
  lower.cpp
  yesno.cpp
  truncatewords.cpp
  join.cpp
  removetags.cpp
  default_if_none.cpp
)

set(GRANTLEE_DEFAULTFILTERS_LIB_SOVERSION 0)

automoc4_add_library(grantlee_defaultfilters_library SHARED
  ${grantlee_defaultfilters_library_SRCS}
)

target_link_libraries(grantlee_defaultfilters_library
  grantlee_core_library
)

set_target_properties(grantlee_defaultfilters_library PROPERTIES
  VERSION    ${GRANTLEE_LIB_MAJOR_VERSION}.${GRANTLEE_LIB_MINOR_VERSION}.${GRANTLEE_LIB_PATCH_VERSION}
  SOVERSION  ${GRANTLEE_DEFAULTFILTERS_LIB_SOVERSION}
)

install(TARGETS grantlee_defaultfilters_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

