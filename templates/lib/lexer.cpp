/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010,2011 Stephen Kelly <steveire@gmail.com>

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


#include "lexer_p.h"

using namespace Grantlee;

typedef State<TextProcessingMachine::Type> TextProcessingState;
typedef TextProcessingMachine::Transition TextProcessingTransition;

typedef LexerObject<TextProcessingState, NullTest, MarksClearer> ChurningState;
typedef LexerObject<TextProcessingState, NullTest, TokenFinalizer> FinalizeTokenState;
typedef LexerObject<TextProcessingTransition, NullTest, TokenFinalizer> EofHandler;

typedef CharacterTransition<'{', MarkStartSyntax> MaybeTemplateSyntaxHandler;

typedef CharacterTransition<'%'> TagHandler;
typedef CharacterTransition<'#'> CommentHandler;
typedef CharacterTransition<'{'> BeginValueHandler;
typedef CharacterTransition<'}'> MaybeEndValueHandler;
typedef CharacterTransition<'\n'> NewlineHandler;
typedef CharacterTransition<'}', MarkEndSyntax> EndTemplateSyntaxHandler;
typedef NegateCharacterTransition<'}'> NotEndTemplateSyntaxHandler;

typedef LexerObject<TextProcessingTransition,
            Negate<
                OrTest<CharacterTest<'{'>,
                OrTest<CharacterTest<'#'>,
                       CharacterTest<'%'> > > > > NotBeginTemplateSyntaxHandler;

template<typename Transition>
void addTransition( TextProcessingState *source, Lexer*lexer, TextProcessingState *target )
{
  Transition *tr = new Transition( lexer, source );
  tr->setTargetState( target );
}

TextProcessingMachine* createMachine( Lexer *lexer )
{
  TextProcessingMachine *machine = new TextProcessingMachine;

  TextProcessingState *notFinished = new TextProcessingState( machine );
  TextProcessingState *finished = new TextProcessingState( machine );
  machine->setInitialState( notFinished );

  TextProcessingState *processingText = new ChurningState( lexer, notFinished );
  TextProcessingState *processingBeginTemplateSyntax = new TextProcessingState( notFinished );
  TextProcessingState *processingTag = new TextProcessingState( notFinished );
  TextProcessingState *processingComment = new TextProcessingState( notFinished );
  TextProcessingState *processingValue = new TextProcessingState( notFinished );
  TextProcessingState *processingEndTag = new TextProcessingState( notFinished );
  TextProcessingState *processingEndComment = new TextProcessingState( notFinished );
  TextProcessingState *processingEndValue = new TextProcessingState( notFinished );
  TextProcessingState *processingPostTemplateSyntax = new FinalizeTokenState( lexer, notFinished );

  notFinished->setInitialState( processingText );

  addTransition<MaybeTemplateSyntaxHandler>( processingText, lexer, processingBeginTemplateSyntax );

  addTransition<TagHandler>(                    processingBeginTemplateSyntax, lexer, processingTag );
  addTransition<CommentHandler>(                processingBeginTemplateSyntax, lexer, processingComment );
  addTransition<BeginValueHandler>(             processingBeginTemplateSyntax, lexer, processingValue );
  addTransition<NotBeginTemplateSyntaxHandler>( processingBeginTemplateSyntax, lexer, processingText );

  addTransition<NewlineHandler>( processingTag, lexer, processingText );
  addTransition<TagHandler>(     processingTag, lexer, processingEndTag );

  addTransition<NewlineHandler>( processingComment, lexer, processingText );
  addTransition<CommentHandler>( processingComment, lexer, processingEndComment );

  addTransition<NewlineHandler>(       processingValue, lexer, processingText );
  addTransition<MaybeEndValueHandler>( processingValue, lexer, processingEndValue );

  addTransition<NewlineHandler>(              processingEndTag, lexer, processingText );
  addTransition<NotEndTemplateSyntaxHandler>( processingEndTag, lexer, processingTag );
  addTransition<EndTemplateSyntaxHandler>(    processingEndTag, lexer, processingPostTemplateSyntax );

  addTransition<NewlineHandler>(              processingEndComment, lexer, processingText );
  addTransition<NotEndTemplateSyntaxHandler>( processingEndComment, lexer, processingComment );
  addTransition<EndTemplateSyntaxHandler>(    processingEndComment, lexer, processingPostTemplateSyntax );

  addTransition<NewlineHandler>(              processingEndValue, lexer, processingText );
  addTransition<NotEndTemplateSyntaxHandler>( processingEndValue, lexer, processingValue );
  addTransition<EndTemplateSyntaxHandler>(    processingEndValue, lexer, processingPostTemplateSyntax );

  processingPostTemplateSyntax->setUnconditionalTransition( processingText );
  {
    EofHandler *handler = new EofHandler( lexer, notFinished );
    handler->setTargetState( finished );
    notFinished->setEndTransition( handler );
  }

  return machine;
}

Lexer::Lexer( const QString &templateString )
  : m_templateString( templateString )
{

}

Lexer::~Lexer()
{
}

void Lexer::clearMarkers()
{
  m_startSyntaxPosition = -1;
  m_endSyntaxPosition = -1;
}

void Lexer::reset()
{
  m_tokenList.clear();
  m_upto = 0;
  m_processedUpto = 0;
  clearMarkers();
}

QList<Token> Lexer::tokenize()
{
  TextProcessingMachine* machine = createMachine( this );

  machine->start();

  QString::const_iterator it = m_templateString.constBegin();
  const QString::const_iterator end = m_templateString.constEnd();

  reset();
  for ( ; it != end; ++it, ++m_upto )
    machine->processCharacter( it );

  machine->finished();

  machine->stop();

  delete machine;

  return m_tokenList;
}

void Lexer::markStartSyntax()
{
  m_startSyntaxPosition = m_upto;
}

void Lexer::markEndSyntax()
{
  m_endSyntaxPosition = m_upto + 1;
}

void Lexer::finalizeToken()
{
  int nextPosition = m_upto;
  const bool validSyntax = m_endSyntaxPosition > m_startSyntaxPosition;
  if ( validSyntax && m_startSyntaxPosition >= 0 ) {
    nextPosition = qMin( m_startSyntaxPosition, m_upto );
  }

  {
    Token token;
    token.content = m_templateString.mid( m_processedUpto, nextPosition - m_processedUpto );
    token.tokenType = TextToken;
    m_tokenList.append( token );
  }

  m_processedUpto = nextPosition;

  if ( !validSyntax )
    return;

  const QString syntaxContent = m_templateString.mid( m_startSyntaxPosition, m_endSyntaxPosition - m_startSyntaxPosition );

  m_processedUpto = m_endSyntaxPosition;
  Token syntaxToken;
  syntaxToken.content = syntaxContent.mid( 2, syntaxContent.size() - 4 ).trimmed();
  if ( syntaxContent.startsWith( QLatin1String( "{{" ) ) ) {
    syntaxToken.tokenType = VariableToken;
  } else if ( syntaxContent.startsWith( QLatin1String( "{%" ) ) ) {
    syntaxToken.tokenType = BlockToken;
  } else if ( syntaxContent.startsWith( QLatin1String( "{#" ) ) ) {
    return;
  }
  m_tokenList.append( syntaxToken );
}
