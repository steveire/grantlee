cmake_minimum_required(VERSION 3.5)

project(contacts_grantlee)

find_package(Grantlee5 REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

add_executable(contacts_linguist
  main_linguist.cpp
  mainwindow.cpp
  contact.cpp
)
target_link_libraries(
  contacts_linguist
  Grantlee5::Templates
)

find_package(Qt5WebKitWidgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
target_link_libraries(
  contacts_linguist
  Qt5::WebKitWidgets
)
set(lupdate_exe Qt5::lupdate)
set(lrelease_exe Qt5::lrelease)

set( LINGUIST_TS_ARGS COMMAND "${CMAKE_SOURCE_DIR}/../../scripts/extract_strings_linguist.py"
  "${CMAKE_SOURCE_DIR}/themes/linguist/bashful/*.html"
  "${CMAKE_SOURCE_DIR}/themes/linguist/sneezy/*.html"
  "${CMAKE_SOURCE_DIR}/themes/linguist/sleepy/*.html"
  ">" "${CMAKE_BINARY_DIR}/output.cpp"
  COMMAND ${lupdate_exe} "${CMAKE_BINARY_DIR}/output.cpp" "-ts" )

set (CONTACTS_LOCALES "fr_FR" "de_DE" )

foreach(CONTACT_LOCALE ${CONTACTS_LOCALES})
    list(APPEND LINGUIST_TS_ARGS "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.ts" )
endforeach()

add_custom_target(linguist_ts
  ${LINGUIST_TS_ARGS}
    VERBATIM
)

foreach(CONTACT_LOCALE ${CONTACTS_LOCALES})
    list(APPEND _LINGUIST_QM_ARGS COMMAND ${lrelease_exe} "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.ts" "-qm" "${CMAKE_BINARY_DIR}/contacts_${CONTACT_LOCALE}.qm" )
endforeach()

add_custom_target(linguist_qm
  ${_LINGUIST_QM_ARGS}
)
