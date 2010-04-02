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

#include "audiotextdocumentoutputter.h"

AudioTextHtmlBuilder::AudioTextHtmlBuilder()
{

}

void AudioTextHtmlBuilder::addAudioTag( const QString &source )
{
  appendLiteralText( QString( "<audio src=\"%1\" />").arg( source ) );
//   appendRawText( QString( "<audio src=\"%1\" />").arg( source ) );
}

AudioPlainTextBuilder::AudioPlainTextBuilder()
{

}

void AudioPlainTextBuilder::addAudioTag(const QString& source)
{
  int ref = addReference( source );

  appendRawText( QString( "[%1]" ).arg( ref ) );
}

AudioTextDocumentDirector::AudioTextDocumentDirector(AbstractAudioBuilder* builder)
  : MarkupDirector(builder), m_builder(builder)
{

}

void AudioTextDocumentDirector::processCustomFragment(const QTextFragment& fragment, const QTextDocument* doc)
{
  if ( fragment.charFormat().objectType() != AudioType )
    return Grantlee::MarkupDirector::processCustomFragment( fragment, doc );

  QString name = fragment.charFormat().property( AudioProperty ).toString();

  m_builder->addAudioTag( name );
}
