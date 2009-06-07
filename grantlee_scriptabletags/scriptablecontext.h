
#ifndef SCRIPTABLE_CONTEXT_H
#define SCRIPTABLE_CONTEXT_H

#include <QObject>
#include <QVariant>

namespace Grantlee
{
class Context;
}

using namespace Grantlee;

class ScriptableContext : public QObject
{
  Q_OBJECT
public:
  ScriptableContext( Context *c, QObject* parent = 0 );

  Context* context() {
    return m_c;
  }

public slots:
  QVariant lookup( const QString &name );
  void insert( const QString &name, const QVariant &variant );
  void push();
  void pop();

  QString render( const QObjectList &list );


private:
  Context *m_c;

};

#endif
