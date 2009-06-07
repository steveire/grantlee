/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
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
