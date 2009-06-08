/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "removetags.h"
#include <QRegExp>


RemoveTagsFilter::RemoveTagsFilter( QObject* parent ): Filter( parent )
{

}


Grantlee::SafeString RemoveTagsFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QStringList tags = argument.rawString().split( " " );
  QString tagRe = QString( "(%1)" ).arg( tags.join( "|" ) );
  QRegExp startTag( QString( "<%1(/?>|(\\s+[^>]*>))" ).arg( tagRe ) );
  QRegExp endTag( QString( "</%1>" ).arg( tagRe ) );

  QString value = Util::getSafeString( input );
  value.replace( startTag, "" );
  value.replace( endTag, "" );
  return value;
}

#include "removetags.moc"

