project(grantlee_textdocument_tests)

include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro(GRANTLEE_TEXTDOCUMENT_UNIT_TESTS)
  foreach(_testname ${ARGN})
    set(_testSrcs ${_testname}.cpp)
    set(moc_output_file "${CMAKE_CURRENT_BINARY_DIR}/${_testname}.moc")
    qt4_generate_moc(${_testname}.cpp ${moc_output_file})
    add_executable(${_testname}_exec ${_testSrcs} ${moc_output_file} )
    add_test(${_testname} ${_testname}_exec )
    target_link_libraries(${_testname}_exec ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY} ${Grantlee_TEXTDOCUMENT_LIBRARY} )
  endforeach(_testname)
endmacro(GRANTLEE_TEXTDOCUMENT_UNIT_TESTS)

grantlee_textdocument_unit_tests(
  htmlbuildertest
  plainmarkupbuildertest
)
