project(grantleetests)

enable_testing()

include_directories( ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro(GRANTLEE_UNIT_TESTS)
  foreach(_testname ${ARGN})
    automoc4_add_executable(${_testname}_exec ${_testname}.cpp )
    add_test(${_testname} ${_testname}_exec )
    target_link_libraries(${_testname}_exec ${QT_QTTEST_LIBRARY} grantlee_core_library)
  endforeach(_testname)
endmacro(GRANTLEE_UNIT_TESTS)

grantlee_unit_tests(
  testbuiltins
  testloadertags
  testdefaulttags
)



