project(grantlee_textdocument)

set (grantlee_textdocument_SRCS
  bbcodebuilder.cpp
  markupdirector.cpp
  plaintextmarkupbuilder.cpp
  texthtmlbuilder.cpp
  mediawikimarkupbuilder.cpp
)

set(Grantlee_TEXTDOCUMENT_LIB_SOVERSION ${Grantlee_VERSION_MAJOR})

add_library(grantlee_gui SHARED
  ${grantlee_textdocument_SRCS}
)
generate_export_header(grantlee_gui)

foreach(file ${grantlee_textdocument_SRCS})
  set(grantlee_textdocument_SRCS_FULLPATHS ${grantlee_textdocument_SRCS_FULLPATHS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set_property(GLOBAL APPEND PROPERTY SOURCE_LIST
  ${grantlee_textdocument_SRCS_FULLPATHS}
)

target_link_libraries(grantlee_gui
  LINK_PUBLIC Qt4::QtGui Qt4::QtCore
)

if (NOT CMAKE_BUILD_TYPE MATCHES TestCocoon)
  set_target_properties(grantlee_gui PROPERTIES
    VERSION    ${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}
    SOVERSION  ${Grantlee_TEXTDOCUMENT_LIB_SOVERSION}
  )
endif()

install(TARGETS grantlee_gui EXPORT grantlee_targets
         RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT TextDocument
         LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT TextDocument
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT TextDocument
)

install(FILES
  abstractmarkupbuilder.h
  bbcodebuilder.h
  markupdirector.h
  plaintextmarkupbuilder.h
  texthtmlbuilder.h
  mediawikimarkupbuilder.h
  ${PROJECT_BINARY_DIR}/grantlee_gui_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT TextDocument
)
