/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_EXPORT_H
#define GRANTLEE_EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
#  ifndef GRANTLEE_EXPORT
#    if defined(GRANTLEE_CORE_LIB_MAKEDLL)
#      define GRANTLEE_EXPORT __declspec(dllexport)
#    else
#      define GRANTLEE_EXPORT __declspec(dllimport)
#    endif
#  endif
#else
#  define GRANTLEE_EXPORT __attribute__((visibility("default")))
#endif

#endif

