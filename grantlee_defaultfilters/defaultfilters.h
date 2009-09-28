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

#ifndef DEFAULTFILTERS_H
#define DEFAULTFILTERS_H

#include "node.h"
#include "filter.h"
#include "interfaces/taglibraryinterface.h"

#include "datetime.h"
#include "integers.h"
#include "lists.h"
#include "logic.h"
#include "misc.h"
#include "strings.h"

#include "grantlee_export.h"

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

    filters.insert( "add", new AddFilter() );
    filters.insert( "addslashes", new AddSlashesFilter() );
    filters.insert( "capfirst", new CapFirstFilter() );
    filters.insert( "center", new CenterFilter() );
    filters.insert( "cut", new CutFilter() );
    filters.insert( "date", new DateFilter() );
    filters.insert( "default", new DefaultFilter() );
    filters.insert( "default_if_none", new DefaultIfNoneFilter() );
    filters.insert( "divisibleby", new DivisibleByFilter() );
    filters.insert( "escape", new EscapeFilter() );
    filters.insert( "escapejs", new EscapeJsFilter() );
    filters.insert( "first", new FirstFilter() );
    filters.insert( "fix_ampersands", new FixAmpersandsFilter() );
    filters.insert( "floatformat", new FloatFormatFilter() );
    filters.insert( "force_escape", new ForceEscapeFilter() );
    filters.insert( "get_digit", new GetDigitFilter() );
    filters.insert( "join", new JoinFilter() );
    filters.insert( "last", new LastFilter() );
    filters.insert( "length", new LengthFilter() );
    filters.insert( "length_is", new LengthIsFilter() );
    filters.insert( "linebreaks", new LineBreaksFilter() );
    filters.insert( "linebreaksbr", new LineBreaksBrFilter() );
    filters.insert( "linenumbers", new LineNumbersFilter() );
    filters.insert( "ljust", new LJustFilter() );
    filters.insert( "lower", new LowerFilter() );
    filters.insert( "make_list", new MakeListFilter() );
    filters.insert( "random", new RandomFilter() );
    filters.insert( "removetags", new RemoveTagsFilter() );
    filters.insert( "rjust", new RJustFilter() );
    filters.insert( "safe", new SafeFilter() );
    filters.insert( "safeseq", new SafeSequenceFilter() );
    filters.insert( "slice", new SliceFilter() );
    filters.insert( "slugify", new SlugifyFilter() );
    filters.insert( "stringformat", new StringFormatFilter() );
    filters.insert( "striptags", new StripTagsFilter() );
    filters.insert( "time", new TimeFilter() );
    filters.insert( "timesince", new TimeSinceFilter() );
    filters.insert( "timeuntil", new TimeUntilFilter() );
    filters.insert( "truncatewords", new TruncateWordsFilter() );
    filters.insert( "unordered_list", new UnorderedListFilter() );
    filters.insert( "upper", new UpperFilter() );
    filters.insert( "wordcount", new WordCountFilter() );
    filters.insert( "wordwrap", new WordWrapFilter() );
    filters.insert( "yesno", new YesNoFilter() );

    return filters;
  }
};

#endif

