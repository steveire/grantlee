
project(grantlee_tubes)

set (grantlee_tubes_SRCS
  cat.cpp
  pump.cpp
  tee.cpp
)

set(Grantlee_TUBES_LIB_SOVERSION 0)

qt4_automoc(${grantlee_tubes_SRCS})

add_library(grantlee_tubes SHARED
  ${grantlee_tubes_SRCS}
)

foreach(file ${grantlee_tubes_SRCS})
  set(grantlee_tubes_SRCS_FULLPATHS ${grantlee_tubes_SRCS_FULLPATHS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set_property(GLOBAL APPEND PROPERTY SOURCE_LIST
  ${grantlee_tubes_SRCS_FULLPATHS}
)

grantlee_generate_export_header(grantlee_tubes)

target_link_libraries(grantlee_tubes
  ${QT_QTCORE_LIBRARY}
)

set_target_properties(grantlee_tubes PROPERTIES
  VERSION    ${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}
  SOVERSION  ${Grantlee_TUBES_LIB_SOVERSION}
)

install(TARGETS grantlee_tubes EXPORT grantlee_targets
         RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT Tubes
         LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT Tubes
         ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Tubes
)

install(FILES
  cat.h
  pump.h
  tee.h
  ${PROJECT_BINARY_DIR}/grantlee_tubes_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/grantlee COMPONENT Tubes
)

install(FILES
  grantlee_tubes.h
  DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Tubes
)

