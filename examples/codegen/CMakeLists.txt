project(codegen_grantlee)

cmake_minimum_required(VERSION 3.0)

find_package(Grantlee5 REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

get_property(Grantlee_PLUGIN_DIR TARGET Grantlee5::defaulttags PROPERTY LOCATION)
get_filename_component(Grantlee_PLUGIN_DIR ${Grantlee_PLUGIN_DIR} PATH)
get_filename_component(Grantlee_PLUGIN_DIR ${Grantlee_PLUGIN_DIR} PATH)
get_filename_component(Grantlee_PLUGIN_DIR ${Grantlee_PLUGIN_DIR} PATH)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

qt5_add_resources(
  codegen_example_RCS_SRCS
  custom_tags.qrc
  OPTIONS -root "/plugins/grantlee/${Grantlee5_VERSION_MAJOR}.${Grantlee5_VERSION_MINOR}/"
)

add_executable(codegen
  codegen.qrc
  comboboxdelegate.cpp
  main.cpp
  methodmodel.cpp
  codegentableview.cpp
  designwidget.cpp
  propertytablewidget.cpp
  mainwindow.cpp
  ${codegen_example_RCS_SRCS}
)
target_compile_definitions(codegen PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0)

find_package(Qt5Widgets REQUIRED)

target_link_libraries(
  codegen
  Grantlee5::Templates
  Qt5::Widgets
)
