/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "slice.h"


SliceFilter::SliceFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString SliceFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  int splitterIndex = argument.rawString().indexOf( ":" );
  QString inputString = Util::getSafeString( input ).rawString();
  if ( splitterIndex >= 0 ) {
    int left = QVariant( argument.rawString().left( splitterIndex ) ).toInt();
    int right = QVariant( argument.rawString().right( splitterIndex ) ).toInt();
    if ( right < 0 ) {
      right = inputString.size() + right;
    }
    return inputString.mid( left, right );
  } else {
    return QString( inputString.at( QVariant( argument ).toInt() ) );
  }
}

#include "slice.moc"

