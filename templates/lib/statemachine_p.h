/*
  This file is part of the Grantlee template system.

  Copyright (c) 2011 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_STATEMACHINE_P_H
#define GRANTLEE_STATEMACHINE_P_H

#include <QtCore/QString>
#include <QtCore/QVector>

namespace Grantlee
{

template <typename TransitionInterface> class State
{
public:
  typedef TransitionInterface Type;
  class Transition : public TransitionInterface
  {
  public:
    typedef TransitionInterface Type;
    explicit Transition(State<TransitionInterface> *parent = {})
    {
      if (parent)
        parent->addTransition(this);
    }
    void setTargetState(State<TransitionInterface> *state)
    {
      m_targetState = state;
    }
    State<TransitionInterface> *targetState() const { return m_targetState; }

  private:
    State<TransitionInterface> *m_targetState;
  };

  explicit State(State<TransitionInterface> *parent = {})
      : m_initialState(0), m_parent(parent), m_endTransition(0),
        m_unconditionalTransition(0)
  {
    if (parent)
      parent->addChild(this);
  }

  virtual ~State()
  {
    qDeleteAll(m_transitions);
    qDeleteAll(m_children);
  }

  void addChild(State<TransitionInterface> *state) { m_children.append(state); }
  QVector<State<TransitionInterface> *> children() const { return m_children; }
  State<TransitionInterface> *parent() const { return m_parent; }

  void setInitialState(State<TransitionInterface> *state)
  {
    m_initialState = state;
  }
  State<TransitionInterface> *initialState() { return m_initialState; }

  void addTransition(Transition *transition)
  {
    m_transitions.append(transition);
  }
  QVector<Transition *> transitions() { return m_transitions; }

  void setEndTransition(Transition *transition)
  {
    delete m_endTransition;
    m_endTransition = transition;
  }
  Transition *endTransition() const { return m_endTransition; }

  void setUnconditionalTransition(State<TransitionInterface> *transition)
  {
    delete m_unconditionalTransition;
    m_unconditionalTransition = transition;
  }
  State<TransitionInterface> *unconditionalTransition() const
  {
    return m_unconditionalTransition;
  }

  void enter() { onEntry(); }
  void exit() { onExit(); }

protected:
  virtual void onEntry() {}
  virtual void onExit() {}

private:
  State<TransitionInterface> *m_initialState;
  QVector<Transition *> m_transitions;
  QVector<State<TransitionInterface> *> m_children;
  State<TransitionInterface> *const m_parent;
  Transition *m_endTransition;
  State<TransitionInterface> *m_unconditionalTransition;
  QString m_stateName;
};

template <typename TransitionInterface>
class StateMachine : public State<TransitionInterface>
{
public:
  typedef typename State<TransitionInterface>::Transition Transition;

  explicit StateMachine(State<TransitionInterface> *parent = {})
      : State<TransitionInterface>(parent), m_currentState(0)
  {
  }

  void finished()
  {
    State<TransitionInterface> *s = m_currentState;
    Q_FOREVER
    {
      Q_ASSERT(s);
      if (!handleFinished(s))
        s = s->parent();
      else
        break;
    }
  }

  void start()
  {
    m_currentState = this->initialState();
    Q_ASSERT(m_currentState);
    performEnter(m_currentState);
  }

  void stop()
  {
    performExit(this);
    m_currentState = 0;
  }

protected:
  State<TransitionInterface> *currentState() const { return m_currentState; }

  void executeTransition(State<TransitionInterface> *sourceState,
                         Transition *transition)
  {
    performExit(sourceState);
    transition->onTransition();
    m_currentState = transition->targetState();
    State<TransitionInterface> *enteredState = m_currentState;
    Q_ASSERT(enteredState);
    performEnter(enteredState);
    triggerUnconditionalTransition(enteredState);
  }

private:
  void performEnter(State<TransitionInterface> *toState)
  {
    toState->enter();
    Q_FOREVER if (toState->initialState())
    {
      toState = toState->initialState();
      Q_ASSERT(toState);
      toState->enter();
      m_currentState = toState;
    }
    else
    {
      Q_ASSERT(toState->children().isEmpty());
      break;
    }
  }

  void performExit(State<TransitionInterface> *fromState)
  {
    State<TransitionInterface> *exitedState = m_currentState;
    do {
      exitedState->exit();
      exitedState = exitedState->parent();
    } while (exitedState && exitedState != fromState);
  }

  void triggerUnconditionalTransition(State<TransitionInterface> *enteredState)
  {
    State<TransitionInterface> *childState = m_currentState;
    do {
      Q_ASSERT(childState);

      if (childState->unconditionalTransition()) {
        Transition *t = new Transition;
        t->setTargetState(childState->unconditionalTransition());
        executeTransition(childState, t);
        delete t;
        return;
      }
      childState = childState->parent();
      Q_ASSERT(childState != enteredState);
      Q_UNUSED(enteredState);
    } while (childState);
  }

  bool handleFinished(State<TransitionInterface> *state)
  {
    Transition *handler = state->endTransition();
    if (!handler)
      return false;

    executeTransition(state, handler);
    return true;
  }

private:
  State<TransitionInterface> *m_currentState;
};
}

#endif
