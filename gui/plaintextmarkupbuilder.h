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

#ifndef GRANTLEE_PLAINTEXTMARKUPBUILDER_H
#define GRANTLEE_PLAINTEXTMARKUPBUILDER_H

#define LETTERSINALPHABET 26
#define DIGITSOFFSET 10

#include "abstractmarkupbuilder.h"

#include "grantlee_gui_export.h"

namespace Grantlee
{

class PlainTextMarkupBuilderPrivate;

/**
  @brief The PlainTextHTMLMarkupBuilder creates a simple marked up plain text document.

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

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_GUI_EXPORT PlainTextMarkupBuilder : public AbstractMarkupBuilder
{
public:
  /** Construct a new PlainTextHTMLMarkupBuilder. */
  PlainTextMarkupBuilder();

  virtual ~PlainTextMarkupBuilder();

  /* reimp */ void beginStrong();
  /* reimp */ void endStrong();
  /* reimp */ void beginEmph();
  /* reimp */ void endEmph();
  /* reimp */ void beginUnderline();
  /* reimp */ void endUnderline();
  /* reimp */ void beginStrikeout();
  /* reimp */ void endStrikeout();

  /* reimp */ void beginAnchor( const QString &href = QString(), const QString &name = QString() );

  /* reimp */ void endAnchor();

  /* reimp */ void endParagraph();
  /* reimp */ void addNewline();

  /* reimp */ void insertHorizontalRule( int width = -1 );

  /* reimp */ void insertImage( const QString &src, qreal width, qreal height );


  /* reimp */ void beginList( QTextListFormat::Style style );

  /* reimp */ void endList();

  /* reimp */ void beginListItem();

  /* reimp */ void endListItem();


  /* reimp */ void beginSuperscript();

  /* reimp */ void endSuperscript();

  /* reimp */ void beginSubscript();

  /* reimp */ void endSubscript();

  /* reimp */ void appendLiteralText( const QString &text );

  /**
  Returns the finalised plain text markup, including references at the end.
  */
  /* reimp */ QString& getResult();

private:
  PlainTextMarkupBuilderPrivate *d_ptr;
  Q_DECLARE_PRIVATE( PlainTextMarkupBuilder )

};

}

#endif
