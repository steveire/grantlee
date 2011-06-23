
#include "commandprocessor.h"

#include <QLineEdit>
#include <QTextEdit>

#include "commandparser.h"
#include "commandtokenizer.h"
#include "script.h"

CommandProcessor::CommandProcessor(QLineEdit *le, QTextEdit *te, QObject *parent)
  : QObject(parent), m_le(le), m_te(te)
{
  connect(m_le, SIGNAL(editingFinished()), SLOT(processCommand()));
}

void CommandProcessor::processCommand()
{
  const QString command = m_le->text();
  CommandTokenizer tokenizer(command);
  QList<Token> tokens = tokenizer.tokenize();

  if (tokenizer.error()) {
    m_te->append(QString("Syntax Error: %1: %2").arg(tokenizer.errorString()).arg(command));
    return;
  }

  CommandParser parser(tokens);
  Script script = parser->parse();
  if (script.error()) {
    m_te->append(QString("Parse Error: %1: %2").arg(script.errorString()).arg(command));
    return;
  }

  script.execute();

  if (script.error()) {
    m_te->append(QString("Runtime Error: %1: %2").arg(script.errorString()).arg(command));
    return;
  }

  m_te->append(script.result());
}

#include "commandprocessor.moc"
