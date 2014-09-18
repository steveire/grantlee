project(grantlee_i18ntags)

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

install(TARGETS grantlee_i18ntags EXPORT grantlee_targets
         LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Templates
)
