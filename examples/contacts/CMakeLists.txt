project(contacts_grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)
find_package(Grantlee REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_GUI_LIB)
add_definitions(-DQT_NO_KEYWORDS)

include_directories(
  ${QT_INCLUDES}
  ${QT_QTGUI_INCLUDE_DIR}
  ${QT_QTSQL_INCLUDE_DIR}
  ${Grantlee_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}
)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set(contacts_example_SRCS
  mainwindow.cpp
  contact.cpp
)

qt4_automoc( contacts_example_MOC_SRCS ${contacts_example_SRCS} )

add_executable(contacts_linguist
  main_linguist.cpp
  ${contacts_example_SRCS}
)

target_link_libraries(
  contacts_linguist
  ${QT_QTGUI_LIBRARIES}
  ${QT_QTWEBKIT_LIBRARIES}
  ${Grantlee_CORE_LIBRARIES}
)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${PROJECT_SOURCE_DIR}/../../scripts" "${PROJECT_SOURCE_DIR}/scripts")

set( LINGUIST_TS_ARGS COMMAND "${CMAKE_SOURCE_DIR}/scripts/extract_strings_linguist.py" "${CMAKE_BINARY_DIR}/output.cpp"
  "${CMAKE_SOURCE_DIR}/themes/linguist/bashful/*.html"
  "${CMAKE_SOURCE_DIR}/themes/linguist/sneezy/*.html"
  "${CMAKE_SOURCE_DIR}/themes/linguist/sleepy/*.html"
  COMMAND "${QT_LUPDATE_EXECUTABLE}" "${CMAKE_BINARY_DIR}/output.cpp" "-ts" )

set (CONTACTS_lOCALES "fr_FR" "de_DE" )

foreach(CONTACT_LOCALE ${CONTACTS_lOCALES})
    set( LINGUIST_TS_ARGS ${LINGUIST_TS_ARGS} "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.ts" )
endforeach()

add_custom_target(linguist_ts
  ${LINGUIST_TS_ARGS}
)

set( LINGUIST_QM_ARGS COMMAND "${QT_LRELEASE_EXECUTABLE}" )

foreach(CONTACT_LOCALE ${CONTACTS_lOCALES})
    set( _LINGUIST_QM_ARGS ${_LINGUIST_QM_ARGS} ${LINGUIST_QM_ARGS} "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.ts" "-qm" "${CMAKE_BINARY_DIR}/contacts_${CONTACT_LOCALE}.qm" )
endforeach()

add_custom_target(linguist_qm
  ${_LINGUIST_QM_ARGS}
)

