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
void addTransition( TextProcessingState *source, Lexer*lexer, TextProcessingState *target )
{
  Transition *tr = new Transition( lexer, source );
  tr->setTargetState( target );
}

TextProcessingMachine* createMachine( Lexer *lexer, bool trim = false )
{
  TextProcessingMachine *machine = new TextProcessingMachine;

  TextProcessingState *notFinished = new TextProcessingState( machine );
  TextProcessingState *finished = new TextProcessingState( machine );
  machine->setInitialState( notFinished );

  TextProcessingState *processingText = new ChurningState( lexer, notFinished );
  TextProcessingState *processingPostNewline = new TextProcessingState( notFinished );
  TextProcessingState *processingBeginTemplateSyntax = new TextProcessingState( notFinished );
  TextProcessingState *processingTag = new TextProcessingState( notFinished );
  TextProcessingState *processingComment = new TextProcessingState( notFinished );
  TextProcessingState *processingValue = new TextProcessingState( notFinished );
  TextProcessingState *maybeProcessingValue = new TextProcessingState( notFinished );
  TextProcessingState *processingEndTag = new TextProcessingState( notFinished );
  TextProcessingState *processingEndComment = new TextProcessingState( notFinished );
  TextProcessingState *processingEndValue = new TextProcessingState( notFinished );
  TextProcessingState *processingPostTemplateSyntax;
  if ( trim )
    processingPostTemplateSyntax = new TextProcessingState( notFinished );
  else
    processingPostTemplateSyntax = new FinalizeTokenState( lexer, notFinished );
  TextProcessingState *processingPostTemplateSyntaxWhitespace = new TextProcessingState( notFinished );

  if ( trim )
    notFinished->setInitialState( processingPostNewline );
  else
    notFinished->setInitialState( processingText );

  if ( trim ) {
    addTransition<NewlineHandler>(               processingText, lexer, processingPostNewline );

    addTransition<NewlineHandler>(               processingPostNewline, lexer, processingPostNewline );
    addTransition<MaybeTemplateSyntaxHandler>(   processingPostNewline, lexer, processingBeginTemplateSyntax );
    addTransition<NonWhitespaceLineTextHandler>( processingPostNewline, lexer, processingText );
  }
  addTransition<MaybeTemplateSyntaxHandler>( processingText, lexer, processingBeginTemplateSyntax );

  addTransition<TagStartHandler>(                    processingBeginTemplateSyntax, lexer, processingTag );
  addTransition<CommentStartHandler>(                processingBeginTemplateSyntax, lexer, processingComment );
  addTransition<BeginValueHandler>(             processingBeginTemplateSyntax, lexer, maybeProcessingValue );

  if ( trim ) {
    addTransition<NotBeginTemplateSyntaxOrNewlineHandler>( processingBeginTemplateSyntax, lexer, processingText );
    addTransition<NewlineHandler>(                processingBeginTemplateSyntax, lexer, processingPostNewline );
  } else {
    addTransition<NotBeginTemplateSyntaxHandler>( processingBeginTemplateSyntax, lexer, processingText );
  }

  addTransition<NewlineHandler>( processingTag, lexer, trim ? processingPostNewline : processingText );
  addTransition<TagEndHandler>(     processingTag, lexer, processingEndTag );

  addTransition<NewlineHandler>( processingComment, lexer, trim ? processingPostNewline : processingText );
  addTransition<CommentEndHandler>( processingComment, lexer, processingEndComment );

  addTransition<TagStartHandler>(                    maybeProcessingValue, lexer, processingTag );
  addTransition<CommentStartHandler>(                maybeProcessingValue, lexer, processingComment );
  addTransition<NotTagCommentOrNewlineHandler>(       maybeProcessingValue, lexer, processingValue );
  addTransition<NewlineHandler>(       maybeProcessingValue, lexer, trim ? processingPostNewline : processingText );

  addTransition<NewlineHandler>(       processingValue, lexer, trim ? processingPostNewline : processingText );
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

  if (!trim) {
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
    EofHandler *handler = new EofHandler( lexer, notFinished );
    handler->setTargetState( finished );
    notFinished->setEndTransition( handler );
  }

  if ( trim ) {
    {
      EofHandlerWithTrimming *handler = new EofHandlerWithTrimming( lexer, processingPostTemplateSyntaxWhitespace );
      handler->setTargetState( finished );
      processingPostTemplateSyntaxWhitespace->setEndTransition( handler );
    }
    {
      EofHandlerWithTrimming *handler = new EofHandlerWithTrimming( lexer, processingPostTemplateSyntax );
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

void Lexer::markNewline()
{
  m_newlinePosition = m_upto;
  ++m_lineCount;
}

void Lexer::finalizeToken()
{
  int nextPosition = m_upto;
  const bool validSyntax = m_endSyntaxPosition > m_startSyntaxPosition && ( m_startSyntaxPosition >= m_processedUpto );

  if ( validSyntax && m_startSyntaxPosition >= 0 ) {
    nextPosition = m_startSyntaxPosition - 1;
  }
  finalizeToken( nextPosition, validSyntax );
}

void Lexer::finalizeTokenWithTrimmedWhitespace()
{
  int nextPosition = m_upto;
  const bool validSyntax = m_endSyntaxPosition > m_startSyntaxPosition;
  if ( validSyntax && m_startSyntaxPosition > 0 ) {
    if ( m_newlinePosition >= 0 && m_newlinePosition >= m_processedUpto )
      nextPosition = qMin( m_startSyntaxPosition - 1, m_newlinePosition );
    else
      nextPosition = m_startSyntaxPosition - 1;
  }
  finalizeToken( nextPosition, validSyntax );
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

  const QChar differentiator = *( m_templateString.constData() + m_startSyntaxPosition );
  if ( differentiator == QLatin1Char( '#' ) )
    return;

  Token syntaxToken;
  syntaxToken.content = m_templateString.mid( m_startSyntaxPosition + 1, m_endSyntaxPosition - m_startSyntaxPosition - 3 ).trimmed();
  syntaxToken.linenumber = m_lineCount;

  if ( differentiator == QLatin1Char( '{' ) ) {
    syntaxToken.tokenType = VariableToken;
  } else if ( differentiator == QLatin1Char( '%' ) ) {
    syntaxToken.tokenType = BlockToken;
  }
  m_tokenList.append( syntaxToken );
}
