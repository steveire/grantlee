/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#include <QtCore/QtPlugin>

#ifndef __COVERAGESCANNER__
#define GRANTLEE_EXPORT_PLUGIN(PLUGIN, PLUGINCLASS)                 \
  Q_EXPORT_PLUGIN2(PLUGIN, PLUGINCLASS)                             \

#else
#define GRANTLEE_EXPORT_PLUGIN(PLUGIN, PLUGINCLASS)                 \
  Q_EXPORT_PLUGIN2(PLUGIN, PLUGINCLASS)                             \
  static void plugin_load(void)   __attribute__ ((constructor));    \
  static void plugin_unload(void) __attribute__ ((destructor));     \
                                                                    \
  static void plugin_load(void)                                     \
  {                                                                 \
    __coveragescanner_filename(#PLUGIN);                            \
  }                                                                 \
                                                                    \
  static void plugin_unload(void)                                   \
  {                                                                 \
    __coveragescanner_save();                                       \
  }                                                                 \

#endif

