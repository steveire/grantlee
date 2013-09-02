project(contacts_grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Grantlee REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${QT_DEFINITIONS})

include_directories(
  ${QT_INCLUDES}
  ${Grantlee_INCLUDE_DIRS}
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
  Qt4::QtGui
  Qt4::QtWebKit
  ${Grantlee_CORE_LIBRARIES}
)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${PROJECT_SOURCE_DIR}/../../scripts" "${PROJECT_SOURCE_DIR}/scripts")

set( LINGUIST_TS_ARGS COMMAND "${CMAKE_SOURCE_DIR}/scripts/extract_strings_linguist.py"
  "${CMAKE_SOURCE_DIR}/themes/linguist/bashful/*.html"
  "${CMAKE_SOURCE_DIR}/themes/linguist/sneezy/*.html"
  "${CMAKE_SOURCE_DIR}/themes/linguist/sleepy/*.html"
  "> ${CMAKE_BINARY_DIR}/output.cpp"
  COMMAND "${QT_LUPDATE_EXECUTABLE}" "${CMAKE_BINARY_DIR}/output.cpp" "-ts" )

set (CONTACTS_LOCALES "fr_FR" "de_DE" )

foreach(CONTACT_LOCALE ${CONTACTS_LOCALES})
    set( LINGUIST_TS_ARGS ${LINGUIST_TS_ARGS} "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.ts" )
endforeach()

add_custom_target(linguist_ts
  ${LINGUIST_TS_ARGS}
)

set( LINGUIST_QM_ARGS COMMAND "${QT_LRELEASE_EXECUTABLE}" )

foreach(CONTACT_LOCALE ${CONTACTS_LOCALES})
    set( _LINGUIST_QM_ARGS ${_LINGUIST_QM_ARGS} ${LINGUIST_QM_ARGS} "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.ts" "-qm" "${CMAKE_BINARY_DIR}/contacts_${CONTACT_LOCALE}.qm" )
endforeach()

add_custom_target(linguist_qm
  ${_LINGUIST_QM_ARGS}
)

find_package(KDE4)

if (KDE4_FOUND)
  set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

  find_package(Gettext)
  add_executable(contacts_kde
    main_kde.cpp
    kdelocalizer.cpp
    ${contacts_example_SRCS}
  )

  include_directories(
    ${KDE_INCLUDES}
  )

  target_link_libraries(
    contacts_kde
    Qt4::QtGui
    Qt4::QtWebKit
    ${KDE4_KIO_LIBS}
    ${KDE4_KDEUI_LIBS}

    ${Grantlee_CORE_LIBRARIES}
  )

  set( GETTEXT_PO_ARGS COMMAND "${CMAKE_SOURCE_DIR}/scripts/extract_strings_gettext.py"
    "${CMAKE_SOURCE_DIR}/themes/gettext/bashful/*.html"
    "${CMAKE_SOURCE_DIR}/themes/gettext/sneezy/*.html"
    "${CMAKE_SOURCE_DIR}/themes/gettext/sleepy/*.html"
    "> ${CMAKE_BINARY_DIR}/output.cpp"
                        COMMAND "${GETTEXT_XGETTEXT_EXECUTABLE}" "${CMAKE_BINARY_DIR}/output.cpp" "-o" ${CMAKE_SOURCE_DIR}/contacts.pot )

  foreach(CONTACT_LOCALE ${CONTACTS_LOCALES})
      set( GETTEXT_PO_ARGS ${GETTEXT_PO_ARGS} COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/contacts.pot" "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.po" )
  endforeach()

  add_custom_target(gettext_po
    ${GETTEXT_PO_ARGS}
  )

  foreach(CONTACT_LOCALE ${CONTACTS_LOCALES})

    set (GETTEXT_MO_ARGS ${GETTEXT_MO_ARGS}
      COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --quiet --update --backup=none -s "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.po" "${CMAKE_SOURCE_DIR}/contacts.pot"
      COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${CONTACT_LOCALE}/LC_MESSAGES/"
      COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o "${CMAKE_BINARY_DIR}/${CONTACT_LOCALE}/LC_MESSAGES/contacts_kde.mo" "${CMAKE_SOURCE_DIR}/${CONTACT_LOCALE}.po"
    )

    install(DIRECTORY "${CMAKE_BINARY_DIR}/${CONTACT_LOCALE}" DESTINATION share/locale)
  endforeach()

  add_custom_target(gettext_mo
    ${GETTEXT_MO_ARGS}
  )

endif()
