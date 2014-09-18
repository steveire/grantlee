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
  Q_PLUGIN_METADATA(IID "org.grantlee.TagLibraryInterface")
public:
  DefaultFiltersLibrary( QObject *parent = 0 )
      : QObject( parent ) {
  }

  virtual QHash<QString, Filter*> filters( const QString &name = QString() ) {
    Q_UNUSED( name );

    QHash<QString, Filter*> filters;

    filters.insert( QStringLiteral( "add" ), new AddFilter() );
    filters.insert( QStringLiteral( "addslashes" ), new AddSlashesFilter() );
    filters.insert( QStringLiteral( "capfirst" ), new CapFirstFilter() );
    filters.insert( QStringLiteral( "center" ), new CenterFilter() );
    filters.insert( QStringLiteral( "cut" ), new CutFilter() );
    filters.insert( QStringLiteral( "date" ), new DateFilter() );
    filters.insert( QStringLiteral( "default" ), new DefaultFilter() );
    filters.insert( QStringLiteral( "default_if_none" ), new DefaultIfNoneFilter() );
    filters.insert( QStringLiteral( "dictsort" ), new DictSortFilter() );
    filters.insert( QStringLiteral( "divisibleby" ), new DivisibleByFilter() );
    filters.insert( QStringLiteral( "escape" ), new EscapeFilter() );
    filters.insert( QStringLiteral( "escapejs" ), new EscapeJsFilter() );
    filters.insert( QStringLiteral( "first" ), new FirstFilter() );
    filters.insert( QStringLiteral( "fix_ampersands" ), new FixAmpersandsFilter() );
    filters.insert( QStringLiteral( "floatformat" ), new FloatFormatFilter() );
    filters.insert( QStringLiteral( "force_escape" ), new ForceEscapeFilter() );
    filters.insert( QStringLiteral( "get_digit" ), new GetDigitFilter() );
    filters.insert( QStringLiteral( "join" ), new JoinFilter() );
    filters.insert( QStringLiteral( "last" ), new LastFilter() );
    filters.insert( QStringLiteral( "length" ), new LengthFilter() );
    filters.insert( QStringLiteral( "length_is" ), new LengthIsFilter() );
    filters.insert( QStringLiteral( "linebreaks" ), new LineBreaksFilter() );
    filters.insert( QStringLiteral( "linebreaksbr" ), new LineBreaksBrFilter() );
    filters.insert( QStringLiteral( "linenumbers" ), new LineNumbersFilter() );
    filters.insert( QStringLiteral( "ljust" ), new LJustFilter() );
    filters.insert( QStringLiteral( "lower" ), new LowerFilter() );
    filters.insert( QStringLiteral( "make_list" ), new MakeListFilter() );
    filters.insert( QStringLiteral( "random" ), new RandomFilter() );
    filters.insert( QStringLiteral( "removetags" ), new RemoveTagsFilter() );
    filters.insert( QStringLiteral( "rjust" ), new RJustFilter() );
    filters.insert( QStringLiteral( "safe" ), new SafeFilter() );
    filters.insert( QStringLiteral( "safeseq" ), new SafeSequenceFilter() );
    filters.insert( QStringLiteral( "slice" ), new SliceFilter() );
    filters.insert( QStringLiteral( "slugify" ), new SlugifyFilter() );
    filters.insert( QStringLiteral( "stringformat" ), new StringFormatFilter() );
    filters.insert( QStringLiteral( "striptags" ), new StripTagsFilter() );
    filters.insert( QStringLiteral( "time" ), new TimeFilter() );
    filters.insert( QStringLiteral( "timesince" ), new TimeSinceFilter() );
    filters.insert( QStringLiteral( "timeuntil" ), new TimeUntilFilter() );
    filters.insert( QStringLiteral( "title" ), new TitleFilter() );
    filters.insert( QStringLiteral( "truncatewords" ), new TruncateWordsFilter() );
    filters.insert( QStringLiteral( "unordered_list" ), new UnorderedListFilter() );
    filters.insert( QStringLiteral( "upper" ), new UpperFilter() );
    filters.insert( QStringLiteral( "wordcount" ), new WordCountFilter() );
    filters.insert( QStringLiteral( "wordwrap" ), new WordWrapFilter() );
    filters.insert( QStringLiteral( "yesno" ), new YesNoFilter() );

    return filters;
  }
};

#endif
