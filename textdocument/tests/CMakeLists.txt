project(grantlee_textdocument_tests)

find_package(Qt5Test 5.2.0 REQUIRED)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

add_library(textdocument_test_builtins STATIC
  coverageobject.cpp
)
target_link_libraries(textdocument_test_builtins Qt5::Test)

macro(GRANTLEE_TEXTDOCUMENT_UNIT_TESTS)
  foreach(_testname ${ARGN})
    add_executable(${_testname}_exec ${_testname}.cpp)
    add_test(${_testname} ${_testname}_exec )
    target_link_libraries(${_testname}_exec Grantlee::TextDocument textdocument_test_builtins)
    set_property(GLOBAL APPEND PROPERTY TEST_COVERAGE "${CMAKE_CURRENT_BINARY_DIR}/${_testname}_exec" )
  endforeach(_testname)
endmacro(GRANTLEE_TEXTDOCUMENT_UNIT_TESTS)

grantlee_textdocument_unit_tests(
  htmlbuildertest
  plainmarkupbuildertest
)
