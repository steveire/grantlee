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
#include <QtTest/QtTest>
#include <QtTest/qtestevent.h>

#include "coverageobject.h"
#include "markupdirector.h"
#include "texthtmlbuilder.h"

using namespace Grantlee;

class TestHtmlOutput : public CoverageObject
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
  void testNewlinesThroughQTextCursor();
};

void TestHtmlOutput::testSingleFormat()
{
  auto doc = new QTextDocument();

  // One format
  doc->setHtml(QStringLiteral("This <b>text</b> is bold."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();
  QRegularExpression regex(
      QStringLiteral("^<p>This <strong>text</strong> is bold.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testDoubleFormat()
{
  auto doc = new QTextDocument();

  // One format
  doc->setHtml(QStringLiteral("Some <b><i>formatted</i></b> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();
  QRegularExpression regex(
      QStringLiteral("^<p>Some "
                     "(<strong><em>|<em><strong>)formatted(</em></strong>|</"
                     "strong></em>) text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testAnchor()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("A <a href=\"http://www.kde.org\">link</a> to KDE."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  QRegularExpression regex(QStringLiteral(
      "^<p>A <a href=\"http://www.kde.org\">link</a> to KDE.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testAnchorWithFormattedContent()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral(
      "A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  QRegularExpression regex(QStringLiteral(
      "^<p>A <a href=\"http://www.kde.org\"><strong>formatted</strong> "
      "link</a> to KDE.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testAdjacentAnchors()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("Two <a href=\"http://www.kde.org\">links</a><a "
                     "href=\"http://www.google.com\">next</a> to eachother."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  QRegularExpression regex(QStringLiteral(
      "^<p>Two <a href=\"http://www.kde.org\">links</a><a "
      "href=\"http://www.google.com\">next</a> to eachother.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testNestedFormatting()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral("This <b>text is <i>italic</i> and</b> bold."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  QRegularExpression regex(QStringLiteral(
      "^<p>This <strong>text is <em>italic</em> and</strong> bold.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testSpan()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral(
      "Some <span style=\"color:#ff0000;\">formatted</span> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Some <span style=\"color:#ff0000;\">formatted</span> "
                     "text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testDoubleSpan()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral("Some <span "
                              "style=\"color:#ff0000;background-color:#00ff00;"
                              "\">formatted</span> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(QStringLiteral(
      "^<p>Some <span "
      "style=\"(color:#ff0000|background-color:#00ff00);\"><span "
      "style=\"(color:#ff0000|background-color:#00ff00);\">formatted</span></"
      "span> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testSpanNesting()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral(
      "Paragraph <span style=\"background-color:#00ff00;\">with some <span "
      "style=\"color:#ff0000;\">formatted</span> nested</span> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(QStringLiteral(
      "^<p>Paragraph <span style=\"background-color:#00ff00;\">with some <span "
      "style=\"color:#ff0000;\">formatted</span> nested</span> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testDoubleStartDifferentFinish()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("Paragraph <i><b>with</b> some formatted</i> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Paragraph <em><strong>with</strong> some "
                     "formatted</em> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testDoubleStartDifferentFinishReverseOrder()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("Paragraph <b><i>with</i> some formatted</b> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Paragraph <strong><em>with</em> some "
                     "formatted</strong> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testDifferentStartDoubleFinish()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("Paragraph <i>with some <b>formatted<b></i> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Paragraph <em>with some "
                     "<strong>formatted</strong></em> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testDifferentStartDoubleFinishReverseOrder()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("Paragraph <b>with some <i>formatted</i></b> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Paragraph <strong>with some "
                     "<em>formatted</em></strong> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testOverlap()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral(
      "Paragraph <b>with <i>some</i></b><i> formatted</i> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Paragraph <strong>with <em>some</em></strong><em> "
                     "formatted</em> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testEdgeCaseLeft()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral("Paragraph <b>with some formatted text.</b>"));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(QStringLiteral(
      "^<p>Paragraph <strong>with some formatted text.</strong></p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testEdgeCaseRight()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral("<b>Paragraph with some formatted</b> text."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(QStringLiteral(
      "^<p><strong>Paragraph with some formatted</strong> text.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testImage()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("Paragraph with an inline <img "
                     "src=\"http://kde.org/img/kde41.png\" /> image."));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Paragraph with an inline <img "
                     "src=\"http://kde.org/img/kde41.png\" /> image.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testImageResized()
{
  QString result;
  QRegularExpression regex;
  TextHTMLBuilder *hb;
  MarkupDirector *md;
  auto doc = new QTextDocument();

  // width
  doc->setHtml(QStringLiteral(
      "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "width=\"10\" /> image."));

  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "width=\"10\" /> image.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // height
  doc->setHtml(QStringLiteral(
      "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "height=\"10\" /> image."));

  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "height=\"10\" /> image.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // height and width
  doc->setHtml(QStringLiteral(
      "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "height=\"10\" width=\"10\" /> image."));

  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
      "width=\"10\" height=\"10\" /> image.</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testEachFormatTagSingly()
{
  QString result;
  QRegularExpression regex;
  TextHTMLBuilder *hb;
  MarkupDirector *md;
  auto doc = new QTextDocument();

  // Test bold
  doc->setHtml(QStringLiteral("Some <b>formatted</b> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^<p>Some <strong>formatted</strong> text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Italic
  doc->setHtml(QStringLiteral("Some <i>formatted</i> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^<p>Some <em>formatted</em> text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Underline
  doc->setHtml(QStringLiteral("Some <u>formatted</u> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^<p>Some <u>formatted</u> text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Strikeout
  doc->setHtml(QStringLiteral("Some <s>formatted</s> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^<p>Some <s>formatted</s> text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Superscript
  doc->setHtml(QStringLiteral("Some <sup>formatted</sup> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^<p>Some <sup>formatted</sup> text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Subscript
  doc->setHtml(QStringLiteral("Some <sub>formatted</sub> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^<p>Some <sub>formatted</sub> text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Foreground
  doc->setHtml(QStringLiteral(
      "Some <span style=\"color:#ff0000;\">formatted</span> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(
      QStringLiteral("^<p>Some <span style=\"color:#ff0000;\">formatted</span> "
                     "text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Background
  doc->setHtml(QStringLiteral(
      "Some <span style=\"background-color:#ff0000;\">formatted</span> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^<p>Some <span style=\"background-color:#ff0000;\">formatted</span> "
      "text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Font Family
  doc->setHtml(QStringLiteral(
      "Some <span style=\"font-family:courier;\">formatted</span> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^<p>Some <span style=\"font-family:courier;\">formatted</span> "
      "text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());

  // Test Font Size
  doc->setHtml(QStringLiteral(
      "Some <span style=\"font-size:20pt;\">formatted</span> text."));
  hb = new TextHTMLBuilder();
  md = new MarkupDirector(hb);
  md->processDocument(doc);
  result = hb->getResult();

  regex = QRegularExpression(QStringLiteral(
      "^<p>Some <span style=\"font-size:20pt;\">formatted</span> "
      "text.</p>\\n$"));
  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testHorizontalRule()
{
  auto doc = new QTextDocument();
  doc->setHtml(
      QStringLiteral("<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr "
                     "/><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>"));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Foo</p>\\n<hr />\\n<p>Bar</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testNewlines()
{
  auto doc = new QTextDocument();
  doc->setHtml(QStringLiteral("<p>Foo<br /><br />\n<p>Bar</p>"));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Foo</p>\\n<p>&nbsp;<p>&nbsp;</p>\\n<p>Bar</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

void TestHtmlOutput::testNewlinesThroughQTextCursor()
{
  auto doc = new QTextDocument(this);
  QTextCursor cursor(doc);
  cursor.movePosition(QTextCursor::Start);
  cursor.insertText(QStringLiteral("Foo"));
  cursor.insertText(QStringLiteral("\n"));
  cursor.insertText(QStringLiteral("\n"));
  cursor.insertText(QStringLiteral("\n"));
  cursor.insertText(QStringLiteral("Bar"));

  auto hb = new TextHTMLBuilder();
  auto md = new MarkupDirector(hb);
  md->processDocument(doc);
  auto result = hb->getResult();

  auto regex = QRegularExpression(
      QStringLiteral("^<p>Foo</p>\\n<p>&nbsp;<p>&nbsp;<p>Bar</p>\\n$"));

  QVERIFY(regex.match(result).hasMatch());
}

QTEST_MAIN(TestHtmlOutput)
#include "htmlbuildertest.moc"
