/*
  This file is part of the Grantlee template system.

  Copyright (c) 2017 Matthias Fehring <kontakt@buschmann23.de>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef L10N_FILESIZE_H
#define L10N_FILESIZE_H

#include "node.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class L10nFileSizeNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  L10nFileSizeNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;
};

class L10nFileSizeVarNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  L10nFileSizeVarNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;
};

class L10nFileSizeNode : public Node
{
  Q_OBJECT
public:
  L10nFileSizeNode(const Grantlee::FilterExpression &size,
                   const Grantlee::FilterExpression &unitSystem,
                   const Grantlee::FilterExpression &precision,
                   const Grantlee::FilterExpression &multiplier,
                   QObject *parent = nullptr);

  void render(OutputStream *stream, Context *c) const override;

private:
  Grantlee::FilterExpression m_size;
  Grantlee::FilterExpression m_unitSystem;
  Grantlee::FilterExpression m_precision;
  Grantlee::FilterExpression m_multiplier;
};

class L10nFileSizeVarNode : public Node
{
  Q_OBJECT
public:
  L10nFileSizeVarNode(const Grantlee::FilterExpression &size,
                      const Grantlee::FilterExpression &unitSystem,
                      const Grantlee::FilterExpression &precision,
                      const Grantlee::FilterExpression &multiplier,
                      const QString &resultName,
                      QObject *parent = nullptr);

  void render(OutputStream *stream, Context *c) const override;

private:
  Grantlee::FilterExpression m_size;
  Grantlee::FilterExpression m_unitSystem;
  Grantlee::FilterExpression m_precision;
  Grantlee::FilterExpression m_multiplier;
  QString m_resultName;
};

#endif
