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


#include "markupdirector.h"
#include "markupdirector_p.h"

#include <QtCore/QFlags>
#include <QtCore/QMap>
#include <QtCore/QStack>
#include <QtCore/QString>
#include <QtGui/QTextDocument>
#include <QtGui/QTextDocumentFragment>
#include <QtGui/QTextFrame>
#include <QtGui/QTextTable>
#include <QtGui/QTextList>
#include <QtGui/QTextCursor>
#include <QtGui/QTextCharFormat>
#include <QtGui/QColor>
#include <QtGui/QBrush>

#include "abstractmarkupbuilder.h"

using namespace Grantlee;

MarkupDirector::MarkupDirector()
  : AbstractMarkupBuilder(), d_ptr( new MarkupDirectorPrivate( this ) )
{
}

MarkupDirector::~MarkupDirector()
{
  delete d_ptr;
}

void MarkupDirector::processDocumentContents( QTextFrame::iterator start, QTextFrame::iterator end )
{
  while ( !start.atEnd() && start != end ) {
    QTextFrame *frame = start.currentFrame();
    if ( frame ) {
      QTextTable *table = qobject_cast<QTextTable*>( frame );
      if ( table ) {
        start = processTable( start, table );
      } else {
        start = processFrame( start, frame );
      }
    } else {
      QTextBlock block = start.currentBlock();
      Q_ASSERT( block.isValid() );
      start = processBlock( start, block );
    }
  }
}

QTextFrame::iterator MarkupDirector::processFrame( QTextFrame::iterator it, QTextFrame* frame )
{
  if ( frame ) {
    processDocumentContents( frame->begin(), frame->end() );
  }
  if ( !it.atEnd() )
    return ++it;
  return it;
}

QTextFrame::iterator MarkupDirector::processBlock( QTextFrame::iterator it, const QTextBlock &block )
{
  if ( block.isValid() ) {
    QTextBlockFormat fmt = block.blockFormat();
    QTextObject *object = block.document()->objectForFormat( fmt );
    if ( object ) {
      return processObject( it, block, object );
    } else {
      return processBlockContents( it, block );
    }
  }

  if ( !it.atEnd() )
    return ++it;
  return it;
}

QTextFrame::iterator MarkupDirector::processTable( QTextFrame::iterator it, QTextTable* table )
{
  QTextTableFormat format = table->format();

  QVector<QTextLength> colLengths = format.columnWidthConstraints();

  QTextLength tableWidth = format.width();
  QString sWidth;

  if ( tableWidth.type() == QTextLength::PercentageLength ) {
    sWidth = "%1%";
    sWidth = sWidth.arg( tableWidth.rawValue() );
  } else if ( tableWidth.type() == QTextLength::FixedLength ) {
    sWidth = "%1";
    sWidth = sWidth.arg( tableWidth.rawValue() );
  }

  beginTable( format.cellPadding(), format.cellSpacing(), sWidth );

  int headerRowCount = format.headerRowCount();

  QList<QTextTableCell> alreadyProcessedCells;

  for ( int row = 0; row < table->rows(); ++row ) {
    // Put a thead element around here somewhere?
    // if (row < headerRowCount)
    // {
    // beginTableHeader();
    // }

    beginTableRow();

    // Header attribute should really be on cells, not determined by number of rows.
    //http://www.webdesignfromscratch.com/html-tables.cfm


    for ( int column = 0; column < table->columns(); ++column ) {

      QTextTableCell tableCell = table->cellAt( row, column );

      int columnSpan = tableCell.columnSpan();
      int rowSpan = tableCell.rowSpan();
      if ( ( rowSpan > 1 ) || ( columnSpan > 1 ) ) {
        if ( alreadyProcessedCells.contains( tableCell ) ) {
          // Already processed this cell. Move on.
          continue;
        } else {
          alreadyProcessedCells.append( tableCell );
        }
      }

      QTextLength cellWidth = colLengths.at( column );

      QString sCellWidth;

      if ( cellWidth.type() == QTextLength::PercentageLength ) {
        sCellWidth = "%1%";
        sCellWidth = sCellWidth.arg( cellWidth.rawValue() );
      } else if ( cellWidth.type() == QTextLength::FixedLength ) {
        sCellWidth = "%1";
        sCellWidth = sCellWidth.arg( cellWidth.rawValue() );
      }

      // TODO: Use THEAD instead
      if ( row < headerRowCount ) {
        beginTableHeaderCell( sCellWidth, columnSpan, rowSpan );
      } else {
        beginTableCell( sCellWidth, columnSpan, rowSpan );
      }

      processTableCell( tableCell, table );

      if ( row < headerRowCount ) {
        endTableHeaderCell();
      } else {
        endTableCell();
      }
    }
    endTableRow();
  }
  endTable();


  if ( !it.atEnd() )
    return ++it;
  return it;
}

void MarkupDirector::processTableCell( const QTextTableCell &tableCell, QTextTable* table )
{
  Q_UNUSED( table )
  processDocumentContents( tableCell.begin(), tableCell.end() );
}

QPair<QTextFrame::iterator, QTextBlock> MarkupDirector::processList( QTextFrame::iterator it, const QTextBlock &_block, QTextList *list )
{
  QList<QTextList*> lists;

  QTextListFormat::Style style = list->format().style();
  beginList( style );
  QTextBlock block = _block;
  while ( block.isValid() && block.textList() ) {
    beginListItem();
    processBlockContents( it, block );
    endListItem();

    if ( !it.atEnd() )
      ++it;
    block = block.next();
    if ( block.isValid() ) {
      QTextObject *obj = block.document()->objectForFormat( block.blockFormat() );
      QTextBlockGroup *group = qobject_cast<QTextBlockGroup *>( obj );
      if ( group && group != list ) {
        QPair<QTextFrame::iterator, QTextBlock> pair = processBlockGroup( it, block, group );
        it = pair.first;
        block = pair.second;
      }
    }
  }
  endList();
  return qMakePair( it, block );
}

QTextFrame::iterator MarkupDirector::processBlockContents( QTextFrame::iterator frameIt, const QTextBlock &block )
{
  QTextBlockFormat blockFormat = block.blockFormat();
  Qt::Alignment blockAlignment = blockFormat.alignment();

  // TODO: decide when to use <h1> etc.

  if ( blockFormat.hasProperty( QTextFormat::BlockTrailingHorizontalRulerWidth ) ) {
    insertHorizontalRule();
    if ( !frameIt.atEnd() )
      return ++frameIt;
    return frameIt;
  }

  QTextBlock::iterator it = block.begin();

  // The beginning is the end. This is an empty block. Insert a newline and move on.
  if ( it.atEnd() ) {
    addNewline();
    if ( !frameIt.atEnd() )
      return ++frameIt;
    return frameIt;
  }

  // Don't have p tags inside li tags.
  if ( !block.textList() ) {
    // Don't instruct builders to use margins. The rich text widget doesn't have an action for them yet,
    // So users can't edit them. See bug http://bugs.kde.org/show_bug.cgi?id=160600
    beginParagraph( blockAlignment //,
//                                blockFormat.topMargin(),
//                                blockFormat.bottomMargin(),
//                                blockFormat.leftMargin(),
//                                blockFormat.rightMargin()
                                );
  }

  while ( !it.atEnd() ) {
    it = processFragment( it, it.fragment(), block.document() );
  }

  // Don't have p tags inside li tags.
  if ( !block.textList() ) {
    endParagraph();
  }

  if ( !frameIt.atEnd() )
    return ++frameIt;
  return frameIt;
}


QTextBlock::iterator MarkupDirector::processFragment( QTextBlock::iterator it, const QTextFragment &fragment, QTextDocument const *doc )
{
  Q_D( MarkupDirector );
  QTextCharFormat charFormat = fragment.charFormat();
  QTextObject *textObject = doc->objectForFormat( charFormat );
  if ( textObject )
    return processCharTextObject( it, fragment, textObject );

  if ( fragment.text().at( 0 ).category() == QChar::Separator_Line ) {
    addNewline();

    if ( !it.atEnd() )
      return ++it;
    return it;
  }

  // The order of closing and opening tags can determine whether generated html is valid or not.
  // When processing a document with formatting which appears as '<b><i>Some</i> formatted<b> text',
  // the correct generated output will contain '<strong><em>Some</em> formatted<strong> text'.
  // However, processing text which appears as '<i><b>Some</b> formatted<i> text' might be incorrectly rendered
  // as '<strong><em>Some</strong> formatted</em> text' if tags which start at the same fragment are
  // opened out of order. Here, tags are not nested properly, and the html would
  // not be valid or render correctly by unforgiving parsers (like QTextEdit).
  // One solution is to make the order of opening tags dynamic. In the above case, the em tag would
  // be opened before the strong tag '<em><strong>Some</strong> formatted</em> text'. That would
  // require knowledge of which tag is going to close first. That might be possible by examining
  // the 'next' QTextFragment while processing one.
  //
  // The other option is to do pessimistic closing of tags.
  // In the above case, this means that if a fragment has two or more formats applied (bold and italic here),
  // and one of them is closed, then all tags should be closed first. They will of course be reopened
  // if necessary while processing the next fragment.
  // The above case would be rendered as '<strong><em>Some</em></strong><em> formatted</em> text'.
  //
  // The first option is taken here, as the redundant opening and closing tags in the second option
  // didn't appeal.
  // See testDoubleStartDifferentFinish, testDoubleStartDifferentFinishReverseOrder

  d->processOpeningElements( it );

  // If a sequence such as '<br /><br />' is imported into a document with setHtml, LineSeparator
  // characters are inserted. Here I make sure to put them back.
  QStringList sl = fragment.text().split( QChar( QChar::LineSeparator ) );
  QStringListIterator i( sl );
  bool paraClosed = false;
  while ( i.hasNext() ) {
    appendLiteralText( i.next() );
    if ( i.hasNext() ) {
      if ( i.peekNext().isEmpty() ) {
        if ( !paraClosed ) {
          endParagraph();
          paraClosed = true;
        }
        addNewline();
      } else if ( paraClosed ) {
        beginParagraph( /* blockAlignment */ );
        paraClosed = false;
      }
    }
  }
  if ( !it.atEnd() )
    ++it;

  d->processClosingElements( it );

  return it;
}

QTextFrame::iterator MarkupDirector::processObject( QTextFrame::iterator it, const QTextBlock &block, QTextObject *object )
{
  QTextBlockGroup *group = qobject_cast<QTextBlockGroup *>( object );
  if ( group ) {
    return processBlockGroup( it, block, group ).first;
  }
  if ( !it.atEnd() )
    return ++it;
  return it;
}

QPair<QTextFrame::iterator, QTextBlock> MarkupDirector::skipBlockGroup( QTextFrame::iterator it, const QTextBlock &_block, QTextBlockGroup *blockGroup )
{
  QTextBlock block = _block;
  QTextBlock lastBlock = _block;
  QTextFrame::iterator lastIt = it;
  QTextObject *obj = block.document()->objectForFormat( block.blockFormat() );
  QTextBlockGroup *nextGroup;

  if ( !obj )
    return qMakePair( lastIt, lastBlock );

  QTextBlockGroup *group = qobject_cast<QTextBlockGroup *>( obj );
  if ( !group )
    return qMakePair( lastIt, lastBlock );

  while ( block.isValid() ) {
    if ( !group )
      break;

    block = block.next();
    if ( !it.atEnd() )
      ++it;

    obj = block.document()->objectForFormat( block.blockFormat() );
    if ( obj )
      continue;

    nextGroup = qobject_cast<QTextBlockGroup *>( obj );

    if ( group == blockGroup || !nextGroup ) {
      lastBlock = block;
      lastIt = it;
    }
    group = nextGroup;
  }
  return qMakePair( lastIt, lastBlock );
}

QPair<QTextFrame::iterator, QTextBlock> MarkupDirector::processBlockGroup( QTextFrame::iterator it, const QTextBlock &block, QTextBlockGroup *blockGroup )
{
  QTextList *list = qobject_cast<QTextList *>( blockGroup );
  if ( list ) {
    return processList( it, block, list );
  }
  return skipBlockGroup( it, block, blockGroup );
}

void MarkupDirector::processDocument( QTextDocument *doc )
{
  processFrame( QTextFrame::iterator(), doc->rootFrame() );
}

QTextBlock::iterator MarkupDirector::processCharTextObject( QTextBlock::iterator it, const QTextFragment &fragment, QTextObject *textObject )
{
  QTextCharFormat fragmentFormat = fragment.charFormat();
  if ( fragmentFormat.isImageFormat() ) {
    QTextImageFormat imageFormat = fragmentFormat.toImageFormat();
    return processImage( it, imageFormat, textObject->document() );
  }
  if ( !it.atEnd() )
    return ++it;
  return it;
}

QTextBlock::iterator MarkupDirector::processImage( QTextBlock::iterator it, const QTextImageFormat &imageFormat, QTextDocument *doc )
{
  Q_UNUSED( doc )
  // TODO: Close any open format elements?
  insertImage( imageFormat.name(), imageFormat.width(), imageFormat.height() );
  if ( !it.atEnd() )
    return ++it;
  return it;
}


