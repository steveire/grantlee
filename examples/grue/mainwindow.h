
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QLineEdit;
class QTextEdit;

class CommandProcessor;

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);

private:
  QLineEdit *m_lineEdit;
  QTextEdit *m_textEdit;
  CommandProcessor *m_commandProcessor;
};

#endif
