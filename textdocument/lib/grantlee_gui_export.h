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

#ifndef GRANTLEE_GUI_EXPORT_H
#define GRANTLEE_GUI_EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
#  ifndef GRANTLEE_GUI_EXPORT
#    if defined(GRANTLEE_GUI_LIB_MAKEDLL)
#      define GRANTLEE_GUI_EXPORT __declspec(dllexport)
#    else
#      define GRANTLEE_GUI_EXPORT __declspec(dllimport)
#    endif
#  endif
#else
#  define GRANTLEE_GUI_EXPORT __attribute__((visibility("default")))
#endif

#endif

