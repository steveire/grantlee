project(grantlee_core_library)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
)

include(${QT_USE_FILE})

configure_file(grantlee_version.h.cmake ${PROJECT_BINARY_DIR}/grantlee_version.h)

set (grantlee_core_library_SRCS
  context.cpp
  engine.cpp
  enginestate.cpp
  filterexpression.cpp
  lexer.cpp
  mutabletemplate.cpp
  node.cpp
  parser.cpp
  safestring.cpp
  template.cpp
  templateloader.cpp
  util.cpp
  variable.cpp
)

set(Grantlee_CORE_LIB_SOVERSION 0)

qt4_automoc(${grantlee_core_library_SRCS})

add_library(grantlee_core_library SHARED
  ${grantlee_core_library_SRCS}
)

target_link_libraries(grantlee_core_library
  ${QT_QTCORE_LIBRARY}
)

set_target_properties(grantlee_core_library PROPERTIES
  VERSION    ${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}
  SOVERSION  ${Grantlee_CORE_LIB_SOVERSION}
  DEFINE_SYMBOL GRANTLEE_CORE_LIB_MAKEDLL
)

install(TARGETS grantlee_core_library
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(FILES
  context.h
  engine.h
  enginestate.h
  exception.h
  filter.h
  filterexpression.h
  global.h
  grantlee_export.h
  mutabletemplate.h
  node.h
  parser.h
  safestring.h
  taglibraryinterface.h
  template.h
  templateloader.h
  token.h
  variable.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT Devel
)

install(FILES
  grantlee.h
  DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel
)

