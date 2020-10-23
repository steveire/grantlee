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

#include <QtCore/QRegularExpression>
#include <QtCore>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtGui/QTextLine>
#include <QtTest/QtTest>
#include <QtTest/qtestevent.h>

#include "coverageobject.h"
#include "markupdirector.h"
#include "plaintextmarkupbuilder.h"
#include "testutils.h"
#include "texthtmlbuilder.h"

#include <memory>

using namespace Grantlee;

class TestPlainMarkupOutput : public CoverageObject
{
  Q_OBJECT
private Q_SLOTS:

  // Test paragraph contents:
  void testSingleFormat();
  void testDoubleFormat();
  void testDoubleStartDifferentFinish();
  void testDoubleStartDifferentFinishReverseOrder();
  void testDifferentStartDoubleFinish();
  void testDifferentStartDoubleFinishReverseOrder();
  void testOverlap();
  void testAnchor();
  void testAnchorWithFormattedContent();
  void testAdjacentAnchors();
  void testNestedFormatting();
  void testSpan();
  void testDoubleSpan();
  void testSpanNesting();
  void testEdgeCaseLeft();
  void testEdgeCaseRight();
  void testImage();
  void testImageResized();
  void testEachFormatTagSingly();
  void testHorizontalRule();
  void testNewlines();
  void testEmptyParagraphs();
  void testNewlinesThroughQTextCursor();
  void testBrInsideParagraph();
  void testLongDocument();
  void testNestedList();
};

void TestPlainMarkupOutput::testSingleFormat()
{
  QTextDocument doc;

  // One format
  doc.setHtml(QStringLiteral("This <b>text</b> is bold."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();
  QRegularExpression regex(QStringLiteral("^This \\*text\\* is bold.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDoubleFormat()
{
  QTextDocument doc;

  // One format
  doc.setHtml(QStringLiteral("Some <b><i>formatted</i></b> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();
  QRegularExpression regex(
      QStringLiteral("^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testAnchor()
{
  QTextDocument doc;
  doc.setHtml(
      QStringLiteral("A <a href=\"http://www.kde.org\">link</a> to KDE."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  QRegularExpression regex(QStringLiteral(
      "^A link\\[1\\] to KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

  regex.match(result).hasMatch();

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testAnchorWithFormattedContent()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral(
      "A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  QRegularExpression regex(
      QStringLiteral("^A \\*formatted\\* link\\[1\\] to "
                     "KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testAdjacentAnchors()
{
  QTextDocument doc;
  doc.setHtml(
      QStringLiteral("Two <a href=\"http://www.kde.org\">links</a><a "
                     "href=\"http://www.google.com\">next</a> to eachother."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  QRegularExpression regex(QStringLiteral(
      "^Two links\\[1\\]next\\[2\\] to eachother.\\n\\n--------\\n\\[1\\] "
      "http://www.kde.org\\n\\[2\\] http://www.google.com\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testNestedFormatting()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("This <b>text is <i>italic</i> and</b> bold."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  QRegularExpression regex(
      QStringLiteral("^This \\*text is /italic/ and\\* bold.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testSpan()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral(
      "Some <span style=\"color:#ff0000;\">formatted</span> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDoubleSpan()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("Some <span "
                             "style=\"color:#ff0000;background-color:#00ff00;"
                             "\">formatted</span> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testSpanNesting()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral(
      "Paragraph <span style=\"background-color:#00ff00;\">with some <span "
      "style=\"color:#ff0000;\">formatted</span> nested</span> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Paragraph with some formatted nested text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinish()
{
  QTextDocument doc;
  doc.setHtml(
      QStringLiteral("Paragraph <i><b>with</b> some formatted</i> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Paragraph /\\*with\\* some formatted/ text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinishReverseOrder()
{
  QTextDocument doc;
  doc.setHtml(
      QStringLiteral("Paragraph <b><i>with</i> some formatted</b> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Paragraph \\*/with/ some formatted\\* text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDifferentStartDoubleFinish()
{
  QTextDocument doc;
  doc.setHtml(
      QStringLiteral("Paragraph <i>with some <b>formatted<b></i> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Paragraph /with some \\*formatted\\*/ text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDifferentStartDoubleFinishReverseOrder()
{
  QTextDocument doc;
  doc.setHtml(
      QStringLiteral("Paragraph <b>with some <i>formatted</i></b> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Paragraph \\*with some /formatted/\\* text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testOverlap()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral(
      "Paragraph <b>with <i>some</i></b><i> formatted</i> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Paragraph \\*with /some/\\*/ formatted/ text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testEdgeCaseLeft()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("Paragraph <b>with some formatted text.</b>"));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Paragraph \\*with some formatted text.\\*\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testEdgeCaseRight()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("<b>Paragraph with some formatted</b> text."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^\\*Paragraph with some formatted\\* text.\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testImage()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("Paragraph with an inline <img "
                             "src=\"http://kde.org/img/kde41.png\" /> image."));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(QStringLiteral(
      "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
      "http://kde.org/img/kde41.png\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testImageResized()
{
  QString result;
  QRegularExpression regex;
  std::unique_ptr<PlainTextMarkupBuilder> hb;
  std::unique_ptr<MarkupDirector> md;
  QTextDocument doc;

  // width
  doc.setHtml(QStringLiteral(
      "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "width=\"10\" /> image."));

  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
      "http://kde.org/img/kde41.png\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // height
  doc.setHtml(QStringLiteral(
      "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "height=\"10\" /> image."));

  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
      "http://kde.org/img/kde41.png\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // height and width
  doc.setHtml(QStringLiteral(
      "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "height=\"10\" width=\"10\" /> image."));

  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
      "http://kde.org/img/kde41.png\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testEachFormatTagSingly()
{
  QString result;
  QRegularExpression regex;
  std::unique_ptr<PlainTextMarkupBuilder> hb;
  std::unique_ptr<MarkupDirector> md;
  QTextDocument doc;

  // Test bold
  doc.setHtml(QStringLiteral("Some <b>formatted</b> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some \\*formatted\\* text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Italic
  doc.setHtml(QStringLiteral("Some <i>formatted</i> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some /formatted/ text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Underline
  doc.setHtml(QStringLiteral("Some <u>formatted</u> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some _formatted_ text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Strikeout
  doc.setHtml(QStringLiteral("Some <s>formatted</s> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some -formatted- text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Superscript
  doc.setHtml(QStringLiteral("Some <sup>formatted</sup> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^Some \\^\\{formatted\\} text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Subscript
  doc.setHtml(QStringLiteral("Some <sub>formatted</sub> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex
      = QRegularExpression(QStringLiteral("^Some _\\{formatted\\} text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Foreground
  doc.setHtml(QStringLiteral(
      "Some <span style=\"color:#ff0000;\">formatted</span> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Background
  doc.setHtml(QStringLiteral(
      "Some <span style=\"background-color:#ff0000;\">formatted</span> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Font Family
  doc.setHtml(QStringLiteral(
      "Some <span style=\"font-family:courier;\">formatted</span> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Font Size
  doc.setHtml(QStringLiteral(
      "Some <span style=\"font-size:20pt;\">formatted</span> text."));
  hb = testMakeUnique<PlainTextMarkupBuilder>();
  md = testMakeUnique<MarkupDirector>(hb.get());
  md->processDocument(&doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testHorizontalRule()
{
  QTextDocument doc;
  doc.setHtml(
      QStringLiteral("<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr "
                     "/><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>"));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^Foo\\n--------------------\\nBar\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testNewlines()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("<p>Foo</p>\n<br /><br />\n<p>Bar</p>"));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testEmptyParagraphs()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("<p>Foo</p>\n<br /><br />\n<p>Bar</p>"));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testNewlinesThroughQTextCursor()
{
  auto doc = new QTextDocument(this);
  QTextCursor cursor(doc);
  cursor.movePosition(QTextCursor::Start);
  cursor.insertText(QStringLiteral("Foo"));
  cursor.insertText(QStringLiteral("\n"));
  cursor.insertText(QStringLiteral("\n"));
  cursor.insertText(QStringLiteral("\n"));
  cursor.insertText(QStringLiteral("Bar"));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(doc);
  auto result = hb.getResult();

  auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testBrInsideParagraph()
{

  QTextDocument doc;
  doc.setHtml(QStringLiteral("<p>Foo<br /><br /><br />Bar</p>"));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  // Two paragraphs separated by two line breaks

  auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testLongDocument()
{
  QTextDocument doc;

  QFile sourceHtml(QFINDTESTDATA("sourcehtml"));
  QVERIFY(sourceHtml.open(QIODevice::ReadOnly));
  doc.setHtml(QString::fromLatin1(sourceHtml.readAll().constData()));

  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();
  QVERIFY2(
      result.startsWith(QLatin1String("Hello,\nThis is some text\nIt shows how "
                                      "grantlee is used from kmail\n")),
      qPrintable(result));
  QVERIFY2(result.endsWith(QLatin1String("This is the end of the signature\n")),
           qPrintable(result));
}

void TestPlainMarkupOutput::testNestedList()
{
  QTextDocument doc;
  doc.setHtml(QStringLiteral("<ol>\n"
                             "  <li>Elem1</li>\n"
                             "  <li>Elem2\n"
                             "    <ul>\n"
                             "      <li>Subelem1</li>\n"
                             "      <li>Subelem2</li>\n"
                             "    </ul>\n"
                             "  </li>\n"
                             "  <li>Elem3</li>\n"
                             "  <li>Elem4\n"
                             "    <ul>\n"
                             "      <li>Subelem3</li>\n"
                             "      <li>Subelem4</li>\n"
                             "    </ul>\n"
                             "  </li>\n"
                             "</ol>"
                             "<ul type=\"circle\">\n"
                             "  <li>Elem5</li>\n"
                             "</ul>"));
  PlainTextMarkupBuilder hb;
  MarkupDirector md(&hb);
  md.processDocument(&doc);
  auto result = hb.getResult();

  const QString expected = QStringLiteral("     1. Elem1 \n"
                                          "     2. Elem2 \n"
                                          "         *  Subelem1 \n"
                                          "         *  Subelem2 \n"
                                          "     3. Elem3 \n"
                                          "     4. Elem4 \n"
                                          "         *  Subelem3 \n"
                                          "         *  Subelem4 \n"
                                          "     o  Elem5\n");
  QCOMPARE(result, expected);
}

QTEST_MAIN(TestPlainMarkupOutput)
#include "plainmarkupbuildertest.moc"
