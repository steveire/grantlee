/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_RENDERCONTEXT_H
#define GRANTLEE_RENDERCONTEXT_H

#include <QtCore/QVariantHash>

#include "grantlee_core_export.h"

namespace Grantlee
{

class ContextPrivate;
class TemplateImpl;
class Node;

class RenderContextPrivate;

// ### BIC Rewrite this and Context in terms of a common base class.

/**
 * @brief Provides storage facility for state while rendering a template.
 */
class GRANTLEE_CORE_EXPORT RenderContext
{
public:
  QVariant& data ( Node * const scopeNode );

  bool contains( Node * const scopeNode ) const;

  ~RenderContext();

private:
  RenderContext();

  void push();

  void pop();

private:
  friend class ContextPrivate;
  friend class TemplateImpl;

  Q_DISABLE_COPY(RenderContext)
  Q_DECLARE_PRIVATE(RenderContext)
  RenderContextPrivate * const d_ptr;
};

}

#endif
