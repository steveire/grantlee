
find_package(Qt5Core 5.2.0 REQUIRED)
find_package(Qt5Script 5.2.0)

add_subdirectory(lib)

if (BUILD_MAIN_PLUGINS)
  add_subdirectory(loadertags)
  add_subdirectory(defaulttags)

  add_subdirectory(defaultfilters)
endif()


if (BUILD_I18N_PLUGIN)
  add_subdirectory(i18n)
endif()

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()
