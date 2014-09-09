project(books_grantlee)

cmake_minimum_required(VERSION 3.0)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

add_executable(books
  main.cpp
  bookwindow.cpp
  bookwrapper.cpp
  bookdelegate.cpp
  books.qrc
)

target_link_libraries(
  books
  ${Grantlee_TEMPLATES_LIBRARIES}
)

if (Grantlee_Qt5_TRANSITIONAL)
  find_package(Qt5 REQUIRED Sql Widgets)
  target_link_libraries(
    books
    Qt5::Widgets
    Qt5::Sql
  )
else()
  target_link_libraries(
    books
    Qt4::QtGui
    Qt4::QtSql
  )
endif()
