/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "lower.h"

LowerFilter::LowerFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString LowerFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).rawString().toLower();
}

#include "lower.moc"