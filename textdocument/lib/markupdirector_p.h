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

#ifndef GRANTLEE_MARKUPDIRECTOR_P_H
#define GRANTLEE_MARKUPDIRECTOR_P_H

#include "markupdirector.h"

#include <QtCore/QSet>

//@cond PRIVATE

namespace Grantlee
{

/**
  @internal
  Maintainability class for MarkupDirector
*/
class MarkupDirectorPrivate
{
  MarkupDirectorPrivate(MarkupDirector *md) : q_ptr(md), m_openFontPointSize(0), m_fontPointSizeToOpen(0) {}

  Q_DECLARE_PUBLIC(MarkupDirector)
  MarkupDirector *const q_ptr;

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
  QList<int> m_openElements;

  // Elements that have yet to be opened. Used while determine the order to
  // open them.
  QSet<int> m_elementsToOpen;
};
}

#endif
//@endcond
