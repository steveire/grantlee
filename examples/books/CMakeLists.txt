cmake_minimum_required(VERSION 3.5)

project(books_grantlee)

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

if (GRANTLEE_BUILD_WITH_QT6)
    find_package(Qt6Widgets REQUIRED)
    find_package(Qt6Sql REQUIRED)
else()
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Sql REQUIRED)
endif()

add_executable(books
  main.cpp
  bookwindow.cpp
  bookwrapper.cpp
  bookdelegate.cpp
  books.qrc
)

target_link_libraries(
  books
  Grantlee5::Templates
)

if (GRANTLEE_BUILD_WITH_QT6)
    target_link_libraries(books
      Qt6::Widgets
      Qt6::Sql
    )
else()
    target_link_libraries(books
      Qt5::Widgets
      Qt5::Sql
    )
endif()
