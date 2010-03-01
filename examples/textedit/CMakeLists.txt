project(textedit_grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)
find_package(Grantlee REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_GUI_LIB)
add_definitions(-DQT_NO_KEYWORDS)

include(${Grantlee_USE_FILE})

include_directories(
  ${QT_INCLUDES}
  ${QT_QTGUI_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}
)

## Grantlee plugin

grantlee_add_plugin(customtags
  customtagslibrary

  FILTERS
    filters
)

target_link_libraries(customtags ${Grantlee_GUI_LIBRARIES})


## Application

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set(textedit_example_SRCS
  main.cpp
  textedit.cpp
)

qt4_automoc( textedit_example_MOC_SRCS ${textedit_example_SRCS} )

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
  ${QT_QTCORE_LIBRARIES}
  ${QT_QTGUI_LIBRARIES}
  ${QT_QTWEBKIT_LIBRARIES}
  ${Grantlee_CORE_LIBRARIES}
)



