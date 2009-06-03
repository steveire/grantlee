/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "variable.h"

#include <QStringList>
#include <QMetaEnum>

#include "context.h"
#include "util_p.h"

using namespace Grantlee;

namespace Grantlee
{

class VariablePrivate
{
public:
  VariablePrivate(Variable *variable)
    : q_ptr(variable)
  {

  }

  QVariant resolvePart( const QVariant &variant, const QString &s ) const;

  Error m_error;
  QString m_errorString;

  QString m_varString;
  QVariant m_literal;
  QStringList m_lookups;
  bool m_translate;

  Q_DECLARE_PUBLIC(Variable)
  Variable *q_ptr;
};

}

Variable::Variable(const Variable &other)
  : d_ptr(new VariablePrivate(this))
{
  d_ptr->m_varString = other.d_ptr->m_varString;
  d_ptr->m_literal = other.d_ptr->m_literal;
  d_ptr->m_lookups = other.d_ptr->m_lookups;
  d_ptr->m_translate = other.d_ptr->m_translate;
}

Variable::Variable()
  : d_ptr(new VariablePrivate(this))
{
}

Variable::~Variable()
{
  delete d_ptr;
}

Variable &Variable::operator=(const Variable &other)
{
  d_ptr->m_varString = other.d_ptr->m_varString;
  d_ptr->m_literal = other.d_ptr->m_literal;
  d_ptr->m_lookups = other.d_ptr->m_lookups;
  d_ptr->m_translate = other.d_ptr->m_translate;
  return *this;
}

Variable::Variable(const QString &var)
  : d_ptr(new VariablePrivate(this))
{
  Q_D(Variable);
  d->m_varString = var;

  QVariant v(var);
  if (v.convert(QVariant::Double))
  {
    d->m_literal = v;
    if (!var.contains(".") && !var.contains("e"))
    {
      if (var.endsWith("."))
      {
        // error
      }

      d->m_literal = v.toInt();
    }
  } else {
    QString localVar = var;
    if (var.startsWith("_(") && var.endsWith(")"))
    {
      d->m_translate = true;
      localVar = var.mid(2, var.size() - 3 );
    }
    if ( ( localVar.startsWith( "\"" ) && localVar.endsWith( "\"" ) )
      || ( localVar.startsWith( "'" ) && localVar.endsWith( "'" ) ) )
    {
      d->m_literal = Util::unescapeStringLiteral(localVar);
    } else {
      d->m_lookups = localVar.split(".");
    }
  }
}

bool Variable::isConstant() const
{
  Q_D(const Variable);
  return !d->m_literal.isNull();
}

QString Variable::toString() const
{
  Q_D(const Variable);
  return d->m_varString;
}

bool Variable::isTrue(Context *c) const
{
  return Util::variantIsTrue(resolve(c));
}

QVariant Variable::resolve(Context *c) const
{
  Q_D(const Variable);
  QVariant var;
  if (!d->m_lookups.isEmpty())
  {
    int i = 0;
    var = c->lookup(d->m_lookups.at(i++));
    while (i < d->m_lookups.size())
    {
      var = d->resolvePart(var, d->m_lookups.at(i++));
      if (!var.isValid())
        return var;
    }
  } else {
    var = d->m_literal;
  }


  if (d->m_translate)
  {
//     return gettext(var.toString());
  }
  return var;
}

void Variable::setError(Error type, const QString &message)
{
  Q_D(Variable);
  d->m_error = type;
  d->m_errorString = message;
}

Error Variable::error() const
{
  Q_D(const Variable);
  return d->m_error;
}

QString Variable::errorString() const
{
  Q_D(const Variable);
  return d->m_errorString;
}

QVariant VariablePrivate::resolvePart( const QVariant &var, const QString &nextPart ) const
{
  QVariant returnVar;

//   Should be
// * QVariantMap key lookup
// * Property? (member in django)
// * list index

  if ( QVariant::Map == var.type() )
  {
    QVariantMap map = var.toMap();
    return map.value( nextPart );
  }
  else if ( QMetaType::QObjectStar == var.userType() )
  {
    // Can't be const because of invokeMethod.
    const QObject *obj = var.value<QObject *>();
    const QMetaObject *metaObj = obj->metaObject();

    QMetaProperty mp;
    for (int i = 0; i < metaObj->propertyCount(); ++i)
    {
      // TODO only read-only properties should be allowed here.
      // This might also handle the variant messing I hit before.
      mp = metaObj->property(i);

      if (QString(mp.name()) != nextPart)
        continue;

      return mp.read(obj);

    }
    return QVariant();
  } else {
    // List index test

    bool ok = false;
    int listIndex = nextPart.toInt(&ok);
    if (!ok)
      return QVariant();
    QVariantList varList = Util::variantToList(var);

    if (listIndex >= varList.size())
      return QVariant();
    return varList.at(listIndex);
  }

  return QVariant();
}
