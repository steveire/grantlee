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

#include "filterexpression.h"

#include "parser.h"
#include "filter.h"
#include "util_p.h"
#include "exception.h"

namespace Grantlee
{
class FilterExpressionPrivate
{
  FilterExpressionPrivate( FilterExpression *fe )
      : q_ptr( fe ) {
  }

  Variable m_variable;
  QList<ArgFilter> m_filters;
  QStringList m_filterNames;

  Q_DECLARE_PUBLIC( FilterExpression )
  FilterExpression *q_ptr;
};

}

using namespace Grantlee;

static const char * FILTER_SEPARATOR = "|";
static const char * FILTER_ARGUMENT_SEPARATOR = ":";
static const char * ALLOWED_VARIABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_\\.";

static const char * varChars = "\\w\\.";
static const char * numChars = "[-+\\.]?\\d[\\d\\.e]*";
static const QString filterSep( QRegExp::escape( FILTER_SEPARATOR ) );
static const QString argSep( QRegExp::escape( FILTER_ARGUMENT_SEPARATOR ) );
static const QString i18nOpen( QRegExp::escape( "_(" ) );
static const QString i18nClose( QRegExp::escape( ")" ) );

static const QString constantString = QString(
                                        "(?:%3%1%4|"
                                        "%3%2%4|"
                                        "%1|"
                                        "%2)"
                                      )
                                      .arg( "\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\"" )
                                      .arg( "\'[^\'\\\\]*(?:\\\\.[^\'\\\\]*)*\'" )
                                      .arg( i18nOpen )
                                      .arg( i18nClose );

static const QString filterRawString = QString(
                                         "^%1|"                      /* Match "strings" and 'strings' incl i18n */
                                         "^[%2]+|"                   /* Match variables */
                                         "%3|"                       /* Match numbers */
                                         "%4\\w+|"                   /* Match filters */
                                         "%5(?:%1|[%2]+|%3|%4\\w+)"  /* Match arguments to filters, which may be strings, */
                                       )                             /* variables, numbers or another filter in the chain */
                                       /* 1 */  .arg( constantString )
                                       /* 2 */  .arg( varChars )
                                       /* 3 */  .arg( numChars )
                                       /* 4 */  .arg( filterSep )
                                       /* 5 */  .arg( argSep );

static const QRegExp sFilterRe( filterRawString );

FilterExpression::FilterExpression( const QString &varString, Parser *parser )
    : d_ptr( new FilterExpressionPrivate( this ) )
{
  Q_D( FilterExpression );

  int pos = 0;
  int lastPos = 0;
  int len;
  QString subString;

  QString vs = varString;

  // This is one fo the few contstructors that can throw so we make sure to delete its d->pointer.
  try {
    while ( ( pos = sFilterRe.indexIn( vs, pos ) ) != -1 ) {
      len = sFilterRe.matchedLength();
      subString = vs.mid( pos, len );
      int ssSize = subString.size();

      if ( pos != lastPos ) {
        throw Grantlee::Exception( TagSyntaxError,
            QString( "Could not parse some characters: \"%1\"" ).arg( vs.mid( lastPos, pos ) ) );
      }

      if ( subString.startsWith( FILTER_SEPARATOR ) ) {
        subString = subString.right( ssSize - 1 );
        Filter::Ptr f = parser->getFilter( subString );

        Q_ASSERT( f );

        d->m_filterNames << subString;
        d->m_filters << qMakePair<Filter::Ptr, Variable>( f, Variable() );

      } else if ( subString.startsWith( FILTER_ARGUMENT_SEPARATOR ) ) {
        subString = subString.right( ssSize - 1 );
        int lastFilter = d->m_filters.size();
        if ( subString.isEmpty() )
          throw Grantlee::Exception( EmptyVariableError,
              QString( "Missing argument to filter: %1" ).arg( d->m_filterNames[lastFilter -1] ) );

        d->m_filters[lastFilter -1].second = Variable( subString );
      } else {
        if ( subString.contains( "._" ) || ( subString.startsWith( "_" ) && !subString.startsWith( "_(" ) ) ) {
          throw Grantlee::Exception( TagSyntaxError,
              QString( "Variables and attributes may not begin with underscores: %1" ).arg( subString ) );
        }
        // Token is _("translated"), or "constant", or a variable;
        d->m_variable = Variable( subString );
      }

      pos += len;
      lastPos = pos;
    }

    QString remainder = vs.right( vs.size() - lastPos );
    if ( !remainder.isEmpty() ) {
      throw Grantlee::Exception( TagSyntaxError,
          QString( "Could not parse the remainder, %1 from %2" ).arg( remainder ).arg( varString ) );
    }
  } catch (...)
  {
    delete d_ptr;
    throw;
  }
}

FilterExpression::FilterExpression( const FilterExpression &other )
    : d_ptr( new FilterExpressionPrivate( this ) )
{
  d_ptr->m_variable = other.d_ptr->m_variable;
  d_ptr->m_filters = other.d_ptr->m_filters;
  d_ptr->m_filterNames = other.d_ptr->m_filterNames;
}

FilterExpression::FilterExpression()
    : d_ptr( new FilterExpressionPrivate( this ) )
{
}

bool FilterExpression::isValid() const
{
  Q_D( const FilterExpression );
  return d->m_variable.isValid();
}

FilterExpression::~FilterExpression()
{
  delete d_ptr;
}

Variable FilterExpression::variable() const
{
  Q_D( const FilterExpression );
  return d->m_variable;
}

FilterExpression &FilterExpression::operator=( const FilterExpression & other )
{
  d_ptr->m_variable = other.d_ptr->m_variable;
  d_ptr->m_filters = other.d_ptr->m_filters;
  return *this;
}

QVariant FilterExpression::resolve( Context *c ) const
{
  Q_D( const FilterExpression );
  QVariant var = d->m_variable.resolve( c );

  foreach( ArgFilter argfilter, d->m_filters ) {
    Filter::Ptr filter = argfilter.first;
    Variable argVar = argfilter.second;
    QVariant arg = argVar.resolve( c );

    if (arg.isValid())
    {
      Grantlee::SafeString argString;
      if ( arg.userType() == qMetaTypeId<Grantlee::SafeString>() ) {
        argString = arg.value<Grantlee::SafeString>();
      } else if ( arg.type() == QVariant::String ) {
        argString = Grantlee::SafeString( arg.toString() );
      }
      if ( argVar.isConstant() ) {
        argString = Util::markSafe( argString );
      }
      if (!argString.isEmpty())
      {
        arg = argString;
      }
    }

    SafeString varString = Util::getSafeString( var );
    if ( filter->needsAutoescape() ) {
      var = filter->doFilter( var, arg, c->autoescape() );
    } else {
      var = filter->doFilter( var, arg );
    }
    if ( var.userType() == qMetaTypeId<Grantlee::SafeString>() || var.type() == QVariant::String )
    {
      if ( filter->isSafe() && varString.isSafe() ) {
        var = Util::markSafe( Util::getSafeString( var ) );
      } else if ( varString.needsEscape() ) {
        var = Util::markForEscaping( Util::getSafeString( var ) );
      } else {
        var = Util::getSafeString( var );
      }
    }
  }

  return var;
}

QVariantList FilterExpression::toList( Context *c ) const
{
  QVariant var = resolve( c );
  return Util::variantToList( var );
}

bool FilterExpression::isTrue( Context *c ) const
{
  return Util::variantIsTrue( resolve( c ) );
}

QStringList FilterExpression::filters() const
{
  Q_D( const FilterExpression );
  return d->m_filterNames;
}
