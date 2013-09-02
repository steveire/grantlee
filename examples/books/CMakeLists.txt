project(books_grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Grantlee REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_GUI_LIB)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

include_directories(
  ${QT_INCLUDES}
  ${Grantlee_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}
)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set(books_example_SRCS
  main.cpp
  bookwindow.cpp
  bookwrapper.cpp
  bookdelegate.cpp
)

set(books_example_RCSS
  books.qrc
)

set(books_example_UI
  bookwindow.ui
)

qt4_automoc( books_example_MOC_SRCS ${books_example_SRCS} )

QT4_WRAP_UI(books_example_UI_SRCS ${books_example_UI})

qt4_add_resources(
  books_example_RCS_SRCS
  ${books_example_RCSS}
)

add_executable(books
  ${books_example_SRCS}
  ${books_example_RCS_SRCS}
  ${books_example_UI_SRCS}
)

target_link_libraries(
  books
  Qt4::QtGui
  Qt4::QtSql
  ${Grantlee_CORE_LIBRARIES}
)



