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
  @brief The MarkupDirector class controls and instructs a builder object to create markup output.

  The MarkupDirector is used with a subclass of AbstractMarkupBuilder to create a marked up document output.

  Usage can be quite simple.

  @code

    QTextDocument *doc = editor->document(); // editor is a QTextEdit

    AbstractMarkupBuilder *builder = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector(builder);
    md->processDocument(doc);
    browser->setHtml(builder->getResult()); // browser is a QTextBrowser.

  @endcode

  Or with a different builder:

  @code
    AbstractMarkupBuilder *builder = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(builder);
    md->processDocument(doc);
    browser->setPlainText(builder->getResult());
  @endcode

  The MarkupDirector also provides API for processing just part of a QTextDocument, such as a QTextFrame or a QTextBlock. The appropriate method may then be called with an invalid iterator as appropriate.

  @code
    // ... Do some processing to get a QTextFrame.
    QTextFrame *frame = getFrame();

    AbstractMarkupBuilder *builder = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(builder);

    // Create output from only the frame.
    md->processFrame(QTextFrame::iterator(), frame);
    browser->setPlainText(builder->getResult());
  @endcode

  The behaviour of the MarkupDirector can be customized by subclassing. Support for custom types can also be added by implementing the processCustomFragment method.

  @see @ref custom_qtextobject

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_GUI_EXPORT MarkupDirector
{
public:
  /**
    Construct a new MarkupDirector
  */
  MarkupDirector( AbstractMarkupBuilder *builder );

  /**
    Destructor
  */
  virtual ~MarkupDirector();

  /**
    Constructs the output by directing the builder to create the markup.
  */
  virtual void processDocument( QTextDocument* doc );

  /**
    Directs the builder to create output for the single @p frame. If calling this method directly, an invalid QTextFrame::iterator may be used.
  */
  virtual QTextFrame::iterator processFrame( QTextFrame::iterator it, QTextFrame *frame );

  /**
    Directs the builder to create output for the single @p block. If calling this method directly, an invalid QTextFrame::iterator may be used.

    This method does not process the contents of the @p block, but uses the processBlockContents method to do so.
  */
  virtual QTextFrame::iterator processBlock( QTextFrame::iterator it, const QTextBlock &block );

  /**
    Directs the builder to create output for the single @p textObject. If calling this method directly, an invalid QTextFrame::iterator may be used.

    The block @p block is the container of the @p textObject.
  */
  virtual QTextFrame::iterator processObject( QTextFrame::iterator it, const QTextBlock &block, QTextObject *textObject );

  /**
    Directs the builder to create output for the single @p textBlockGroup. If calling this method directly, an invalid QTextFrame::iterator may be used.

    The block @p block is the first block in the @p textBlockGroup.
  */
  virtual QPair<QTextFrame::iterator, QTextBlock> processBlockGroup( QTextFrame::iterator it, const QTextBlock &block, QTextBlockGroup *textBlockGroup );

  /**
    Directs the builder to create output for the single @p textList. If calling this method directly, an invalid QTextFrame::iterator may be used.

    The block @p block is the first block in the @p textList.
  */
  virtual QPair<QTextFrame::iterator, QTextBlock> processList( QTextFrame::iterator it, const QTextBlock &block, QTextList *textList );

  /**
    Directs the builder to create output for the contents of the single @p block. If calling this method directly, an invalid QTextFrame::iterator may be used.
  */
  virtual QTextFrame::iterator processBlockContents( QTextFrame::iterator it, const QTextBlock &block );

  /**
    Hook for instructing the builder to create output for the @p fragemnt with a custom type. @p doc is the document the fragment is in.
  */
  virtual void processCustomFragment( const QTextFragment &fragment, QTextDocument const *doc );

  /**
    Directs the builder to create output for the contents of the single @p fragment. If calling this method directly, an invalid QTextBlock::iterator may be used. @p doc is the document the fragment is in.
  */
  virtual QTextBlock::iterator processFragment( QTextBlock::iterator it, const QTextFragment &fragment, QTextDocument const *doc );

  /**
    Directs the builder to create output for the contents of the single @p textObject. The @p textObject is represented in the QTextDocument with the QTextFragment @p fragment.

    If calling this method directly, an invalid QTextBlock::iterator may be used.
  */
  virtual QTextBlock::iterator processCharTextObject( QTextBlock::iterator it, const QTextFragment &fragment, QTextObject *textObject );

  /**
    Directs the builder to create output for the image represented by the @p imageFormat.

    If calling this method directly, an invalid QTextBlock::iterator may be used. @p doc is the document the fragment is in.
  */
  virtual QTextBlock::iterator processImage( QTextBlock::iterator it, const QTextImageFormat &imageFormat, QTextDocument *doc );

  /**
    Directs the builder to create output for the contents of the single @p table.

    If calling this method directly, an invalid QTextFrame::iterator may be used.
  */
  virtual QTextFrame::iterator processTable( QTextFrame::iterator it, QTextTable *table );

  /**
    Directs the builder to create output for the contents of the single @p tableCell. The tableCell is in the @p table.
  */
  virtual void processTableCell( const QTextTableCell &tableCell, QTextTable *table );

protected:
  /**
    Processes the document between @p begin and @p end
  */
  void processDocumentContents( QTextFrame::iterator begin, QTextFrame::iterator end );

  /**
    Iterates the iterator @p it to the first block after @p blockGroup. @p _block is any block in the @p blockGroup.

    The return pair is the iterator pointing after the end of @p blockGroup and the first block after @p blockGroup.
  */
  QPair<QTextFrame::iterator, QTextBlock> skipBlockGroup( QTextFrame::iterator it, const QTextBlock &_block, QTextBlockGroup *blockGroup );

  /**
    Returns a list of tags contained in @p openingTags sorted so they can be opened in order and will be closed in the correct order.

    @p openingTags should be a set of tags opened at the fragment pointed to by @p it.
  */
  QList< int > sortOpeningOrder( QSet< int > openingTags, QTextBlock::iterator it ) const;

  /**
    Directs the builder to close the appropriate tags at the position of @p it.
  */
  virtual void processClosingElements( QTextBlock::iterator it );

  /**
    Directs the builder to open the appropriate tags at the position of @p it.
  */
  virtual void processOpeningElements( QTextBlock::iterator it );

  /**
    Returns the tags that should be closed at the position of @p it.
  */
  virtual QSet< int > getElementsToClose( QTextBlock::iterator it ) const;

  /**
    Returns the tags that should be opened at the position of @p it.
  */
  virtual QList< int > getElementsToOpen( QTextBlock::iterator it );

  /**
    Flags for the tags that may be open.
  */
  enum OpenElementValues {
    None = 0x0,                   /// No tags are open
    SuperScript = 0x01,           /// A superscript tag is open
    SubScript = 0x02,             /// A subscript tag is open
    Anchor = 0x04,                /// An anchor tag is open
    SpanForeground = 0x08,        /// A foreground altering span tag is open.
    SpanBackground = 0x10,        /// A background altering span tag is open.
    SpanFontFamily = 0x20,        /// A font family altering span tag is open.
    SpanFontPointSize = 0x40,     /// A font size altering span tag is open.
    Strong = 0x80,                /// A strong tag is open.
    Emph = 0x100,                 /// A emphasis tag is open.
    Underline = 0x200,            /// An underline tag is open.
    StrikeOut = 0x400             /// A strikeout tag is open.
  };

protected:
#ifndef Q_QDOC
  MarkupDirectorPrivate * const d_ptr;
#endif

  /**
    The builder this MarkupDirector is operating on. This is available when subclassing to customize behaviour.
  */
  AbstractMarkupBuilder *m_builder;

#ifndef Q_QDOC
private:
  Q_DECLARE_PRIVATE(MarkupDirector)
#endif
};

}

#endif
