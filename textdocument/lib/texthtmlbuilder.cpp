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

#include "texthtmlbuilder.h"

#include <QtCore/QList>
#include <QtCore/QBuffer>
#include <QtGui/QTextDocument>

namespace Grantlee
{

class TextHTMLBuilderPrivate
{
public:
  TextHTMLBuilderPrivate(TextHTMLBuilder *b) : q_ptr(b) {}

  QList<QTextListFormat::Style> currentListItemStyles;
  QString m_text;

  TextHTMLBuilder *q_ptr;

  Q_DECLARE_PUBLIC(TextHTMLBuilder)
};
}

using namespace Grantlee;

TextHTMLBuilder::TextHTMLBuilder()
    : AbstractMarkupBuilder(), d_ptr(new TextHTMLBuilderPrivate(this))
{
}

TextHTMLBuilder::~TextHTMLBuilder() { delete d_ptr; }

void TextHTMLBuilder::beginStrong()
{
  Q_D(TextHTMLBuilder);
  ;
  d->m_text.append(QStringLiteral("<strong>"));
}

void TextHTMLBuilder::endStrong()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</strong>"));
}

void TextHTMLBuilder::beginEmph()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<em>"));
}

void TextHTMLBuilder::endEmph()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</em>"));
}

void TextHTMLBuilder::beginUnderline()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<u>"));
}

void TextHTMLBuilder::endUnderline()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</u>"));
}

void TextHTMLBuilder::beginStrikeout()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<s>"));
}

void TextHTMLBuilder::endStrikeout()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</s>"));
}

void TextHTMLBuilder::beginForeground(const QBrush &brush)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(
      QStringLiteral("<span style=\"color:%1;\">").arg(brush.color().name()));
}

void TextHTMLBuilder::endForeground()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginBackground(const QBrush &brush)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<span style=\"background-color:%1;\">")
                       .arg(brush.color().name()));
}

void TextHTMLBuilder::endBackground()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginAnchor(const QString &href, const QString &name)
{
  Q_D(TextHTMLBuilder);
  if (!href.isEmpty()) {
    if (!name.isEmpty()) {
      d->m_text.append(
          QStringLiteral("<a href=\"%1\" name=\"%2\">").arg(href, name));
    } else {
      d->m_text.append(QStringLiteral("<a href=\"%1\">").arg(href));
    }
  } else {
    if (!name.isEmpty()) {
      d->m_text.append(QStringLiteral("<a name=\"%1\">").arg(name));
    }
  }
}

void TextHTMLBuilder::endAnchor()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</a>"));
}

void TextHTMLBuilder::beginFontFamily(const QString &family)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(
      QStringLiteral("<span style=\"font-family:%1;\">").arg(family));
}

void TextHTMLBuilder::endFontFamily()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginFontPointSize(int size)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<span style=\"font-size:%1pt;\">")
                       .arg(QString::number(size)));
}

void TextHTMLBuilder::endFontPointSize()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginParagraph(Qt::Alignment al, qreal topMargin,
                                     qreal bottomMargin, qreal leftMargin,
                                     qreal rightMargin)
{
  Q_D(TextHTMLBuilder);
  // Don't put paragraph tags inside li tags. Qt bug reported.
  //     if (currentListItemStyles.size() != 0)
  //     {
  QString styleString;
  if (topMargin != 0) {
    styleString.append(QStringLiteral("margin-top:%1;").arg(topMargin));
  }
  if (bottomMargin != 0) {
    styleString.append(QStringLiteral("margin-bottom:%1;").arg(bottomMargin));
  }
  if (leftMargin != 0) {
    styleString.append(QStringLiteral("margin-left:%1;").arg(leftMargin));
  }
  if (rightMargin != 0) {
    styleString.append(QStringLiteral("margin-right:%1;").arg(rightMargin));
  }

  // Using == doesn't work here.
  // Using bitwise comparison because an alignment can contain a vertical and
  // a
  // horizontal part.
  if (al & Qt::AlignRight) {
    d->m_text.append(QStringLiteral("<p align=\"right\" "));
  } else if (al & Qt::AlignHCenter) {
    d->m_text.append(QStringLiteral("<p align=\"center\" "));
  } else if (al & Qt::AlignJustify) {
    d->m_text.append(QStringLiteral("<p align=\"justify\" "));
  } else if (al & Qt::AlignLeft) {
    d->m_text.append(QStringLiteral("<p"));
  } else {
    d->m_text.append(QStringLiteral("<p"));
  }

  if (!styleString.isEmpty()) {
    d->m_text.append(QStringLiteral(" \"") + styleString + QLatin1Char('"'));
  }
  d->m_text.append(QLatin1Char('>'));
  //     }
}

void TextHTMLBuilder::beginHeader(int level)
{
  Q_D(TextHTMLBuilder);
  switch (level) {
  case 1:
    d->m_text.append(QStringLiteral("<h1>"));
    break;
  case 2:
    d->m_text.append(QStringLiteral("<h2>"));
    break;
  case 3:
    d->m_text.append(QStringLiteral("<h3>"));
    break;
  case 4:
    d->m_text.append(QStringLiteral("<h4>"));
    break;
  case 5:
    d->m_text.append(QStringLiteral("<h5>"));
    break;
  case 6:
    d->m_text.append(QStringLiteral("<h6>"));
    break;
  default:
    break;
  }
}

void TextHTMLBuilder::endHeader(int level)
{
  Q_D(TextHTMLBuilder);
  switch (level) {
  case 1:
    d->m_text.append(QStringLiteral("</h1>"));
    break;
  case 2:
    d->m_text.append(QStringLiteral("</h2>"));
    break;
  case 3:
    d->m_text.append(QStringLiteral("</h3>"));
    break;
  case 4:
    d->m_text.append(QStringLiteral("</h4>"));
    break;
  case 5:
    d->m_text.append(QStringLiteral("</h5>"));
    break;
  case 6:
    d->m_text.append(QStringLiteral("</h6>"));
    break;
  default:
    break;
  }
}

void TextHTMLBuilder::endParagraph()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</p>\n"));
}

void TextHTMLBuilder::addNewline()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<p>&nbsp;"));
}

void TextHTMLBuilder::insertHorizontalRule(int width)
{
  Q_D(TextHTMLBuilder);
  if (width != -1) {
    d->m_text.append(QStringLiteral("<hr width=\"%1\" />\n").arg(width));
  }
  d->m_text.append(QStringLiteral("<hr />\n"));
}

void TextHTMLBuilder::insertImage(const QString &src, qreal width, qreal height)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<img src=\"%1\" ").arg(src));
  if (width != 0)
    d->m_text.append(QStringLiteral("width=\"%2\" ").arg(width));
  if (height != 0)
    d->m_text.append(QStringLiteral("height=\"%2\" ").arg(height));
  d->m_text.append(QStringLiteral("/>"));
}

void TextHTMLBuilder::insertImage(const QImage &image, qreal width, qreal height)
{
  Q_D(TextHTMLBuilder);
  QByteArray data;
  QBuffer buffer(&data);
  image.save(&buffer, "PNG");
  d->m_text.append(QStringLiteral("<img src=\"data:image/png;base64,%1\" ").arg(QString::fromLatin1(data.toBase64())));
  if (width != 0)
    d->m_text.append(QStringLiteral("width=\"%2\" ").arg(width));
  if (height != 0)
    d->m_text.append(QStringLiteral("height=\"%2\" ").arg(height));
  d->m_text.append(QStringLiteral("/>"));
}

void TextHTMLBuilder::beginList(QTextListFormat::Style type)
{
  Q_D(TextHTMLBuilder);
  d->currentListItemStyles.append(type);
  switch (type) {
  case QTextListFormat::ListDisc:
    d->m_text.append(QStringLiteral("<ul type=\"disc\">\n"));
    break;
  case QTextListFormat::ListCircle:
    d->m_text.append(QStringLiteral("\n<ul type=\"circle\">\n"));
    break;
  case QTextListFormat::ListSquare:
    d->m_text.append(QStringLiteral("\n<ul type=\"square\">\n"));
    break;
  case QTextListFormat::ListDecimal:
    d->m_text.append(QStringLiteral("\n<ol type=\"1\">\n"));
    break;
  case QTextListFormat::ListLowerAlpha:
    d->m_text.append(QStringLiteral("\n<ol type=\"a\">\n"));
    break;
  case QTextListFormat::ListUpperAlpha:
    d->m_text.append(QStringLiteral("\n<ol type=\"A\">\n"));
    break;
  case QTextListFormat::ListLowerRoman:
    d->m_text.append(QStringLiteral("\n<ol type=\"i\">\n"));
    break;
  case QTextListFormat::ListUpperRoman:
    d->m_text.append(QStringLiteral("\n<ol type=\"I\">\n"));
    break;
  default:
    break;
  }
}
void TextHTMLBuilder::endList()
{
  Q_D(TextHTMLBuilder);
  switch (d->currentListItemStyles.last()) {
  case QTextListFormat::ListDisc:
  case QTextListFormat::ListCircle:
  case QTextListFormat::ListSquare:
    d->m_text.append(QStringLiteral("</ul>\n"));
    break;
  case QTextListFormat::ListDecimal:
  case QTextListFormat::ListLowerAlpha:
  case QTextListFormat::ListUpperAlpha:
  case QTextListFormat::ListLowerRoman:
  case QTextListFormat::ListUpperRoman:
    d->m_text.append(QStringLiteral("</ol>\n"));
    break;
  default:
    break;
  }
  d->currentListItemStyles.removeLast();
}
void TextHTMLBuilder::beginListItem()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<li>"));
}

void TextHTMLBuilder::endListItem()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</li>\n"));
}

void TextHTMLBuilder::beginSuperscript()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<sup>"));
}

void TextHTMLBuilder::endSuperscript()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</sup>"));
}

void TextHTMLBuilder::beginSubscript()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<sub>"));
}

void TextHTMLBuilder::endSubscript()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</sub>"));
}

void TextHTMLBuilder::beginTable(qreal cellpadding, qreal cellspacing,
                                 const QString &width)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(
      QStringLiteral("<table cellpadding=\"%1\" cellspacing=\"%2\" "
                     "width=\"%3\" border=\"1\">")
          .arg(cellpadding)
          .arg(cellspacing)
          .arg(width));
}

void TextHTMLBuilder::beginTableRow()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("<tr>"));
}

void TextHTMLBuilder::beginTableHeaderCell(const QString &width, int colspan,
                                           int rowspan)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(
      QStringLiteral("<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">")
          .arg(width)
          .arg(colspan)
          .arg(rowspan));
}

void TextHTMLBuilder::beginTableCell(const QString &width, int colspan,
                                     int rowspan)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(
      QStringLiteral("<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">")
          .arg(width)
          .arg(colspan)
          .arg(rowspan));
}

void TextHTMLBuilder::endTable()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</table>"));
}

void TextHTMLBuilder::endTableRow()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</tr>"));
}

void TextHTMLBuilder::endTableHeaderCell()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</th>"));
}

void TextHTMLBuilder::endTableCell()
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(QStringLiteral("</td>"));
}

void TextHTMLBuilder::appendLiteralText(const QString &text)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(text.toHtmlEscaped());
}

void TextHTMLBuilder::appendRawText(const QString &text)
{
  Q_D(TextHTMLBuilder);
  d->m_text.append(text);
}

QString TextHTMLBuilder::getResult()
{
  Q_D(TextHTMLBuilder);
  auto ret = d->m_text;
  d->m_text.clear();
  return ret;
}
