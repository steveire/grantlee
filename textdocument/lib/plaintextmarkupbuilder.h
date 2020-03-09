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

#ifndef GRANTLEE_PLAINTEXTMARKUPBUILDER_H
#define GRANTLEE_PLAINTEXTMARKUPBUILDER_H

#ifdef Q_OS_WIN
#pragma warning(disable : 4250)
#endif

#define LETTERSINALPHABET 26
#define DIGITSOFFSET 10

#include "abstractmarkupbuilder.h"
#include "grantlee_textdocument_export.h"
#include "markupdirector.h"

class QBrush;

namespace Grantlee
{

class PlainTextMarkupBuilderPrivate;

/// @headerfile plaintextmarkupbuilder.h grantlee/plaintextmarkupbuilder.h

/**
  @brief Creates a simple marked up plain text document

  This class creates a simple plain text markup.

  Text that may be represented as

  @code
    A paragraph with <b>bold</b> text, <i>italic</i> text,
    and <u>underlined</u> text.
  @endcode

  would be output as

  @code
    A paragraph with *bold* text /italic/ text,
    and _underlined_ text.
  @endcode

  The markup is intended to be simple, plain and easily human readable. No
  markup is created for different font-familiy, font-size, foreground or
  background colors.

  Lists are marked up by preceding the list element with '*' for disc, 'o' for
  circle, 'X' for square, or a letter or number. Lists are also indented if
  nested. eg:

  @code
    A. One
    B. Two
      o Three
      o Four
        \* Five
        \* Six
    C. Seven
  @endcode

  External references such as external urls and images are represented in the
  body text as a reference, and references are maintained at the bottom of the
  output.

  Eg,
  @code
    Here is a link to <a href="http://www.kde.org">KDE</a> and
    the <a href="http://pim.kde.org">KDEPIM project</a>.
  @endcode

  becomes:

  @code
    Here is a link to KDE[1], and
    the KDEPIM project[2].

    ---- References ----
    [1] http://www.kde.org
    [2] http://pim.kde.org
  @endcode

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_TEXTDOCUMENT_EXPORT PlainTextMarkupBuilder
    : virtual public AbstractMarkupBuilder
{
public:
  PlainTextMarkupBuilder();

  ~PlainTextMarkupBuilder() override;

  void beginStrong() override;
  void endStrong() override;
  void beginEmph() override;
  void endEmph() override;
  void beginUnderline() override;
  void endUnderline() override;
  void beginStrikeout() override;
  void endStrikeout() override;

  void beginAnchor(const QString &href = {}, const QString &name = {}) override;

  void endAnchor() override;

  void beginForeground(const QBrush &brush) override;

  void endForeground() override;

  void beginBackground(const QBrush &brush) override;

  void endBackground() override;

  void beginFontFamily(const QString &family) override;

  void endFontFamily() override;

  void beginFontPointSize(int size) override;

  void endFontPointSize() override;

  void beginParagraph(Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0,
                      qreal bottom = 0.0, qreal left = 0.0,
                      qreal right = 0.0) override;

  void endParagraph() override;
  void addNewline() override;

  void insertHorizontalRule(int width = -1) override;

  void insertImage(const QString &src, qreal width, qreal height) override;

  void beginList(QTextListFormat::Style style) override;

  void endList() override;

  void beginListItem() override;

  void endListItem() override;

  void beginSuperscript() override;

  void endSuperscript() override;

  void beginSubscript() override;

  void endSubscript() override;

  void beginTable(qreal cellpadding, qreal cellspacing,
                  const QString &width) override;

  void beginTableRow() override;

  void beginTableHeaderCell(const QString &width, int colSpan,
                            int rowSpan) override;

  void beginTableCell(const QString &width, int colSpan, int rowSpan) override;

  void endTable() override;

  void endTableRow() override;

  void endTableHeaderCell() override;

  void endTableCell() override;

  void beginHeader(int level) override;

  void endHeader(int level) override;

  void appendLiteralText(const QString &text) override;

  void appendRawText(const QString &text) override;

  /**
    Adds a reference to @p reference to the internal list of references in the
    document.
  */
  int addReference(const QString &reference);

  /**
    Returns the finalised plain text markup, including references at the end.
  */
  QString getResult() override;

private:
  PlainTextMarkupBuilderPrivate *const d_ptr;
  Q_DECLARE_PRIVATE(PlainTextMarkupBuilder)
};
}

#endif
