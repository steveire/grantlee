project(grue)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)
find_package(Grantlee REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_GUI_LIB)
add_definitions(-DQT_NO_KEYWORDS)

include_directories(
  ${QT_INCLUDES}
  ${QT_QTGUI_INCLUDE_DIR}
  ${Grantlee_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}
)

set(grue_example_SRCS
  commandprocessor.cpp
  main.cpp
  mainwindow.cpp
)

qt4_automoc( grue_example_MOC_SRCS ${grue_example_SRCS} )

add_executable(grue
  ${grue_example_SRCS}
)

target_link_libraries(grue
  ${QT_QTGUI_LIBRARIES}
  ${Grantlee_TUBES_LIBRARIES}
)

