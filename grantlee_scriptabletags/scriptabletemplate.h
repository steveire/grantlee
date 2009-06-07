
#ifndef SCRIPTABLE_TEMPLATE_H
#define SCRIPTABLE_TEMPLATE_H

#include <QObject>
#include <QVariant>

#include <QScriptValue>

namespace Grantlee
{
class Template;
class Node;
}

class QScriptContext;

class ScriptableContext;

using namespace Grantlee;

Q_DECLARE_METATYPE( Grantlee::Template* )

QScriptValue ScriptableTemplateConstructor( QScriptContext *context,
    QScriptEngine *engine );

class ScriptableTemplate : public QObject
{
  Q_OBJECT
public:
  ScriptableTemplate( Template *t, QObject* parent = 0 );

public slots:
  void setContent( const QString &content );

  QString render( ScriptableContext *c );

  QObjectList getNodesByType( const char * className );

  QObjectList nodeList() const;

  void setNodeList( const QObjectList &list );

private:
  Template *m_template;

};

#endif
