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

#include <QtTest>
#include <QtCore>
#include <qtestevent.h>
#include <QtCore/QRegExp>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

#include "coverageobject.h"
#include "markupdirector.h"
#include "texthtmlbuilder.h"

using namespace Grantlee;

class TestHtmlOutput: public CoverageObject
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
  QTextDocument *doc = new QTextDocument();

  // One format
  doc->setHtml( QStringLiteral( "This <b>text</b> is bold." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();
    QRegExp regex(QStringLiteral("^<p>This <strong>text</strong> is bold.</p>\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testDoubleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml( QStringLiteral( "Some <b><i>formatted</i></b> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();
    QRegExp regex(QStringLiteral("^<p>Some (<strong><em>|<em><strong>)formatted(</em></strong>|</strong></em>) text.</p>\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testAnchor()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "A <a href=\"http://www.kde.org\">link</a> to KDE." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex(QStringLiteral("^<p>A <a href=\"http://www.kde.org\">link</a> to KDE.</p>\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testAnchorWithFormattedContent()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex(QStringLiteral("^<p>A <a href=\"http://www.kde.org\"><strong>formatted</strong> link</a> to KDE.</p>\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testAdjacentAnchors()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex(QStringLiteral("^<p>Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother.</p>\\n$"));

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testNestedFormatting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "This <b>text is <i>italic</i> and</b> bold." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex(QStringLiteral("^<p>This <strong>text is <em>italic</em> and</strong> bold.</p>\\n$"));

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Some <span style=\"color:#ff0000;\">formatted</span> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Some <span style=\"color:#ff0000;\">formatted</span> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testDoubleSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Some <span style=\"color:#ff0000;background-color:#00ff00;\">formatted</span> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Some <span style=\"(color:#ff0000|background-color:#00ff00);\"><span style=\"(color:#ff0000|background-color:#00ff00);\">formatted</span></span> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testSpanNesting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testDoubleStartDifferentFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <i><b>with</b> some formatted</i> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph <em><strong>with</strong> some formatted</em> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testDoubleStartDifferentFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b><i>with</i> some formatted</b> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph <strong><em>with</em> some formatted</strong> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testDifferentStartDoubleFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <i>with some <b>formatted<b></i> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph <em>with some <strong>formatted</strong></em> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testDifferentStartDoubleFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b>with some <i>formatted</i></b> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph <strong>with some <em>formatted</em></strong> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testOverlap()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b>with <i>some</i></b><i> formatted</i> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph <strong>with <em>some</em></strong><em> formatted</em> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testEdgeCaseLeft()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b>with some formatted text.</b>" ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph <strong>with some formatted text.</strong></p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testEdgeCaseRight()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<b>Paragraph with some formatted</b> text." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p><strong>Paragraph with some formatted</strong> text.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testImage()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" /> image." ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" /> image.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testImageResized()
{
    QString result;
    QRegExp regex;
    TextHTMLBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();

    // width
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" /> image." ) );

    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" /> image.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // height
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" /> image." ) );

    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" /> image.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // height and width
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" width=\"10\" /> image." ) );

    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" height=\"10\" /> image.</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testEachFormatTagSingly()
{
    QString result;
    QRegExp regex;
    TextHTMLBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();


    // Test bold
    doc->setHtml( QStringLiteral( "Some <b>formatted</b> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <strong>formatted</strong> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));


    // Test Italic
    doc->setHtml( QStringLiteral( "Some <i>formatted</i> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <em>formatted</em> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Underline
    doc->setHtml( QStringLiteral( "Some <u>formatted</u> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <u>formatted</u> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Strikeout
    doc->setHtml( QStringLiteral( "Some <s>formatted</s> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <s>formatted</s> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Superscript
    doc->setHtml( QStringLiteral( "Some <sup>formatted</sup> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <sup>formatted</sup> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Subscript
    doc->setHtml( QStringLiteral( "Some <sub>formatted</sub> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <sub>formatted</sub> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Foreground
    doc->setHtml( QStringLiteral( "Some <span style=\"color:#ff0000;\">formatted</span> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <span style=\"color:#ff0000;\">formatted</span> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));


    // Test Background
    doc->setHtml( QStringLiteral( "Some <span style=\"background-color:#ff0000;\">formatted</span> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <span style=\"background-color:#ff0000;\">formatted</span> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Font Family
    doc->setHtml( QStringLiteral( "Some <span style=\"font-family:courier;\">formatted</span> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <span style=\"font-family:courier;\">formatted</span> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Font Size
    doc->setHtml( QStringLiteral( "Some <span style=\"font-size:20pt;\">formatted</span> text." ) );
    hb = new TextHTMLBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QStringLiteral( "^<p>Some <span style=\"font-size:20pt;\">formatted</span> text.</p>\\n$" ) );
    QVERIFY(regex.exactMatch(result));

}

void TestHtmlOutput::testHorizontalRule()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr /><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>" ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Foo</p>\\n<hr />\\n<p>Bar</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testNewlines()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<p>Foo<br /><br />\n<p>Bar</p>" ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Foo</p>\\n<p>&nbsp;<p>&nbsp;</p>\\n<p>Bar</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestHtmlOutput::testNewlinesThroughQTextCursor()
{
    QTextDocument *doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText( QStringLiteral( "Foo" ) );
    cursor.insertText( QStringLiteral( "\n" ) );
    cursor.insertText( QStringLiteral( "\n" ) );
    cursor.insertText( QStringLiteral( "\n" ) );
    cursor.insertText( QStringLiteral( "Bar" ) );

    TextHTMLBuilder *hb = new TextHTMLBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QStringLiteral( "^<p>Foo</p>\\n<p>&nbsp;<p>&nbsp;<p>Bar</p>\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

QTEST_MAIN(TestHtmlOutput)
#include "htmlbuildertest.moc"
