project(grantlee_i18ntags)

include(${Grantlee_USE_FILE})

include_directories(
  ${QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../corelib
)

grantlee_add_plugin(grantlee_i18ntags
  i18ntags

  TAGS
   i18n
   i18nc
   i18np
   i18ncp
   l10n_money
   with_locale
)

install(TARGETS grantlee_i18ntags
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
