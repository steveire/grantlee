
cmake_minimum_required(VERSION 2.8.11)

project(htmlapps)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include(${Grantlee_USE_FILE})

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

grantlee_add_plugin(customplugin
  customplugin
  TAGS
    rssfeed
)

target_link_libraries(customplugin
  ${Grantlee_TEMPLATES_LIBRARIES}
)
if (Grantlee_Qt5_TRANSITIONAL)
  find_package(Qt5 REQUIRED XmlPatterns Network)
  target_link_libraries(customplugin
    Qt5::XmlPatterns
    Qt5::Network
  )
else()
  target_link_libraries(customplugin
    Qt4::QtXmlPatterns
    Qt4::QtNetwork
  )
endif()

add_executable(htmlapps
  main.cpp
  mainwindow.cpp
  localrequestmanager.cpp
  templatereply.cpp
)
target_link_libraries(htmlapps
  ${Grantlee_TEMPLATES_LIBRARIES}
)
if (Grantlee_Qt5_TRANSITIONAL)
  find_package(Qt5WebKitWidgets REQUIRED)
  target_link_libraries(htmlapps
    Qt5::WebKitWidgets
  )
else()
  target_link_libraries(htmlapps
    Qt4::QtWebKit
  )
endif()
