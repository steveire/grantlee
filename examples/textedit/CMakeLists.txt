project(textedit_grantlee)

cmake_minimum_required(VERSION 3.0)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(${Grantlee_USE_FILE})

## Grantlee plugin

grantlee_add_plugin(customtags
  customtagslibrary
  audiotextdocumentoutputter

  FILTERS
    filters
)

target_link_libraries(customtags ${Grantlee_TEXTDOCUMENT_LIBRARIES})


## Application

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)


add_executable(textedit
  main.cpp
  audiotextedit.cpp
  audioobject.cpp
  textedit.cpp
  textedit.qrc
)

target_link_libraries(
  textedit
  Qt4::QtWebKit
  Qt4::phonon
  ${Grantlee_TEMPLATES_LIBRARIES}
)
