project(grantlee_core_library)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
)

configure_file(grantlee_version.h.cmake ${PROJECT_BINARY_DIR}/grantlee_version.h)

set (grantlee_core_library_SRCS
  template.cpp
  templateloader.cpp
  context.cpp
  variable.cpp
  filterexpression.cpp
  node.cpp
  filter.cpp
  lexer.cpp
  parser.cpp
  util.cpp
  safestring.cpp
)

set(GRANTLEE_CORE_LIB_SOVERSION 0)

automoc4_add_library(grantlee_core_library SHARED
  ${grantlee_core_library_SRCS}
)

target_link_libraries(grantlee_core_library
  ${QT_QTCORE_LIBRARY}
)

set_target_properties(grantlee_core_library PROPERTIES
  VERSION    ${GRANTLEE_VERSION_MAJOR}.${GRANTLEE_VERSION_MINOR}.${GRANTLEE_VERSION_PATCH}
  SOVERSION  ${GRANTLEE_CORE_LIB_SOVERSION}
)

install(TARGETS grantlee_core_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(FILES
  node.h
  filter.h
  filterexpression.h
  template.h
  templateloader.h
  context.h
  variable.h
  filterexpression.h
  parser.h
  grantlee_export.h
  grantlee.h
  safestring.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT Devel
)
