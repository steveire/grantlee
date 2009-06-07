/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "comment.h"

#include <QStringList>
#include <QtPlugin>

#include "parser.h"


CommentNodeFactory::CommentNodeFactory()
{

}

Node* CommentNodeFactory::getNode( const QString &tagContent, Parser *p, QObject *parent ) const
{
  Q_UNUSED( tagContent );

  p->skipPast( "endcomment" );

  return new CommentNode( parent );
}

CommentNode::CommentNode( QObject *parent )
    : Node( parent )
{

}

QString CommentNode::render( Context *c )
{
  Q_UNUSED( c );
  return QString();
}

