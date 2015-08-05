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

#include <QtTest/QtTest>
#include <QtCore>
#include <QtTest/qtestevent.h>
#include <QtCore/QRegularExpression>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

#include "markupdirector.h"
#include "plaintextmarkupbuilder.h"
#include "texthtmlbuilder.h"
#include "coverageobject.h"

using namespace Grantlee;

class TestPlainMarkupOutput: public CoverageObject
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

};

void TestPlainMarkupOutput::testSingleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml( QStringLiteral( "This <b>text</b> is bold." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();
    QRegularExpression regex(QStringLiteral("^This \\*text\\* is bold.\\n$"));

    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testDoubleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml( QStringLiteral( "Some <b><i>formatted</i></b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();
    QRegularExpression regex(QStringLiteral("^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testAnchor()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "A <a href=\"http://www.kde.org\">link</a> to KDE." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex(QStringLiteral("^A link\\[1\\] to KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

    regex.match(result).hasMatch();


    QVERIFY(regex.match(result).hasMatch());


}

void TestPlainMarkupOutput::testAnchorWithFormattedContent()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegularExpression regex(QStringLiteral("^A \\*formatted\\* link\\[1\\] to KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testAdjacentAnchors()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex(QStringLiteral("^Two links\\[1\\]next\\[2\\] to eachother.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n\\[2\\] http://www.google.com\\n$"));

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testNestedFormatting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "This <b>text is <i>italic</i> and</b> bold." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex(QStringLiteral("^This \\*text is /italic/ and\\* bold.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Some <span style=\"color:#ff0000;\">formatted</span> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Some formatted text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDoubleSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Some <span style=\"color:#ff0000;background-color:#00ff00;\">formatted</span> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Some formatted text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testSpanNesting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph with some formatted nested text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <i><b>with</b> some formatted</i> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph /\\*with\\* some formatted/ text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b><i>with</i> some formatted</b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph \\*/with/ some formatted\\* text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testDifferentStartDoubleFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <i>with some <b>formatted<b></i> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph /with some \\*formatted\\*/ text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testDifferentStartDoubleFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b>with some <i>formatted</i></b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph \\*with some /formatted/\\* text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testOverlap()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b>with <i>some</i></b><i> formatted</i> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph \\*with /some/\\*/ formatted/ text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testEdgeCaseLeft()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph <b>with some formatted text.</b>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph \\*with some formatted text.\\*\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testEdgeCaseRight()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<b>Paragraph with some formatted</b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^\\*Paragraph with some formatted\\* text.\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testImage()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" /> image." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testImageResized()
{
    QString result;
    QRegularExpression regex;
    PlainTextMarkupBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();

    // width
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" /> image." ) );

    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();


    regex = QRegularExpression( QStringLiteral( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // height
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" /> image." ) );

    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // height and width
    doc->setHtml( QStringLiteral( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" width=\"10\" /> image." ) );

    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testEachFormatTagSingly()
{
    QString result;
    QRegularExpression regex;
    PlainTextMarkupBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();


    // Test bold
    doc->setHtml( QStringLiteral( "Some <b>formatted</b> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some \\*formatted\\* text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());


    // Test Italic
    doc->setHtml( QStringLiteral( "Some <i>formatted</i> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some /formatted/ text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // Test Underline
    doc->setHtml( QStringLiteral( "Some <u>formatted</u> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some _formatted_ text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // Test Strikeout
    doc->setHtml( QStringLiteral( "Some <s>formatted</s> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some -formatted- text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // Test Superscript
    doc->setHtml( QStringLiteral( "Some <sup>formatted</sup> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some \\^\\{formatted\\} text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // Test Subscript
    doc->setHtml( QStringLiteral( "Some <sub>formatted</sub> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some _\\{formatted\\} text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // Test Foreground
    doc->setHtml( QStringLiteral( "Some <span style=\"color:#ff0000;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());


    // Test Background
    doc->setHtml( QStringLiteral( "Some <span style=\"background-color:#ff0000;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Family
    doc->setHtml( QStringLiteral( "Some <span style=\"font-family:courier;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Size
    doc->setHtml( QStringLiteral( "Some <span style=\"font-size:20pt;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression( QStringLiteral( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.match(result).hasMatch());

}

void TestPlainMarkupOutput::testHorizontalRule()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr /><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Foo\\n--------------------\\nBar\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testNewlines()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<p>Foo</p>\n<br /><br />\n<p>Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testEmptyParagraphs()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<p>Foo</p>\n<br /><br />\n<p>Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput::testNewlinesThroughQTextCursor()
{
    QTextDocument *doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText( QStringLiteral( "Foo" ) );
    cursor.insertText( QStringLiteral( "\n" ) );
    cursor.insertText( QStringLiteral( "\n" ) );
    cursor.insertText( QStringLiteral( "\n" ) );
    cursor.insertText( QStringLiteral( "Bar" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}

void TestPlainMarkupOutput:: testBrInsideParagraph()
{

    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QStringLiteral( "<p>Foo<br /><br /><br />Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    // Two paragraphs separated by two line breaks

    QRegularExpression regex = QRegularExpression( QStringLiteral( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.match(result).hasMatch());
}


QTEST_MAIN(TestPlainMarkupOutput)
#include "plainmarkupbuildertest.moc"
