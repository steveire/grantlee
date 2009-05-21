/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "variable.h"

#include <QVariant>
#include <QStringList>
#include <QMetaEnum>

#include "context.h"

#include <QDebug>

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
      localVar = var.mid(2, var.size() - 1 );
    }
    if ( ( localVar.startsWith( "\"" ) && localVar.endsWith( "\"" ) )
      || ( localVar.startsWith( "'" ) && localVar.endsWith( "'" ) ) )
    {
      d->m_literal = localVar.mid(1, localVar.size() - 2 ).replace("\\'", "'");
    } else {
      d->m_lookups = localVar.split(".");
    }
  }
}

QString Variable::toString() const
{
  Q_D(const Variable);
  return d->m_varString;
}

bool Variable::isTrue(Context *c) const
{
  QVariant variant = resolve(c);

  if (!variant.isValid())
    return false;

  switch (variant.type())
  {
  case QVariant::Bool:
  {
    return variant.toBool();
    break;
  }
  case QVariant::String:
  {
    return !variant.toString().isEmpty();
    break;
  }
  case QVariant::Int:
  {
    return (variant.toInt() > 0);
  }
  default:
    break;
  }

//     if it contains QObject pointer
//     {
//       if pointer is null:
//         return false;
//       return true;
//     }

//     if (it contains number)
//     {
//       //       QString varString = var.toString();
//       if (number == 0)
//         return false;
//       return true;
//     }
//     if (it contains boolean)
//     {
//       return boolean;
//     }
//     if (it contains collection)
//     {
//       return collection.size() > 0;
//     }
//
//     if (it contains string)
//     {
//       QString varString = var.toString();
//       if (varString.isEmpty())
//         return false;
//       return true;
//     }
// etc for QPoint etc.

  return false;

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

QVariant VariablePrivate::resolvePart( const QVariant &var, const QString &nextPart ) const
{
  QVariant returnVar;

//   Should be
// * QVariantMap key lookup
// * Property? (member in django)
// // * method call
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

//     QMetaMethod mm;
//     for (int i = 0; i < metaObj->methodCount(); ++i)
//     {
//       mm = metaObj->method(i);
//
//       if ( mm.parameterNames().size() > 0 )
//         continue;
//
//       // Remove the '()' from the end of the signature. It is already normalized and takes no args.
//       QString methodName(mm.signature());
//       methodName.chop(2);
//
//       if (methodName != nextPart)
//         continue;
//
//       if (mm.access() != QMetaMethod::Public)
//         continue;
//
//       if (mm.methodType() == QMetaMethod::Signal)
//         continue;
//
//       // TODO: It would be much nicer if methods could return any type that QVariant handles.
// //       qDebug() << "erte" << QVariant::metaObject().className();
// // Works outside of invokeMethod.
//   //   MyClass *myc= new MyClass();
//   //   qDebug() << myc->getSomething();
//   //   QVariant v2 = myc->getSomething();
//   //   qDebug() << v2;
//
//       QString varTypeString("QVariant");
//       if ( mm.typeName() != varTypeString )
//         continue;
//
//
// //       qDebug() << (mm.methodType() == QMetaMethod::Slot) << mm.signature() << methodName <<  mm.parameterNames() << mm.parameterTypes() << mm.typeName();
//
//       metaObj->invokeMethod(obj, qPrintable(methodName),
//                             Qt::DirectConnection,
//                             Q_RETURN_ARG(QVariant, returnVar));
//
// //       qDebug() << returnVar;
// //       if (returnVar.isValid() )
//       return returnVar;
//     }
  } else {
    // List index test

    bool ok = false;
    int listIndex = nextPart.toInt(&ok);
    if (!ok)
      return QVariant();
    if ( QVariant::List == var.type() )
    {
      QVariantList varList = var.toList();
      if (listIndex >= varList.size())
        return QVariant();
      return varList.at(listIndex);
    } else if ( QVariant::StringList == var.type() )
    {
      QStringList strList = var.toStringList();
      return strList.at(listIndex);
    } else if ( QVariant::ByteArray == var.type() )
    {
      QByteArray ba = var.toByteArray();
      return ba.at(listIndex);
    } else if ( QVariant::String == var.type() )
    {
      QString str = var.toString();
      return str.at(listIndex);
    }
  }

  return QVariant();
}
