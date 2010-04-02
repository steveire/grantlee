project(grantlee_gui)

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
)

include(${QT_USE_FILE})

set (grantlee_gui_SRCS
  bbcodebuilder.cpp
  markupdirector.cpp
  plaintextmarkupbuilder.cpp
  texthtmlbuilder.cpp
  mediawikimarkupbuilder.cpp
)

set(Grantlee_GUI_LIB_SOVERSION 0)

qt4_automoc(${grantlee_gui_SRCS})

add_library(grantlee_gui SHARED
  ${grantlee_gui_SRCS}
)

target_link_libraries(grantlee_gui
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

set_target_properties(grantlee_gui PROPERTIES
  VERSION    ${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}
  SOVERSION  ${Grantlee_GUI_LIB_SOVERSION}
  DEFINE_SYMBOL GRANTLEE_GUI_LIB_MAKEDLL
)

install(TARGETS grantlee_gui
         RUNTIME DESTINATION ${BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
         COMPONENT Devel
)

install(FILES
  abstractmarkupbuilder.h
  bbcodebuilder.h
  markupdirector.h
  plaintextmarkupbuilder.h
  texthtmlbuilder.h
  mediawikimarkupbuilder.h
  grantlee_gui_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT Devel
)
