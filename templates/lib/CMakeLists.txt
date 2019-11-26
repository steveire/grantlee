
configure_file(grantlee_version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/grantlee_version.h)

set(Grantlee_PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR} )

configure_file(grantlee_config_p.h.cmake  ${CMAKE_CURRENT_BINARY_DIR}/grantlee_config_p.h)

set (grantlee_templates_SRCS
  abstractlocalizer.cpp
  cachingloaderdecorator.cpp
  customtyperegistry.cpp
  context.cpp
  engine.cpp
  filter.cpp
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

set(Grantlee5_TEMPLATES_LIB_SOVERSION ${Grantlee5_VERSION_MAJOR})

# Help IDEs find some non-compiled files.
set(header_only_sources
  customtyperegistry_p.h
  engine_p.h
  exception.h
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

set_source_files_properties(taglibraryinterface.h PROPERTIES SKIP_AUTOMOC TRUE)

add_library(Grantlee_Templates SHARED
  ${grantlee_templates_SRCS}
)
add_library(Grantlee5::Templates ALIAS Grantlee_Templates)
generate_export_header(Grantlee_Templates)
set_property(TARGET Grantlee_Templates PROPERTY EXPORT_NAME Templates)
target_compile_features(Grantlee_Templates
  PRIVATE
    cxx_auto_type
  PUBLIC
    cxx_override
)

if (CMAKE_GENERATOR MATCHES "Visual Studio")

  set_property(TARGET Grantlee_Templates PROPERTY DEBUG_POSTFIX "d")

  foreach(cfg ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${cfg} CFG)
    set_target_properties(Grantlee_Templates
      PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY_${CFG} "${CMAKE_CURRENT_BINARY_DIR}"
      RUNTIME_OUTPUT_DIRECTORY_${CFG} "${CMAKE_CURRENT_BINARY_DIR}"
      )
  endforeach()
endif()
target_compile_definitions(Grantlee_Templates
  PRIVATE
    PLUGINS_PREFER_DEBUG_POSTFIX=$<CONFIG:Debug>
)

if (Qt5Qml_FOUND)
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

  target_sources(Grantlee_Templates PRIVATE ${scriptabletags_SRCS})
  target_include_directories(Grantlee_Templates PRIVATE ../scriptabletags)
  target_link_libraries(Grantlee_Templates
    LINK_PRIVATE Qt5::Qml
  )
endif()

if (BUILD_TESTS)
  set(GRANTLEE_TESTS_EXPORT "GRANTLEE_TEMPLATES_EXPORT")
endif()

configure_file(grantlee_test_export.h.in "${CMAKE_CURRENT_BINARY_DIR}/grantlee_test_export.h")

target_link_libraries(Grantlee_Templates
  LINK_PUBLIC Qt5::Core
)

set_target_properties(Grantlee_Templates PROPERTIES
  VERSION    ${Grantlee5_VERSION}
  SOVERSION  ${Grantlee5_TEMPLATES_LIB_SOVERSION}
)

install(TARGETS Grantlee_Templates EXPORT grantlee_targets
         RUNTIME DESTINATION bin COMPONENT Templates
         LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT Templates
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Templates
         INCLUDES DESTINATION include
)

install(FILES
  abstractlocalizer.h
  cachingloaderdecorator.h
  context.h
  engine.h
  exception.h
  filter.h
  filterexpression.h
  ${CMAKE_CURRENT_BINARY_DIR}/grantlee_templates_export.h
  ${CMAKE_CURRENT_BINARY_DIR}/grantlee_version.h
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
  DESTINATION include/grantlee COMPONENT Templates
)

install(FILES
  grantlee_templates.h
  DESTINATION include COMPONENT Templates
)
