project(grantlee_textdocument)

set (grantlee_textdocument_SRCS
  bbcodebuilder.cpp
  markupdirector.cpp
  plaintextmarkupbuilder.cpp
  texthtmlbuilder.cpp
  mediawikimarkupbuilder.cpp
)

set(Grantlee5_TEXTDOCUMENT_LIB_SOVERSION ${Grantlee5_VERSION_MAJOR})

# Help IDEs find some non-compiled files.
set(header_only_sources
  abstractmarkupbuilder.h
  grantlee_textdocument.h
  markupdirector_p.h
)
list(APPEND grantlee_textdocument_SRCS ${header_only_sources})

add_library(Grantlee_TextDocument SHARED
  ${grantlee_textdocument_SRCS}
)
generate_export_header(Grantlee_TextDocument)
add_library(Grantlee::TextDocument ALIAS Grantlee_TextDocument)
set_property(TARGET Grantlee_TextDocument PROPERTY EXPORT_NAME TextDocument)


target_link_libraries(Grantlee_TextDocument
  LINK_PUBLIC Qt5::Gui
)

if (NOT CMAKE_BUILD_TYPE MATCHES TestCocoon)
  set_target_properties(Grantlee_TextDocument PROPERTIES
    VERSION    ${Grantlee5_VERSION_MAJOR}.${Grantlee5_VERSION_MINOR}.${Grantlee5_VERSION_PATCH}
    SOVERSION  ${Grantlee5_TEXTDOCUMENT_LIB_SOVERSION}
  )
endif()

install(TARGETS Grantlee_TextDocument EXPORT grantlee_targets
         RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT TextDocument
         LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT TextDocument
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT TextDocument
         INCLUDES DESTINATION include
)

install(FILES
  abstractmarkupbuilder.h
  bbcodebuilder.h
  markupdirector.h
  plaintextmarkupbuilder.h
  texthtmlbuilder.h
  mediawikimarkupbuilder.h
  ${PROJECT_BINARY_DIR}/grantlee_textdocument_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT TextDocument
)

install(FILES
  grantlee_textdocument.h
  DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT TextDocument
)
