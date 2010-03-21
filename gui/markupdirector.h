/*
  This file is part of the Grantlee template system.

  Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

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


#ifndef GRANTLEE_MARKUPDIRECTOR_H
#define GRANTLEE_MARKUPDIRECTOR_H

#include <QtGui/QTextDocument>
#include <QtGui/QTextFrame>

#include "abstractmarkupbuilder.h"
#include "grantlee_gui_export.h"

class QTextTable;
class QTextTableCell;
class QTextList;
class QTextCharFormat;

namespace Grantlee
{

class MarkupDirectorPrivate;

/// @headerfile markupdirector.h grantlee/markupdirector.h

/**
  @brief The Markupdirector class controls and instructs a builder object to create markup output.

  The MarkupDirector is used with a subclass of AbstractMarkupBuilder to create a marked up document output.

  Usage can be quite simple.

  @code

    QTextDocument *doc = editor->document(); // editor is a QTextEdit

    AbstractMarkupBuilder *builder = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector(builder);
    md->constructContent(doc);
    browser.setHtml(builder->getResult()); // browser is a QTextBrowser.

  @endcode

  Or with a different builder:

  @code
    AbstractMarkupBuilder *builder = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(builder);
    md->constructContent(doc);
    browser.setPlainText(builder->getResult());
  @endcode

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_GUI_EXPORT MarkupDirector : public virtual AbstractMarkupBuilder
{
public:
  /**
    Construct a new MarkupDirector
  */
  MarkupDirector();

  /**
    Destructor
  */
  virtual ~MarkupDirector();

  /**
    Constructs the output by directing the builder to create the markup.
  */
  virtual void processDocument( QTextDocument* doc );
  virtual QTextFrame::iterator processFrame( QTextFrame::iterator it, QTextFrame *frame );
  virtual QTextFrame::iterator processBlock( QTextFrame::iterator it, const QTextBlock &block );
  virtual QTextFrame::iterator processObject( QTextFrame::iterator it, const QTextBlock &block, QTextObject *textObject );
  virtual QPair<QTextFrame::iterator, QTextBlock> processBlockGroup( QTextFrame::iterator it, const QTextBlock &block, QTextBlockGroup *textBlockGroup );
  virtual QPair<QTextFrame::iterator, QTextBlock> processList( QTextFrame::iterator it, const QTextBlock &block, QTextList *textList );
  virtual QTextFrame::iterator processBlockContents( QTextFrame::iterator it, const QTextBlock &block );
  virtual QTextBlock::iterator processFragment( QTextBlock::iterator it, const QTextFragment &fragment, QTextDocument const *doc );
  virtual QTextBlock::iterator processCharTextObject( QTextBlock::iterator it, const QTextFragment &fragment, QTextObject *textObject );
  virtual QTextBlock::iterator processImage( QTextBlock::iterator it, const QTextImageFormat &imageFormat, QTextDocument *doc );
  virtual QTextFrame::iterator processTable( QTextFrame::iterator it, QTextTable *table );
  virtual void processTableCell( const QTextTableCell &tableCell, QTextTable *table );

protected:
  void processDocumentContents( QTextFrame::iterator begin, QTextFrame::iterator end );

  QPair<QTextFrame::iterator, QTextBlock> skipBlockGroup( QTextFrame::iterator it, const QTextBlock &_block, QTextBlockGroup *blockGroup );

private:
  Q_DECLARE_PRIVATE( MarkupDirector )
  MarkupDirectorPrivate * const d_ptr;
};

}

#endif
