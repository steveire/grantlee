/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QHash>
#include <QVariant>
#include <QStringList>

#include "grantlee_export.h"

namespace Grantlee
{

class GRANTLEE_EXPORT Context //: public QObject
{
  //Q_OBJECT

public:

  enum Error
  {
    None,
    VariableStringInvalid,
    VariableNotInContext,
    ObjectReturnTypeInvalid
  };

  /**
  Sets every key in the hash as a property name with the variant as the value.
  */
  Context(QHash<QString, QVariant> hash);

  ~Context();

  /**
  Returns a QVariant, even in the case of it containing a list which can be iterated over.
  Have to think about whether QMap should work too, or would its keys already be resolved.
  */
  QVariant lookup(const QString &str) const;
  void insert(const QString &name, QVariant);
  void push();
  void pop();

private:
  QList<QHash<QString, QVariant> > m_variantHashStack;

  QStringList m_templateSearchDirs;

};

}

#endif
