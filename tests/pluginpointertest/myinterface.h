
#ifndef MYINTERFACE_H
#define MYINTERFACE_H

class MyInterface
{
public:
  virtual ~MyInterface() {}

  virtual int double_it( int input ) = 0;
};

Q_DECLARE_INTERFACE( MyInterface, "org.example.MyInterface/1.0" )

#endif
