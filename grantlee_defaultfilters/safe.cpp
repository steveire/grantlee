/*
Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "safe.h"

#include "util_p.h"

SafeFilter::SafeFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString SafeFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::markSafe( Util::getSafeString( input ) );
}
