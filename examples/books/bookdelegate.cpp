/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "bookdelegate.h"

#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>

BookDelegate::BookDelegate(QObject *parent)
    : QSqlRelationalDelegate(parent), star(QPixmap(":images/star.png"))
{
}

void BookDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
  if (index.column() != 5) {
    QStyleOptionViewItem opt = option;
    opt.rect.adjust(0, 0, -1, -1); // since we draw the grid ourselves
    QSqlRelationalDelegate::paint(painter, opt, index);
  } else {
    const QAbstractItemModel *model = index.model();
    QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled)
                                  ? (option.state & QStyle::State_Active)
                                        ? QPalette::Normal
                                        : QPalette::Inactive
                                  : QPalette::Disabled;

    if (option.state & QStyle::State_Selected)
      painter->fillRect(option.rect,
                        option.palette.color(cg, QPalette::Highlight));

    int rating = model->data(index, Qt::DisplayRole).toInt();
    int width = star.width();
    int height = star.height();
    int x = option.rect.x();
    int y = option.rect.y() + (option.rect.height() / 2) - (height / 2);
    for (int i = 0; i < rating; ++i) {
      painter->drawPixmap(x, y, star);
      x += width;
    }
    drawFocus(
        painter, option,
        option.rect.adjusted(0, 0, -1, -1)); // since we draw the grid ourselves
  }

  QPen pen = painter->pen();
  painter->setPen(option.palette.color(QPalette::Mid));
  painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
  painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
  painter->setPen(pen);
}

QSize BookDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
  if (index.column() == 5)
    return QSize(5 * star.width(), star.height()) + QSize(1, 1);

  return QSqlRelationalDelegate::sizeHint(option, index)
         + QSize(1, 1); // since we draw the grid ourselves
}

bool BookDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index)
{
  if (index.column() != 5)
    return QSqlRelationalDelegate::editorEvent(event, model, option, index);

  if (event->type() == QEvent::MouseButtonPress) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    int stars = qBound(
        0, int(0.7
               + qreal(mouseEvent->pos().x() - option.rect.x()) / star.width()),
        5);
    model->setData(index, QVariant(stars));
    return false; // so that the selection can change
  }

  return true;
}

QWidget *BookDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
  if (index.column() != 4)
    return QSqlRelationalDelegate::createEditor(parent, option, index);

  // for editing the year, return a spinbox with a range from -1000 to 2100.
  QSpinBox *sb = new QSpinBox(parent);
  sb->setFrame(false);
  sb->setMaximum(2100);
  sb->setMinimum(-1000);

  return sb;
}
