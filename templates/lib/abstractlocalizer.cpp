/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "abstractlocalizer.h"

#include "util.h"

#include <QtCore/QDateTime>

using namespace Grantlee;

AbstractLocalizer::AbstractLocalizer()
{

}

AbstractLocalizer::~AbstractLocalizer()
{

}

QString AbstractLocalizer::localize( const QVariant& variant ) const
{
  if ( variant.userType() == qMetaTypeId<QDate>() )
    return localizeDate( variant.toDate() );
  if ( variant.userType() == qMetaTypeId<QTime>() )
    return localizeTime( variant.toTime() );
  if ( variant.userType() == qMetaTypeId<QDateTime>() )
    return localizeDateTime( variant.toDateTime() );
  else if ( isSafeString( variant ) )
    return localizeString( getSafeString( variant ).get() );
  else if ( variant.userType() == qMetaTypeId<double>() || variant.userType() == qMetaTypeId<float>() )
    return localizeNumber( variant.toReal() );
  else if ( variant.canConvert<int>() )
    return localizeNumber( variant.toInt() );
  return QString();
}
