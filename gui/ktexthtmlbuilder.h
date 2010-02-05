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

#ifndef KTEXTHTMLBUILDER_H
#define KTEXTHTMLBUILDER_H

#include "kabstractmarkupbuilder.h"

class KTextHTMLBuilderPrivate;

/**
@brief The KTextHTMLBuilder creates a clean html markup output.

This class creates html output which is as minimal as possible and restricted to the rich text features supported in Qt. (http://doc.trolltech.com/4.4/richtext-html-subset.htm)

The output contains only the body content, not the head element or other metadata.

eg:

@code
  <p>
    This is some <strong>formatted content</strong> in a paragraph.
  </p>
@endcode

instead of the content produced by qt:

@code
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
<html><head><meta name="qrichtext" content="1" /><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /><style type="text/css">
p, li { white-space: pre-wrap; }
</style></head><body style=" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;">
<p style=" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">This is some <span style=" font-weight:600;">formatted content</span> in a paragraph. </p></body></html>
@endcode

Such tags should be created separately. For example:

@code
        AbstractMarkupBuilder *b = new KTextHTMLBuilder();
        KMarkupDirector *md = new KMarkupDirector(b);
        md->constructContent();
        QString cleanHtml("<head>\n<title>%1</title>\n</head>\n<body>%2</body>\n</html>")
                    .arg(document.metaInformation(QTextDocument::DocumentTitle))
                    .arg(b->getOutput());
        QFile.write(cleanHtml);
@endcode

Font formatting information on elements is represented by individual span elements.
eg:
@code
    <span style"color:blue;"><span style="background-color:red;">Blue text on red background</span></span>
@endcode
instead of
@code
    <span style="color:blue;background-color:red;">Blue text on red background</span>
@endcode
 It my be possible to change this if necessary.

@todo Move this to kdelibs when tested and prooven.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class KTextHTMLBuilder : public KAbstractMarkupBuilder
{
public:

    /**
    Creates a new KTextHTMLBuilder.
    */
    KTextHTMLBuilder();
    virtual ~KTextHTMLBuilder();

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
    virtual void beginBackground(const QBrush &brush);
    virtual void endBackground();
    virtual void beginAnchor(const QString &href = QString(), const QString &name = QString());
    virtual void endAnchor();

    // Maybe this stuff should just be added to a list, and then when I add literal text,
    // add some kind of style attribute in one span instead of many.
    virtual void beginFontFamily(const QString &family);
    virtual void endFontFamily();

    /**
      Begin a new font point size
      @param size The new size to begin.
    */
    virtual void beginFontPointSize(int size);
    virtual void endFontPointSize();

    /**
    Begin a new paragraph
    @param al The new paragraph alignment
    @param topMargin The new paragraph topMargin
    @param bottomMargin The new paragraph bottomMargin
    @param leftMargin The new paragraph leftMargin
    @param rightMargin The new paragraph rightMargin
    */
    virtual void beginParagraph(Qt::Alignment al = Qt::AlignLeft, qreal topMargin = 0.0, qreal bottomMargin = 0.0, qreal leftMargin = 0.0, qreal rightMargin = 0.0);

    /**
    Begin a new header element.
    @param level The new level to begin.
    */
    virtual void beginHeader(int level);

    /**
    End a header element.
    @param level The new level to end.
    */
    virtual void endHeader(int level);

    virtual void endParagraph();
    virtual void addNewline();

    virtual void insertHorizontalRule(int width = -1);

    virtual void insertImage(const QString &src, qreal width, qreal height);

    virtual void beginList(QTextListFormat::Style type);

    virtual void endList();

    virtual void beginListItem();
    virtual void endListItem();

    virtual void beginSuperscript();

    virtual void endSuperscript();

    virtual void beginSubscript();

    virtual void endSubscript();


    virtual void beginTable(qreal cellpadding, qreal cellspacing, const QString &width);

    virtual void beginTableRow();
    virtual void beginTableHeaderCell(const QString &width, int colspan, int rowspan);

    virtual void beginTableCell(const QString &width, int colspan, int rowspan);

    virtual void endTable();
    virtual void endTableRow();
    virtual void endTableHeaderCell();
    virtual void endTableCell();

    /**
    Reimplemented from AbstractMarkupBuilder.

    This implementation escapes the text before appending so that

    @verbatim
        A sample <b>bold</b> word.
    @endverbatim

    becomes

    @verbatim
         A sample &lt;b&gt;bold&lt;/b&gt; word.
    @endverbatim

    */
    virtual void appendLiteralText(const QString &text);


    virtual QString& getResult();

private:
    KTextHTMLBuilderPrivate *d_ptr;
    Q_DECLARE_PRIVATE(KTextHTMLBuilder)

};

#endif
