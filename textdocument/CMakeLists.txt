
if (Grantlee_Qt5_TRANSITIONAL)
  find_package(Qt5Gui 5.2.0 REQUIRED)
else()
  find_package(Qt4 4.5.0 REQUIRED QtGui)
endif()

add_subdirectory(lib)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()
