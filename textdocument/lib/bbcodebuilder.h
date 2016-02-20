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

#ifndef GRANTLEE_BBCODEBUILDER_H
#define GRANTLEE_BBCODEBUILDER_H

#include "abstractmarkupbuilder.h"

namespace Grantlee
{

/**
  @brief Builder to create BBCode from a QTextDocument
*/
class BBCodeBuilder : public AbstractMarkupBuilder
{
public:

  /**
    Creates a new BBCodeBuilder.
  */
  BBCodeBuilder();

  ~BBCodeBuilder() override;

  /* reimp */ void beginStrong() override;
  /* reimp */ void endStrong() override;
  /* reimp */ void beginEmph() override;
  /* reimp */ void endEmph() override;
  /* reimp */ void beginUnderline() override;
  /* reimp */ void endUnderline() override;
  /* reimp */ void beginStrikeout() override;
  /* reimp */ void endStrikeout() override;
  /* reimp */ void beginForeground( const QBrush &brush ) override;
  /* reimp */ void endForeground() override;

  // Background colour not supported by BBCode.

  /* reimp */ void beginAnchor( const QString &href = QString(), const QString &name = QString() ) override;
  /* reimp */ void endAnchor() override;

  // Font family not supported by BBCode.

  /**
    Begin an element of font size @p size. Note that this size is in pixels, and must be converted before
    it is suitable for use in BBCode.
    @param size The size of font to begin.
  */
  /* reimp */ void beginFontPointSize( int size ) override;
  /* reimp */ void endFontPointSize() override;

  /* reimp */ void beginParagraph( Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0 ) override;

  /* reimp */ void endParagraph() override;
  /* reimp */ void addNewline() override;

  /* reimp */ void insertImage( const QString &src, qreal width, qreal height ) override;

  /* reimp */ void beginList( QTextListFormat::Style type ) override;

  /* reimp */ void endList() override;


  /* reimp */ void beginListItem() override;

  /* reimp */ void beginSuperscript() override;

  /* reimp */ void endSuperscript() override;

  /* reimp */ void beginSubscript() override;

  /* reimp */ void endSubscript() override;


  /* reimp */ void beginTable( qreal, qreal, const QString & ) override;

  /* reimp */ void beginTableRow() override;


  /* reimp */ void appendLiteralText( const QString &text ) override;

  /**
    Escapes @p text appropriately for BBCode.
  */
  const QString escape( const QString &text ) const;

  /* reimp */ QString getResult() override;

private:
  QList<QTextListFormat::Style> m_currentListItemStyles;

  QString m_text;

  Qt::Alignment m_currentAlignment;

};

}

#endif
