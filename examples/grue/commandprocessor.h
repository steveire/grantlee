
#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QtCore/QObject>

class QLineEdit;
class QTextEdit;

class CommandProcessor : public QObject
{
  Q_OBJECT
public:
  CommandProcessor(QLineEdit *le, QTextEdit *te, QObject *parent = 0);

private slots:
  void processCommand();

private:
  QLineEdit *m_le;
  QTextEdit *m_te;
};

#endif
