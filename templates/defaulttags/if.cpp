/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>
  Copyright (c) 2015 Daniel Nicoletti <dantti12@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "if.h"

#include "../lib/exception.h"
#include "parser.h"

IfNodeFactory::IfNodeFactory()
{

}

class IfNodeParser
{
public:
    IfNodeParser( Context *c, const QList<IfNodeToken *> &tokens )
    {
        this->c = c;
        this->tokens = tokens;
        iterator = tokens.constBegin();
    }

    Context *c;
    QList<IfNodeToken *> tokens;
    QList<IfNodeToken *>::ConstIterator iterator;

    bool expression( uint rbp );
};


class IfNodeToken
{
public:
    IfNodeToken(uint _lbp = 0) : lbp(_lbp)
    {}
    virtual ~IfNodeToken() {}

    virtual bool led( IfNodeParser *p, bool left, IfNodeToken *right ) const
    {
        Q_UNUSED( p )
        Q_UNUSED( left )
        Q_UNUSED( right )
        return false;
    }

    virtual bool nud( IfNodeParser *p ) const
    {
        Q_UNUSED( p )
        return false;
    }

    uint lbp;
    FilterExpression filterExpression;
};

class IfNodeLiteralToken : public IfNodeToken
{
public:
    IfNodeLiteralToken(const FilterExpression &expr) : IfNodeToken(1)
    {
        filterExpression = expr;
    }

    virtual bool nud( IfNodeParser *p ) const
    { return filterExpression.isTrue( p->c ); }
};

class IfNodeOrToken : public IfNodeToken
{
public:
    IfNodeOrToken() : IfNodeToken(1) {}
    virtual bool led( IfNodeParser *p, bool left, IfNodeToken *right ) const
    {
        return left || right->nud( p );
    }
};

class IfNodeAndToken : public IfNodeToken
{
public:
    IfNodeAndToken() : IfNodeToken(2) {}

    virtual bool led( IfNodeParser *p, bool left, IfNodeToken *right ) const
    {
        return left && right->nud( p );
    }
};

class IfNodeNotToken : public IfNodeToken
{
public:
    IfNodeNotToken() : IfNodeToken(3) {}

    virtual bool nud( IfNodeParser *p ) const
    {
        if (++p->iterator != p->tokens.constEnd() ) {
            IfNodeToken *token = *p->iterator;
            return ! token->nud( p );
        }
        return false;
    }

    virtual bool led( IfNodeParser *p, bool left, IfNodeToken *right ) const
    {
        return ! right->nud( p );
    }
};

class IfNodeInToken : public IfNodeToken
{
public:
    IfNodeInToken() : IfNodeToken(4) {}
};

class IfNodeOperatorEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorEqualToken() : IfNodeToken(5) {}
};


class IfNodeOperatorNotEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorNotEqualToken() : IfNodeToken(5) {}
};

class IfNodeOperatorLessThanToken : public IfNodeToken
{
public:
    IfNodeOperatorLessThanToken() : IfNodeToken(5) {}
};

class IfNodeOperatorGreaterThanToken : public IfNodeToken
{
public:
    IfNodeOperatorGreaterThanToken() : IfNodeToken(5) {}
};

class IfNodeOperatorLessEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorLessEqualToken() : IfNodeToken(5) {}
};

class IfNodeOperatorGreaterEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorGreaterEqualToken() : IfNodeToken(5) {}
};

class IfNodeEndToken : public IfNodeToken
{
public:
    IfNodeEndToken() : IfNodeToken(0) {}
};

QList<IfNodeToken *> tokenize(const QStringList &tokens, Grantlee::Parser *parser)
{
    QList<IfNodeToken *> ret;

    QString lastToken;
    Q_FOREACH (const QString &token, tokens) {
        if (token == QLatin1String("or")) {
            ret.append(new IfNodeOrToken);
        } else if (token == QLatin1String("and")) {
            ret.append(new IfNodeAndToken);
        } else if (token == QLatin1String("not")) {
            ret.append(new IfNodeNotToken);
        } else if (token == QLatin1String("in")) {
            ret.append(new IfNodeInToken);
        } else if (token == QLatin1String("==")) {
            ret.append(new IfNodeOperatorEqualToken);
        } else if (token == QLatin1String("!=")) {
            ret.append(new IfNodeOperatorNotEqualToken);
        } else if (token == QLatin1String("<")) {
            ret.append(new IfNodeOperatorLessThanToken);
        } else if (token == QLatin1String(">")) {
            ret.append(new IfNodeOperatorGreaterThanToken);
        } else if (token == QLatin1String("<=")) {
            ret.append(new IfNodeOperatorLessEqualToken);
        } else if (token == QLatin1String(">=")) {
            ret.append(new IfNodeOperatorGreaterEqualToken);
        } else {
            ret.append(new IfNodeLiteralToken(FilterExpression(token.trimmed(), parser)));
        }
    }

    ret.append(new IfNodeEndToken);

    return ret;
}


Node* IfNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );
  expr.takeAt( 0 );
  if ( expr.size() <= 0 ) {
    throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'if' statement requires at least one argument" ) );
  }

  QList<IfNodeToken *> tokens = tokenize( expr, p );
  IfNode *n = new IfNode( tokens, p );

  NodeList trueList = p->parse( n, QStringList() << QStringLiteral( "else" ) << QStringLiteral( "endif" ) );
  n->setTrueList( trueList );
  NodeList falseList;
  if ( p->takeNextToken().content.trimmed() == QStringLiteral( "else" ) ) {
    falseList = p->parse( n, QStringLiteral( "endif" ) );
    n->setFalseList( falseList );
    // skip past the endif tag
    p->removeNextToken();
  } // else empty falseList.

  return n;
}


IfNode::IfNode( QList<QPair<bool, FilterExpression > > boolVars, int linkType, QObject *parent )
    : Node( parent ),
    m_boolVars( boolVars ),
    m_linkType( linkType )
{

}

IfNode::IfNode(const QList<IfNodeToken *> &tokens, QObject *parent) : Node( parent )
  , m_tokens( tokens )
{

}

void IfNode::setTrueList( NodeList trueList )
{
  m_trueList = trueList;
}

void IfNode::setFalseList( NodeList falseList )
{
  m_falseList = falseList;
}



void IfNode::render( OutputStream *stream, Context *c ) const
{
  // Evaluate the expression. rendering variables with the context as needed. and processing nodes recursively
  // in either trueList or falseList as determined by booleanExpression.

    IfNodeParser nodeParser( c, m_tokens );
    if ( nodeParser.expression( 0) ) {
        renderTrueList( stream, c );
    } else {
        renderFalseList( stream, c );
    }
    return;


  if ( m_linkType == OrLink ) {
    for ( int i = 0; i < m_boolVars.size(); i++ ) {
      QPair<bool, FilterExpression> pair = m_boolVars.at( i );
      bool negate = pair.first;

      bool isTrue = pair.second.isTrue( c );

      if ( isTrue != negate ) {
        renderTrueList( stream, c );
        return;
      }
    }
//     return renderFalseList(c);
  } else {
    bool renderTrue = true;
    for ( int i = 0; i < m_boolVars.size(); i++ ) {
      QPair<bool, FilterExpression> pair = m_boolVars.at( i );
      bool negate = pair.first;

      bool isTrue = pair.second.isTrue( c );

      // Karnaugh map:
      //          VariantIsTrue
      //          \ 0   1
      //         0| 0 | 1 |
      // negate  1| 1 | 0 |

      if (( !isTrue && !negate )
          || ( isTrue && negate ) ) {
        renderTrue = false;
        break;
      }
    }
    if ( renderTrue ) {
      renderTrueList( stream, c );
      return;
    }
  }

  renderFalseList( stream, c );
}

void IfNode::renderTrueList( OutputStream *stream, Context *c ) const
{
  return m_trueList.render( stream, c );
}

void IfNode::renderFalseList( OutputStream *stream, Context *c ) const
{
  return m_falseList.render( stream, c );
}


bool IfNodeParser::expression(uint rbp)
{
    if (iterator == tokens.constEnd()) {
        return false;
    }

    IfNodeToken *t = *iterator;
    IfNodeToken *token = *++iterator;
    bool left = t->nud( this );
    while ( rbp < token->lbp) {

        t = token;
        token = *++iterator;
        left = t->led( this, left, token );
    }
    return left;
}
