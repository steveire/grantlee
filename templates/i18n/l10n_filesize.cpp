/*
  This file is part of the Grantlee template system.

  Copyright (c) 2017 Matthias Fehring <kontakt@buschmann23.de>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "l10n_filesize.h"

#include "abstractlocalizer.h"
#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "template.h"
#include "util.h"

L10nFileSizeNodeFactory::L10nFileSizeNodeFactory() {}

Node *L10nFileSizeNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
    QStringList parts = smartSplit(tagContent);
    parts.removeFirst(); // not interested in the name of the tag
    if (parts.isEmpty()) {
        throw Grantlee::Exception(Grantlee::TagSyntaxError, QStringLiteral("Error: l10n_filesize requires at least the file size as first parameter"));
    }

    Grantlee::FilterExpression size(parts.at(0), p);

    Grantlee::FilterExpression precision;
    if (parts.size() > 1) {
        precision = Grantlee::FilterExpression(parts.at(1), p);
    }

    Grantlee::FilterExpression unitSystem;
    if (parts.size() > 2) {
        unitSystem = Grantlee::FilterExpression(parts.at(2), p);
    }

    Grantlee::FilterExpression multiplier;
    if (parts.size() > 3) {
        multiplier = Grantlee::FilterExpression(parts.at(3), p);
    }

    return new L10nFileSizeNode(size, unitSystem, precision, multiplier, p);
}

L10nFileSizeVarNodeFactory::L10nFileSizeVarNodeFactory() {}

Grantlee::Node *L10nFileSizeVarNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  QStringList parts = smartSplit(tagContent);
  parts.removeFirst(); // not interested in the name of the tag
  if (parts.size() < 2) {
      throw Grantlee::Exception(Grantlee::TagSyntaxError, QStringLiteral("Error: l10n_filesize_var tag takes at least 2 arguments, the file size and the variable name"));
  }

  Grantlee::FilterExpression size(parts.at(0), p);

  Grantlee::FilterExpression precision;
  if (parts.size() > 1) {
      precision = Grantlee::FilterExpression(parts.at(1), p);
  }

  Grantlee::FilterExpression unitSystem;
  if (parts.size() > 2) {
      unitSystem = Grantlee::FilterExpression(parts.at(2), p);
  }

  Grantlee::FilterExpression multiplier;
  if (parts.size() > 3) {
      multiplier = Grantlee::FilterExpression(parts.at(3), p);
  }

  auto resultName = parts.last();

  return new L10nFileSizeVarNode(size, unitSystem, precision, multiplier, resultName, p);
}

L10nFileSizeNode::L10nFileSizeNode(const Grantlee::FilterExpression &size,
                                   const Grantlee::FilterExpression &unitSystem,
                                   const Grantlee::FilterExpression &precision,
                                   const Grantlee::FilterExpression &multiplier,
                                   QObject *parent)
    : Node(parent), m_size(size), m_unitSystem(unitSystem), m_precision(precision), m_multiplier(multiplier)
{
}

void L10nFileSizeNode::render(OutputStream *stream, Context *c) const
{
    bool convertNumbers = true;

    qreal size = m_size.resolve(c).toReal(&convertNumbers);
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert input file size into a floating point number.");
        return;
    }

    int unitSystem = m_unitSystem.isValid() ? m_unitSystem.resolve(c).toInt(&convertNumbers) : 10;
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert unit system for file size into integer value. Using default decimal system as default.");
        unitSystem = 10;
    }

    int precision = m_precision.isValid() ? m_precision.resolve(c).toInt(&convertNumbers) : 2;
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert decimal precision for file size into an integer value. Using default value 2.");
        precision = 2;
    }

    qreal multiplier = m_multiplier.isValid() ? m_multiplier.resolve(c).toReal(&convertNumbers) : 1.0f;
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert multiplier file size into a floating point number. Using default value 1.0.");
        multiplier = 1.0f;
    }

    if (multiplier == 0.0f) {
        qWarning("%s", "It makes no sense to multiply the file size by zero. Using default value 1.0.");
        multiplier = 1.0f;
    }

    const std::pair<qreal,QString> fspair = calcFileSize(size, unitSystem, multiplier);

    QString resultString;

    if (precision == 2) {
        resultString = c->localizer()->localizeNumber(fspair.first) + QChar(QChar::Space) + fspair.second;
    } else {
        QLocale l(c->localizer()->currentLocale());
        resultString = l.toString(fspair.first, 'f', precision) + QChar(QChar::Space) + fspair.second;
    }

    streamValueInContext(stream, resultString, c);
}

L10nFileSizeVarNode::L10nFileSizeVarNode(const Grantlee::FilterExpression &size,
                                         const Grantlee::FilterExpression &unitSystem,
                                         const Grantlee::FilterExpression &precision,
                                         const Grantlee::FilterExpression &multiplier,
                                         const QString &resultName, QObject *parent)
    : Node(parent), m_size(size), m_unitSystem(unitSystem), m_precision(precision), m_multiplier(multiplier),
      m_resultName(resultName)
{
}

void L10nFileSizeVarNode::render(OutputStream *stream, Context *c) const
{
    Q_UNUSED(stream)
    bool convertNumbers = true;

    qreal size = m_size.resolve(c).toReal(&convertNumbers);
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert input file size into a floating point number.");
        return;
    }

    int unitSystem = m_unitSystem.isValid() ? m_unitSystem.resolve(c).toInt(&convertNumbers) : 10;
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert unit system for file size into integer value. Using default decimal system.");
        unitSystem = 10;
    }

    int precision = m_precision.isValid() ? m_precision.resolve(c).toInt(&convertNumbers) : 2;
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert decimal precision for file size into an integer value. Using default value 2.");
        precision = 2;
    }

    qreal multiplier = m_multiplier.isValid() ? m_multiplier.resolve(c).toReal(&convertNumbers) : 1.0f;
    if (!convertNumbers) {
        qWarning("%s", "Failed to convert multiplier file size into a floating point number. Using default value 1.0.");
        multiplier = 1.0f;
    }

    if (multiplier == 0.0f) {
        qWarning("%s", "It makes no sense to mulitply the file size by zero. Using default value 1.0.");
        multiplier = 1.0f;
    }

    const std::pair<qreal,QString> fspair = calcFileSize(size, unitSystem, multiplier);

    QString resultString;

    if (precision == 2) {
        resultString = c->localizer()->localizeNumber(fspair.first) + QChar(QChar::Space) + fspair.second;
    } else {
        QLocale l(c->localizer()->currentLocale());
        resultString = l.toString(fspair.first, 'f', precision) + QChar(QChar::Space) + fspair.second;
    }

    c->insert(m_resultName, resultString);
}
