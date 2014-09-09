#include "AssetEditor.h"
#include "QMainWindow"
#include "QDockWidget"
#include "QVBoxLayout"
#include "Assets/AssetType.h"
#include "QSplitter"

namespace Nomad
{

namespace Editor
{

AssetEditor::AssetEditor(AssetType *t, AssetType::CreateInterface *c, UIInterface *ui)
    : _asset(t)
  {
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(0, 0, 0, 0);
  setLayout(l);

  QSplitter *splitter = new QSplitter(this);
  l->addWidget(splitter);

  if (auto e = t->createEditor(c))
    {
    splitter->addWidget(e);
    }

  if (auto e = t->createPreview(ui))
    {
    splitter->addWidget(e);
    }
  }

AssetEditor *AssetEditor::build(AssetType *t, AssetType::CreateInterface *c, UIInterface *ui)
  {
  return new AssetEditor(t, c, ui);
  }

void AssetEditor::makeDockable(QMainWindow *mw)
  {
  if(qobject_cast<QDockWidget*>(parent()))
    {
    return;
    }

  QDockWidget *w = new QDockWidget(asset()->relativePath());
  w->setAllowedAreas(Qt::AllDockWidgetAreas);

  w->setWidget(this);

  mw->addDockWidget(Qt::RightDockWidgetArea, w);
  }

void AssetEditor::showEditor()
  {
  auto tl = getTopLevel();
  tl->show();
  tl->raise();
  }

QWidget *AssetEditor::getTopLevel()
  {
  if(auto p = qobject_cast<QDockWidget*>(parent()))
    {
    return p;
    }

  return this;
  }
}

}
