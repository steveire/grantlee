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

  virtual ~BBCodeBuilder();

  /* reimp */ void beginStrong();
  /* reimp */ void endStrong();
  /* reimp */ void beginEmph();
  /* reimp */ void endEmph();
  /* reimp */ void beginUnderline();
  /* reimp */ void endUnderline();
  /* reimp */ void beginStrikeout();
  /* reimp */ void endStrikeout();
  /* reimp */ void beginForeground( const QBrush &brush );
  /* reimp */ void endForeground();

  // Background colour not supported by BBCode.

  /* reimp */ void beginAnchor( const QString &href = QString(), const QString &name = QString() );
  /* reimp */ void endAnchor();

  // Font family not supported by BBCode.

  /**
    Begin an element of font size @p size. Note that this size is in pixels, and must be converted before
    it is suitable for use in BBCode.
    @param size The size of font to begin.
  */
  /* reimp */ void beginFontPointSize( int size );
  /* reimp */ void endFontPointSize();

  /* reimp */ void beginParagraph( Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0 );

  /* reimp */ void endParagraph();
  /* reimp */ void addNewline();

  /* reimp */ void insertImage( const QString &src, qreal width, qreal height );

  /* reimp */ void beginList( QTextListFormat::Style type );

  /* reimp */ void endList();


  /* reimp */ void beginListItem();

  /* reimp */ void beginSuperscript();

  /* reimp */ void endSuperscript();

  /* reimp */ void beginSubscript();

  /* reimp */ void endSubscript();


  /* reimp */ void beginTable( qreal, qreal, const QString & );

  /* reimp */ void beginTableRow();


  /* reimp */ void appendLiteralText( const QString &text );

  /**
    Escapes @p text appropriately for BBCode.
  */
  const QString escape( const QString &text ) const;

  /* reimp */ QString getResult();

private:
  QList<QTextListFormat::Style> m_currentListItemStyles;

  QString m_text;

  Qt::Alignment m_currentAlignment;

};

}

#endif
