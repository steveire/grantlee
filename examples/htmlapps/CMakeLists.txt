
cmake_minimum_required(VERSION 2.6)

project(htmlapps)

find_package(Qt4 REQUIRED)
find_package(Grantlee REQUIRED)

include(${Grantlee_USE_FILE})

add_definitions(${QT_DEFINITIONS})

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}
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
  ${QT_QTXMLPATTERNS_LIBRARIES}
  ${QT_QTNETWORK_LIBRARIES}
)

add_executable(htmlapps
  ${htmlapps_SRCS}
)

target_link_libraries(htmlapps
  ${Grantlee_TEMPLATES_LIBRARIES}
  ${QT_QTCORE_LIBRARIES}
  ${QT_QTGUI_LIBRARIES}
  ${QT_QTWEBKIT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARIES}
)
