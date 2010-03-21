
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QtCore/QObject>
#include "myinterface.h"

class MyObject : public QObject, public MyInterface
{
  Q_OBJECT
  Q_INTERFACES( MyInterface )
public:
  MyObject( int stop = 4, QObject *parent = 0 );

  int double_it( int input );

private:
  QByteArray m_data;
};

#endif

