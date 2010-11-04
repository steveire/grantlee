/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef I18N_TAGS_H
#define I18N_TAGS_H

#include "taglibraryinterface.h"

#include "i18n.h"
#include "i18nc.h"
#include "i18np.h"
#include "i18ncp.h"
#include "l10n_money.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class I18nTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  I18nTagLibrary() {
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
    Q_UNUSED( name );

    QHash<QString, AbstractNodeFactory*> nodeFactories;
    nodeFactories.insert( QLatin1String( "i18n" ), new I18nNodeFactory() );
    nodeFactories.insert( QLatin1String( "i18n_var" ), new I18nVarNodeFactory() );
    nodeFactories.insert( QLatin1String( "i18nc" ), new I18ncNodeFactory() );
    nodeFactories.insert( QLatin1String( "i18nc_var" ), new I18ncVarNodeFactory() );
    nodeFactories.insert( QLatin1String( "i18np" ), new I18npNodeFactory() );
    nodeFactories.insert( QLatin1String( "i18np_var" ), new I18npVarNodeFactory() );
    nodeFactories.insert( QLatin1String( "i18ncp" ), new I18ncpNodeFactory() );
    nodeFactories.insert( QLatin1String( "i18ncp_var" ), new I18ncpVarNodeFactory() );
    nodeFactories.insert( QLatin1String( "l10n_money" ), new L10nMoneyNodeFactory() );
    nodeFactories.insert( QLatin1String( "l10n_money_var" ), new L10nMoneyVarNodeFactory() );
    // TODO: Compat and block tags
    return nodeFactories;
  }
};


#endif

