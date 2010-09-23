/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef CONTAINERACCESSOR_H
#define CONTAINERACCESSOR_H

#include <QtCore/QVariant>

namespace Grantlee
{

template<typename Container_>
struct Getter
{
  typedef Container_ Container;
};

template<typename Container>
struct KeyGetter : public Getter<Container>
{
  static typename Container::key_type get(const typename Container::const_iterator it) {
    return it.key();
  }
};

template<typename Container>
struct MappedValueGetter : public Getter<Container>
{
  static typename Container::mapped_type get(const typename Container::const_iterator it) {
    return *it;
  }
};

template<typename Container, typename T = typename Container::key_type>
struct Finder
{
  static typename Container::const_iterator find( const Container &container, const QString &nextPart )
  {
    {
      // Compile error if key_type is not a number.
      static const QString s = QString::number( T() );
      Q_UNUSED(s)
    }

    QVariant v = nextPart;
    if (!v.canConvert<typename Container::key_type>() || !v.convert(QVariant::Double))
      return container.end();
    typename Container::key_type key = v.value<typename Container::key_type>();
    return container.find( key );
  }
};

template<typename Container>
struct Finder<Container, QString>
{
  static typename Container::const_iterator find( const Container &container, const QString &nextPart )
  {
    return container.find( nextPart );
  }
};

}

#endif
