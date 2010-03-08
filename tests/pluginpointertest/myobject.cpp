
#include "myobject.h"

#include <QtPlugin>


MyObject::MyObject( int stop, QObject *parent )
  : QObject( parent )
{
  m_data.fill( 'o', 2000000 );

  if ( stop > 0 )
    ( void ) new MyObject( --stop, this );

}

int MyObject::double_it( int input )
{
  return input * 2;
}

Q_EXPORT_PLUGIN2( myplugin, MyObject )
