project(textedit_grantlee)

cmake_minimum_required(VERSION 2.8.9)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include(${Grantlee_USE_FILE})

include_directories(
  ${QT_INCLUDES}
  ${QT_QTGUI_INCLUDE_DIR}
  ${Grantlee_INCLUDE_DIRS}
)

## Grantlee plugin

grantlee_add_plugin(customtags
  customtagslibrary
  audiotextdocumentoutputter

  FILTERS
    filters
)

target_link_libraries(customtags ${Grantlee_GUI_LIBRARIES})


## Application

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set(textedit_example_SRCS
  main.cpp
  audiotextedit.cpp
  audioobject.cpp
  textedit.cpp
)

set(textedit_example_RCSS
  textedit.qrc
)

qt4_add_resources(
  textedit_example_RCS_SRCS
  ${textedit_example_RCSS}
)

add_executable(textedit
  ${textedit_example_SRCS}
  ${textedit_example_RCS_SRCS}
  ${textedit_example_UI_SRCS}
)

target_link_libraries(
  textedit
  Qt4::QtCore
  Qt4::QtGui
  Qt4::QtWebKit
  Qt4::phonon
  ${Grantlee_CORE_LIBRARIES}
)



