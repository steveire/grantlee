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

    /**
    Creates a new MediaWikiMarkupBuilder
    */
    MediaWikiMarkupBuilder();
    virtual ~MediaWikiMarkupBuilder();

    virtual void beginStrong();
    virtual void endStrong();
    virtual void beginEmph();
    virtual void endEmph();
    virtual void beginUnderline();
    virtual void endUnderline();
    virtual void beginStrikeout();
    virtual void endStrikeout();

    virtual void endParagraph();
    virtual void addNewline();

    virtual void beginAnchor(const QString &href = QString(), const QString &name = QString());
    virtual void endAnchor();

    virtual void beginHeader(int level);
    virtual void endHeader(int level);

    virtual void beginList(QTextListFormat::Style type);



    virtual void endList();


    virtual void beginListItem();
    virtual void endListItem();


    virtual void appendLiteralText(const QString &text);

    const QString escape(const QString &s);

    virtual QString& getResult();

private:
    QList<QTextListFormat::Style> currentListItemStyles;

    QString m_text;
};

}

#endif

