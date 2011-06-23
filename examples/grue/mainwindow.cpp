
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>

#include "commandprocessor.h"

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent), m_textEdit(new QTextEdit(this)), m_lineEdit(new QLineEdit(this)),
    m_commandProcessor(new CommandProcessor(m_lineEdit, m_textEdit))
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_textEdit);
  layout->addWidget(m_lineEdit);
}

#include "mainwindow.moc"
