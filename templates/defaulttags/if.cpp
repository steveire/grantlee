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
#include "util.h"

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

    QVariant expression( uint rbp = 0 );
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
    IfNodeToken(uint _lbp = 0, Type _type = End) : lbp(_lbp), type(_type)
    {}
    virtual ~IfNodeToken() {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        Q_UNUSED( p )
        Q_UNUSED( left )
        return QVariant();
    }

    virtual QVariant nud( IfNodeParser *p ) const
    {
        Q_UNUSED( p )
        return QVariant();
    }

    Type type;
    uint lbp;
    QString token;
    FilterExpression filterExpression;
};

class IfNodeLiteralToken : public IfNodeToken
{
public:
    IfNodeLiteralToken(const FilterExpression &expr) : IfNodeToken(1, IfNodeToken::Literal)
    {
        filterExpression = expr;
    }

    virtual QVariant nud( IfNodeParser *p ) const
    {
        return filterExpression.resolve( p->c );
    }
};

class IfNodeOrToken : public IfNodeToken
{
public:
    IfNodeOrToken() : IfNodeToken(1, IfNodeToken::Or) {}
    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        if ( Grantlee::variantIsTrue( left ) ) {
            p->token = p->tokens.last(); // Skip processing right part
            return true;
        } else {
            const QVariant &right = p->expression( 1 );
            return Grantlee::variantIsTrue( right );
        }
    }
};

class IfNodeAndToken : public IfNodeToken
{
public:
    IfNodeAndToken() : IfNodeToken(2, IfNodeToken::And) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return Grantlee::variantIsTrue( left ) && Grantlee::variantIsTrue( right );
    }
};

class IfNodeNotToken : public IfNodeToken
{
public:
    IfNodeNotToken() : IfNodeToken(3, IfNodeToken::Not) {}

    virtual QVariant nud( IfNodeParser *p ) const
    {
        const QVariant &right = p->expression( 3 );
        return ! Grantlee::variantIsTrue( right );
    }
};

class IfNodeInToken : public IfNodeToken
{
public:
    IfNodeInToken() : IfNodeToken(4, IfNodeToken::In) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return Grantlee::contains( left, right );
    }
};

class IfNodeNotInToken : public IfNodeToken
{
public:
    IfNodeNotInToken() : IfNodeToken(4, IfNodeToken::In) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return ! Grantlee::contains( left, right );
    }
};

class IfNodeOperatorEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return Grantlee::equals( left, right );
    }
};


class IfNodeOperatorNotEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorNotEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return ! Grantlee::equals( left, right );
    }
};

class IfNodeOperatorLessThanToken : public IfNodeToken
{
public:
    IfNodeOperatorLessThanToken() : IfNodeToken(5, IfNodeToken::Operators) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return Grantlee::lessThan( left, right );
    }
};

class IfNodeOperatorGreaterThanToken : public IfNodeToken
{
public:
    IfNodeOperatorGreaterThanToken() : IfNodeToken(5, IfNodeToken::Operators) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return ! Grantlee::lessThan( left, right );
    }
};

class IfNodeOperatorLessEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorLessEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return Grantlee::lessThan( left, right ) || Grantlee::equals( left, right );
    }
};

class IfNodeOperatorGreaterEqualToken : public IfNodeToken
{
public:
    IfNodeOperatorGreaterEqualToken() : IfNodeToken(5, IfNodeToken::Operators) {}

    virtual QVariant led( IfNodeParser *p, QVariant left ) const
    {
        const QVariant &right = p->expression( 2 );
        return ! Grantlee::lessThan( left, right ) || Grantlee::equals( left, right );
    }
};

class IfNodeEndToken : public IfNodeToken
{
};

QList<IfNodeToken *> tokenize(const QStringList &tokens, Grantlee::Parser *parser)
{
    QList<IfNodeToken *> ret;

    QString lastToken;
    IfNodeToken::Type lastType = IfNodeToken::None;
    QStringList::ConstIterator it = tokens.constBegin();
    while ( it != tokens.constEnd() ) {
        const QString &token = *it;
        IfNodeToken *ifNodeToken;
        if (token == QLatin1String("or")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'or' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOrToken;
        } else if (token == QLatin1String("and")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'and' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeAndToken;
        } else if (token == QLatin1String("not")) {
            if (++it != tokens.constEnd() && *it == QLatin1String("in") ) {
                if (lastType != IfNodeToken::Literal) {
                    qDeleteAll(ret);
                    throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'not in' requires a previous argument" ) );
                }
                ifNodeToken = new IfNodeNotInToken;
            } else {
                --it;
                ifNodeToken = new IfNodeNotToken;
            }
        } else if (token == QLatin1String("in")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'in' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeInToken;
        } else if (token == QLatin1String("==")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'==' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorEqualToken;
        } else if (token == QLatin1String("!=")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'!=' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorNotEqualToken;
        } else if (token == QLatin1String("<")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'<' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorLessThanToken;
        } else if (token == QLatin1String(">")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'>' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorGreaterThanToken;
        } else if (token == QLatin1String("<=")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'<=' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorLessEqualToken;
        } else if (token == QLatin1String(">=")) {
            if (lastType != IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'>=' requires a previous argument" ) );
            }
            ifNodeToken = new IfNodeOperatorGreaterEqualToken;
        } else {
            if (lastType == IfNodeToken::Literal) {
                qDeleteAll(ret);
                throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "'%1' can not be followed by another literal" ).arg(lastToken.trimmed()) );
            }
            ifNodeToken = new IfNodeLiteralToken(FilterExpression(token.trimmed(), parser));
        }

        ifNodeToken->token = token;
        lastToken = token;
        lastType = ifNodeToken->type;

        ret.append(ifNodeToken);

        ++it;
    }

    if (lastType != IfNodeToken::Literal) {
        qDeleteAll(ret);
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

IfNode::IfNode(const QList<IfNodeToken *> &tokens, QObject *parent) : Node( parent )
  , m_tokens( tokens )
{

}

IfNode::~IfNode()
{
    qDeleteAll(m_tokens);
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
  QVariant result = nodeParser.expression();
  if ( Grantlee::variantIsTrue( result ) ) {
    renderTrueList( stream, c );
  } else {
    renderFalseList( stream, c );
  }
}

void IfNode::renderTrueList( OutputStream *stream, Context *c ) const
{
  return m_trueList.render( stream, c );
}

void IfNode::renderFalseList( OutputStream *stream, Context *c ) const
{
  return m_falseList.render( stream, c );
}

QVariant IfNodeParser::expression(uint rbp)
{
    if (iterator == tokens.constEnd()) {
        return false;
    }

    IfNodeToken *t = token;
    token = next();
    QVariant left = t->nud( this );
    while ( rbp < token->lbp ) {
        t = token;
        token = next();
        left = t->led( this, left );
    }

    return left;
}
