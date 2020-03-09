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

#include "comboboxdelegate.h"
#include "comboboxdelegate_p.h"

#include <QApplication>
#include <QDebug>

ComboBoxEditorCreator::ComboBoxEditorCreator(const QStringList &data,
                                             ComboBoxDelegate::Type type)
    : QItemEditorCreatorBase(), m_data(data), m_type(type)
{
}

ComboBoxEditorCreator::~ComboBoxEditorCreator() {}

QWidget *ComboBoxEditorCreator::createWidget(QWidget *parent) const
{
  ViewComboBox *vcb = new ViewComboBox(parent);
  vcb->addItems(m_data);

  if (m_type == ComboBoxDelegate::Editable)
    vcb->setEditable(true);

  return vcb;
}

QByteArray ComboBoxEditorCreator::valuePropertyName() const
{
  return QByteArray("choice");
}

ViewComboBox::ViewComboBox(QWidget *parent) : QComboBox(parent) {}

QString ViewComboBox::choice() const { return currentText(); }

void ViewComboBox::setChoice(const QString &choice)
{
  const int index = findData(choice, Qt::DisplayRole, Qt::MatchFixedString);
  if (index >= 0)
    setCurrentIndex(index);
  else
    setEditText(choice);
}

ComboBoxDelegate::ComboBoxDelegate(const QStringList &data, Type type,
                                   QObject *parent)
    : QItemDelegate(parent)
{
  QItemEditorFactory *factory = new QItemEditorFactory;
  QItemEditorCreatorBase *creator = new ComboBoxEditorCreator(data, type);
  factory->registerEditor(QVariant::String, creator);

  setItemEditorFactory(factory);
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
  QWidget *w = QItemDelegate::createEditor(parent, option, index);
  ViewComboBox *viewComboBox = qobject_cast<ViewComboBox *>(w);
  Q_ASSERT(viewComboBox);
  return viewComboBox;
}

static QSize textSize(const QFont &font, const QString &text)
{
  QFontMetrics fm(font);
  QSize size = fm.size(Qt::TextSingleLine, text);
  const int textMargin
      = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
  return QSize(size.width() + 2 * textMargin, size.height());
}

QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
  if (m_sizes.contains(index.row())) {
    return m_sizes.value(index.row());
  }
  QSize sz;
  QVariant fontData = index.data(Qt::FontRole);
  QFont fnt = qvariant_cast<QFont>(fontData).resolve(option.font);
  for (int i = 0; i < sizeof sTypes / sizeof *sTypes; ++i) {
    QString text = *(sTypes + i);
    QSize s = textSize(fnt, text);
    sz = sz.expandedTo(s);
  }

  QStyleOptionComboBox opt;
  opt.editable = true;
  opt.frame = true;
  opt.currentText = index.data().toString();

  sz = qApp->style()->sizeFromContents(QStyle::CT_ComboBox, &opt, sz);
  m_sizes.insert(index.row(), sz);

  return sz;
}

#include "moc_comboboxdelegate_p.cpp"
