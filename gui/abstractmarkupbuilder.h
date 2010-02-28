/*
    This file is part of KDE.

    Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

// Any class for which you cannot create instances is an abstract class.

#ifndef GRANTLEE_ABSTRACTMARKUPBUILDER_H
#define GRANTLEE_ABSTRACTMARKUPBUILDER_H


#include <QString>
#include <QBrush>
#include <QTextList>
#include <QTextDocument>

#include "grantlee_gui_export.h"

namespace Grantlee
{

/**
@brief The AbstractMarkupBuilder class serves as a base class for creating marked up plain text output.

The AbstractMarkupBuilder is used by the MarkupDirector to create marked up output such as html or markdown.

Subclasses can reimplement whichever methods they choose. None of the methods are pure virtual and all default to an empty function
to allow a clean fall-through. The exception is appendLiteralText, which appends its argument to the text being built.

See PlainTextMarkupBuilder and HTMLBuilder for example implementations.

@note For maintenance, if an extra tag is needed which is not provided by the virtual methods, the ExtraElement can be used.

eg,

@code

builder->beginExtraElement(AbstractMarkupBuilder::DivTag);
// ...
builder->endExtraElement(AbstractMarkupBuilder::DivTag);

@endcode

@author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_GUI_EXPORT AbstractMarkupBuilder
{
public:

    /** For future compatibility.
    This enum can be used to insert extra tags not supported by the virtual methods. */
    enum ExtraElement { UserElement = 100 };

    /** Destructor */
    virtual ~AbstractMarkupBuilder() {}

    /** Begin a bold element in the markup */
    virtual void beginStrong() { }

    /** Close the bold element in the markup */
    virtual void endStrong() { }

    /** Begin an emphasised element in the markup */
    virtual void beginEmph() { }

    /** Close the emphasised element in the markup */
    virtual void endEmph() { }

    /** Begin an underlined element in the markup */
    virtual void beginUnderline() { }

    /** Close the underlined element in the markup */
    virtual void endUnderline() { }

    /** Begin a struck out element in the markup */
    virtual void beginStrikeout() { }

    /** Close the struck out element in the markup */
    virtual void endStrikeout() { }

    /** Begin a decorarated foreground element in the markup (A text color) */
    virtual void beginForeground(const QBrush &brush) {
        Q_UNUSED(brush);
    }

    /** Close the decorarated foreground element in the markup */
    virtual void endForeground() { }

    /** Begin a decorarated background element in the markup (A text background color) */
    virtual void beginBackground(const QBrush &brush) {
        Q_UNUSED(brush);
    }

    /** Close the decorarated background element in the markup */
    virtual void endBackground() { }

    /** Begin a url anchor element in the markup
    @param href The href of the anchor.
    @param name The name of the anchor.
    */
    virtual void beginAnchor(const QString &href = QString(), const QString &name = QString()) {
        Q_UNUSED(href);
        Q_UNUSED(name);
    }

    /** Close the anchor element */
    virtual void endAnchor() { }

    /** Begin a new font familiy element in the markup
    @param family The name of the font family to begin.
    */
    virtual void beginFontFamily(const QString &family) {
        Q_UNUSED(family);
    }
    /** End font family element */
    virtual void endFontFamily() { }

    /** Begin a new font point size element in the markup
    @param int The point size to begin.
    */
    virtual void beginFontPointSize(int size) {
        Q_UNUSED(size);
    }
    /** End font point size element */
    virtual void endFontPointSize() { }

    /** Begin a new paragraph in the markup
    @param a The alignment of the new paragraph.
    @param top The top margin of the new paragraph.
    @param bottom The bottom margin of the new paragraph.
    @param left The left margin of the new paragraph.
    @param right The right margin of the new paragraph.
    */
    virtual void beginParagraph(Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0) {
        Q_UNUSED(a);
        Q_UNUSED(top);
        Q_UNUSED(bottom);
        Q_UNUSED(left);
        Q_UNUSED(right);
    }


    /** Close the paragraph in the markup. */
    virtual void endParagraph() { }
    /** Add a newline to the markup. */
    virtual void addNewline() { }

    /** Insert a horizontal rule into the markup.
    @param width The width of the rule. Default is full width.
    */
    virtual void insertHorizontalRule(int width = -1) {
        Q_UNUSED(width);
    }

    /**
    Insert a new image element into the markup.
    @param url The url of the image
    @param width The width of the image
    @param height The height of the image.
    */
    virtual void insertImage(const QString &url, qreal width, qreal height) {
        Q_UNUSED(url);
        Q_UNUSED(width);
        Q_UNUSED(height);
    }

    /**
    Begin a new list element in the markup.
    A list element contains list items, and may contain other lists.
    @param style The style of list to create.
    */
    virtual void beginList(QTextListFormat::Style style) {
        Q_UNUSED(style);
    }

    /**
    Close the list.
    */
    virtual void endList() { }

    /** Begin a new list item in the markup */
    virtual void beginListItem() { }

    /** End the list item */
    virtual void endListItem() { }

    /** Begin a superscript element */
    virtual void beginSuperscript() { }

    /** End superscript element */
    virtual void endSuperscript() { }

    /** Begin a subscript element */
    virtual void beginSubscript() { }

    /** End subscript element */
    virtual void endSubscript() { }

    /**
      Begin a table element.

      @param cellpadding The padding attribute for the table.
      @param cellspacing The spacing attribute for the table.
      @param width The width of the table. May be either an integer, or a percentage value.
    */
    virtual void beginTable(qreal cellpadding, qreal cellspacing, const QString &width) {
        Q_UNUSED(cellpadding);
        Q_UNUSED(cellspacing);
        Q_UNUSED(width);
    }

    /**
    Begins a new table row.
    */
    virtual void beginTableRow() { }

    /**
    Begin a new table header cell.
    @param width The width of the cell.
    @param colSpan The column span of the cell.
    @param rowSpan The row span of the cell.
    */
    virtual void beginTableHeaderCell(const QString &width, int colSpan, int rowSpan) {
        Q_UNUSED(width);
        Q_UNUSED(colSpan);
        Q_UNUSED(rowSpan);
    }

    /**
    Begin a new table cell.
    @param width The width of the cell.
    @param colSpan The column span of the cell.
    @param rowSpan The row span of the cell.
    */
    virtual void beginTableCell(const QString &width, int colSpan, int rowSpan) {
        Q_UNUSED(width);
        Q_UNUSED(colSpan);
        Q_UNUSED(rowSpan);
    }

    /** End a table element */
    virtual void endTable() { }

    /** End a table row */
    virtual void endTableRow() { }

    /** End a table header cell */
    virtual void endTableHeaderCell() { }

    /** End a table cell */
    virtual void endTableCell() { }

    /**
    Begin a level @p level header.
    @param level An integer between 1 and 6
    */
    virtual void beginHeader(int level) {
        Q_UNUSED(level);
    }

    /**
    End a level @p level header.
    @param level An integer between 1 and 6
    */
    virtual void endHeader(int level) {
        Q_UNUSED(level);
    }


    /** Begin an extra identified element. Override this to support more elements
    in the future in a BC way.

    @param type The type of element to create
    @param args Arguments for the element.
    */
    virtual void beginExtraElement(int type, QVariantList args) {
        Q_UNUSED(type);
        Q_UNUSED(args);
    }

    /** End extra tag.

    @param type The type of the tag to end.
    */
    virtual void endExtraElement(int type) {
        Q_UNUSED(type);
    }

    /**
    Append the plain text @p text to the markup.

    @param The text to append.
    */
    virtual void appendLiteralText(const QString &text) = 0;

    /** Return the fully marked up result of the building process. This may contain metadata etc, such as a head element in html.

    @return The fully marked up text.
    */
    virtual QString& getResult() = 0;

};

}

#endif
