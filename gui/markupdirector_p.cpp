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

#include "markupdirector_p.h"

using namespace Grantlee;

void MarkupDirector::Private::processClosingElements( QTextBlock::iterator it )
{
  // The order of closing elements is determined by the order they were opened in.
  // The order of opened elements is in the openElements member list.
  // see testDifferentStartDoubleFinish and testDifferentStartDoubleFinishReverseOrder

  QSet<int> elementsToClose = getElementsToClose( it );

  int previousSize;
  int remainingSize = elementsToClose.size();
  while ( !elementsToClose.isEmpty() ) {
    if ( !openElements.isEmpty() ) {
      int tag = openElements.last();
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
        openElements.removeLast();
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
        elementsToClose.insert( openElements.last() );
      }
    }
  }
}

QSet< int > MarkupDirector::Private::getElementsToClose( QTextBlock::iterator it )
{
  QSet<int> closedElements;

  if ( !it.atEnd() ) {

    QTextFragment fragment = it.fragment();
    if ( fragment.isValid() ) {
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
           ( openElements.contains( StrikeOut )
             || elementsToOpen.contains( StrikeOut ) ) ) {
        closedElements.insert( StrikeOut );
      }

      if ( !fontUnderline &&
           ( openElements.contains( Underline )
             || elementsToOpen.contains( Underline ) )
           && !( openElements.contains( Anchor )
                 || elementsToOpen.contains( Anchor ) )
         ) {
        closedElements.insert( Underline );
      }

      if ( !fontItalic &&
           ( openElements.contains( Emph )
             || elementsToOpen.contains( Emph ) ) ) {
        closedElements.insert( Emph );
      }

      if ( fontWeight != QFont::Bold &&
           ( openElements.contains( Strong )
             || elementsToOpen.contains( Strong ) ) ) {
        closedElements.insert( Strong );
      }

      if (( openElements.contains( SpanFontPointSize )
            || elementsToOpen.contains( SpanFontPointSize ) )
          && ( openFontPointSize != fontPointSize )
         ) {
        closedElements.insert( SpanFontPointSize );
      }

      if (( openElements.contains( SpanFontFamily )
            || elementsToOpen.contains( SpanFontFamily ) )
          && ( openFontFamily != fontFamily )
         ) {
        closedElements.insert( SpanFontFamily );
      }

      if (( openElements.contains( SpanBackground ) && ( openBackground != fontBackground ) )
          || ( elementsToOpen.contains( SpanBackground ) && ( backgroundToOpen != fontBackground ) ) ) {
        closedElements.insert( SpanBackground );
      }

      if (( openElements.contains( SpanForeground ) && ( openForeground != fontForeground ) )
          || ( elementsToOpen.contains( SpanForeground ) && ( foregroundToOpen != fontForeground ) ) ) {
        closedElements.insert( SpanForeground );
      }

      if (( openElements.contains( Anchor ) && ( openAnchorHref != anchorHref ) )
          || ( elementsToOpen.contains( Anchor ) && ( anchorHrefToOpen != anchorHref ) ) ) {
        closedElements.insert( Anchor );
      }


      if ( !subscript &&
           ( openElements.contains( SubScript )
             || elementsToOpen.contains( SubScript ) ) ) {
        closedElements.insert( SubScript );
      }

      if ( !superscript &&
           ( openElements.contains( SuperScript )
             || elementsToOpen.contains( SuperScript ) ) ) {
        closedElements.insert( SuperScript );
      }

    }
  } else {
    // End of block?. Close all open tags.
    QSet< int > elementsToClose = openElements.toSet();
    closedElements = elementsToClose.unite( elementsToOpen );
  }
  return closedElements;
}

QList< int > MarkupDirector::Private::sortOpeningOrder( QSet< int > openingOrder, QTextBlock::iterator it )
{
  QList< int > sortedOpenedElements;

  // This is an insertion sort in a way. elements in openingOrder are assumed to be out of order.
  // The rest of the block is traversed until there are no more elements to sort, or the end is reached.
  while ( openingOrder.size() != 0 ) {
    if ( !it.atEnd() ) {
      it++;

      if ( it.fragment().isValid() ) {
        // Because I've iterated, this returns the elements that will
        // be closed by the next fragment.
        QSet<int> elementsToClose = getElementsToClose( it );

        // The exact order these are opened in is irrelevant, as all will be closed on the same block.
        // See testDoubleFormat.
        foreach( int tag, elementsToClose ) {
          if ( openingOrder.remove( tag ) ) {
            sortedOpenedElements.prepend( tag );
          }
        }
      }
    } else {
      // End of block. Need to close all open elements.
      // Order irrelevant in this case.
      foreach( int tag, openingOrder ) {
        sortedOpenedElements.prepend( tag );
      }
      break;
    }
  }
  return sortedOpenedElements;
}

QList< int > MarkupDirector::Private::getElementsToOpen( QTextBlock::iterator it )
{
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

  if ( superscript && !( openElements.contains( SuperScript ) ) ) {
    elementsToOpen.insert( SuperScript );
  }

  if ( subscript && !( openElements.contains( SubScript ) ) ) {
    elementsToOpen.insert( SubScript );
  }

  if ( !anchorHref.isEmpty()
       && !( openElements.contains( Anchor ) )
       && ( openAnchorHref != anchorHref )
     ) {
    elementsToOpen.insert( Anchor );
    anchorHrefToOpen = anchorHref;
  }

  if ( fontForeground != Qt::NoBrush
       && !( openElements.contains( SpanForeground ) ) // Can only open one foreground element at a time.
       && ( fontForeground != openForeground )
       && !(( openElements.contains( Anchor )       // If inside an anchor, only open a foreground span tag if
              || elementsToOpen.contains( Anchor ) ) // it is not blue. Qt sort of enforces links being blue
            && ( fontForeground == Qt::blue ) )  // and underlined. See qt bug 203510.
     ) {
    elementsToOpen.insert( SpanForeground );
    foregroundToOpen = fontForeground;
  }

  if ( fontBackground != Qt::NoBrush
       && !( openElements.contains( SpanBackground ) )
       && ( fontBackground != openBackground )
     ) {
    elementsToOpen.insert( SpanBackground );
    backgroundToOpen = fontBackground;
  }


  if ( !fontFamily.isEmpty()
       && !( openElements.contains( SpanFontFamily ) )
       && ( fontFamily != openFontFamily )
     ) {
    elementsToOpen.insert( SpanFontFamily );
    fontFamilyToOpen = fontFamily;
  }

  if (( QTextCharFormat().font().pointSize() != fontPointSize )   // Different from the default.
      && !( openElements.contains( SpanFontPointSize ) )
      && ( fontPointSize != openFontPointSize )
     ) {
    elementsToOpen.insert( SpanFontPointSize );
    fontPointSizeToOpen = fontPointSize;
  }

//   Only open a new bold tag if one is not already open.
//   eg, <b>some <i>mixed</i> format</b> should be as is, rather than
//   <b>some </b><b><i>mixed</i></b><b> format</b>

  if ( fontWeight == QFont::Bold && !( openElements.contains( Strong ) ) ) {
    elementsToOpen.insert( Strong );
  }

  if ( fontItalic && !( openElements.contains( Emph ) ) ) {
    elementsToOpen.insert( Emph );
  }

  if ( fontUnderline
       && !( openElements.contains( Underline ) )
       && !( openElements.contains( Anchor )
             || elementsToOpen.contains( Anchor ) ) // Can't change the underline state of a link.
     ) {
    elementsToOpen.insert( Underline );
  }

  if ( fontStrikeout && !( openElements.contains( StrikeOut ) ) ) {
    elementsToOpen.insert( StrikeOut );
  }

  if ( elementsToOpen.size() <= 1 ) {
    return elementsToOpen.toList();
  }
  return sortOpeningOrder( elementsToOpen, QTextBlock::iterator( it ) );

}

void MarkupDirector::Private::processOpeningElements( QTextBlock::iterator it )
{
  QTextFragment fragment = it.fragment();

  if ( fragment.isValid() ) {
    QTextCharFormat fragmentFormat = fragment.charFormat();
    QList<int> elementsToOpenList = getElementsToOpen( it );

    foreach( int tag, elementsToOpenList ) {
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
        openFontPointSize = fragmentFormat.font().pointSize();
        break;
      case SpanFontFamily:
        m_builder->beginFontFamily( fragmentFormat.fontFamily() );
        openFontFamily = fragmentFormat.fontFamily();
        break;
      case SpanBackground:
        m_builder->beginBackground( fragmentFormat.background() );
        openBackground = fragmentFormat.background();
        break;
      case SpanForeground:
        m_builder->beginForeground( fragmentFormat.foreground() );
        openForeground = fragmentFormat.foreground();
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
        openAnchorHref = fragmentFormat.anchorHref();
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
      openElements.append( tag );
      elementsToOpen.remove( tag );
    }
  }
}

