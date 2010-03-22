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

/// @headerfile outputstream.h grantlee/outputstream.h

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

  The OutputStream is used to escape the content streamed to it. By default, the escaping is html escaping, converting "&" to "&amp;" for example. If generating non-html output, the escape method may be overriden to perform a different escaping, or non at all.

  If overriding the escape method, the clone method must also be overriden to return an OutputStream with the same escaping behaviour.

  @code
    class NoEscapeStream : public Grantlee::OutputStream
    {
    public:
      // ...

      QString escape( const QString &input ) const
      {
        return input;
      }

      QSharedPointer<OutputStream> clone( QTextStream *stream ) const
      {
        QSharedPointer<OutputStream> clonedStream = QSharedPointer<OutputStream>( new NoEscapeStream( stream ) );
        return clonedStream;
      }
    };
  @endcode

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT OutputStream
{
public:
  /**
    Creates a null OutputStream. Content streamed to this OutputStream is sent to /dev/null
  */
  OutputStream();

  /**
    Creates an OutputStream which will stream content to @p stream with appropriate escaping.
  */
  explicit OutputStream( QTextStream *stream );

  /**
    Destructor
  */
  virtual ~OutputStream();

  /**
    Returns an escaped version of @p input. Does not write anything to the stream.
  */
  virtual QString escape( const QString &input ) const;

  /**
    Returns an escaped version of @p input. Does not write anything to the stream.
  */
  QString escape( const SafeString &input ) const;

  /**
    Returns an cloned OutputStream with the same filtering behaviour.
  */
  virtual QSharedPointer<OutputStream> clone( QTextStream *stream ) const;

  /**
    Returns @p after escaping it, unless @p input is "safe", in which case, @p input is returned unmodified.
  */
  QString conditionalEscape( const Grantlee::SafeString &input ) const;

  /**
    Writes @p input to the stream after escaping it.
  */
  OutputStream& operator<<( const QString &input );

  /**
    Writes @p input to the stream after escaping it if necessary.
  */
  OutputStream& operator<<( const SafeString &input );

  /**
    Reads the content of @p stream and writes it unmodified to the result stream.
  */
  OutputStream& operator<<( QTextStream *stream );

private:
  QTextStream *m_stream;
  Q_DISABLE_COPY( OutputStream )
};

}

#endif
