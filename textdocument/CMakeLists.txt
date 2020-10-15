
if (GRANTLEE_BUILD_WITH_QT6)
    find_package(Qt6Gui 6.0.0 REQUIRED)
else()
    find_package(Qt5Gui 5.2.0 REQUIRED)
endif()

add_subdirectory(lib)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()
