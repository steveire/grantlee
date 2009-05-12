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

Variable::Variable()
{

}

Variable::Variable(const QString &var)
{
  m_varString = var;

  QVariant v(var);
  if (v.convert(QVariant::Double))
  {
    m_literal = v;
    if (!var.contains(".") && !var.contains("e"))
    {
      if (var.endsWith("."))
      {
        // error
      }

      m_literal = v.toInt();
    }
  } else {
    QString localVar = var;
    if (var.startsWith("_(") && var.endsWith(")"))
    {
      m_translate = true;
      localVar = var.mid(2, var.size() - 1 );
    }
    if ( ( localVar.startsWith( "\"" ) && localVar.endsWith( "\"" ) )
      || ( localVar.startsWith( "'" ) && localVar.endsWith( "'" ) ) )
    {
      m_literal = localVar.mid(1, localVar.size() - 2 ).replace("\\'", "'");
    } else {
      m_lookups = localVar.split(".");
    }
  }
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
  QVariant var;
  if (!m_lookups.isEmpty())
  {
    int i = 0;
    var = c->lookup(m_lookups.at(i++));
    while (i < m_lookups.size())
    {
      var = resolvePart(var, m_lookups.at(i++));
      if (!var.isValid())
        return var;
    }
  } else {
    var = m_literal;
  }


  if (m_translate)
  {
//     return gettext(var.toString());
  }
  return var;
}

QVariant Variable::resolvePart( QVariant var, const QString &nextPart ) const
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
