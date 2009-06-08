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

// TODO: Figure out the Qt way to do libraries. This is copied from kdemacros.h
#define __HAVE_GCC_VISIBILITY

#ifdef __HAVE_GCC_VISIBILITY
#define KDE_NO_EXPORT __attribute__ ((visibility("hidden")))
#define KDE_EXPORT __attribute__ ((visibility("default")))
#define KDE_IMPORT __attribute__ ((visibility("default")))
#elif defined(_WIN32) || defined(_WIN64)
#define KDE_NO_EXPORT
#define KDE_EXPORT __declspec(dllexport)
#define KDE_IMPORT __declspec(dllimport)
#else
#define KDE_NO_EXPORT
#define KDE_EXPORT
#define KDE_IMPORT
#endif


#ifndef GRANTLEE_EXPORT
# if defined(MAKE_GRANTLEE_LIB)
/* We are building this library */
#  define GRANTLEE_EXPORT KDE_EXPORT
// #  define GRANTLEE_EXPORT Q_DECL_EXPORT
# else
/* We are using this library */
#  define GRANTLEE_EXPORT KDE_IMPORT
// #  define GRANTLEE_EXPORT Q_DECL_IMPORT
# endif
#endif

# ifndef GRANTLEE_EXPORT_DEPRECATED
#  define GRANTLEE_EXPORT_DEPRECATED KDE_DEPRECATED GRANTLEE_EXPORT
# endif

#endif

