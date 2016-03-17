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
typedef LexerObject<TextProcessingTransition, NullTest, TokenFinalizerWithTrimming> EofHandlerWithTrimming;

typedef CharacterTransition<'{'> MaybeTemplateSyntaxHandler;

typedef CharacterTransition<'%', MarkStartSyntax> TagStartHandler;
typedef CharacterTransition<'#', MarkStartSyntax> CommentStartHandler;
typedef CharacterTransition<'%'> TagEndHandler;
typedef CharacterTransition<'#'> CommentEndHandler;
typedef CharacterTransition<'{', MarkStartSyntax> BeginValueHandler;
typedef CharacterTransition<'}'> MaybeEndValueHandler;
typedef CharacterTransition<'\n', MarkNewline> NewlineHandler;
typedef CharacterTransition<'}', MarkEndSyntax> EndTemplateSyntaxHandler;
typedef NegateCharacterTransition<'}'> NotEndTemplateSyntaxHandler;

typedef LexerObject<TextProcessingTransition,
            Negate<
                OrTest<CharacterTest<'{'>,
                OrTest<CharacterTest<'#'>,
                       CharacterTest<'%'> > > > > NotBeginTemplateSyntaxHandler;

typedef LexerObject<TextProcessingTransition,
            Negate<
                OrTest<CharacterTest<'{'>,
                OrTest<CharacterTest<'#'>,
                OrTest<CharacterTest<'%'>,
                       CharacterTest<'\n'> > > > > > NotBeginTemplateSyntaxOrNewlineHandler;

typedef LexerObject<TextProcessingTransition,
            Negate<
                OrTest<CharacterTest<'#'>,
                OrTest<CharacterTest<'%'>,
                       CharacterTest<'\n'> > > > > NotTagCommentOrNewlineHandler;

typedef LexerObject<TextProcessingTransition,
            Negate<
                OrTest<IsSpace,
                       CharacterTest<'{'> > > > NonWhitespaceLineTextHandler;

typedef LexerObject<TextProcessingTransition,
            AndTest<Negate<CharacterTest<'\n'> >,
                    IsSpace> > WhitespaceNonNewlineHandler;

typedef LexerObject<TextProcessingTransition,
            Negate<
                OrTest<CharacterTest<'{'>,
                       IsSpace> >,
            TokenFinalizer> FinalizingLineTextHandler;

typedef CharacterTransition<'\n', TokenFinalizerWithTrimmingAndNewline> SyntaxBoundaryNewlineHandler;
typedef CharacterTransition<'{', FinalizeAndMarkStartSyntax> SyntaxBoundaryHandler;

template<typename Transition>
void addTransition( TextProcessingState *source, Lexer *lexer, TextProcessingState *target )
{
  auto tr = new Transition( lexer, source );
  tr->setTargetState( target );
}

TextProcessingMachine* createMachine( Lexer *lexer, Lexer::TrimType type )
{
  auto machine = new TextProcessingMachine;

  auto notFinished = new TextProcessingState( machine );
  auto finished = new TextProcessingState( machine );
  machine->setInitialState( notFinished );

  auto processingText = new ChurningState( lexer, notFinished );
  auto processingPostNewline = new TextProcessingState( notFinished );
  auto processingBeginTemplateSyntax = new TextProcessingState( notFinished );
  auto processingTag = new TextProcessingState( notFinished );
  auto processingComment = new TextProcessingState( notFinished );
  auto processingValue = new TextProcessingState( notFinished );
  auto maybeProcessingValue = new TextProcessingState( notFinished );
  auto processingEndTag = new TextProcessingState( notFinished );
  auto processingEndComment = new TextProcessingState( notFinished );
  auto processingEndValue = new TextProcessingState( notFinished );
  TextProcessingState *processingPostTemplateSyntax;

  if ( type == Lexer::SmartTrim )
    processingPostTemplateSyntax = new TextProcessingState( notFinished );
  else
    processingPostTemplateSyntax = new FinalizeTokenState( lexer, notFinished );
  auto processingPostTemplateSyntaxWhitespace = new TextProcessingState( notFinished );

  if ( type == Lexer::SmartTrim )
    notFinished->setInitialState( processingPostNewline );
  else
    notFinished->setInitialState( processingText );

  if ( type == Lexer::SmartTrim ) {
    addTransition<NewlineHandler>(               processingText, lexer, processingPostNewline );

    addTransition<NewlineHandler>(               processingPostNewline, lexer, processingPostNewline );
    addTransition<MaybeTemplateSyntaxHandler>(   processingPostNewline, lexer, processingBeginTemplateSyntax );
    addTransition<NonWhitespaceLineTextHandler>( processingPostNewline, lexer, processingText );
  }
  addTransition<MaybeTemplateSyntaxHandler>( processingText, lexer, processingBeginTemplateSyntax );

  addTransition<TagStartHandler>(                    processingBeginTemplateSyntax, lexer, processingTag );
  addTransition<CommentStartHandler>(                processingBeginTemplateSyntax, lexer, processingComment );
  addTransition<BeginValueHandler>(             processingBeginTemplateSyntax, lexer, maybeProcessingValue );

  if ( type == Lexer::SmartTrim ) {
    addTransition<NotBeginTemplateSyntaxOrNewlineHandler>( processingBeginTemplateSyntax, lexer, processingText );
    addTransition<NewlineHandler>(                processingBeginTemplateSyntax, lexer, processingPostNewline );
  } else {
    addTransition<NotBeginTemplateSyntaxHandler>( processingBeginTemplateSyntax, lexer, processingText );
  }

  addTransition<NewlineHandler>( processingTag, lexer, type == Lexer::SmartTrim ? processingPostNewline : processingText );
  addTransition<TagEndHandler>(     processingTag, lexer, processingEndTag );

  addTransition<NewlineHandler>( processingComment, lexer, type == Lexer::SmartTrim ? processingPostNewline : processingText );
  addTransition<CommentEndHandler>( processingComment, lexer, processingEndComment );

  addTransition<TagStartHandler>(                    maybeProcessingValue, lexer, processingTag );
  addTransition<CommentStartHandler>(                maybeProcessingValue, lexer, processingComment );
  addTransition<NotTagCommentOrNewlineHandler>(       maybeProcessingValue, lexer, processingValue );
  addTransition<NewlineHandler>(       maybeProcessingValue, lexer, type == Lexer::SmartTrim ? processingPostNewline : processingText );

  addTransition<NewlineHandler>(       processingValue, lexer, type == Lexer::SmartTrim ? processingPostNewline : processingText );
  addTransition<MaybeEndValueHandler>( processingValue, lexer, processingEndValue );

  addTransition<NewlineHandler>(              processingEndTag, lexer, processingPostNewline );
  addTransition<NotEndTemplateSyntaxHandler>( processingEndTag, lexer, processingTag );
  addTransition<EndTemplateSyntaxHandler>(    processingEndTag, lexer, processingPostTemplateSyntax );

  addTransition<NewlineHandler>(              processingEndComment, lexer, processingPostNewline );
  addTransition<NotEndTemplateSyntaxHandler>( processingEndComment, lexer, processingComment );
  addTransition<EndTemplateSyntaxHandler>(    processingEndComment, lexer, processingPostTemplateSyntax );

  addTransition<NewlineHandler>(              processingEndValue, lexer, processingPostNewline );
  addTransition<NotEndTemplateSyntaxHandler>( processingEndValue, lexer, processingValue );
  addTransition<EndTemplateSyntaxHandler>(    processingEndValue, lexer, processingPostTemplateSyntax );

  if ( type != Lexer::SmartTrim ) {
    processingPostTemplateSyntax->setUnconditionalTransition( processingText );
  } else {
    addTransition<SyntaxBoundaryNewlineHandler>( processingPostTemplateSyntax, lexer, processingPostNewline );
    addTransition<WhitespaceNonNewlineHandler>(  processingPostTemplateSyntax, lexer, processingPostTemplateSyntaxWhitespace );
    addTransition<FinalizingLineTextHandler>(    processingPostTemplateSyntax, lexer, processingText );
    addTransition<SyntaxBoundaryHandler>(        processingPostTemplateSyntax, lexer, processingBeginTemplateSyntax );

    // NOTE: We only have to transition to this if there was whitespace before the opening tag.
    // Maybe store that in an external state property?
    // Actually, this may be a bug if we try to finalize with trimming and there is no leading whitespace.
    addTransition<SyntaxBoundaryNewlineHandler>( processingPostTemplateSyntaxWhitespace, lexer, processingPostNewline );
    addTransition<FinalizingLineTextHandler>(    processingPostTemplateSyntaxWhitespace, lexer, processingText );
    addTransition<SyntaxBoundaryHandler>(        processingPostTemplateSyntaxWhitespace, lexer, processingBeginTemplateSyntax );
  }

  {
    auto handler = new EofHandler( lexer, notFinished );
    handler->setTargetState( finished );
    notFinished->setEndTransition( handler );
  }

  if ( type == Lexer::SmartTrim ) {
    {
      auto handler = new EofHandlerWithTrimming( lexer, processingPostTemplateSyntaxWhitespace );
      handler->setTargetState( finished );
      processingPostTemplateSyntaxWhitespace->setEndTransition( handler );
    }
    {
      auto handler = new EofHandlerWithTrimming( lexer, processingPostTemplateSyntax );
      handler->setTargetState( finished );
      processingPostTemplateSyntax->setEndTransition( handler );
    }
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
  m_newlinePosition = -1;
}

void Lexer::reset()
{
  m_tokenList.clear();
  m_lineCount = 0;
  m_upto = 0;
  m_processedUpto = 0;
  clearMarkers();
}

QList<Token> Lexer::tokenize(TrimType type)
{
  auto machine = createMachine( this, type );

  machine->start();

  auto it = m_templateString.constBegin();
  const auto end = m_templateString.constEnd();

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

void Lexer::markNewline()
{
  m_newlinePosition = m_upto;
  ++m_lineCount;
}

void Lexer::finalizeToken()
{
  auto nextPosition = m_upto;
  const auto validSyntax = m_endSyntaxPosition > m_startSyntaxPosition && ( m_startSyntaxPosition >= m_processedUpto );

  if ( validSyntax ) {
    Q_ASSERT( m_startSyntaxPosition >= 0 );
    nextPosition = m_startSyntaxPosition - 1;
  }
  finalizeToken( nextPosition, validSyntax );
}

void Lexer::finalizeTokenWithTrimmedWhitespace()
{
  auto nextPosition = m_upto;
  // We know this to be true because the state machine has already guaranteed
  // it. This method is only called from transition and state actions which
  // occur after valid syntax.
  // TODO Investigate performance and other implications of changing the state
  // machine to assure similar in finalizeToken()
  Q_ASSERT( m_endSyntaxPosition > m_startSyntaxPosition );

  Q_ASSERT( m_startSyntaxPosition >= 0 );
  if ( m_newlinePosition >= 0 && m_newlinePosition >= m_processedUpto )
    nextPosition = qMin( m_startSyntaxPosition - 1, m_newlinePosition );
  else
    nextPosition = m_startSyntaxPosition - 1;
  finalizeToken( nextPosition, true );
}

void Lexer::finalizeToken( int nextPosition, bool processSyntax )
{
  {
    Token token;
    token.content = m_templateString.mid( m_processedUpto, nextPosition - m_processedUpto );
    token.tokenType = TextToken;
    token.linenumber = m_lineCount;
    m_tokenList.append( token );
  }

  m_processedUpto = nextPosition;

  if ( !processSyntax )
    return;

  m_processedUpto = m_endSyntaxPosition;

  const auto differentiator = *( m_templateString.constData() + m_startSyntaxPosition );
  if ( differentiator == QLatin1Char( '#' ) )
    return;

  Token syntaxToken;
  syntaxToken.content = m_templateString.mid( m_startSyntaxPosition + 1, m_endSyntaxPosition - m_startSyntaxPosition - 3 ).trimmed();
  syntaxToken.linenumber = m_lineCount;

  if ( differentiator == QLatin1Char( '{' ) ) {
    syntaxToken.tokenType = VariableToken;
  } else {
    Q_ASSERT( differentiator == QLatin1Char( '%' ) );
    syntaxToken.tokenType = BlockToken;
  }
  m_tokenList.append( syntaxToken );
}
