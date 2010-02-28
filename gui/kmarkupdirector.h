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


#ifndef KMARKUPDIRECTOR_H
#define KMARKUPDIRECTOR_H


#include "kabstractmarkupbuilder.h"
#include <QTextDocument>

#include "grantlee_gui_export.h"

class QTextFrame;
class QTextTable;
class QTextTableCell;
class QTextList;
class QTextCharFormat;

/**
@brief The Markupdirector class controls and instructs a builder object to create markup output.

The KMarkupDirector is used with a subclass of AbstractMarkupBuilder to create a marked up document output.

Usage can be quite simple.

@code

    QTextDocument *doc = editor->document(); // editor is a QTextEdit

    AbstractMarkupBuilder *builder = new HTMLBuilder();
    KMarkupDirector *md = new MarkupDirector(builder);
    md->constructContent(doc);
    browser.setHtml(builder->getResult()); // browser is a QTextBrowser.

@endcode

Or with a different builder:

@code
    AbstractMarkupBuilder *builder = new PlainTextMarkupBuilder();
    KMarkupDirector *md = new MarkupDirector(builder);
    md->constructContent(doc);
    browser.setPlainText(builder->getResult());
@endcode

@todo Move this to kdelibs when tested and prooven.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class GRANTLEE_GUI_EXPORT KMarkupDirector
{
public:
    /**
    Construct a new KMarkupDirector
    */
    KMarkupDirector(KAbstractMarkupBuilder* builder);

    /** Destructor */
    virtual ~KMarkupDirector();

    /**
    Constructs the output by directing the builder to create the markup.
    */
    virtual void constructContent(QTextDocument* doc);

protected:

    /**
    Processes the frame by iterating over its child frames and blocks and processing them as needed.
    */
    void processFrame(QTextFrame *frame);

    /**
    Processes the table by iterating over its rows and columns, processing their contents.
    */
    void processTable(QTextTable *table);

    /**
    Processes the table cell by iterating over its contents. May contain another table, nested list etc.
     */
    void processTableCell(const QTextTableCell &cell);

    /**
        Processes a list by iterating over it. Nested lists are processed by a recursive call.
        @param block The first block in a list.
    */
    void processList(const QTextBlock &block);

    /**
    Processes the contents of a QTextBlock. The block is traversed and each QTextFragment is processed individually.

    A QTextFragment is a fragment of continuous text with continuous formatting.

    Eg, a block of text represented by

    @code
        Some long <b>formatted paragraph</b> of several pieces <b><i>of decorated</i> text</b> .
    @endcode

    would contain the fragments

    @li Some long
    @li formatted paragraph
    @li of several pieces
    @li of decorated
    @li text

    @param block The block to process.
    */
    void processBlockContents(const QTextBlock &block);

    /**
    Processes the document between the iterators @p start and @p end inclusive.
    */
    void processDocumentContents(QTextFrame::iterator start, QTextFrame::iterator end);

    /**
        Process a block.

        Note: If block is the first item in a list, the entire (maybe nested) list will be processed.
        If block is part of a nested list, but is not the first item, it is ignored.
        @param block The block to process.
    */

    void processBlock(const QTextBlock &block);

    /**
    Processes a QTextFragment.

    @param fragment The fragment to process.
    */
    void processFragment(const QTextFragment &fragment);

private:
    class Private;
    friend class Private;
    Private *const d;
};

#endif
