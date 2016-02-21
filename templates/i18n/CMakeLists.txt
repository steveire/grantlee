project(grantlee_i18ntags)

add_library(grantlee_i18ntags MODULE
  i18ntags.cpp
  i18n.cpp
  i18nc.cpp
  i18np.cpp
  i18ncp.cpp
  l10n_money.cpp
  with_locale.cpp
)
set_property(TARGET grantlee_i18ntags PROPERTY EXPORT_NAME i18ntags)
target_link_libraries(grantlee_i18ntags Grantlee5::Templates)
target_compile_features(grantlee_i18ntags PRIVATE cxx_auto_type)
grantlee_adjust_plugin_name(grantlee_i18ntags)

install(TARGETS grantlee_i18ntags EXPORT grantlee_targets
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Templates
)
