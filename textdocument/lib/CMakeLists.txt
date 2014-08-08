project(grantlee_textdocument)

set (grantlee_textdocument_SRCS
  bbcodebuilder.cpp
  markupdirector.cpp
  plaintextmarkupbuilder.cpp
  texthtmlbuilder.cpp
  mediawikimarkupbuilder.cpp
)

set(Grantlee_TEXTDOCUMENT_LIB_SOVERSION ${Grantlee_VERSION_MAJOR})

# Help IDEs find some non-compiled files.
set(header_only_sources
  abstractmarkupbuilder.h
  grantlee_textdocument.h
  markupdirector_p.h
)
list(APPEND grantlee_textdocument_SRCS ${header_only_sources})

add_library(grantlee_gui SHARED
  ${grantlee_textdocument_SRCS}
)
generate_export_header(grantlee_gui
  EXPORT_MACRO_NAME GRANTLEE_TEXTDOCUMENT_EXPORT
  EXPORT_FILE_NAME grantlee_textdocument_export.h

)

target_link_libraries(grantlee_gui
  LINK_PUBLIC Qt5::Gui
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
