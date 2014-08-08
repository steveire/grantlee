
cmake_minimum_required(VERSION 2.8.11)

project(htmlapps)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

add_library(customplugin MODULE
  customplugin.cpp
  rssfeed.cpp
)
grantlee_adjust_plugin_name(customplugin)

find_package(Qt5 REQUIRED XmlPatterns Network)

target_link_libraries(customplugin
  ${Grantlee_TEMPLATES_LIBRARIES}
  Qt5::XmlPatterns
  Qt5::Network
)

add_executable(htmlapps
  main.cpp
  mainwindow.cpp
  localrequestmanager.cpp
  templatereply.cpp
)

find_package(Qt5WebKitWidgets REQUIRED)

target_link_libraries(htmlapps
  ${Grantlee_TEMPLATES_LIBRARIES}
  Qt5::WebKitWidgets
)
