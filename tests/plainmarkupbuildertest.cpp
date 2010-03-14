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

#include <QtTest>
#include <QtCore>
#include <qtest_kde.h>
#include <qtestevent.h>
#include <QRegExp>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>

#include "markupdirector.h"
#include "plaintextmarkupbuilder.h"
#include "texthtmlbuilder.h"

using namespace Grantlee;

class TestPlainMarkupOutput: public QObject
{
    Q_OBJECT
private slots:

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
    doc->setHtml("This <b>text</b> is bold.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex(QString("^This \\*text\\* is bold.\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testDoubleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml("Some <b><i>formatted</i></b> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex(QString("^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testAnchor()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("A <a href=\"http://www.kde.org\">link</a> to KDE.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex(QString("^A link\\[1\\] to KDE.\\n\\n---- References ----\\n\\[1\\] http://www.kde.org\\n$"));

    regex.exactMatch(result);


    QVERIFY(regex.exactMatch(result));


}

void TestPlainMarkupOutput::testAnchorWithFormattedContent()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();


    QRegExp regex(QString("^A \\*formatted\\* link\\[1\\] to KDE.\\n\\n---- References ----\\n\\[1\\] http://www.kde.org\\n$"));

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testAdjacentAnchors()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex(QString("^Two links\\[1\\]next\\[2\\] to eachother.\\n\\n---- References ----\\n\\[1\\] http://www.kde.org\\n\\[2\\] http://www.google.com\\n$"));

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testNestedFormatting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("This <b>text is <i>italic</i> and</b> bold.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex(QString("^This \\*text is /italic/ and\\* bold.\\n$"));

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Some <span style=\"color:#ff0000;\">formatted</span> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Some formatted text.\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDoubleSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Some <span style=\"color:#ff0000;background-color:#00ff00;\">formatted</span> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();


    QRegExp regex = QRegExp("^Some formatted text.\\n$");

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testSpanNesting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph with some formatted nested text.\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph <i><b>with</b> some formatted</i> text.");


    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph /\\*with\\* some formatted/ text.\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDoubleStartDifferentFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph <b><i>with</i> some formatted</b> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph \\*/with/ some formatted\\* text.\\n$");

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testDifferentStartDoubleFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph <i>with some <b>formatted<b></i> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph /with some \\*formatted\\*/ text.\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testDifferentStartDoubleFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph <b>with some <i>formatted</i></b> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph \\*with some /formatted/\\* text.\\n$");

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testOverlap()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph <b>with <i>some</i></b><i> formatted</i> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph \\*with /some/\\*/ formatted/ text.\\n$");

    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testEdgeCaseLeft()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph <b>with some formatted text.</b>");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph \\*with some formatted text.\\*\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testEdgeCaseRight()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("<b>Paragraph with some formatted</b> text.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^\\*Paragraph with some formatted\\* text.\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testImage()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" /> image.");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( doc );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Paragraph with an inline \\[1\\] image.\\n\\n---- References ----\\n\\[1\\] http://kde.org/img/kde41.png\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testImageResized()
{
    QString result;
    QRegExp regex;
    QTextDocument *doc = new QTextDocument();

    PlainTextMarkupOutputter outputter;

    // width
    doc->setHtml("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" /> image.");

    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Paragraph with an inline \\[1\\] image.\\n\\n---- References ----\\n\\[1\\] http://kde.org/img/kde41.png\\n$");
    QVERIFY(regex.exactMatch(result));

    // height
    doc->setHtml("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" /> image.");

    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Paragraph with an inline \\[1\\] image.\\n\\n---- References ----\\n\\[1\\] http://kde.org/img/kde41.png\\n$");
    QVERIFY(regex.exactMatch(result));

    // height and width
    doc->setHtml("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" width=\"10\" /> image.");

    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Paragraph with an inline \\[1\\] image.\\n\\n---- References ----\\n\\[1\\] http://kde.org/img/kde41.png\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testEachFormatTagSingly()
{
    QString result;
    QRegExp regex;
    QTextDocument *doc = new QTextDocument();

    PlainTextMarkupOutputter outputter;

    // Test bold
    doc->setHtml("Some <b>formatted</b> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some \\*formatted\\* text.\\n$");
    QVERIFY(regex.exactMatch(result));


    // Test Italic
    doc->setHtml("Some <i>formatted</i> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some /formatted/ text.\\n$");
    QVERIFY(regex.exactMatch(result));

    // Test Underline
    doc->setHtml("Some <u>formatted</u> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some _formatted_ text.\\n$");
    QVERIFY(regex.exactMatch(result));

    // Test Strikeout
    doc->setHtml("Some <s>formatted</s> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some -formatted- text.\\n$");
    QVERIFY(regex.exactMatch(result));

    // Test Superscript
    doc->setHtml("Some <sup>formatted</sup> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some \\^\\{formatted\\} text.\\n$");
    QVERIFY(regex.exactMatch(result));

    // Test Subscript
    doc->setHtml("Some <sub>formatted</sub> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some _\\{formatted\\} text.\\n$");
    QVERIFY(regex.exactMatch(result));

    // Test Foreground
    doc->setHtml("Some <span style=\"color:#ff0000;\">formatted</span> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some formatted text.\\n$");
    QVERIFY(regex.exactMatch(result));


    // Test Background
    doc->setHtml("Some <span style=\"background-color:#ff0000;\">formatted</span> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some formatted text.\\n$");
    QVERIFY(regex.exactMatch(result));

    // Test Font Family
    doc->setHtml("Some <span style=\"font-family:courier;\">formatted</span> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some formatted text.\\n$");
    QVERIFY(regex.exactMatch(result));

    // Test Font Size
    doc->setHtml("Some <span style=\"font-size:20pt;\">formatted</span> text.");
    outputter.processDocument( doc );
    result = outputter.getResult();

    regex = QRegExp("^Some formatted text.\\n$");
    QVERIFY(regex.exactMatch(result));

}

void TestPlainMarkupOutput::testHorizontalRule()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr /><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument(doc);
    QString result = outputter.getResult();


    QRegExp regex = QRegExp("^Foo\\n--------------------\\nBar\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testNewlines()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("<p>Foo</p>\n<br /><br />\n<p>Bar</p>");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument(doc);
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Foo\\n\\n\\nBar\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testEmptyParagraphs()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml("<p>Foo</p>\n<br /><br />\n<p>Bar</p>");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument(doc);
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Foo\\n\\n\\nBar\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput::testNewlinesThroughQTextEdit()
{
    QTextEdit *te = new QTextEdit();
    QTextCursor cursor = te->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText("Foo");
    cursor.insertText("\n");
    cursor.insertText("\n");
    cursor.insertText("\n");
    cursor.insertText("Bar");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument( te->document() );
    QString result = outputter.getResult();

    QRegExp regex = QRegExp("^Foo\\n\\n\\nBar\\n$");

    QVERIFY(regex.exactMatch(result));
}

void TestPlainMarkupOutput:: testBrInsideParagraph()
{

    QTextDocument *doc = new QTextDocument();
    doc->setHtml("<p>Foo<br /><br /><br />Bar</p>");

    PlainTextMarkupOutputter outputter;
    outputter.processDocument(doc);
    QString result = outputter.getResult();

    // Two paragraphs separated by two line breaks

    QRegExp regex = QRegExp("^Foo\\n\\n\\nBar\\n$");

    QVERIFY(regex.exactMatch(result));
}


QTEST_MAIN(TestPlainMarkupOutput)
#include "plainmarkupbuildertest.moc"
