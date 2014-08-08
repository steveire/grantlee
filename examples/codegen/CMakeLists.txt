project(codegen_grantlee)

cmake_minimum_required(VERSION 2.8.11)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

qt4_add_resources(
  codegen_example_RCS_SRCS
  codegen.qrc
)

qt4_add_resources(
  codegen_example_RCS_SRCS
  custom_tags.qrc
  OPTIONS -root "/plugins/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}/"
)

qt4_wrap_ui(codegen_example_UI_SRCS designwidget.ui)

add_executable(codegen
  comboboxdelegate.cpp
  main.cpp
  methodmodel.cpp
  codegentableview.cpp
  designwidget.cpp
  propertytablewidget.cpp
  mainwindow.cpp
  ${codegen_example_RCS_SRCS}
  ${codegen_example_UI_SRCS}
)

target_link_libraries(
  codegen
  Qt4::QtGui
  ${Grantlee_TEMPLATES_LIBRARIES}
)
