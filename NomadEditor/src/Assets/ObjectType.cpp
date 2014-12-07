#include "Assets/ObjectType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NObject.h"
#include "ProjectInterface.h"
#include "UIInterface.h"
#include "QVBoxLayout"
#include "QToolButton"
#include "QTabWidget"
#include "QTabBar"
#include "QLabel"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ObjectType, Nomad)

void ObjectType::createTypeInformation(
    Shift::PropertyInformationTyped<ObjectType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

const char *ObjectType::extension() const
  {
  return "ent";
  }

Asset *ObjectType::defaultCreate(CreateInterface *)
  {
  return assetParent()->add<Object>();
  }

QWidget *ObjectType::createEditor(ProjectInterface *p, CreateInterface *c)
  {
  auto w = new QWidget;
  auto l = new QVBoxLayout;
  l->setContentsMargins(0, 0, 0, 0);
  l->setSpacing(4);
  w->setLayout(l);

  auto l2 = new QHBoxLayout;
  l->addLayout(l2);
  auto add = new QToolButton;
  add->setText("+");
  auto remove = new QToolButton;
  remove->setText("-");
  l2->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
  l2->addWidget(add);
  l2->addWidget(remove);

  auto ass = asset(c);
  Object *o = nullptr;
  if (ass)
    {
    o = ass->castTo<Object>();
    }
  add->setEnabled(o != nullptr);
  remove->setEnabled(o != nullptr);

  auto tabs = new QTabWidget(w);
  l->addWidget(tabs);

  QObject::connect(add, &QToolButton::clicked,
    [this, tabs, o, p, c]()
    {
    p->addInterface(o);
    updateWidget(tabs, p, c);
    });

  QObject::connect(remove, &QToolButton::clicked,
    [this, tabs, o, p, c]()
    {
    int idx = tabs->currentIndex();
    auto bar = tabs->tabBar();
    auto data = (Interface*)bar->tabData(idx).toULongLong();
    if(data)
      {
      o->removeAttribute(data);
      updateWidget(tabs, p, c);
      }
    });

  updateWidget(tabs, p, c);

  return w;
  }

QWidget *ObjectType::createPreview(UIInterface *, CreateInterface *)
  {
  return nullptr;
  }

void ObjectType::updateWidget(QTabWidget *w, ProjectInterface *p, CreateInterface *c)
  {
  w->clear();
  bool noWidgets = true;

  auto bar = w->tabBar();

  auto ass = asset(c);
  Object *o = nullptr;
  if (ass)
    {
    o = ass->castTo<Object>();
    }

  if (o)
    {
    xForeach(auto i, o->interfaces())
      {
      auto ifc = i->findInterface<UICreator>();
      QWidget *widg = nullptr;
      if (ifc)
        {
        widg = ifc->createEditor(i, c, p);
        }

      if (!widg)
        {
        auto w = new QLabel("No UI for component.");
        w->setAlignment(Qt::AlignCenter);
        w->setEnabled(false);
        widg = w;
        }

      w->addTab(widg, i->name().data());
      int index = bar->count();
      bar->setTabData(index-1, (qulonglong)i);
      noWidgets = false;
      }
    }

  if (noWidgets)
    {
    auto noInt = new QLabel("Add components using the + above.");
    noInt->setAlignment(Qt::AlignCenter);
    noInt->setEnabled(false);
    w->addTab(noInt, "No Components");
    }
  }

}

}
