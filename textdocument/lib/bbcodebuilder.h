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

  void beginStrong() override;
  void endStrong() override;
  void beginEmph() override;
  void endEmph() override;
  void beginUnderline() override;
  void endUnderline() override;
  void beginStrikeout() override;
  void endStrikeout() override;
  void beginForeground( const QBrush &brush ) override;
  void endForeground() override;

  // Background colour not supported by BBCode.

  void beginAnchor( const QString &href = QString(), const QString &name = QString() ) override;
  void endAnchor() override;

  // Font family not supported by BBCode.

  /**
    Begin an element of font size @p size. Note that this size is in pixels, and must be converted before
    it is suitable for use in BBCode.
    @param size The size of font to begin.
  */
  void beginFontPointSize( int size ) override;
  void endFontPointSize() override;

  void beginParagraph( Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0 ) override;

  void endParagraph() override;
  void addNewline() override;

  void insertImage( const QString &src, qreal width, qreal height ) override;

  void beginList( QTextListFormat::Style type ) override;

  void endList() override;


  void beginListItem() override;

  void beginSuperscript() override;

  void endSuperscript() override;

  void beginSubscript() override;

  void endSubscript() override;


  void beginTable( qreal, qreal, const QString & ) override;

  void beginTableRow() override;


  void appendLiteralText( const QString &text ) override;

  /**
    Escapes @p text appropriately for BBCode.
  */
  const QString escape( const QString &text ) const;

  QString getResult() override;

private:
  QList<QTextListFormat::Style> m_currentListItemStyles;

  QString m_text;

  Qt::Alignment m_currentAlignment;

};

}

#endif
