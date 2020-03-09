/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "textedit.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextEdit>
#include <QTextList>
#include <QToolBar>
#include <QtDebug>

#include "grantlee_paths.h"
#include <grantlee/context.h>
#include <grantlee/engine.h>

#include "audioobject.h"

#ifdef Q_WS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif

TextEdit::TextEdit(QWidget *parent) : QMainWindow(parent)
{
  setToolButtonStyle(Qt::ToolButtonFollowStyle);
  setupFileActions();
  setupEditActions();
  setupTextActions();

  {
    QMenu *helpMenu = new QMenu(tr("Help"), this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(tr("About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
  }

  abstractTextEdit = new QTextEdit(this);

  abstractTextEdit->append("This is the abstract");

  textEdit = new QTextEdit(this);
  connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this,
          SLOT(currentCharFormatChanged(QTextCharFormat)));
  connect(textEdit, SIGNAL(cursorPositionChanged()), this,
          SLOT(cursorPositionChanged()));

  QObject *audioObjectInterface = new AudioObject;
  textEdit->document()->documentLayout()->registerHandler(AudioType,
                                                          audioObjectInterface);

  QSplitter *hSplitter = new QSplitter(this);

  QSplitter *splitter = new QSplitter(Qt::Vertical, hSplitter);
  splitter->addWidget(abstractTextEdit);
  splitter->addWidget(textEdit);

  webView = new QTextEdit(this);
  hSplitter->addWidget(webView);

  setCentralWidget(hSplitter);
  textEdit->setFocus();
  setCurrentFileName(QString());

  colorChanged(textEdit->textColor());
  alignmentChanged(textEdit->alignment());

  connect(textEdit->document(), SIGNAL(modificationChanged(bool)), actionSave,
          SLOT(setEnabled(bool)));
  connect(textEdit->document(), SIGNAL(modificationChanged(bool)), this,
          SLOT(setWindowModified(bool)));
  connect(textEdit->document(), SIGNAL(undoAvailable(bool)), actionUndo,
          SLOT(setEnabled(bool)));
  connect(textEdit->document(), SIGNAL(redoAvailable(bool)), actionRedo,
          SLOT(setEnabled(bool)));

  setWindowModified(textEdit->document()->isModified());
  actionSave->setEnabled(textEdit->document()->isModified());
  actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
  actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

  connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
  connect(actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

  actionCut->setEnabled(false);
  actionCopy->setEnabled(false);

  connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
  connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
  connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

  connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut,
          SLOT(setEnabled(bool)));
  connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy,
          SLOT(setEnabled(bool)));

#ifndef QT_NO_CLIPBOARD
  connect(QApplication::clipboard(), SIGNAL(dataChanged()), this,
          SLOT(clipboardDataChanged()));
#endif

  QString initialFile = ":/example.html";
  const QStringList args = QCoreApplication::arguments();
  if (args.count() == 2)
    initialFile = args.at(1);

  if (!load(initialFile))
    fileNew();
}

void TextEdit::closeEvent(QCloseEvent *e)
{
  if (maybeSave())
    e->accept();
  else
    e->ignore();
}

void TextEdit::setupFileActions()
{
  QToolBar *tb = new QToolBar(this);
  tb->setWindowTitle(tr("File Actions"));
  addToolBar(tb);

  QMenu *menu = new QMenu(tr("&File"), this);
  menuBar()->addMenu(menu);

  QAction *a;

  QIcon newIcon
      = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));
  a = new QAction(newIcon, tr("&New"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::New);
  connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
  menu->addAction(a);

  a = new QAction(
      QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png")),
      tr("&Open..."), this);
  a->setShortcut(QKeySequence::Open);
  connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
  menu->addAction(a);

  menu->addSeparator();

  actionSave = a = new QAction(
      QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png")),
      tr("&Save"), this);
  a->setShortcut(QKeySequence::Save);
  connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
  a->setEnabled(false);
  menu->addAction(a);

  a = new QAction(tr("Save &As..."), this);
  a->setPriority(QAction::LowPriority);
  connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
  menu->addAction(a);
  menu->addSeparator();

  a = new QAction(QIcon::fromTheme("export_themed_html"),
                  tr("&Export Themed HTML"), this);
  a->setPriority(QAction::LowPriority);
  connect(a, SIGNAL(triggered()), this, SLOT(exportThemedHtml()));
  tb->addAction(a);
  menu->addAction(a);

  a = new QAction(QIcon::fromTheme("add_audio"), tr("&Add Audio..."), this);
  a->setPriority(QAction::LowPriority);
  connect(a, SIGNAL(triggered()), this, SLOT(addAudio()));
  tb->addAction(a);
  menu->addAction(a);

  menu->addSeparator();

  a = new QAction(QIcon::fromTheme("export_plain_text"),
                  tr("&Export Plain Text"), this);
  a->setPriority(QAction::LowPriority);
  connect(a, SIGNAL(triggered()), this, SLOT(exportPlainText()));
  tb->addAction(a);
  menu->addAction(a);

  menu->addSeparator();

  a = new QAction(tr("&Quit"), this);
  a->setShortcut(Qt::CTRL + Qt::Key_Q);
  connect(a, SIGNAL(triggered()), this, SLOT(close()));
  menu->addAction(a);
}

void TextEdit::setupEditActions()
{
  QMenu *menu = new QMenu(tr("&Edit"), this);
  menuBar()->addMenu(menu);

  QAction *a;
  a = actionUndo = new QAction(
      QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png")),
      tr("&Undo"), this);
  a->setShortcut(QKeySequence::Undo);
  menu->addAction(a);
  a = actionRedo = new QAction(
      QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png")),
      tr("&Redo"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Redo);
  menu->addAction(a);
  menu->addSeparator();
  a = actionCut = new QAction(
      QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png")),
      tr("Cu&t"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Cut);
  menu->addAction(a);
  a = actionCopy = new QAction(
      QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png")),
      tr("&Copy"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Copy);
  menu->addAction(a);
  a = actionPaste = new QAction(
      QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png")),
      tr("&Paste"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Paste);
  menu->addAction(a);
#ifndef QT_NO_CLIPBOARD
  actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
#endif
}

void TextEdit::setupTextActions()
{
  QMenu *menu = new QMenu(tr("F&ormat"), this);
  menuBar()->addMenu(menu);

  actionTextBold = new QAction(
      QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png")),
      tr("&Bold"), this);
  actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
  actionTextBold->setPriority(QAction::LowPriority);
  QFont bold;
  bold.setBold(true);
  actionTextBold->setFont(bold);
  connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
  menu->addAction(actionTextBold);
  actionTextBold->setCheckable(true);

  actionTextItalic
      = new QAction(QIcon::fromTheme("format-text-italic",
                                     QIcon(rsrcPath + "/textitalic.png")),
                    tr("&Italic"), this);
  actionTextItalic->setPriority(QAction::LowPriority);
  actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
  QFont italic;
  italic.setItalic(true);
  actionTextItalic->setFont(italic);
  connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
  menu->addAction(actionTextItalic);
  actionTextItalic->setCheckable(true);

  actionTextUnderline
      = new QAction(QIcon::fromTheme("format-text-underline",
                                     QIcon(rsrcPath + "/textunder.png")),
                    tr("&Underline"), this);
  actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
  actionTextUnderline->setPriority(QAction::LowPriority);
  QFont underline;
  underline.setUnderline(true);
  actionTextUnderline->setFont(underline);
  connect(actionTextUnderline, SIGNAL(triggered()), this,
          SLOT(textUnderline()));
  menu->addAction(actionTextUnderline);
  actionTextUnderline->setCheckable(true);

  menu->addSeparator();

  QActionGroup *grp = new QActionGroup(this);
  connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(textAlign(QAction *)));

  // Make sure the alignLeft  is always left of the alignRight
  if (QApplication::isLeftToRight()) {
    actionAlignLeft
        = new QAction(QIcon::fromTheme("format-justify-left",
                                       QIcon(rsrcPath + "/textleft.png")),
                      tr("&Left"), grp);
    actionAlignCenter
        = new QAction(QIcon::fromTheme("format-justify-center",
                                       QIcon(rsrcPath + "/textcenter.png")),
                      tr("C&enter"), grp);
    actionAlignRight
        = new QAction(QIcon::fromTheme("format-justify-right",
                                       QIcon(rsrcPath + "/textright.png")),
                      tr("&Right"), grp);
  } else {
    actionAlignRight
        = new QAction(QIcon::fromTheme("format-justify-right",
                                       QIcon(rsrcPath + "/textright.png")),
                      tr("&Right"), grp);
    actionAlignCenter
        = new QAction(QIcon::fromTheme("format-justify-center",
                                       QIcon(rsrcPath + "/textcenter.png")),
                      tr("C&enter"), grp);
    actionAlignLeft
        = new QAction(QIcon::fromTheme("format-justify-left",
                                       QIcon(rsrcPath + "/textleft.png")),
                      tr("&Left"), grp);
  }
  actionAlignJustify
      = new QAction(QIcon::fromTheme("format-justify-fill",
                                     QIcon(rsrcPath + "/textjustify.png")),
                    tr("&Justify"), grp);

  actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
  actionAlignLeft->setCheckable(true);
  actionAlignLeft->setPriority(QAction::LowPriority);
  actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
  actionAlignCenter->setCheckable(true);
  actionAlignCenter->setPriority(QAction::LowPriority);
  actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
  actionAlignRight->setCheckable(true);
  actionAlignRight->setPriority(QAction::LowPriority);
  actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
  actionAlignJustify->setCheckable(true);
  actionAlignJustify->setPriority(QAction::LowPriority);

  menu->addActions(grp->actions());

  menu->addSeparator();

  QPixmap pix(16, 16);
  pix.fill(Qt::black);
  actionTextColor = new QAction(pix, tr("&Color..."), this);
  connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
  menu->addAction(actionTextColor);
}

bool TextEdit::load(const QString &f)
{
  if (!QFile::exists(f))
    return false;
  QFile file(f);
  if (!file.open(QFile::ReadOnly))
    return false;

  QByteArray data = file.readAll();
  QTextCodec *codec = Qt::codecForHtml(data);
  QString str = codec->toUnicode(data);
  if (Qt::mightBeRichText(str)) {
    textEdit->setHtml(str);
  } else {
    str = QString::fromLocal8Bit(data);
    textEdit->setPlainText(str);
  }

  auto tc = textEdit->document()->find("[audio]");
  textEdit->setTextCursor(tc);
  addAudio();
  tc.movePosition(QTextCursor::Start);
  textEdit->setTextCursor(tc);

  setCurrentFileName(f);
  return true;
}

bool TextEdit::maybeSave()
{
  if (!textEdit->document()->isModified())
    return true;
  if (fileName.startsWith(QStringLiteral(":/")))
    return true;
  QMessageBox::StandardButton ret;
  ret = QMessageBox::warning(
      this, tr("Application"), tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  if (ret == QMessageBox::Save)
    return fileSave();
  else if (ret == QMessageBox::Cancel)
    return false;
  return true;
}

void TextEdit::setCurrentFileName(const QString &fileName)
{
  this->fileName = fileName;
  textEdit->document()->setModified(false);

  QString shownName;
  if (fileName.isEmpty())
    shownName = "untitled.txt";
  else
    shownName = QFileInfo(fileName).fileName();

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Rich Text")));
  setWindowModified(false);
}

void TextEdit::fileNew()
{
  if (maybeSave()) {
    textEdit->clear();
    setCurrentFileName(QString());
  }
}

void TextEdit::fileOpen()
{
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open File..."), QString(),
      tr("HTML-Files (*.htm *.html);;All Files (*)"));
  if (!fn.isEmpty())
    load(fn);
}

bool TextEdit::fileSave()
{
  if (fileName.isEmpty())
    return fileSaveAs();

  QTextDocumentWriter writer(fileName);
  bool success = writer.write(textEdit->document());
  if (success)
    textEdit->document()->setModified(false);
  return success;
}

bool TextEdit::fileSaveAs()
{
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save as..."), QString(),
      tr("ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)"));
  if (fn.isEmpty())
    return false;
  if (!(fn.endsWith(".odt", Qt::CaseInsensitive)
        || fn.endsWith(".htm", Qt::CaseInsensitive)
        || fn.endsWith(".html", Qt::CaseInsensitive)))
    fn += ".odt"; // default
  setCurrentFileName(fn);
  return fileSave();
}

void TextEdit::exportThemedHtml()
{
  Grantlee::Engine *engine = new Grantlee::Engine();
  engine->addDefaultLibrary("customtags");
  engine->addDefaultLibrary("grantlee_scriptabletags");

  QSharedPointer<Grantlee::FileSystemTemplateLoader> loader(
      new Grantlee::FileSystemTemplateLoader());
  qDebug() << GRANTLEE_TEMPLATE_PATH;
  loader->setTemplateDirs(QStringList() << GRANTLEE_TEMPLATE_PATH);
  engine->addTemplateLoader(loader);

  engine->setPluginPaths(QStringList()
                         << GRANTLEE_PLUGIN_PATH
                         << QApplication::applicationDirPath() + "/");

  QString themeName = "default.html";
  Grantlee::Template t = engine->loadByName(themeName);
  Grantlee::Context c;

  c.insert("abstract", abstractTextEdit->document());
  c.insert("content", textEdit->document());

  QString result = t->render(&c);

  webView->setPlainText(result);
}

void TextEdit::exportPlainText()
{
  Grantlee::Engine *engine = new Grantlee::Engine();
  engine->addDefaultLibrary("customtags");

  QSharedPointer<Grantlee::FileSystemTemplateLoader> loader(
      new Grantlee::FileSystemTemplateLoader());
  loader->setTemplateDirs(QStringList() << GRANTLEE_TEMPLATE_PATH);
  engine->addTemplateLoader(loader);

  engine->setPluginPaths(QStringList()
                         << GRANTLEE_PLUGIN_PATH
                         << QApplication::applicationDirPath() + "/");

  QString themeName = "default.txt";
  Grantlee::Template t = engine->loadByName(themeName);
  Grantlee::Context c;

  c.insert("abstract", abstractTextEdit->document());
  c.insert("content", textEdit->document());

  QString result = t->render(&c);

  webView->setPlainText(result);
}

void TextEdit::textBold()
{
  QTextCharFormat fmt;
  fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
  mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
  QTextCharFormat fmt;
  fmt.setFontUnderline(actionTextUnderline->isChecked());
  mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
  QTextCharFormat fmt;
  fmt.setFontItalic(actionTextItalic->isChecked());
  mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString &f)
{
  QTextCharFormat fmt;
  fmt.setFontFamily(f);
  mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString &p)
{
  qreal pointSize = p.toFloat();
  if (p.toFloat() > 0) {
    QTextCharFormat fmt;
    fmt.setFontPointSize(pointSize);
    mergeFormatOnWordOrSelection(fmt);
  }
}

void TextEdit::textStyle(int styleIndex)
{
  QTextCursor cursor = textEdit->textCursor();

  if (styleIndex != 0) {
    QTextListFormat::Style style = QTextListFormat::ListDisc;

    switch (styleIndex) {
    default:
    case 1:
      style = QTextListFormat::ListDisc;
      break;
    case 2:
      style = QTextListFormat::ListCircle;
      break;
    case 3:
      style = QTextListFormat::ListSquare;
      break;
    case 4:
      style = QTextListFormat::ListDecimal;
      break;
    case 5:
      style = QTextListFormat::ListLowerAlpha;
      break;
    case 6:
      style = QTextListFormat::ListUpperAlpha;
      break;
    case 7:
      style = QTextListFormat::ListLowerRoman;
      break;
    case 8:
      style = QTextListFormat::ListUpperRoman;
      break;
    }

    cursor.beginEditBlock();

    QTextBlockFormat blockFmt = cursor.blockFormat();

    QTextListFormat listFmt;

    if (cursor.currentList()) {
      listFmt = cursor.currentList()->format();
    } else {
      listFmt.setIndent(blockFmt.indent() + 1);
      blockFmt.setIndent(0);
      cursor.setBlockFormat(blockFmt);
    }

    listFmt.setStyle(style);

    cursor.createList(listFmt);

    cursor.endEditBlock();
  } else {
    // ####
    QTextBlockFormat bfmt;
    bfmt.setObjectIndex(-1);
    cursor.mergeBlockFormat(bfmt);
  }
}

void TextEdit::textColor()
{
  QColor col = QColorDialog::getColor(textEdit->textColor(), this);
  if (!col.isValid())
    return;
  QTextCharFormat fmt;
  fmt.setForeground(col);
  mergeFormatOnWordOrSelection(fmt);
  colorChanged(col);
}

void TextEdit::textAlign(QAction *a)
{
  if (a == actionAlignLeft)
    textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
  else if (a == actionAlignCenter)
    textEdit->setAlignment(Qt::AlignHCenter);
  else if (a == actionAlignRight)
    textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
  else if (a == actionAlignJustify)
    textEdit->setAlignment(Qt::AlignJustify);
}

void TextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
  colorChanged(format.foreground().color());
}

void TextEdit::cursorPositionChanged()
{
  alignmentChanged(textEdit->alignment());
}

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
  actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
#endif
}

void TextEdit::about()
{
  QMessageBox::about(
      this, tr("About"),
      tr("This example demonstrates Qt's "
         "rich text editing facilities in action, providing an example "
         "document for you to experiment with."));
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
  QTextCursor cursor = textEdit->textCursor();
  if (!cursor.hasSelection())
    cursor.select(QTextCursor::WordUnderCursor);
  cursor.mergeCharFormat(format);
  textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::colorChanged(const QColor &c)
{
  QPixmap pix(16, 16);
  pix.fill(c);
  actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(Qt::Alignment a)
{
  if (a & Qt::AlignLeft) {
    actionAlignLeft->setChecked(true);
  } else if (a & Qt::AlignHCenter) {
    actionAlignCenter->setChecked(true);
  } else if (a & Qt::AlignRight) {
    actionAlignRight->setChecked(true);
  } else if (a & Qt::AlignJustify) {
    actionAlignJustify->setChecked(true);
  }
}

void TextEdit::addAudio()
{
  //   QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
  //                                             QString(), tr("Audio-Files
  //                                             (*.ogg );;All Files (*)"));
  //   if (fn.isEmpty())
  //       return;

  AudioFormat audioFormat;

  // read file into Phonon object?

  //   audioFormat.setProperty( AudioProperty, fn );
  audioFormat.setProperty(AudioProperty, "horse.ogg");

  textEdit->textCursor().insertText(QString(QChar::ObjectReplacementCharacter),
                                    audioFormat);
}
