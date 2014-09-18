project(grantlee_templates)

configure_file(grantlee_version.h.cmake ${PROJECT_BINARY_DIR}/grantlee_version.h)

set(Grantlee_PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR} )

configure_file(grantlee_config_p.h.cmake  ${PROJECT_BINARY_DIR}/grantlee_config_p.h)

# BIC: remove
remove_definitions(-DQT_NO_CAST_TO_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

set (grantlee_templates_SRCS
  abstractlocalizer.cpp
  cachingloaderdecorator.cpp
  customtyperegistry.cpp
  context.cpp
  engine.cpp
  filterexpression.cpp
  lexer.cpp
  metatype.cpp
  node.cpp
  nodebuiltins.cpp
  nulllocalizer.cpp
  outputstream.cpp
  parser.cpp
  qtlocalizer.cpp
  rendercontext.cpp
  safestring.cpp
  template.cpp
  templateloader.cpp
  textprocessingmachine.cpp
  typeaccessors.cpp
  util.cpp
  variable.cpp
)

set(scriptabletags_FILES
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

foreach(file ${scriptabletags_FILES})
  list(APPEND scriptabletags_SRCS ${CMAKE_SOURCE_DIR}/templates/scriptabletags/${file})
endforeach()

set(Grantlee_TEMPLATES_LIB_SOVERSION ${Grantlee_VERSION_MAJOR})

# Help IDEs find some non-compiled files.
set(header_only_sources
  containeraccessor.h
  customtyperegistry_p.h
  engine_p.h
  exception.h
  filter.h
  grantlee_core.h
  grantlee_export_plugin_p.h
  grantlee_latin1literal_p.h
  grantlee_tags_p.h
  grantlee_templates.h
  lexer_p.h
  metaenumvariable_p.h
  nodebuiltins_p.h
  nulllocalizer_p.h
  pluginpointer_p.h
  statemachine_p.h
  taglibraryinterface.h
  template_p.h
  textprocessingmachine_p.h
  token.h
  typeaccessor.h
)
list(APPEND grantlee_templates_SRCS ${header_only_sources})

add_library(grantlee_core SHARED
  ${grantlee_templates_SRCS}
  ${scriptabletags_SRCS}
)
generate_export_header(grantlee_core)
target_include_directories(grantlee_core PRIVATE ../scriptabletags)

if (BUILD_TESTS)
  set(GRANTLEE_TESTS_EXPORT "GRANTLEE_CORE_EXPORT")
endif()

configure_file(grantlee_test_export.h.in "${CMAKE_CURRENT_BINARY_DIR}/grantlee_test_export.h")

file(READ "${CMAKE_CURRENT_BINARY_DIR}/grantlee_test_export.h" _content)

file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/grantlee_core_export.h" "${_content}")

target_link_libraries(grantlee_core
  LINK_PUBLIC Qt5::Core
  LINK_PRIVATE Qt5::Script
)

if (NOT CMAKE_BUILD_TYPE MATCHES TestCocoon)
  set_target_properties(grantlee_core PROPERTIES
    VERSION    ${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}
    SOVERSION  ${Grantlee_TEMPLATES_LIB_SOVERSION}
  )
endif()

install(TARGETS grantlee_core EXPORT grantlee_targets
         RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT Templates
         LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT Templates
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Templates
         INCLUDES DESTINATION include
)

install(FILES
  abstractlocalizer.h
  cachingloaderdecorator.h
  containeraccessor.h
  context.h
  engine.h
  exception.h
  filter.h
  filterexpression.h
  ${PROJECT_BINARY_DIR}/grantlee_core_export.h
  ${PROJECT_BINARY_DIR}/grantlee_version.h
  metatype.h
  node.h
  outputstream.h
  parser.h
  qtlocalizer.h
  rendercontext.h
  safestring.h
  taglibraryinterface.h
  template.h
  templateloader.h
  typeaccessor.h
  token.h
  util.h
  variable.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT Templates
)

install(FILES
  grantlee_core.h
  grantlee_templates.h
  DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Templates
)
