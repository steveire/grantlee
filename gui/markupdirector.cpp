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

MarkupDirector::MarkupDirector( AbstractMarkupBuilder *builder )
  : d_ptr( new MarkupDirectorPrivate(this) ), m_builder( builder )
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

  m_builder->beginTable( format.cellPadding(), format.cellSpacing(), sWidth );

  int headerRowCount = format.headerRowCount();

  QList<QTextTableCell> alreadyProcessedCells;

  for ( int row = 0; row < table->rows(); ++row ) {
    // Put a thead element around here somewhere?
    // if (row < headerRowCount)
    // {
    // beginTableHeader();
    // }

    m_builder->beginTableRow();

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
        m_builder->beginTableHeaderCell( sCellWidth, columnSpan, rowSpan );
      } else {
        m_builder->beginTableCell( sCellWidth, columnSpan, rowSpan );
      }

      processTableCell( tableCell, table );

      if ( row < headerRowCount ) {
        m_builder->endTableHeaderCell();
      } else {
        m_builder->endTableCell();
      }
    }
    m_builder->endTableRow();
  }
  m_builder->endTable();


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
  m_builder->beginList( style );
  QTextBlock block = _block;
  while ( block.isValid() && block.textList() ) {
    m_builder->beginListItem();
    processBlockContents( it, block );
    m_builder->endListItem();

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
  m_builder->endList();
  return qMakePair( it, block );
}

QTextFrame::iterator MarkupDirector::processBlockContents( QTextFrame::iterator frameIt, const QTextBlock &block )
{
  QTextBlockFormat blockFormat = block.blockFormat();
  Qt::Alignment blockAlignment = blockFormat.alignment();

  // TODO: decide when to use <h1> etc.

  if ( blockFormat.hasProperty( QTextFormat::BlockTrailingHorizontalRulerWidth ) ) {
    m_builder->insertHorizontalRule();
    if ( !frameIt.atEnd() )
      return ++frameIt;
    return frameIt;
  }

  QTextBlock::iterator it = block.begin();

  // The beginning is the end. This is an empty block. Insert a newline and move on.
  if ( it.atEnd() ) {
    m_builder->addNewline();
    if ( !frameIt.atEnd() )
      return ++frameIt;
    return frameIt;
  }

  // Don't have p tags inside li tags.
  if ( !block.textList() ) {
    // Don't instruct builders to use margins. The rich text widget doesn't have an action for them yet,
    // So users can't edit them. See bug http://bugs.kde.org/show_bug.cgi?id=160600
    m_builder->beginParagraph( blockAlignment //,
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
    m_builder->endParagraph();
  }

  if ( !frameIt.atEnd() )
    return ++frameIt;
  return frameIt;
}


QTextBlock::iterator MarkupDirector::processFragment( QTextBlock::iterator it, const QTextFragment &fragment, QTextDocument const *doc )
{
//   Q_D( MarkupDirector );
  QTextCharFormat charFormat = fragment.charFormat();
  QTextObject *textObject = doc->objectForFormat( charFormat );
  if ( textObject )
    return processCharTextObject( it, fragment, textObject );

  if ( fragment.text().at( 0 ).category() == QChar::Separator_Line ) {
    m_builder->addNewline();

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

  processOpeningElements( it );

  // If a sequence such as '<br /><br />' is imported into a document with setHtml, LineSeparator
  // characters are inserted. Here I make sure to put them back.
  QStringList sl = fragment.text().split( QChar( QChar::LineSeparator ) );
  QStringListIterator i( sl );
  bool paraClosed = false;
  while ( i.hasNext() ) {
    m_builder->appendLiteralText( i.next() );
    if ( i.hasNext() ) {
      if ( i.peekNext().isEmpty() ) {
        if ( !paraClosed ) {
          m_builder->endParagraph();
          paraClosed = true;
        }
        m_builder->addNewline();
      } else if ( paraClosed ) {
        m_builder->beginParagraph( /* blockAlignment */ );
        paraClosed = false;
      }
    }
  }
  if ( !it.atEnd() )
    ++it;

  processClosingElements( it );

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
  m_builder->insertImage( imageFormat.name(), imageFormat.width(), imageFormat.height() );
  if ( !it.atEnd() )
    return ++it;
  return it;
}

void MarkupDirector::processClosingElements( QTextBlock::iterator it )
{
  Q_D( MarkupDirector );
  // The order of closing elements is determined by the order they were opened in.
  // The order of opened elements is in the openElements member list.
  // see testDifferentStartDoubleFinish and testDifferentStartDoubleFinishReverseOrder

  if ( d->m_openElements.isEmpty() )
    return;

  QSet<int> elementsToClose = getElementsToClose( it );

  int previousSize;
  int remainingSize = elementsToClose.size();
  while ( !elementsToClose.isEmpty() ) {
    int tag = d->m_openElements.last();
    if ( elementsToClose.contains( tag ) ) {
      switch ( tag ) {
      case Strong:
        m_builder->endStrong();
        break;
      case Emph:
        m_builder->endEmph();
        break;
      case Underline:
        m_builder->endUnderline();
        break;
      case StrikeOut:
        m_builder->endStrikeout();
        break;
      case SpanFontPointSize:
        m_builder->endFontPointSize();
        break;
      case SpanFontFamily:
        m_builder->endFontFamily();
        break;
      case SpanBackground:
        m_builder->endBackground();
        break;
      case SpanForeground:
        m_builder->endForeground();
        break;
      case Anchor:
        m_builder->endAnchor();
        break;
      case SubScript:
        m_builder->endSubscript();
        break;
      case SuperScript:
        m_builder->endSuperscript();
        break;

      default:
        break;
      }
      d->m_openElements.removeLast();
      elementsToClose.remove( tag );
    }
    previousSize = remainingSize;
    remainingSize = elementsToClose.size();

    if ( previousSize == remainingSize ) {
      // Iterated once through without closing any tags.
      // This means that there's overlap in the tags, such as
      // 'text with <b>some <i>formatting</i></b><i> tags</i>'
      // See testOverlap.
      // The top element in openElements must be a blocker, so close it on next iteration.
      elementsToClose.insert( d->m_openElements.last() );
    }
  }
}


void MarkupDirector::processOpeningElements( QTextBlock::iterator it )
{
  Q_D( MarkupDirector );
  QTextFragment fragment = it.fragment();

  if ( !fragment.isValid() )
    return;

  QTextCharFormat fragmentFormat = fragment.charFormat();
  QList<int> elementsToOpenList = getElementsToOpen( it );

  Q_FOREACH( int tag, elementsToOpenList ) {
    switch ( tag ) {
    case Strong:
      m_builder->beginStrong();
      break;
    case Emph:
      m_builder->beginEmph();
      break;
    case Underline:
      m_builder->beginUnderline();
      break;
    case StrikeOut:
      m_builder->beginStrikeout();
      break;
    case SpanFontPointSize:
      m_builder->beginFontPointSize( fragmentFormat.font().pointSize() );
      d->m_openFontPointSize = fragmentFormat.font().pointSize();
      break;
    case SpanFontFamily:
      m_builder->beginFontFamily( fragmentFormat.fontFamily() );
      d->m_openFontFamily = fragmentFormat.fontFamily();
      break;
    case SpanBackground:
      m_builder->beginBackground( fragmentFormat.background() );
      d->m_openBackground = fragmentFormat.background();
      break;
    case SpanForeground:
      m_builder->beginForeground( fragmentFormat.foreground() );
      d->m_openForeground = fragmentFormat.foreground();
      break;
    case Anchor: {
      // TODO: Multiple anchor names here.
      QStringList anchorNames = fragmentFormat.anchorNames();
      if ( !anchorNames.isEmpty() ) {
        while ( !anchorNames.isEmpty() ) {
          QString n = anchorNames.last();
          anchorNames.removeLast();
          if ( anchorNames.isEmpty() ) {
            // Doesn't matter if anchorHref is empty.
            m_builder->beginAnchor( fragmentFormat.anchorHref(), n );
            break;
          } else {
            // Empty <a> tags allow multiple names for the same section.
            m_builder->beginAnchor( QString(), n );
            m_builder->endAnchor();
          }
        }
      } else {
        m_builder->beginAnchor( fragmentFormat.anchorHref() );
      }
      d->m_openAnchorHref = fragmentFormat.anchorHref();
      break;
    }
    case SuperScript:
      m_builder->beginSuperscript();
      break;
    case SubScript:
      m_builder->beginSubscript();
      break;
    default:
      break;
    }
    d->m_openElements.append( tag );
    d->m_elementsToOpen.remove( tag );
  }
}


QSet< int > MarkupDirector::getElementsToClose( QTextBlock::iterator it ) const
{
  Q_D( const MarkupDirector );
  QSet<int> closedElements;

  if ( it.atEnd() ) {
    // End of block?. Close all open tags.
    QSet< int > elementsToClose = d->m_openElements.toSet();
    return elementsToClose.unite( d->m_elementsToOpen );
  }

  QTextFragment fragment = it.fragment();

  if ( !fragment.isValid() )
    return closedElements;

  QTextCharFormat fragmentFormat = fragment.charFormat();

  int fontWeight = fragmentFormat.fontWeight();
  bool fontItalic = fragmentFormat.fontItalic();
  bool fontUnderline = fragmentFormat.fontUnderline();
  bool fontStrikeout = fragmentFormat.fontStrikeOut();

  QBrush fontForeground = fragmentFormat.foreground();
  QBrush fontBackground = fragmentFormat.background();

  QString fontFamily = fragmentFormat.fontFamily();
  int fontPointSize = fragmentFormat.font().pointSize();
  QString anchorHref = fragmentFormat.anchorHref();

  QTextCharFormat::VerticalAlignment vAlign = fragmentFormat.verticalAlignment();
  bool superscript = ( vAlign == QTextCharFormat::AlignSuperScript );
  bool subscript = ( vAlign == QTextCharFormat::AlignSubScript );


  if ( !fontStrikeout &&
        ( d->m_openElements.contains( StrikeOut )
          || d->m_elementsToOpen.contains( StrikeOut ) ) ) {
    closedElements.insert( StrikeOut );
  }

  if ( !fontUnderline &&
        ( d->m_openElements.contains( Underline )
          || d->m_elementsToOpen.contains( Underline ) )
        && !( d->m_openElements.contains( Anchor )
              || d->m_elementsToOpen.contains( Anchor ) )
      ) {
    closedElements.insert( Underline );
  }

  if ( !fontItalic &&
        ( d->m_openElements.contains( Emph )
          || d->m_elementsToOpen.contains( Emph ) ) ) {
    closedElements.insert( Emph );
  }

  if ( fontWeight != QFont::Bold &&
        ( d->m_openElements.contains( Strong )
          || d->m_elementsToOpen.contains( Strong ) ) ) {
    closedElements.insert( Strong );
  }

  if (( d->m_openElements.contains( SpanFontPointSize )
        || d->m_elementsToOpen.contains( SpanFontPointSize ) )
      && ( d->m_openFontPointSize != fontPointSize )
      ) {
    closedElements.insert( SpanFontPointSize );
  }

  if (( d->m_openElements.contains( SpanFontFamily )
        || d->m_elementsToOpen.contains( SpanFontFamily ) )
      && ( d->m_openFontFamily != fontFamily )
      ) {
    closedElements.insert( SpanFontFamily );
  }

  if (( d->m_openElements.contains( SpanBackground ) && ( d->m_openBackground != fontBackground ) )
      || ( d->m_elementsToOpen.contains( SpanBackground ) && ( d->m_backgroundToOpen != fontBackground ) ) ) {
    closedElements.insert( SpanBackground );
  }

  if (( d->m_openElements.contains( SpanForeground ) && ( d->m_openForeground != fontForeground ) )
      || ( d->m_elementsToOpen.contains( SpanForeground ) && ( d->m_foregroundToOpen != fontForeground ) ) ) {
    closedElements.insert( SpanForeground );
  }

  if (( d->m_openElements.contains( Anchor ) && ( d->m_openAnchorHref != anchorHref ) )
      || ( d->m_elementsToOpen.contains( Anchor ) && ( d->m_anchorHrefToOpen != anchorHref ) ) ) {
    closedElements.insert( Anchor );
  }

  if ( !subscript &&
        ( d->m_openElements.contains( SubScript )
          || d->m_elementsToOpen.contains( SubScript ) ) ) {
    closedElements.insert( SubScript );
  }

  if ( !superscript &&
        ( d->m_openElements.contains( SuperScript )
          || d->m_elementsToOpen.contains( SuperScript ) ) ) {
    closedElements.insert( SuperScript );
  }
  return closedElements;
}

QList< int > MarkupDirector::getElementsToOpen( QTextBlock::iterator it )
{
  Q_D( MarkupDirector );
  QTextFragment fragment = it.fragment();
  if ( !fragment.isValid() ) {
    return QList< int >();
  }
  QTextCharFormat fragmentFormat = fragment.charFormat();

  int fontWeight = fragmentFormat.fontWeight();
  bool fontItalic = fragmentFormat.fontItalic();
  bool fontUnderline = fragmentFormat.fontUnderline();
  bool fontStrikeout = fragmentFormat.fontStrikeOut();

  QBrush fontForeground = fragmentFormat.foreground();
  QBrush fontBackground = fragmentFormat.background();

  QString fontFamily = fragmentFormat.fontFamily();
  int fontPointSize = fragmentFormat.font().pointSize();
  QString anchorHref = fragmentFormat.anchorHref();

  QTextCharFormat::VerticalAlignment vAlign = fragmentFormat.verticalAlignment();
  bool superscript = ( vAlign == QTextCharFormat::AlignSuperScript );
  bool subscript = ( vAlign == QTextCharFormat::AlignSubScript );

  if ( superscript && !( d->m_openElements.contains( SuperScript ) ) ) {
    d->m_elementsToOpen.insert( SuperScript );
  }

  if ( subscript && !( d->m_openElements.contains( SubScript ) ) ) {
    d->m_elementsToOpen.insert( SubScript );
  }

  if ( !anchorHref.isEmpty()
       && !( d->m_openElements.contains( Anchor ) )
       && ( d->m_openAnchorHref != anchorHref )
     ) {
    d->m_elementsToOpen.insert( Anchor );
    d->m_anchorHrefToOpen = anchorHref;
  }

  if ( fontForeground != Qt::NoBrush
       && !( d->m_openElements.contains( SpanForeground ) ) // Can only open one foreground element at a time.
       && ( fontForeground != d->m_openForeground )
       && !(( d->m_openElements.contains( Anchor )          // Links can't have a foreground color.
              || d->m_elementsToOpen.contains( Anchor ) ) )
     ) {
    d->m_elementsToOpen.insert( SpanForeground );
    d->m_foregroundToOpen = fontForeground;
  }

  if ( fontBackground != Qt::NoBrush
       && !( d->m_openElements.contains( SpanBackground ) )
       && ( fontBackground != d->m_openBackground )
     ) {
    d->m_elementsToOpen.insert( SpanBackground );
    d->m_backgroundToOpen = fontBackground;
  }


  if ( !fontFamily.isEmpty()
       && !( d->m_openElements.contains( SpanFontFamily ) )
       && ( fontFamily != d->m_openFontFamily )
     ) {
    d->m_elementsToOpen.insert( SpanFontFamily );
    d->m_fontFamilyToOpen = fontFamily;
  }

  if (( QTextCharFormat().font().pointSize() != fontPointSize )   // Different from the default.
      && !( d->m_openElements.contains( SpanFontPointSize ) )
      && ( fontPointSize != d->m_openFontPointSize )
     ) {
    d->m_elementsToOpen.insert( SpanFontPointSize );
    d->m_fontPointSizeToOpen = fontPointSize;
  }

//   Only open a new bold tag if one is not already open.
//   eg, <b>some <i>mixed</i> format</b> should be as is, rather than
//   <b>some </b><b><i>mixed</i></b><b> format</b>

  if ( fontWeight == QFont::Bold && !( d->m_openElements.contains( Strong ) ) ) {
    d->m_elementsToOpen.insert( Strong );
  }

  if ( fontItalic && !( d->m_openElements.contains( Emph ) ) ) {
    d->m_elementsToOpen.insert( Emph );
  }

  if ( fontUnderline
       && !( d->m_openElements.contains( Underline ) )
       && !( d->m_openElements.contains( Anchor )
             || d->m_elementsToOpen.contains( Anchor ) ) // Can't change the underline state of a link.
     ) {
    d->m_elementsToOpen.insert( Underline );
  }

  if ( fontStrikeout && !( d->m_openElements.contains( StrikeOut ) ) ) {
    d->m_elementsToOpen.insert( StrikeOut );
  }

  if ( d->m_elementsToOpen.size() <= 1 ) {
    return d->m_elementsToOpen.toList();
  }
  return sortOpeningOrder( d->m_elementsToOpen, it );

}

QList< int > MarkupDirector::sortOpeningOrder( QSet< int > openingOrder, QTextBlock::iterator it ) const
{
  QList< int > sortedOpenedElements;

  // This is an insertion sort in a way. elements in openingOrder are assumed to be out of order.
  // The rest of the block is traversed until there are no more elements to sort, or the end is reached.
  while ( openingOrder.size() != 0 ) {
    if ( !it.atEnd() ) {
      it++;

      if ( !it.atEnd() ) {
        // Because I've iterated, this returns the elements that will
        // be closed by the next fragment.
        QSet<int> elementsToClose = getElementsToClose( it );

        // The exact order these are opened in is irrelevant, as all will be closed on the same block.
        // See testDoubleFormat.
        Q_FOREACH( int tag, elementsToClose ) {
          if ( openingOrder.remove( tag ) ) {
            sortedOpenedElements.prepend( tag );
          }
        }
      }
    } else {
      // End of block. Need to close all open elements.
      // Order irrelevant in this case.
      Q_FOREACH( int tag, openingOrder ) {
        sortedOpenedElements.prepend( tag );
      }
      break;
    }
  }
  return sortedOpenedElements;
}
