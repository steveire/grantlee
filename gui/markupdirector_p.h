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

#ifndef GRANTLEE_MARKUPDIRECTOR_P_H
#define GRANTLEE_MARKUPDIRECTOR_P_H

#include "markupdirector.h"

#include <QtCore/QSet>

//@cond PRIVATE

namespace Grantlee
{

/**
  @internal
  Maintainability class for KMarkupDirector
*/
class MarkupDirectorPrivate
{
public:
  MarkupDirectorPrivate( MarkupDirector *md )
      : q_ptr( md ) {
  }

  Q_DECLARE_PUBLIC( MarkupDirector )
  MarkupDirector * q_ptr;

  void processClosingElements( QTextBlock::iterator it );
  void processOpeningElements( QTextBlock::iterator it );
  QSet< int > getElementsToClose( QTextBlock::iterator it ) const;
  QList< int > getElementsToOpen( QTextBlock::iterator it );
  QList< int > sortOpeningOrder( QSet< int > openingOrder, QTextBlock::iterator it ) const;

  enum OpenElementValues {
    None = 0x0,
    SuperScript = 0x01,
    SubScript = 0x02,
    Anchor = 0x04,
    SpanForeground = 0x08,
    SpanBackground = 0x10,
    SpanFontFamily = 0x20,
    SpanFontPointSize = 0x40,
    Strong = 0x80,
    Emph = 0x100,
    Underline = 0x200,
    StrikeOut = 0x400
  };
//     Q_DECLARE_FLAGS(OpenElements, OpenElementValues)

  QString m_openAnchorHref;
  QString m_anchorHrefToOpen;
  QString m_openAnchorName;

  QBrush m_openForeground;
  QBrush m_foregroundToOpen;
  QBrush m_openBackground;
  QBrush m_backgroundToOpen;
  int m_openFontPointSize;
  int m_fontPointSizeToOpen;
  QString m_openFontFamily;
  QString m_fontFamilyToOpen;

  // An ordered list containing the order elements were opened in.
  QList< int > m_openElements;

  // Elements that have yet to be opened. Used while determine the order to open them.
  QSet< int > m_elementsToOpen;

};
// Q_DECLARE_OPERATORS_FOR_FLAGS(KMarkupDirector::Private::OpenElements)

}

#endif
//@endcond
