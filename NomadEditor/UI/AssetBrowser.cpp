#include "AssetBrowser.h"
#include "ui_AssetBrowser.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "Application.h"
#include "shift/Properties/sdata.inl"
#include "QFileSystemModel"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(AssetBrowserData, NomadEditor)

void AssetBrowserData::createTypeInformation(
    Shift::PropertyInformationTyped<AssetBrowserData> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  if (data.registerAttributes)
    {
    auto block = info->createChildrenBlock(data);
    }
  }

AssetBrowser::AssetBrowser(ProjectInterface *ifc, QWidget *parent) :
  QDockWidget(parent),
  _project(ifc),
  _ui(new Ui::AssetBrowser)
  {
  _ui->setupUi(this);

  _project->addProjectChanged(this, SLOT(setupProject()));

  auto scratch = _project->getScratchParent();
  _browser = scratch->add<AssetBrowserData>();

  _model = new QFileSystemModel;

  setupProject();
  }

AssetBrowser::~AssetBrowser()
  {
  delete _ui;
  }

void AssetBrowser::setupProject()
  {
  auto currentProject = _project->getCurrentProject();
  _ui->dockWidgetContents->setEnabled(currentProject != nullptr);
  _ui->treeView->setModel(nullptr);

  if(!currentProject)
    {
    return;
    }

  QFileInfo project = currentProject->path().toQString();
  QString path = project.dir().absolutePath();
  qDebug() << path;
  _model->setRootPath(path);
  _ui->treeView->setModel(_model);
  _ui->treeView->setRootIndex(_model->index(path));

  rebuildUI();
  }

void AssetBrowser::rebuildUI()
  {
  }

}

}
