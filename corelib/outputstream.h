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

#ifndef GRANTLEE_OUTPUTSTREAM_H
#define GRANTLEE_OUTPUTSTREAM_H

#include <QtCore/QTextStream>
#include <QtCore/QSharedPointer>

#include "grantlee_core_export.h"

namespace Grantlee
{

class SafeString;

/**
  @brief The OutputStream class is used to render templates to a QTextStream

  A OutputStream instance may be passed to the render method of a Template to render
  the template to a stream.

  @code
    QFile outputFile("./output");
    outputFile.open(QFile::WriteOnly);
    QTextStream tstream( &outputFile );

    OutputStream os(&tstream);
    t->render( &os, &context );
  @endcode

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT OutputStream
{
public:
  OutputStream();
  explicit OutputStream( QTextStream *stream );
  virtual ~OutputStream();

  virtual QString escape( const QString &input ) const;
  QString escape( const SafeString &input ) const;

  virtual QSharedPointer<OutputStream> clone( QTextStream *stream ) const;

  QString conditionalEscape( const Grantlee::SafeString &input ) const;

  OutputStream& operator<<( const QString &input );

  OutputStream& operator<<( const SafeString &input );

  OutputStream& operator<<( QTextStream *stream );

protected:
  QTextStream *m_stream;

private:
  Q_DISABLE_COPY( OutputStream )
};

}

#endif
