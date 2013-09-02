
cmake_minimum_required(VERSION 2.6)

project(htmlapps)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(${Grantlee_USE_FILE})

include_directories(
  ${QT_INCLUDES}
)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set(htmlapps_SRCS
  main.cpp
  mainwindow.cpp
  localrequestmanager.cpp
  templatereply.cpp
)

qt4_automoc( htmlapps_MOC_SRCS ${htmlapps_SRCS} )

grantlee_add_plugin(customplugin
  customplugin
  TAGS
    rssfeed
)

target_link_libraries(customplugin
  ${Grantlee_CORE_LIBRARIES}
  Qt4::QtXmlPatterns
  Qt4::QtNetwork
)

add_executable(htmlapps
  ${htmlapps_SRCS}
)

target_link_libraries(htmlapps
  ${Grantlee_TEMPLATES_LIBRARIES}
  Qt4::QtCore
  Qt4::QtGui
  Qt4::QtWebKit
  Qt4::QtNetwork
)
