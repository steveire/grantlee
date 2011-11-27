/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef DEFAULTFILTERS_H
#define DEFAULTFILTERS_H

#include "datetime.h"
#include "integers.h"
#include "lists.h"
#include "logic.h"
#include "misc.h"
#include "stringfilters.h"

#include "taglibraryinterface.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class DefaultFiltersLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  DefaultFiltersLibrary( QObject *parent = 0 )
      : QObject( parent ) {
  }

  virtual QHash<QString, Filter*> filters( const QString &name = QString() ) {
    Q_UNUSED( name );

    QHash<QString, Filter*> filters;

    filters.insert( QLatin1String( "add" ), new AddFilter() );
    filters.insert( QLatin1String( "addslashes" ), new AddSlashesFilter() );
    filters.insert( QLatin1String( "capfirst" ), new CapFirstFilter() );
    filters.insert( QLatin1String( "center" ), new CenterFilter() );
    filters.insert( QLatin1String( "cut" ), new CutFilter() );
    filters.insert( QLatin1String( "date" ), new DateFilter() );
    filters.insert( QLatin1String( "default" ), new DefaultFilter() );
    filters.insert( QLatin1String( "default_if_none" ), new DefaultIfNoneFilter() );
    filters.insert( QLatin1String( "dictsort" ), new DictSortFilter() );
    filters.insert( QLatin1String( "divisibleby" ), new DivisibleByFilter() );
    filters.insert( QLatin1String( "escape" ), new EscapeFilter() );
    filters.insert( QLatin1String( "escapejs" ), new EscapeJsFilter() );
    filters.insert( QLatin1String( "first" ), new FirstFilter() );
    filters.insert( QLatin1String( "fix_ampersands" ), new FixAmpersandsFilter() );
    filters.insert( QLatin1String( "floatformat" ), new FloatFormatFilter() );
    filters.insert( QLatin1String( "force_escape" ), new ForceEscapeFilter() );
    filters.insert( QLatin1String( "get_digit" ), new GetDigitFilter() );
    filters.insert( QLatin1String( "join" ), new JoinFilter() );
    filters.insert( QLatin1String( "last" ), new LastFilter() );
    filters.insert( QLatin1String( "length" ), new LengthFilter() );
    filters.insert( QLatin1String( "length_is" ), new LengthIsFilter() );
    filters.insert( QLatin1String( "linebreaks" ), new LineBreaksFilter() );
    filters.insert( QLatin1String( "linebreaksbr" ), new LineBreaksBrFilter() );
    filters.insert( QLatin1String( "linenumbers" ), new LineNumbersFilter() );
    filters.insert( QLatin1String( "ljust" ), new LJustFilter() );
    filters.insert( QLatin1String( "lower" ), new LowerFilter() );
    filters.insert( QLatin1String( "make_list" ), new MakeListFilter() );
    filters.insert( QLatin1String( "random" ), new RandomFilter() );
    filters.insert( QLatin1String( "removetags" ), new RemoveTagsFilter() );
    filters.insert( QLatin1String( "rjust" ), new RJustFilter() );
    filters.insert( QLatin1String( "safe" ), new SafeFilter() );
    filters.insert( QLatin1String( "safeseq" ), new SafeSequenceFilter() );
    filters.insert( QLatin1String( "slice" ), new SliceFilter() );
    filters.insert( QLatin1String( "slugify" ), new SlugifyFilter() );
    filters.insert( QLatin1String( "stringformat" ), new StringFormatFilter() );
    filters.insert( QLatin1String( "striptags" ), new StripTagsFilter() );
    filters.insert( QLatin1String( "time" ), new TimeFilter() );
    filters.insert( QLatin1String( "timesince" ), new TimeSinceFilter() );
    filters.insert( QLatin1String( "timeuntil" ), new TimeUntilFilter() );
    filters.insert( QLatin1String( "title" ), new TitleFilter() );
    filters.insert( QLatin1String( "truncatewords" ), new TruncateWordsFilter() );
    filters.insert( QLatin1String( "unordered_list" ), new UnorderedListFilter() );
    filters.insert( QLatin1String( "upper" ), new UpperFilter() );
    filters.insert( QLatin1String( "wordcount" ), new WordCountFilter() );
    filters.insert( QLatin1String( "wordwrap" ), new WordWrapFilter() );
    filters.insert( QLatin1String( "yesno" ), new YesNoFilter() );

    return filters;
  }
};

#endif

