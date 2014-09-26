#include "MessageList.h"
#include "QVBoxLayout"
#include "QStyledItemDelegate"
#include "QPainter"
#include "QFont"
#include "QStaticText"
#include "QTextOption"

namespace Nomad
{

namespace Editor
{

class Item : public QListWidgetItem
  {
public:
  QStaticText title;
  QStaticText context;
  };

class Delegate : public QStyledItemDelegate
  {
public:
  QListWidget *_owner;

  Delegate(QListWidget *parent=0)
    : QStyledItemDelegate(parent),
      _owner(parent)
    {
    _monoFont.setStyleHint(QFont::Monospace);
    _monoFont.setFamily("Courier");
    }

  QListWidgetItem *initItem(MessageList::Type t, const Eks::DetailedCodeLocation &, const Eks::String &ctx, const Eks::String &msg)
    {
    Item *item = new Item();

    QTextOption o;
    o.setWrapMode(QTextOption::NoWrap);

    QColor col(Qt::white);
    if (t == MessageList::Error)
      {
      col = Qt::red;
      }
    else if (t == MessageList::Warning)
      {
      col = QColor(255, 128, 0);
      }
    item->setBackgroundColor(col.lighter());

    item->title.setText(msg.data());
    item->title.setTextOption(o);
    QString ctxq(ctx.data());
    ctxq.replace('\n', "<br>");
    item->context.setText(ctxq);
    item->context.setTextOption(o);
    return item;
    }

  void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
    painter->save();

    Item *i = static_cast<Item*>(_owner->item(index.row()));
    if (!i)
      {
      xAssertFail();
      return;
      }

    painter->fillRect(option.rect, i->backgroundColor());
    if(option.state & QStyle::State_Selected)
      {
      painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));
      }

    painter->translate(option.rect.topLeft());
    int x = 0;
    int y = 0;

    painter->drawStaticText(QPointF(x, y), i->title);
    y += i->title.size().height();
    painter->setFont(_monoFont);
    painter->drawStaticText(QPointF(x, y), i->context);

    painter->restore();
    }

  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &index) const
    {
    Item *i = static_cast<Item*>(_owner->item(index.row()));
    if (!i)
      {
      xAssertFail();
      return QSize();
      }

    int x = std::max(i->title.size().width(), i->context.size().width());
    int y = i->title.size().height() + i->context.size().height();

    return QSize(x, y);
    }

  QFont _monoFont;
  };

MessageList::MessageList(QWidget *parent)
  : QWidget(parent)
  {
  auto l = new QVBoxLayout();
  l->setContentsMargins(0, 0, 0, 0);
  setLayout(l);

  _list = new QListWidget(this);
  _list->setItemDelegate(new Delegate(_list));
  l->addWidget(_list);

  clear();
  }

void MessageList::clear()
  {
  _list->clear();
  _list->setVisible(false);
  }

void MessageList::add(Type type, const Eks::DetailedCodeLocation &t, const Eks::String &ctx, const Eks::String &msg)
  {
  _list->setVisible(true);
  _list->addItem(static_cast<Delegate*>(_list->itemDelegate())->initItem(type, t, ctx, msg));
  }
}

}
