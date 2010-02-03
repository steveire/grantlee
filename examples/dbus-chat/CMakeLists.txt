project(dbus_chat_grantlee)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/ )

find_package(Qt4 REQUIRED)
find_package(Automoc4 REQUIRED)
find_package(Grantlee REQUIRED)


add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_GUI_LIB)

include_directories(
  ${QT_INCLUDES}
  ${QT_QTGUI_INCLUDE_DIR}
  ${Grantlee_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}
)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set(dbus_chat_example_SRCS
  chat.cpp
  chatitem.cpp
  chat_adaptor.cpp
  chat_interface.cpp
)

set(dbus_chat_example_UI
  chatmainwindow.ui
  chatsetnickname.ui
)

QT4_WRAP_UI(dbus_chat_example_UI_SRCS
  ${dbus_chat_example_UI}
)

automoc4_add_executable(dbus_chat
  ${dbus_chat_example_SRCS}
  ${dbus_chat_example_UI_SRCS}
)

target_link_libraries(
  dbus_chat
  ${QT_QTCORE_LIBRARIES}
  ${QT_QTGUI_LIBRARIES}
  ${QT_QTDBUS_LIBRARIES}
  ${Grantlee_LIBRARY}
)



