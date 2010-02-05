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

#include "ktexthtmlbuilder.h"

#include <QTextDocument>
#include <QList>

class KTextHTMLBuilderPrivate
{
public:
    KTextHTMLBuilderPrivate(KTextHTMLBuilder *b) : q_ptr(b) {

    }

    QList<QTextListFormat::Style> currentListItemStyles;
    QString m_text;

    KTextHTMLBuilder *q_ptr;

    Q_DECLARE_PUBLIC(KTextHTMLBuilder)

};

KTextHTMLBuilder::KTextHTMLBuilder() : d_ptr(new KTextHTMLBuilderPrivate(this))
{

}

KTextHTMLBuilder::~KTextHTMLBuilder()
{
   delete d_ptr;
}

void KTextHTMLBuilder::beginStrong()
{
    Q_D(KTextHTMLBuilder);;
    d->m_text.append("<strong>");
}

void KTextHTMLBuilder::endStrong()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</strong>");
}

void KTextHTMLBuilder::beginEmph()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<em>");
}

void KTextHTMLBuilder::endEmph()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</em>");
}

void KTextHTMLBuilder::beginUnderline()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<u>");
}

void KTextHTMLBuilder::endUnderline()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</u>");
}

void KTextHTMLBuilder::beginStrikeout()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<s>");
}

void KTextHTMLBuilder::endStrikeout()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</s>");
}

void KTextHTMLBuilder::beginForeground(const QBrush &brush)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<span style=\"color:%1;\">").arg(brush.color().name()));
}

void KTextHTMLBuilder::endForeground()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</span>");
}

void KTextHTMLBuilder::beginBackground(const QBrush &brush)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<span style=\"background-color:%1;\">").arg(brush.color().name()));
}

void KTextHTMLBuilder::endBackground()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</span>");
}

void KTextHTMLBuilder::beginAnchor(const QString &href, const QString &name)
{
    Q_D(KTextHTMLBuilder);
    if (!href.isEmpty()) {
        if (!name.isEmpty()) {
            d->m_text.append(QString("<a href=\"%1\" name=\"%2\">").arg(href).arg(name));
        } else {
            d->m_text.append(QString("<a href=\"%1\">").arg(href));
        }
    } else {
        if (!name.isEmpty()) {
            d->m_text.append(QString("<a name=\"%1\">").arg(name));
        }
    }
}

void KTextHTMLBuilder::endAnchor()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</a>");
}

void KTextHTMLBuilder::beginFontFamily(const QString &family)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<span style=\"font-family:%1;\">").arg(family));
}

void KTextHTMLBuilder::endFontFamily()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</span>");
}

void KTextHTMLBuilder::beginFontPointSize(int size)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<span style=\"font-size:%1pt;\">").arg(QString::number(size)));
}

void KTextHTMLBuilder::endFontPointSize()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</span>");
}

void KTextHTMLBuilder::beginParagraph(Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin)
{
    Q_D(KTextHTMLBuilder);
    // Don't put paragraph tags inside li tags. Qt bug reported.
//     if (currentListItemStyles.size() != 0)
//     {
    QString styleString;
    if (topMargin != 0) {
        styleString.append(QString("margin-top:%1;").arg(topMargin));
    }
    if (bottomMargin != 0) {
        styleString.append(QString("margin-bottom:%1;").arg(bottomMargin));
    }
    if (leftMargin != 0) {
        styleString.append(QString("margin-left:%1;").arg(leftMargin));
    }
    if (rightMargin != 0) {
        styleString.append(QString("margin-right:%1;").arg(rightMargin));
    }

    // Using == doesn't work here.
    // Using bitwise comparison because an alignment can contain a vertical and a horizontal part.
    if (al & Qt::AlignRight) {
        d->m_text.append("<p align=\"right\" ");
    } else if (al & Qt::AlignHCenter) {
        d->m_text.append("<p align=\"center\" ");
    } else if (al & Qt::AlignJustify) {
        d->m_text.append("<p align=\"justify\" ");
    } else if (al & Qt::AlignLeft) {
        d->m_text.append("<p");
    } else {
        d->m_text.append("<p");
    }

    if (!styleString.isEmpty()) {
        d->m_text.append(" \"" + styleString + "\"");
    }
    d->m_text.append(">");
//     }
}

void KTextHTMLBuilder::beginHeader(int level)
{
    Q_D(KTextHTMLBuilder);
    switch (level) {
    case 1:
        d->m_text.append("<h1>");
        break;
    case 2:
        d->m_text.append("<h2>");
        break;
    case 3:
        d->m_text.append("<h3>");
        break;
    case 4:
        d->m_text.append("<h4>");
        break;
    case 5:
        d->m_text.append("<h5>");
        break;
    case 6:
        d->m_text.append("<h6>");
        break;
    default:
        break;
    }
}

void KTextHTMLBuilder::endHeader(int level)
{
    Q_D(KTextHTMLBuilder);
    switch (level) {
    case 1:
        d->m_text.append("</h1>");
        break;
    case 2:
        d->m_text.append("</h2>");
        break;
    case 3:
        d->m_text.append("</h3>");
        break;
    case 4:
        d->m_text.append("</h4>");
        break;
    case 5:
        d->m_text.append("</h5>");
        break;
    case 6:
        d->m_text.append("</h6>");
        break;
    default:
        break;
    }
}

void KTextHTMLBuilder::endParagraph()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</p>\n");
}

void KTextHTMLBuilder::addNewline()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<br />\n");
}

void KTextHTMLBuilder::insertHorizontalRule(int width)
{
    Q_D(KTextHTMLBuilder);
    if (width != -1) {
        d->m_text.append(QString("<hr width=\"%1\" />\n").arg(width));
    }
    d->m_text.append("<hr />\n");
}

void KTextHTMLBuilder::insertImage(const QString &src, qreal width, qreal height)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<img src=\"%1\" ").arg(src));
    if (width != 0) d->m_text.append(QString("width=\"%2\" ").arg(width));
    if (height != 0) d->m_text.append(QString("height=\"%2\" ").arg(height));
    d->m_text.append("/>");
}

void KTextHTMLBuilder::beginList(QTextListFormat::Style type)
{
    Q_D(KTextHTMLBuilder);
    d->currentListItemStyles.append(type);
    switch (type) {
    case QTextListFormat::ListDisc:
        d->m_text.append("<ul type=\"disc\">\n");
        break;
    case QTextListFormat::ListCircle:
        d->m_text.append("\n<ul type=\"circle\">\n");
        break;
    case QTextListFormat::ListSquare:
        d->m_text.append("\n<ul type=\"square\">\n");
        break;
    case QTextListFormat::ListDecimal:
        d->m_text.append("\n<ol type=\"1\">\n");
        break;
    case QTextListFormat::ListLowerAlpha:
        d->m_text.append("\n<ol type=\"a\">\n");
        break;
    case QTextListFormat::ListUpperAlpha:
        d->m_text.append("\n<ol type=\"A\">\n");
        break;
    default:
        break;
    }
}
void KTextHTMLBuilder::endList()
{
    Q_D(KTextHTMLBuilder);
    switch (d->currentListItemStyles.last()) {
    case QTextListFormat::ListDisc:
    case QTextListFormat::ListCircle:
    case QTextListFormat::ListSquare:
        d->m_text.append("</ul>\n");
        break;
    case QTextListFormat::ListDecimal:
    case QTextListFormat::ListLowerAlpha:
    case QTextListFormat::ListUpperAlpha:
        d->m_text.append("</ol>\n");
        break;
    default:
        break;
    }
    d->currentListItemStyles.removeLast();
}
void KTextHTMLBuilder::beginListItem()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<li>");
}

void KTextHTMLBuilder::endListItem()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</li>\n");
}

void KTextHTMLBuilder::beginSuperscript()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<sup>");
}

void KTextHTMLBuilder::endSuperscript()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</sup>");
}

void KTextHTMLBuilder::beginSubscript()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<sub>");
}

void KTextHTMLBuilder::endSubscript()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</sub>");
}


void KTextHTMLBuilder::beginTable(qreal cellpadding, qreal cellspacing, const QString &width)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<table cellpadding=\"%1\" cellspacing=\"%2\" width=\"%3\" border=\"1\">")
                     .arg(cellpadding)
                     .arg(cellspacing)
                     .arg(width));
}

void KTextHTMLBuilder::beginTableRow()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("<tr>");
}

void KTextHTMLBuilder::beginTableHeaderCell(const QString &width, int colspan, int rowspan)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
}

void KTextHTMLBuilder::beginTableCell(const QString &width, int colspan, int rowspan)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(QString("<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
}

void KTextHTMLBuilder::endTable()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</table>");
}

void KTextHTMLBuilder::endTableRow()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</tr>");
}

void KTextHTMLBuilder::endTableHeaderCell()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</th>");
}

void KTextHTMLBuilder::endTableCell()
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append("</td>");
}


void KTextHTMLBuilder::appendLiteralText(const QString &text)
{
    Q_D(KTextHTMLBuilder);
    d->m_text.append(Qt::escape(text));
}


QString& KTextHTMLBuilder::getResult()
{
    Q_D(KTextHTMLBuilder);
    return d->m_text;
}
