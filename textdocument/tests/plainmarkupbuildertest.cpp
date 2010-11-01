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
#include <QtGui/QTextEdit>

#include "markupdirector.h"
#include "plaintextmarkupbuilder.h"
#include "texthtmlbuilder.h"

using namespace Grantlee;

class TestPlainMarkupOutput: public QObject
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
    void testNewlinesThroughQTextEdit();
    void testBrInsideParagraph();

};

void TestPlainMarkupOutput::testSingleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml( QLatin1String( "This <b>text</b> is bold." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();
    QRegExp regex(QLatin1String("^This \\*text\\* is bold.\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testDoubleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml( QLatin1String( "Some <b><i>formatted</i></b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();
    QRegExp regex(QLatin1String("^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testAnchor()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "A <a href=\"http://www.kde.org\">link</a> to KDE." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex(QLatin1String("^A link\\[1\\] to KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

    regex.exactMatch(result);


    QVERIFY(regex.exactMatch(result));


}

void TestPlainMarkupOutput::testAnchorWithFormattedContent()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegExp regex(QLatin1String("^A \\*formatted\\* link\\[1\\] to KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testAdjacentAnchors()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex(QLatin1String("^Two links\\[1\\]next\\[2\\] to eachother.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n\\[2\\] http://www.google.com\\n$"));

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testNestedFormatting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "This <b>text is <i>italic</i> and</b> bold." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex(QLatin1String("^This \\*text is /italic/ and\\* bold.\\n$"));

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Some <span style=\"color:#ff0000;\">formatted</span> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Some formatted text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDoubleSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Some <span style=\"color:#ff0000;background-color:#00ff00;\">formatted</span> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegExp regex = QRegExp( QLatin1String( "^Some formatted text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testSpanNesting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Paragraph with some formatted nested text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph <i><b>with</b> some formatted</i> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Paragraph /\\*with\\* some formatted/ text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph <b><i>with</i> some formatted</b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Paragraph \\*/with/ some formatted\\* text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testDifferentStartDoubleFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph <i>with some <b>formatted<b></i> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Paragraph /with some \\*formatted\\*/ text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDifferentStartDoubleFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph <b>with some <i>formatted</i></b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegExp regex = QRegExp( QLatin1String( "^Paragraph \\*with some /formatted/\\* text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testOverlap()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph <b>with <i>some</i></b><i> formatted</i> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegExp regex = QRegExp( QLatin1String( "^Paragraph \\*with /some/\\*/ formatted/ text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testEdgeCaseLeft()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph <b>with some formatted text.</b>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Paragraph \\*with some formatted text.\\*\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testEdgeCaseRight()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "<b>Paragraph with some formatted</b> text." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^\\*Paragraph with some formatted\\* text.\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testImage()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" /> image." ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testImageResized()
{
    QString result;
    QRegExp regex;
    PlainTextMarkupBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();

    // width
    doc->setHtml( QLatin1String( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" /> image." ) );

    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();


    regex = QRegExp( QLatin1String( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // height
    doc->setHtml( QLatin1String( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" /> image." ) );

    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // height and width
    doc->setHtml( QLatin1String( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" width=\"10\" /> image." ) );

    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] http://kde.org/img/kde41.png\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testEachFormatTagSingly()
{
    QString result;
    QRegExp regex;
    PlainTextMarkupBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();


    // Test bold
    doc->setHtml( QLatin1String( "Some <b>formatted</b> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some \\*formatted\\* text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));


    // Test Italic
    doc->setHtml( QLatin1String( "Some <i>formatted</i> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some /formatted/ text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Underline
    doc->setHtml( QLatin1String( "Some <u>formatted</u> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some _formatted_ text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Strikeout
    doc->setHtml( QLatin1String( "Some <s>formatted</s> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some -formatted- text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Superscript
    doc->setHtml( QLatin1String( "Some <sup>formatted</sup> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some \\^\\{formatted\\} text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Subscript
    doc->setHtml( QLatin1String( "Some <sub>formatted</sub> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some _\\{formatted\\} text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Foreground
    doc->setHtml( QLatin1String( "Some <span style=\"color:#ff0000;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));


    // Test Background
    doc->setHtml( QLatin1String( "Some <span style=\"background-color:#ff0000;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Font Family
    doc->setHtml( QLatin1String( "Some <span style=\"font-family:courier;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

    // Test Font Size
    doc->setHtml( QLatin1String( "Some <span style=\"font-size:20pt;\">formatted</span> text." ) );
    hb = new PlainTextMarkupBuilder();
    md = new MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegExp( QLatin1String( "^Some formatted text.\\n$" ) );
    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testHorizontalRule()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr /><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();


    QRegExp regex = QRegExp( QLatin1String( "^Foo\\n--------------------\\nBar\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testNewlines()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "<p>Foo</p>\n<br /><br />\n<p>Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testEmptyParagraphs()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "<p>Foo</p>\n<br /><br />\n<p>Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testNewlinesThroughQTextEdit()
{

    QTextEdit *te = new QTextEdit();
    QTextCursor cursor = te->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText( QLatin1String( "Foo" ) );
    cursor.insertText( QLatin1String( "\n" ) );
    cursor.insertText( QLatin1String( "\n" ) );
    cursor.insertText( QLatin1String( "\n" ) );
    cursor.insertText( QLatin1String( "Bar" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(te->document());
    QString result = hb->getResult();

    QRegExp regex = QRegExp( QLatin1String( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput:: testBrInsideParagraph()
{

    QTextDocument *doc = new QTextDocument();
    doc->setHtml( QLatin1String( "<p>Foo<br /><br /><br />Bar</p>" ) );

    PlainTextMarkupBuilder *hb = new PlainTextMarkupBuilder();
    MarkupDirector *md = new MarkupDirector(hb);
    md->processDocument(doc);
    QString result = hb->getResult();

    // Two paragraphs separated by two line breaks

    QRegExp regex = QRegExp( QLatin1String( "^Foo\\n\\n\\nBar\\n$" ) );

    QVERIFY(regex.exactMatch(result));
}


QTEST_MAIN(TestPlainMarkupOutput)
#include "plainmarkupbuildertest.moc"

