/*
  This file is part of the Grantlee template system.

  Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_MEDIAWIKIMARKUPBUILDER_H
#define GRANTLEE_MEDIAWIKIMARKUPBUILDER_H

#include "abstractmarkupbuilder.h"

namespace Grantlee
{

/**
  @brief Creates MediaWiki markup from a QTextDocument
*/
class MediaWikiMarkupBuilder : public AbstractMarkupBuilder
{
public:

  /**
    Creates a new MediaWikiMarkupBuilder
  */
  MediaWikiMarkupBuilder();
  ~MediaWikiMarkupBuilder() override;

  /* reimp */ void beginStrong() override;
  /* reimp */ void endStrong() override;
  /* reimp */ void beginEmph() override;
  /* reimp */ void endEmph() override;
  /* reimp */ void beginUnderline() override;
  /* reimp */ void endUnderline() override;
  /* reimp */ void beginStrikeout() override;
  /* reimp */ void endStrikeout() override;

  /* reimp */ void endParagraph() override;
  /* reimp */ void addNewline() override;

  /* reimp */ void beginAnchor( const QString &href = QString(), const QString &name = QString() ) override;
  /* reimp */ void endAnchor() override;

  /* reimp */ void beginHeader( int level ) override;
  /* reimp */ void endHeader( int level ) override;

  /* reimp */ void beginList( QTextListFormat::Style type ) override;

  /* reimp */ void endList() override;

  /* reimp */ void beginListItem() override;
  /* reimp */ void endListItem() override;

  /* reimp */ void appendLiteralText( const QString &text ) override;

  /**
    Escapes @p text appropriately for MediaWiki.
  */
  const QString escape( const QString &s );

  /* reimp */ QString getResult() override;

private:
  QList<QTextListFormat::Style> currentListItemStyles;

  QString m_text;
};

}

#endif
