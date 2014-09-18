/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#include "filters.h"

#include <QStringList>
#include <QTextDocument>

#include <grantlee/util.h>

#include "audiotextdocumentoutputter.h"

QVariant CleanHtmlFilter::doFilter(const QVariant& input, const QVariant& argument, bool autoescape) const
{
  QTextDocument *document = qobject_cast<QTextDocument*>(input.value<QObject*>());

  if (!document)
    return QString();

  AudioTextHtmlBuilder *builder = new AudioTextHtmlBuilder();
  Grantlee::MarkupDirector *director = new AudioTextDocumentDirector(builder);

  director->processDocument(document);
  QString result = builder->getResult();
  delete director;
  delete builder;
  return markSafe(result);
}

QVariant PlainTextFilter::doFilter(const QVariant& input, const QVariant& argument, bool autoescape) const
{
  QTextDocument *document = qobject_cast<QTextDocument*>(input.value<QObject*>());

  if (!document)
    return QString();

  AudioPlainTextBuilder *builder = new AudioPlainTextBuilder();
  Grantlee::MarkupDirector *director = new AudioTextDocumentDirector(builder);

  director->processDocument(document);
  QString result = builder->getResult();
  delete director;
  delete builder;
  return markSafe(result);
}
