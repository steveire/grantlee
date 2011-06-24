
find_package(Qt4 4.5.0 REQUIRED QtCore QtScript QtTest)

include(${Grantlee_USE_FILE})

add_subdirectory(lib)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

if (BUILD_MAIN_PLUGINS)
  add_subdirectory(loadertags)
  add_subdirectory(defaulttags)

  add_subdirectory(defaultfilters)

  add_subdirectory(mutabletags)
endif()


if (BUILD_I18N_PLUGIN)
  add_subdirectory(i18n)
endif()


if (BUILD_SCRIPT_PLUGIN)
#  add_subdirectory(scriptabletags)
endif()

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()
