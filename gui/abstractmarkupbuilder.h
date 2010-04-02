/*
  This file is part of the Grantlee template system.

  Copyright (c) 2008,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_ABSTRACTMARKUPBUILDER_H
#define GRANTLEE_ABSTRACTMARKUPBUILDER_H

#include <QtCore/QString>
#include <QtGui/QTextListFormat>

class QBrush;

#include "grantlee_gui_export.h"

namespace Grantlee
{

class AbstractMarkupBuilderPrivate;

/// @headerfile abstractmarkupbuilder.h grantlee/abstractmarkupbuilder.h

/**
  @brief The AbstractMarkupBuilder class serves as a base class for creating marked up plain text output.

  The AbstractMarkupBuilder is used by a MarkupDirector to create marked up output such as html or markdown.

  See PlainTextMarkupBuilder and TextHTMLBuilder for example implementations.

  This interface can be extended to handle custom format types in a QTextDocument. @see @ref custom_qtextobject

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_GUI_EXPORT AbstractMarkupBuilder
{
public:
  /** Destructor */
  virtual ~AbstractMarkupBuilder() {}

  /** Begin a bold element in the markup */
  virtual void beginStrong() = 0;

  /** Close the bold element in the markup */
  virtual void endStrong() = 0;

  /** Begin an emphasised element in the markup */
  virtual void beginEmph() = 0;

  /** Close the emphasised element in the markup */
  virtual void endEmph() = 0;

  /** Begin an underlined element in the markup */
  virtual void beginUnderline() = 0;

  /** Close the underlined element in the markup */
  virtual void endUnderline() = 0;

  /** Begin a struck out element in the markup */
  virtual void beginStrikeout() = 0;

  /** Close the struck out element in the markup */
  virtual void endStrikeout() = 0;

  /** Begin a decorarated foreground element in the markup (A text color) using @p brush */
  virtual void beginForeground( const QBrush &brush ) = 0;

  /** Close the decorarated foreground element in the markup */
  virtual void endForeground() = 0;

  /** Begin a decorarated background element in the markup (A text background color) using @p brush */
  virtual void beginBackground( const QBrush &brush ) = 0;

  /** Close the decorarated background element in the markup */
  virtual void endBackground() = 0;

  /**
    Begin a url anchor element in the markup
    @param href The href of the anchor.
    @param name The name of the anchor.
  */
  virtual void beginAnchor( const QString &href = QString(), const QString &name = QString() ) = 0;

  /** Close the anchor element */
  virtual void endAnchor() = 0;

  /**
    Begin a new font familiy element in the markup
    @param family The name of the font family to begin.
  */
  virtual void beginFontFamily( const QString &family ) = 0;

  /** End font family element */
  virtual void endFontFamily() = 0;

  /**
    Begin a new font point size element in the markup
    @param size The point size to begin.
  */
  virtual void beginFontPointSize( int size ) = 0;

  /** End font point size element */
  virtual void endFontPointSize() = 0;

  /**
    Begin a new paragraph in the markup
    @param a The alignment of the new paragraph.
    @param top The top margin of the new paragraph.
    @param bottom The bottom margin of the new paragraph.
    @param left The left margin of the new paragraph.
    @param right The right margin of the new paragraph.
  */
  virtual void beginParagraph( Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0 ) = 0;

  /** Close the paragraph in the markup. */
  virtual void endParagraph() = 0;
  /** Add a newline to the markup. */
  virtual void addNewline() = 0;

  /**
    Insert a horizontal rule into the markup.
    @param width The width of the rule. Default is full width.
  */
  virtual void insertHorizontalRule( int width = -1 ) = 0;

  /**
    Insert a new image element into the markup.
    @param url The url of the image
    @param width The width of the image
    @param height The height of the image.
  */
  virtual void insertImage( const QString &url, qreal width, qreal height ) = 0;

  /**
    Begin a new list element in the markup.
    A list element contains list items, and may contain other lists.
    @param style The style of list to create.
  */
  virtual void beginList( QTextListFormat::Style style ) = 0;

  /**
    Close the list.
  */
  virtual void endList() = 0;

  /** Begin a new list item in the markup */
  virtual void beginListItem() = 0;

  /** End the list item */
  virtual void endListItem() = 0;

  /** Begin a superscript element */
  virtual void beginSuperscript() = 0;

  /** End superscript element */
  virtual void endSuperscript() = 0;

  /** Begin a subscript element */
  virtual void beginSubscript() = 0;

  /** End subscript element */
  virtual void endSubscript() = 0;

  /**
    Begin a table element.

    @param cellpadding The padding attribute for the table.
    @param cellspacing The spacing attribute for the table.
    @param width The width of the table. May be either an integer, or a percentage value.
  */
  virtual void beginTable( qreal cellpadding, qreal cellspacing, const QString &width ) = 0;

  /**
    Begins a new table row.
  */
  virtual void beginTableRow() = 0;

  /**
    Begin a new table header cell.
    @param width The width of the cell.
    @param colSpan The column span of the cell.
    @param rowSpan The row span of the cell.
  */
  virtual void beginTableHeaderCell( const QString &width, int colSpan, int rowSpan ) = 0;

  /**
    Begin a new table cell.
    @param width The width of the cell.
    @param colSpan The column span of the cell.
    @param rowSpan The row span of the cell.
  */
  virtual void beginTableCell( const QString &width, int colSpan, int rowSpan ) = 0;

  /** End a table element */
  virtual void endTable() = 0;

  /** End a table row */
  virtual void endTableRow() = 0;

  /** End a table header cell */
  virtual void endTableHeaderCell() = 0;

  /** End a table cell */
  virtual void endTableCell() = 0;

  /**
    Begin a level @p level header.
    @param level An integer between 1 and 6
  */
  virtual void beginHeader( int level ) = 0;

  /**
    End a level @p level header.
    @param level An integer between 1 and 6
  */
  virtual void endHeader( int level ) = 0;

  /**
    Append the plain text @p text to the markup.

    @param text The text to append.
  */
  virtual void appendLiteralText( const QString &text ) = 0;

  /**
    Appends the raw text @p text to the markup. @p text is added unescaped.
  */
  virtual void appendRawText( const QString &text ) = 0;

  /**
    Return the fully marked up result of the building process. This may contain metadata etc, such as a head element in html.

    @return The fully marked up text.
  */
  virtual QString getResult() = 0;
};

}

#endif
