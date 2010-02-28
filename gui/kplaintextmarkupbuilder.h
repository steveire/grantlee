/*
    This file is part of KDE.

    Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef KPLAINTEXTMARKUPBUILDER_H
#define KPLAINTEXTMARKUPBUILDER_H

#define LETTERSINALPHABET 26
#define DIGITSOFFSET 10

#include "kabstractmarkupbuilder.h"

#include "grantlee_gui_export.h"

class KPlainTextMarkupBuilderPrivate;

/**
@brief The KPlainTextMarkupBuilder creates a simple marked up plain text document.

This class creates a simple plain text markup.

Text that may be represented as

@code
    A paragraph with <b>bold</b> text, <i>italic</i> text, and <u>underlined</u> text.
@endcode

would be output as

@code
    A paragraph with *bold* text /italic/ text, and _underlined_ text.
@endcode

The markup is intended to be simple, plain and easily human readable. No markup is created for different font-familiy, font-size, foreground or background colors.

Lists are marked up by preceding the list element with '*' for disc, 'o' for circle, 'X' for square, or a letter or number. Lists are also indented if nested.
eg:

@code
    A. One
    B. Two
        o Three
        o Four
            \* Five
            \* Six
    C. Seven
@endcode

External references such as external urls and images are represented in the body text as a reference, and references are maintained at the bottom of the output.

Eg,
@code
        Here is a link to <a href="http://www.kde.org">KDE</a> and the <a href="http://pim.kde.org">KDEPIM project</a>.
@endcode

becomes:

@code
    Here is a link to KDE[1], and the KDEPIM project[2].

    ---- References ----
    [1] http://www.kde.org
    [2] http://pim.kde.org
@endcode

@todo Move this to kdelibs when tested and in use.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class GRANTLEE_GUI_EXPORT KPlainTextMarkupBuilder : public KAbstractMarkupBuilder
{
public:
    /** Construct a new KPlainTextMarkupBuilder. */
    KPlainTextMarkupBuilder();

    virtual void beginStrong();
    virtual void endStrong();
    virtual void beginEmph();
    virtual void endEmph();
    virtual void beginUnderline();
    virtual void endUnderline();
    virtual void beginStrikeout();
    virtual void endStrikeout();

    virtual void beginAnchor(const QString &href = QString(), const QString &name = QString());

    virtual void endAnchor();

    virtual void endParagraph();
    virtual void addNewline();

    virtual void insertHorizontalRule(int width = -1);

    virtual void insertImage(const QString &src, qreal width, qreal height);


    virtual void beginList(QTextListFormat::Style style);

    virtual void endList();

    virtual void beginListItem();

    virtual void endListItem();


    virtual void beginSuperscript();

    virtual void endSuperscript();

    virtual void beginSubscript();

    virtual void endSubscript();

    virtual void appendLiteralText(const QString &text);

    /**
    Returns the finalised plain text markup, including references at the end.
    */
    virtual QString& getResult();

private:
    KPlainTextMarkupBuilderPrivate *d_ptr;
    Q_DECLARE_PRIVATE(KPlainTextMarkupBuilder)


};

#endif
