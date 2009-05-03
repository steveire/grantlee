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
  m_varString = var.trimmed();
}

QVariant Variable::resolve(Context *c)
{
  QStringList list = m_varString.split(".");
  QString varName = list.takeFirst();
  QVariant var = c->lookup(varName);
  if (!var.isValid())
    return QVariant();
  foreach (const QString &part, list)
  {
    var = resolvePart(var, part);
    if (!var.isValid())
      return var;
  }
  return var;
}

QVariant Variable::resolvePart( QVariant var, const QString &nextPart )
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
