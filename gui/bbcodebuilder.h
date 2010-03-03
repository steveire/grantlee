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

#ifndef GRANTLEE_BBCODEBUILDER_H
#define GRANTLEE_BBCODEBUILDER_H

#include "abstractmarkupbuilder.h"

namespace Grantlee
{

/**
Creates BBCode from a QTextDocument
*/
class BBCodeBuilder : public AbstractMarkupBuilder
{
public:

    /**
    Creates a new BBCodeBuilder.
    */
    BBCodeBuilder();

    virtual void beginStrong();
    virtual void endStrong();
    virtual void beginEmph();
    virtual void endEmph();
    virtual void beginUnderline();
    virtual void endUnderline();
    virtual void beginStrikeout();
    virtual void endStrikeout();
    virtual void beginForeground(const QBrush &brush);
    virtual void endForeground();

    // Background colour not supported by BBCode.

    virtual void beginAnchor(const QString &href = QString(), const QString &name = QString());
    virtual void endAnchor();

    // Font family not supported by BBCode.

    /**
    Begin an element of font size @p size. Note that this size is in pixels, and must be converted before
    it is suitable for use in BBCode.
    @param size The size of font to begin.
    */
    virtual void beginFontPointSize(int size);
    virtual void endFontPointSize();

    virtual void beginParagraph(Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0);

    virtual void endParagraph();
    virtual void addNewline();

    virtual void insertImage(const QString &src, qreal width, qreal height);

    virtual void beginList(QTextListFormat::Style type);

    virtual void endList();


    virtual void beginListItem();

    virtual void beginSuperscript();

    virtual void endSuperscript();

    virtual void beginSubscript();

    virtual void endSubscript();


    virtual void beginTable(qreal, qreal, const QString &);

    virtual void beginTableRow();


    virtual void appendLiteralText(const QString &text);

    const QString escape(const QString &s);

    virtual QString& getResult();

private:
    QList<QTextListFormat::Style> currentListItemStyles;

    QString m_text;

    Qt::Alignment currentAlignment;

};

}

#endif

