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

#include "filterexpression.h"

#include <QtCore/QRegularExpression>

#include "exception.h"
#include "filter.h"
#include "metatype.h"
#include "parser.h"
#include "util.h"

typedef QPair<QSharedPointer<Grantlee::Filter>, Grantlee::Variable> ArgFilter;

namespace Grantlee
{

class FilterExpressionPrivate
{
  FilterExpressionPrivate(FilterExpression *fe) : q_ptr(fe) {}

  Variable m_variable;
  QVector<ArgFilter> m_filters;
  QStringList m_filterNames;

  Q_DECLARE_PUBLIC(FilterExpression)
  FilterExpression *const q_ptr;
};
}

using namespace Grantlee;

static const char FILTER_SEPARATOR = '|';
static const char FILTER_ARGUMENT_SEPARATOR = ':';

static QRegularExpression getFilterRegexp()
{
  const QString filterSep(
      QRegularExpression::escape(QChar::fromLatin1(FILTER_SEPARATOR)));
  const QString argSep(
      QRegularExpression::escape(QChar::fromLatin1(FILTER_ARGUMENT_SEPARATOR)));

  const QLatin1String varChars(
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.");
  const QLatin1String numChars("[-+\\.]?\\d[\\d\\.e]*");
  const QString i18nOpen(QRegularExpression::escape(QStringLiteral("_(")));
  const QLatin1String doubleQuoteStringLiteral(
      "\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\"");
  const QLatin1String singleQuoteStringLiteral(
      "\'[^\'\\\\]*(?:\\\\.[^\'\\\\]*)*\'");
  const QString i18nClose(QRegularExpression::escape(QStringLiteral(")")));
  const QString variable = QLatin1Char('[') + varChars + QStringLiteral("]+");

  const QString localizedExpression
      = QStringLiteral("(?:") + i18nOpen + doubleQuoteStringLiteral + i18nClose
        + QLatin1Char('|') + i18nOpen + singleQuoteStringLiteral + i18nClose
        + QLatin1Char('|') + i18nOpen + numChars + i18nClose + QLatin1Char('|')
        + i18nOpen + variable + i18nClose + QLatin1Char(')');

  const QString constantString = QStringLiteral("(?:")
                                 + doubleQuoteStringLiteral + QLatin1Char('|')
                                 + singleQuoteStringLiteral + QLatin1Char(')');

  const QString filterRawString
      = QLatin1Char('^') + constantString + QLatin1Char('|') + QLatin1Char('^')
        + localizedExpression + QLatin1Char('|') + QLatin1Char('^') + variable
        + QLatin1Char('|') + numChars + QLatin1Char('|') + filterSep
        + QStringLiteral("\\w+|") + argSep + QStringLiteral("(?:")
        + constantString + QLatin1Char('|') + localizedExpression
        + QLatin1Char('|') + variable + QLatin1Char('|') + numChars
        + QLatin1Char('|') + filterSep + QStringLiteral("\\w+)");

  return QRegularExpression(filterRawString);
}

FilterExpression::FilterExpression(const QString &varString, Parser *parser)
    : d_ptr(new FilterExpressionPrivate(this))
{
  Q_D(FilterExpression);

  auto pos = 0;
  auto lastPos = 0;
  int len;
  QString subString;

  auto vs = varString;

  static const auto sFilterRe = getFilterRegexp();

  // This is one fo the few constructors that can throw so we make sure to
  // delete its d->pointer.
  try {
    auto i = sFilterRe.globalMatch(vs);
    while (i.hasNext()) {
      auto match = i.next();
      len = match.capturedLength();
      pos = match.capturedStart();
      subString = match.captured();
      const auto ssSize = subString.size();

      if (pos != lastPos) {
        throw Grantlee::Exception(
            TagSyntaxError,
            QStringLiteral("Could not parse some characters: \"%1\"")
                .arg(vs.mid(lastPos, pos)));
      }

      if (subString.startsWith(QLatin1Char(FILTER_SEPARATOR))) {
        subString = subString.right(ssSize - 1);
        auto f = parser->getFilter(subString);

        Q_ASSERT(f);

        d->m_filterNames << subString;
        d->m_filters << qMakePair(f, Variable());

      } else if (subString.startsWith(QLatin1Char(FILTER_ARGUMENT_SEPARATOR))) {
        if (d->m_filters.isEmpty()
            || d->m_filters.at(d->m_filters.size() - 1).second.isValid()) {
          const auto remainder = vs.right(vs.size() - lastPos);
          throw Grantlee::Exception(
              TagSyntaxError,
              QStringLiteral("Could not parse the remainder, %1 from %2")
                  .arg(remainder, varString));
        }
        subString = subString.right(ssSize - 1);
        const auto lastFilter = d->m_filters.size();
        if (subString.startsWith(QLatin1Char(FILTER_SEPARATOR)))
          throw Grantlee::Exception(
              EmptyVariableError,
              QStringLiteral("Missing argument to filter: %1")
                  .arg(d->m_filterNames[lastFilter - 1]));

        d->m_filters[lastFilter - 1].second = Variable(subString);
      } else {
        // Token is _("translated"), or "constant", or a variable;
        d->m_variable = Variable(subString);
      }

      pos += len;
      lastPos = pos;
    }

    const auto remainder = vs.right(vs.size() - lastPos);
    if (!remainder.isEmpty()) {
      throw Grantlee::Exception(
          TagSyntaxError,
          QStringLiteral("Could not parse the remainder, %1 from %2")
              .arg(remainder, varString));
    }
  } catch (...) {
    delete d_ptr;
    throw;
  }
}

FilterExpression::FilterExpression(const FilterExpression &other)
    : d_ptr(new FilterExpressionPrivate(this))
{
  *this = other;
}

FilterExpression::FilterExpression() : d_ptr(new FilterExpressionPrivate(this))
{
}

bool FilterExpression::isValid() const
{
  Q_D(const FilterExpression);
  return d->m_variable.isValid();
}

FilterExpression::~FilterExpression() { delete d_ptr; }

Variable FilterExpression::variable() const
{
  Q_D(const FilterExpression);
  return d->m_variable;
}

FilterExpression &FilterExpression::operator=(const FilterExpression &other)
{
  if (&other == this)
    return *this;
  d_ptr->m_variable = other.d_ptr->m_variable;
  d_ptr->m_filters = other.d_ptr->m_filters;
  d_ptr->m_filterNames = other.d_ptr->m_filterNames;
  return *this;
}

QVariant FilterExpression::resolve(OutputStream *stream, Context *c) const
{
  Q_D(const FilterExpression);
  auto var = d->m_variable.resolve(c);

  auto it = d->m_filters.constBegin();
  const auto end = d->m_filters.constEnd();
  for (; it != end; ++it) {
    auto filter = it->first;
    filter->setStream(stream);
    const auto argVar = it->second;
    auto arg = argVar.resolve(c);

    if (arg.isValid()) {
      Grantlee::SafeString argString;
      if (arg.userType() == qMetaTypeId<Grantlee::SafeString>()) {
        argString = arg.value<Grantlee::SafeString>();
      } else if (arg.userType() == qMetaTypeId<QString>()) {
        argString = Grantlee::SafeString(arg.value<QString>());
      }
      if (argVar.isConstant()) {
        argString = markSafe(argString);
      }
      if (!argString.get().isEmpty()) {
        arg = argString;
      }
    }

    const auto varString = getSafeString(var);

    var = filter->doFilter(var, arg, c->autoEscape());

    if (var.userType() == qMetaTypeId<Grantlee::SafeString>()
        || var.userType() == qMetaTypeId<QString>()) {
      if (filter->isSafe() && varString.isSafe()) {
        var = markSafe(getSafeString(var));
      } else if (varString.needsEscape()) {
        var = markForEscaping(getSafeString(var));
      } else {
        var = getSafeString(var);
      }
    }
  }
  (*stream) << getSafeString(var).get();
  return var;
}

QVariant FilterExpression::resolve(Context *c) const
{
  OutputStream _dummy;
  return resolve(&_dummy, c);
}

QVariantList FilterExpression::toList(Context *c) const
{
  const auto var = resolve(c);
  if (!var.canConvert<QVariantList>())
    return {};
  return var.value<QVariantList>();
}

bool FilterExpression::isTrue(Context *c) const
{
  return variantIsTrue(resolve(c));
}

QStringList FilterExpression::filters() const
{
  Q_D(const FilterExpression);
  return d->m_filterNames;
}
