/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef TEMPLATE_P_H
#define TEMPLATE_P_H

#include "template.h"

#include "enginestate_p.h"

namespace Grantlee
{

class TemplatePrivate
{
  TemplatePrivate( TemplateImpl *t )
      : q_ptr( t ), m_error( NoError ) {

  }

  void parse();
  NodeList compileString( const QString &str );
  void setError( Error type, const QString &message );

  qint64 m_settingsToken;
  Error m_error;
  QString m_errorString;
  NodeList m_nodeList;

  EngineState m_state;

  Q_DECLARE_PUBLIC( TemplateImpl )
  TemplateImpl *q_ptr;

  friend class Grantlee::Engine;

};

}

#endif
