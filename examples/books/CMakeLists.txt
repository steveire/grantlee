project(books_grantlee)

cmake_minimum_required(VERSION 3.0)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

get_property(Grantlee_PLUGIN_DIR TARGET Grantlee5::defaulttags PROPERTY LOCATION)
get_filename_component(Grantlee_PLUGIN_DIR ${Grantlee_PLUGIN_DIR} PATH)
get_filename_component(Grantlee_PLUGIN_DIR ${Grantlee_PLUGIN_DIR} PATH)
get_filename_component(Grantlee_PLUGIN_DIR ${Grantlee_PLUGIN_DIR} PATH)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

add_executable(books
  main.cpp
  bookwindow.cpp
  bookwrapper.cpp
  bookdelegate.cpp
  books.qrc
)

target_link_libraries(
  books
  Grantlee::Templates
  Qt5::Widgets
  Qt5::Sql

)
