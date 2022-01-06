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

#include "filter.h"

using namespace Grantlee;

Filter::~Filter() = default;

void Filter::setStream(Grantlee::OutputStream *stream) { m_stream = stream; }

SafeString Filter::escape(const QString &input) const
{
  return m_stream->escape(input);
}

SafeString Filter::escape(const SafeString &input) const
{
  if (input.isSafe())
    return {m_stream->escape(input), SafeString::IsSafe};
  return m_stream->escape(input);
}

SafeString Filter::conditionalEscape(const SafeString &input) const
{
  if (!input.isSafe())
    return m_stream->escape(input);
  return input;
}

bool Filter::isSafe() const { return false; }
