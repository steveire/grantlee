project(grantlee_templates)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/../scriptabletags
)

include(${QT_USE_FILE})

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
  set(scriptabletags_SRCS ${scriptabletags_SRCS} ${CMAKE_SOURCE_DIR}/templates/scriptabletags/${file})
endforeach()

set(Grantlee_TEMPLATES_LIB_SOVERSION 0)

set(moc_SRCS ${grantlee_templates_SRCS} ${scriptabletags_SRCS})

qt4_automoc(${moc_SRCS})

add_library(grantlee_core SHARED
  ${grantlee_templates_SRCS}
  ${scriptabletags_SRCS}
)

target_link_libraries(grantlee_core
  ${QT_QTCORE_LIBRARY}
  ${QT_QTSCRIPT_LIBRARY}
)

# QtScript shouldn't be part of the link interface of grantlee_core.
target_link_libraries(grantlee_core
  LINK_INTERFACE_LIBRARIES
  ${QT_QTCORE_LIBRARY}
)

set_target_properties(grantlee_core PROPERTIES
  VERSION    ${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}
  SOVERSION  ${Grantlee_TEMPLATES_LIB_SOVERSION}
  DEFINE_SYMBOL GRANTLEE_TEMPLATES_LIB_MAKEDLL
)

install(TARGETS grantlee_core EXPORT grantlee_targets
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
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
  grantlee_core_export.h
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
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT Devel
)

install(FILES
  grantlee_core.h
  grantlee_templates.h
  DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel
)

