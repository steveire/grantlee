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
        token = *iterator;
    }

    IfNodeToken *next()
    {
        return *++iterator;
    }

    Context *c;
    IfNodeToken *token;
    QList<IfNodeToken *> tokens;
    QList<IfNodeToken *>::ConstIterator iterator;

    bool expression( uint rbp );
};


class IfNodeToken
{
public:
    enum Type {
        None,
        Literal,
        Or,
        And,
        Not,
        In,
        Operators,
        End
    };
    IfNodeToken(uint _lbp = 0, Type _type = End) : type(_type), lbp(_lbp)
    {}
    virtual ~IfNodeToken() {}

    virtual bool led( IfNodeParser *p, bool left ) const
    {
        Q_UNUSED( p )
        Q_UNUSED( left )
        return false;
    }

    virtual bool nud( IfNodeParser *p ) const
    {
        Q_UNUSED( p )
        return false;
    }

    QString token;
    FilterExpression filterExpression;
    Type type;
    uint lbp;
};

class IfNodeLiteralToken : public IfNodeToken
{
public:
    IfNodeLiteralToken(const FilterExpression &expr) : IfNodeToken(1, IfNodeToken::Literal)
    {
        filterExpression = expr;
    }

    virtual bool nud( IfNodeParser *p ) const
    {
        bool ret = filterExpression.isTrue( p->c );
        return ret;
    }
};

class IfNodeOrToken : public IfNodeToken
{
public:
    IfNodeOrToken() : IfNodeToken(1, IfNodeToken::Or) {}
    virtual bool led( IfNodeParser *p, bool left ) const
    {
        bool right = p->expression( 1 );
        return left || right;
    }
};

class IfNodeAndToken : public IfNodeToken
{
public:
    IfNodeAndToken() : IfNodeToken(2, IfNodeToken::And) {}

    virtual bool led( IfNodeParser *p, bool left ) const
    {
        bool right = p->expression( 2 );
        return left && right;
    }
};

class IfNodeNotToken : public IfNodeToken
{
public:
    IfNodeNotToken() : IfNodeToken(3, IfNodeToken::Not) {}

    virtual bool nud( IfNodeParser *p ) const
    {
        return ! p->expression( 3 );
    }

//    virtual bool led( IfNodeParser *p, bool left, IfNodeToken *right ) const
//    {
//        return ! right->nud( p );
//    }
};

class IfNodeInToken : public IfNodeToken
{
public:
    IfNodeInToken() : IfNodeToken(4, IfNodeToken::In) {}
};

class IfNodeOperatorEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}
};


class IfNodeOperatorNotEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorNotEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}
};

class IfNodeOperatorLessThanToken : public IfNodeToken
{
public:
    IfNodeOperatorLessThanToken() : IfNodeToken(5, IfNodeToken::Operators) {}
};

class IfNodeOperatorGreaterThanToken : public IfNodeToken
{
public:
    IfNodeOperatorGreaterThanToken() : IfNodeToken(5, IfNodeToken::Operators) {}
};

class IfNodeOperatorLessEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorLessEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}
};

class IfNodeOperatorGreaterEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorGreaterEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}
};

class IfNodeEndToken : public IfNodeToken
{
};

QList<IfNodeToken *> tokenize(const QStringList &tokens, Grantlee::Parser *parser)
{
    QList<IfNodeToken *> ret;

    QString lastToken;
    IfNodeToken::Type lastType = IfNodeToken::None;
    Q_FOREACH (const QString &token, tokens) {
        IfNodeToken *ifNodeToken;
        if (token == QLatin1String("or")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'or' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOrToken;
        } else if (token == QLatin1String("and")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'and' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeAndToken;
        } else if (token == QLatin1String("not")) {
            ifNodeToken = new IfNodeNotToken;
        } else if (token == QLatin1String("in")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'in' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeInToken;
        } else if (token == QLatin1String("==")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'==' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorEqualToken;
        } else if (token == QLatin1String("!=")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'!=' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorNotEqualToken;
        } else if (token == QLatin1String("<")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'<' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorLessThanToken;
        } else if (token == QLatin1String(">")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'>' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorGreaterThanToken;
        } else if (token == QLatin1String("<=")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'<=' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorLessEqualToken;
        } else if (token == QLatin1String(">=")) {
            if (lastType != IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'>=' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorGreaterEqualToken;
        } else {
            if (lastType == IfNodeToken::Literal) {
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'%1' can not be followed by another literal" ).arg(lastToken.trimmed()) );
            }
            ifNodeToken = new IfNodeLiteralToken(FilterExpression(token.trimmed(), parser));
        }

        ifNodeToken->token = token;
        lastToken = token;
        lastType = ifNodeToken->type;

        ret.append(ifNodeToken);
    }

    if (lastType != IfNodeToken::Literal) {
        throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'%1' must be followed by a literal" ).arg(lastToken.trimmed()) );
    }

    IfNodeToken *end = new IfNodeEndToken;
    end->token = QStringLiteral("IfNodeEndToken");
    ret.append(end);

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

    IfNodeToken *t = token;
    token = next();
    bool left = t->nud( this );
    while ( rbp < token->lbp) {
        t = token;
        token = next();
        left = t->led( this, left );
    }

    return left;
}
