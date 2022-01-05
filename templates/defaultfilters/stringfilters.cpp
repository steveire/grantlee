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

#include "stringfilters.h"

#include "util.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QVariant>

QVariant AddSlashesFilter::doFilter(const QVariant &input,
                                    const QVariant &argument,
                                    bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  auto safeString = getSafeString(input);
  safeString.get()
      .replace(QLatin1Char('\\'), QStringLiteral("\\\\"))
      .get()
      .replace(QLatin1Char('\"'), QStringLiteral("\\\""))
      .get()
      .replace(QLatin1Char('\''), QStringLiteral("\\\'"));
  return safeString;
}

QVariant CapFirstFilter::doFilter(const QVariant &input,
                                  const QVariant &argument,
                                  bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  auto safeString = getSafeString(input);
  if (safeString.get().isEmpty())
    return QString();

  return QVariant(safeString.get().at(0).toUpper()
                  + static_cast<QString>(
                      safeString.get().right(safeString.get().size() - 1)));
}

EscapeJsFilter::EscapeJsFilter() {}

static QList<QPair<QString, QString>> getJsEscapes()
{
  QList<QPair<QString, QString>> jsEscapes;
  jsEscapes << QPair<QString, QString>(QChar::fromLatin1('\\'),
                                       QStringLiteral("\\u005C"))
            << QPair<QString, QString>(QChar::fromLatin1('\''),
                                       QStringLiteral("\\u0027"))
            << QPair<QString, QString>(QChar::fromLatin1('\"'),
                                       QStringLiteral("\\u0022"))
            << QPair<QString, QString>(QChar::fromLatin1('>'),
                                       QStringLiteral("\\u003E"))
            << QPair<QString, QString>(QChar::fromLatin1('<'),
                                       QStringLiteral("\\u003C"))
            << QPair<QString, QString>(QChar::fromLatin1('&'),
                                       QStringLiteral("\\u0026"))
            << QPair<QString, QString>(QChar::fromLatin1('='),
                                       QStringLiteral("\\u003D"))
            << QPair<QString, QString>(QChar::fromLatin1('-'),
                                       QStringLiteral("\\u002D"))
            << QPair<QString, QString>(QChar::fromLatin1(';'),
                                       QStringLiteral("\\u003B"))
            << QPair<QString, QString>(QChar(0x2028), QStringLiteral("\\u2028"))
            << QPair<QString, QString>(QChar(0x2029),
                                       QStringLiteral("\\u2029"));

  for (auto i = 0; i < 32; ++i) {
    jsEscapes << QPair<QString, QString>(
        QChar(i),
        QStringLiteral("\\u00")
            + QStringLiteral("%1").arg(i, 2, 16, QLatin1Char('0')).toUpper());
  }
  return jsEscapes;
}

QVariant EscapeJsFilter::doFilter(const QVariant &input,
                                  const QVariant &argument,
                                  bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  QString retString = getSafeString(input);

  static const auto jsEscapes = getJsEscapes();

  for (auto &escape : jsEscapes) {
    retString = retString.replace(escape.first, escape.second);
  }
  return retString;
}

QVariant FixAmpersandsFilter::doFilter(const QVariant &input,
                                       const QVariant &argument,
                                       bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  auto safeString = getSafeString(input);

  const QRegularExpression fixAmpersandsRegexp(
      QStringLiteral("&(?!(\\w+|#\\d+);)"));

  safeString.get().replace(fixAmpersandsRegexp, QStringLiteral("&amp;"));

  return safeString;
}

QVariant CutFilter::doFilter(const QVariant &input, const QVariant &argument,
                             bool autoescape) const
{
  Q_UNUSED(autoescape)
  auto retString = getSafeString(input);
  auto argString = getSafeString(argument);

  auto inputSafe = retString.isSafe();

  retString.get().remove(argString);

  if (inputSafe && argString.get() != QChar::fromLatin1(';'))
    return markSafe(retString);
  else
    return retString;
}

QVariant SafeFilter::doFilter(const QVariant &input, const QVariant &argument,
                              bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  return markSafe(getSafeString(input));
}

QVariant LineNumbersFilter::doFilter(const QVariant &input,
                                     const QVariant &argument,
                                     bool autoescape) const
{
  Q_UNUSED(argument)
  auto safeString = getSafeString(input);
  auto lines = safeString.get().split(QLatin1Char('\n'));
  auto width = QString::number(lines.size()).size();

  const auto shouldEscape = (autoescape && !safeString.isSafe());
  for (auto i = 0; i < lines.size(); ++i) {
    lines[i]
        = QStringLiteral("%1. %2")
              .arg(i + 1, width)
              .arg(shouldEscape ? QString(escape(lines.at(i))) : lines.at(i));
  }

  return markSafe(lines.join(QChar::fromLatin1('\n')));
}

QVariant LowerFilter::doFilter(const QVariant &input, const QVariant &argument,
                               bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  return getSafeString(input).get().toLower();
}

QVariant StringFormatFilter::doFilter(const QVariant &input,
                                      const QVariant &argument,
                                      bool autoescape) const
{
  Q_UNUSED(autoescape)
  SafeString a;
  if (isSafeString(input))
    a = getSafeString(input);
  else if (input.userType() == qMetaTypeId<QVariantList>()) {
    a = toString(input.value<QVariantList>());
  }

  return SafeString(getSafeString(argument).get().arg(a),
                    getSafeString(input).isSafe());
}

QVariant TitleFilter::doFilter(const QVariant &input, const QVariant &argument,
                               bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)

  QString str = getSafeString(input);

  auto it = str.begin();
  const auto end = str.end();

  auto toUpper = true;
  for (; it != end; ++it) {
    if (toUpper)
      *it = it->toUpper();
    else
      *it = it->toLower();
    toUpper = it->isSpace();
  }

  return str;
}

QVariant TruncateWordsFilter::doFilter(const QVariant &input,
                                       const QVariant &argument,
                                       bool autoescape) const
{
  Q_UNUSED(autoescape)
  auto s = getSafeString(argument);

  bool ok;
  auto numWords = s.get().toInt(&ok);

  if (!ok) {
    return input.value<QString>();
  }

  QString inputString = getSafeString(input);
  auto words = inputString.split(QLatin1Char(' '),
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                                 QString::SkipEmptyParts
#else
                                 Qt::SkipEmptyParts
#endif
  );

  if (words.size() > numWords) {
    words = words.mid(0, numWords);
    if (!words.at(words.size() - 1).endsWith(QStringLiteral("..."))) {
      words << QStringLiteral("...");
    }
  }
  return words.join(QChar::fromLatin1(' '));
}

QVariant UpperFilter::doFilter(const QVariant &input, const QVariant &argument,
                               bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  return getSafeString(input).get().toUpper();
}

QVariant WordCountFilter::doFilter(const QVariant &input,
                                   const QVariant &argument,
                                   bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  return QString::number(
      getSafeString(input).get().split(QLatin1Char(' ')).size());
}

QVariant LJustFilter::doFilter(const QVariant &input, const QVariant &argument,
                               bool autoescape) const
{
  Q_UNUSED(autoescape)
  return getSafeString(input).get().leftJustified(
      getSafeString(argument).get().toInt());
}

QVariant RJustFilter::doFilter(const QVariant &input, const QVariant &argument,
                               bool autoescape) const
{
  Q_UNUSED(autoescape)
  return getSafeString(input).get().rightJustified(
      getSafeString(argument).get().toInt());
}

QVariant CenterFilter::doFilter(const QVariant &input, const QVariant &argument,
                                bool autoescape) const
{
  Q_UNUSED(autoescape)
  QString value = getSafeString(input);
  const auto valueWidth = value.size();
  const auto width = getSafeString(argument).get().toInt();
  const auto totalPadding = width - valueWidth;
  const auto rightPadding = totalPadding >> 1;

  return value.leftJustified(valueWidth + rightPadding).rightJustified(width);
}

QVariant EscapeFilter::doFilter(const QVariant &input, const QVariant &argument,
                                bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  return markForEscaping(getSafeString(input));
}

QVariant ForceEscapeFilter::doFilter(const QVariant &input,
                                     const QVariant &argument,
                                     bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  return markSafe(escape(getSafeString(input)));
}

QVariant RemoveTagsFilter::doFilter(const QVariant &input,
                                    const QVariant &argument,
                                    bool autoescape) const
{
  Q_UNUSED(autoescape)
  const auto tags = getSafeString(argument).get().split(QLatin1Char(' '));
  const auto tagRe
      = QStringLiteral("(%1)").arg(tags.join(QChar::fromLatin1('|')));
  const QRegularExpression startTag(
      QStringLiteral("<%1(/?>|(\\s+[^>]*>))").arg(tagRe));
  const QRegularExpression endTag(QStringLiteral("</%1>").arg(tagRe));

  auto value = getSafeString(input);
  const auto safeInput = value.isSafe();
  value.get().remove(startTag);
  value.get().remove(endTag);
  if (safeInput)
    return markSafe(value);
  return value;
}

QVariant StripTagsFilter::doFilter(const QVariant &input,
                                   const QVariant &argument,
                                   bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  static QRegularExpression tagRe(QStringLiteral("<[^>]*>"),
                                  QRegularExpression::InvertedGreedinessOption);

  QString value = getSafeString(input);
  value.remove(tagRe);
  return value;
}

QVariant WordWrapFilter::doFilter(const QVariant &input,
                                  const QVariant &argument,
                                  bool autoescape) const
{
  Q_UNUSED(autoescape)
  QString _input = getSafeString(input);
  auto width = argument.value<int>();
  auto partList = _input.split(QLatin1Char(' '),
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                               QString::SkipEmptyParts
#else
                               Qt::SkipEmptyParts
#endif
  );
  if (partList.isEmpty())
    return {};
  auto output = partList.takeFirst();
  auto pos = output.size() - output.lastIndexOf(QLatin1Char('\n')) - 1;
  for (const QString &part : partList) {
    QStringList lines;
    if (part.contains(QLatin1Char('\n'))) {
      lines = part.split(QLatin1Char('\n'));
    } else {
      lines.append(part);
    }
    pos += lines.first().size() + 1;
    if (pos > width) {
      output.append(QLatin1Char('\n'));
      pos += lines.last().size();
    } else {
      output.append(QLatin1Char(' '));
      if (lines.size() > 1)
        pos += lines.last().size();
    }
    output.append(part);
  }
  return output;
}

QVariant FloatFormatFilter::doFilter(const QVariant &input,
                                     const QVariant &argument,
                                     bool autoescape) const
{
  Q_UNUSED(autoescape)
  double inputDouble;
  switch (input.type()) {
  case QMetaType::Int:
  case QMetaType::UInt:
  case QMetaType::LongLong:
  case QMetaType::ULongLong:
  case QMetaType::Double:
    inputDouble = input.toDouble();
    break;
  default:
    inputDouble = getSafeString(input).get().toDouble();
  }

  int precision;
  if (argument.isValid())
    precision = getSafeString(argument).get().toInt();
  else
    precision = 1;

  return QString::number(inputDouble, 'f', precision);
}

QVariant SafeSequenceFilter::doFilter(const QVariant &input,
                                      const QVariant &argument,
                                      bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  QVariantList list;
  if (input.userType() == qMetaTypeId<QVariantList>())
    for (const QVariant &item : input.value<QVariantList>())
      list << markSafe(getSafeString(item));
  return list;
}

QVariant LineBreaksFilter::doFilter(const QVariant &input,
                                    const QVariant &argument,
                                    bool autoescape) const
{
  Q_UNUSED(argument)
  auto inputString = getSafeString(input);
  static const QRegularExpression re(QStringLiteral("\n{2,}"));
  QStringList output;

  for (const QString &bit : inputString.get().split(re)) {
    auto _bit = SafeString(bit, inputString.isSafe());
    if (autoescape)
      _bit = conditionalEscape(_bit);
    _bit.get().replace(QLatin1Char('\n'), QStringLiteral("<br />"));
    output.append(QStringLiteral("<p>%1</p>").arg(_bit));
  }
  return markSafe(output.join(QStringLiteral("\n\n")));
}

QVariant LineBreaksBrFilter::doFilter(const QVariant &input,
                                      const QVariant &argument,
                                      bool autoescape) const
{
  Q_UNUSED(argument)
  auto inputString = getSafeString(input);
  if (autoescape && isSafeString(input)) {
    inputString = conditionalEscape(inputString);
  }
  return markSafe(
      inputString.get().replace(QLatin1Char('\n'), QStringLiteral("<br />")));
}

static QString nofailStringToAscii(const QString &input)
{
  QString output;
  output.reserve(input.size());

  auto it = input.constBegin();
  const auto end = input.constEnd();
  static const QChar asciiEndPoint(128);
  for (; it != end; ++it)
    if (*it < asciiEndPoint)
      output.append(*it);

  return output;
}

QVariant SlugifyFilter::doFilter(const QVariant &input,
                                 const QVariant &argument,
                                 bool autoescape) const
{
  Q_UNUSED(argument)
  Q_UNUSED(autoescape)
  QString inputString = getSafeString(input);
  inputString = inputString.normalized(QString::NormalizationForm_KD);
  inputString = nofailStringToAscii(inputString);
  inputString
      = inputString.remove(QRegularExpression(QStringLiteral("[^\\w\\s-]")))
            .trimmed()
            .toLower();
  return markSafe(inputString.replace(
      QRegularExpression(QStringLiteral("[-\\s]+")), QChar::fromLatin1('-')));
}

QVariant FileSizeFormatFilter::doFilter(const QVariant &input,
                                        const QVariant &argument,
                                        bool autoescape) const
{
  QVariant ret;

  Q_UNUSED(autoescape)
  const auto arg = getSafeString(argument);
  bool numberConvert = true;

  qreal size = 0.0f;
  if (input.canConvert<qreal>()) {
    size = input.toReal(&numberConvert);
    if (!numberConvert) {
      qWarning("%s",
               "Failed to convert input file size into floating point value.");
    }
  } else {
    size = getSafeString(input).get().toDouble(&numberConvert);
    if (!numberConvert) {
      qWarning("%s",
               "Failed to convert input file size into floating point value.");
    }
  }

  int unitSystem = 10;
  int precision = 2;
  qreal multiplier = 1.0f;

  if (!arg.get().isEmpty()) {
    const auto argList = arg.get().split(QLatin1Char(','),
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                                         QString::SkipEmptyParts
#else
                                         Qt::SkipEmptyParts
#endif
    );
    const auto numArgs = argList.size();
    if (numArgs > 0) {
      unitSystem = argList.at(0).toInt(&numberConvert);
      if (!numberConvert) {
        qWarning("%s", "Failed to convert filse size format unit system into "
                       "integer. Falling back to default 10.");
        unitSystem = 10;
      }
    }

    if (numArgs > 1) {
      precision = argList.at(1).toInt(&numberConvert);
      if (!numberConvert) {
        qWarning("%s", "Failed to convert file size format decimal precision "
                       "into integer. Falling back to default 2.");
        precision = 2;
      }
    }

    if (numArgs > 2) {
      multiplier = argList.at(2).toDouble(&numberConvert);
      if (!numberConvert) {
        qWarning("%s", "Failed to convert file size format multiplier into "
                       "double value. Falling back to default 1.0");
        multiplier = 1.0f;
      } else {
        if (multiplier == 0.0f) {
          qWarning("%s", "It makes no sense to multiply the file size by zero. "
                         "Using default value 1.0.");
          multiplier = 1.0f;
        }
      }
    }
  }

  const double sizeMult = size * multiplier;

  if (unitSystem == 10) {
    if ((sizeMult > -1000) && (sizeMult < 1000)) {
      precision = 0;
    }
  } else if (unitSystem == 2) {
    if ((sizeMult > -1024) && (sizeMult < 1024)) {
      precision = 0;
    }
  }

  const std::pair<qreal, QString> sizePair
      = calcFileSize(size, unitSystem, multiplier);

  const QString retString = QString::number(sizePair.first, 'f', precision)
                            + QLatin1Char(' ') + sizePair.second;

  ret.setValue(retString);

  return ret;
}

QVariant TruncateCharsFilter::doFilter(const QVariant &input,
                                       const QVariant &argument,
                                       bool autoescape) const
{
  Q_UNUSED(autoescape)
  QString retString = getSafeString(input);
  int count = getSafeString(argument).get().toInt();

  if (retString.length() < count)
    return retString;
  retString.truncate(count);
  retString.append(QStringLiteral("..."));
  return retString;
}
