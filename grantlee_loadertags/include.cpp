/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "include.h"

#include <QStringList>
#include <QtPlugin>

#include "parser.h"
#include "template.h"
#include "templateloader.h"

#include <QDebug>
#include <util_p.h>

IncludeNodeFactory::IncludeNodeFactory()
{

}

Node* IncludeNodeFactory::getNode( const QString &tagContent, Parser *p, QObject *parent ) const
{
  QStringList expr = smartSplit( tagContent );

  QString includeName = expr.at( 1 );
  int size = includeName.size();

  if (( includeName.startsWith( "\"" ) && includeName.endsWith( "\"" ) )
      || ( includeName.startsWith( "'" ) && includeName.endsWith( "'" ) ) ) {
    return new ConstantIncludeNode( includeName.mid( 1, size - 2 ), parent );
  }
  return new IncludeNode( FilterExpression( includeName, p ), parent );
}

IncludeNode::IncludeNode( const FilterExpression &fe, QObject *parent )
    : Node( parent ), m_filterExpression( fe )
{
}

QString IncludeNode::render( Context *c )
{
  QString filename = Util::getSafeString( m_filterExpression.resolve( c ) ).rawString();

  Engine *loader = Engine::instance();

  Template *t = loader->loadByName( filename, this );

  if ( !t )
    return QString();

  return t->render( c );
}

ConstantIncludeNode::ConstantIncludeNode( const QString &name, QObject *parent )
    : Node( parent )
{
  m_name = name;
}

QString ConstantIncludeNode::render( Context *c )
{
  Engine *loader = Engine::instance();

  Template* t = loader->loadByName( m_name, this );

  if ( !t )
    return QString();

  return t->render( c );
}
