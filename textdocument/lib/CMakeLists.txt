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
target_compile_features(Grantlee_TextDocument
  PRIVATE
    cxx_auto_type
  PUBLIC
    cxx_override
)
if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set_property(TARGET Grantlee_TextDocument PROPERTY DEBUG_POSTFIX "d")

  foreach(cfg ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${cfg} CFG)
    set_target_properties(Grantlee_TextDocument
      PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY_${CFG} "${CMAKE_CURRENT_BINARY_DIR}"
      RUNTIME_OUTPUT_DIRECTORY_${CFG} "${CMAKE_CURRENT_BINARY_DIR}"
      )
  endforeach()
endif()

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
  ${CMAKE_CURRENT_BINARY_DIR}/grantlee_textdocument_export.h
  DESTINATION include/grantlee COMPONENT TextDocument
)

install(FILES
  grantlee_textdocument.h
  DESTINATION include COMPONENT TextDocument
)
