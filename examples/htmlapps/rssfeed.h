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

#ifndef RSS_FEED_H
#define RSS_FEED_H

#include <grantlee/filter.h>
#include <grantlee/node.h>

namespace Grantlee
{
class Parser;
class OutputStream;
class Context;
}

class RssFeedNodeFactory : public Grantlee::AbstractNodeFactory
{
  Q_OBJECT
public:
  RssFeedNodeFactory(QObject *parent = 0);

  virtual Grantlee::Node *getNode(const QString &tagContent,
                                  Grantlee::Parser *p) const;
};

class RssFeedNode : public Grantlee::Node
{
  Q_OBJECT
public:
  RssFeedNode(const Grantlee::FilterExpression &url,
              const Grantlee::FilterExpression &query, QObject *parent = 0);

  void setChildNodes(QList<Node *> childNodes);

  virtual void render(Grantlee::OutputStream *stream,
                      Grantlee::Context *c) const;

private:
  Grantlee::FilterExpression m_url;
  Grantlee::FilterExpression m_query;
  QList<Node *> m_childNodes;
};

class XmlRoleNodeFactory : public Grantlee::AbstractNodeFactory
{
  Q_OBJECT
public:
  XmlRoleNodeFactory(QObject *parent = 0);

  virtual Grantlee::Node *getNode(const QString &tagContent,
                                  Grantlee::Parser *p) const;
};

class XmlRoleNode : public Grantlee::Node
{
  Q_OBJECT
public:
  XmlRoleNode(const Grantlee::FilterExpression &query, QObject *parent = 0);

  virtual void render(Grantlee::OutputStream *stream,
                      Grantlee::Context *c) const;

private:
  Grantlee::FilterExpression m_name;
  Grantlee::FilterExpression m_query;
  int m_count;
};

class XmlNamespaceNodeFactory : public Grantlee::AbstractNodeFactory
{
  Q_OBJECT
public:
  XmlNamespaceNodeFactory(QObject *parent = 0);

  virtual Grantlee::Node *getNode(const QString &tagContent,
                                  Grantlee::Parser *p) const;
};

class XmlNamespaceNode : public Grantlee::Node
{
  Q_OBJECT
public:
  XmlNamespaceNode(const Grantlee::FilterExpression &query, const QString &name,
                   QObject *parent = 0);

  virtual void render(Grantlee::OutputStream *stream,
                      Grantlee::Context *c) const;

private:
  QString m_name;
  Grantlee::FilterExpression m_query;
};

class ResizeFilter : public Grantlee::Filter
{
public:
  QVariant doFilter(const QVariant &input,
                    const QVariant &argument = QVariant(),
                    bool autoescape = false) const;
};

#endif
