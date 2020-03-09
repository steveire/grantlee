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

#ifndef GRANTLEE_MEDIAWIKIMARKUPBUILDER_H
#define GRANTLEE_MEDIAWIKIMARKUPBUILDER_H

#include "abstractmarkupbuilder.h"

namespace Grantlee
{

/**
  @brief Creates MediaWiki markup from a QTextDocument
*/
class MediaWikiMarkupBuilder : public AbstractMarkupBuilder
{
public:
  MediaWikiMarkupBuilder();
  ~MediaWikiMarkupBuilder() override;

  void beginStrong() override;
  void endStrong() override;
  void beginEmph() override;
  void endEmph() override;
  void beginUnderline() override;
  void endUnderline() override;
  void beginStrikeout() override;
  void endStrikeout() override;

  void endParagraph() override;
  void addNewline() override;

  void beginAnchor(const QString &href = {}, const QString &name = {}) override;
  void endAnchor() override;

  void beginHeader(int level) override;
  void endHeader(int level) override;

  void beginList(QTextListFormat::Style type) override;

  void endList() override;

  void beginListItem() override;
  void endListItem() override;

  void appendLiteralText(const QString &text) override;

  /**
    Escapes @p text appropriately for MediaWiki.
  */
  const QString escape(const QString &s);

  QString getResult() override;

private:
  QList<QTextListFormat::Style> currentListItemStyles;

  QString m_text;
};
}

#endif
