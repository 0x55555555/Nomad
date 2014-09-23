#include "AssetEditor.h"
#include "QMainWindow"
#include "QDockWidget"
#include "QVBoxLayout"
#include "Assets/AssetType.h"
#include "QSplitter"
#include "QToolBar"
#include "QAction"

namespace Nomad
{

namespace Editor
{

AssetEditor::AssetEditor(AssetType *t, ProjectInterface *ifc, AssetType::CreateInterface *c, UIInterface *ui)
    : _asset(t)
  {
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(0, 0, 0, 0);
  setLayout(l);

  auto tools = new QToolBar(this);
  l->addWidget(tools);
  _reload = tools->addAction("Reload");
  _reload->setEnabled(false);
  connect(_reload, &QAction::triggered, [this, c]()
    {
    _asset->reload(c);
    });


  QSplitter *splitter = new QSplitter(this);
  l->addWidget(splitter);
  splitter->setHandleWidth(4);

  if (auto e = t->createEditor(ifc, c))
    {
    splitter->addWidget(e);
    }

  if (auto e = t->createPreview(ui, c))
    {
    splitter->addWidget(e);
    }
  }

AssetEditor *AssetEditor::build(AssetType *t, ProjectInterface *ifc, AssetType::CreateInterface *c, UIInterface *ui)
  {
  return new AssetEditor(t, ifc, c, ui);
  }

void AssetEditor::onReloadAvailable()
  {
  _reload->setEnabled(true);
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
