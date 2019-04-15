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

#include "datetime.h"

#include "util.h"

#include <QtCore/QDateTime>

QVariant timeSince(const QDateTime &early, const QDateTime &late)
{
  Q_ASSERT(early.isValid());
  Q_ASSERT(late.isValid());

  auto secsSince = early.secsTo(late);

  if (secsSince < 0)
    return SafeString(QStringLiteral("0 minutes"));

  // TODO: i18n
  QStringList singularNames;
  singularNames << QStringLiteral("year") << QStringLiteral("month")
                << QStringLiteral("week") << QStringLiteral("day")
                << QStringLiteral("hour") << QStringLiteral("minute");

  QStringList pluralNames;
  pluralNames << QStringLiteral("years") << QStringLiteral("months")
              << QStringLiteral("weeks") << QStringLiteral("days")
              << QStringLiteral("hours") << QStringLiteral("minutes");

  QList<int> seconds;
  seconds << (60 * 60 * 24 * 365) // year
          << (60 * 60 * 24 * 30)  // month
          << (60 * 60 * 24 * 7)   // week
          << (60 * 60 * 24)       // day
          << (60 * 60)            // hour
          << (60);                // minute

  auto count = secsSince;
  auto i = 0;
  while (i < seconds.size()) {
    count = (secsSince / seconds.at(i));
    ++i;
    if (count != 0)
      break;
  }
  QString firstChunk;

  if (count != 1)
    firstChunk.append(
        QStringLiteral("%1 %2").arg(count).arg(pluralNames.at(i - 1)));
  else {
    firstChunk.append(
        QStringLiteral("%1 %2").arg(count).arg(singularNames.at(i - 1)));
  }
  if (seconds.size() > i) {
    auto count2 = (secsSince - (seconds.at(i - 1) * count)) / seconds.at(i);
    if (count2 != 0) {
      if (count2 > 1)
        firstChunk.append(
            QStringLiteral(", %1 %2").arg(count2).arg(pluralNames.at(i)));
      else
        firstChunk.append(
            QStringLiteral(", %1 %2").arg(count2).arg(singularNames.at(i)));
    }
  }
  return firstChunk;
}

QVariant DateFilter::doFilter(const QVariant &input, const QVariant &argument,
                              bool autoescape) const
{
  Q_UNUSED(autoescape)
  auto d = QDateTime::fromString(getSafeString(input),
                                 QStringLiteral("yyyy-MM-ddThh:mm:ss.zzz"));

  auto argString = getSafeString(argument);

  if (!argString.get().isEmpty())
    return d.toString(argString);

  return d.toString(QStringLiteral("MMM. d, yyyy"));
}

QVariant TimeFilter::doFilter(const QVariant &input, const QVariant &argument,
                              bool autoescape) const
{
  Q_UNUSED(autoescape)
  auto argString = getSafeString(argument);
  return QDateTime::fromString(getSafeString(input),
                               QStringLiteral("yyyy-MM-ddThh:mm:ss"))
      .toString(argString);
}

QVariant TimeSinceFilter::doFilter(const QVariant &input,
                                   const QVariant &argument,
                                   bool autoescape) const
{
  Q_UNUSED(autoescape)
  QDateTime late;
  if (argument.userType() != qMetaTypeId<QDateTime>())
    late = QDateTime::currentDateTime();
  else
    late = argument.value<QDateTime>();

  auto early = input.value<QDateTime>();
  if (!early.isValid())
    return QVariant();
  return timeSince(early, late);
}

QVariant TimeUntilFilter::doFilter(const QVariant &input,
                                   const QVariant &argument,
                                   bool autoescape) const
{
  Q_UNUSED(autoescape)
  QDateTime early;
  if (argument.userType() != qMetaTypeId<QDateTime>())
    early = QDateTime::currentDateTime();
  else
    early = argument.value<QDateTime>();

  auto late = input.value<QDateTime>();
  if (!late.isValid())
    return QVariant();
  return timeSince(early, late);
}
